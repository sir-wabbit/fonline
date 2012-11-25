#include "stdafx.h"

#include "CHexField.h"
#include "common.h"

#include <SimpleLeakDetector/SimpleLeakDetector.hpp>
/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/
// стандартные обьекты карты
#define DRAW_ORDER_HEX  0
//#define DRAW_ORDER_MISC 1 //!Cvet
#define DRAW_ORDER_WALL 2
#define DRAW_ORDER_SCEN 3
#define DRAW_ORDER_ITEM 4
#define DRAW_ORDER_CRIT 5

//специальные объекты
#define SP_SCEN_LIGHT		141 //Light Source
#define SP_SCEN_MARKER		49 //Exit Grid Map Marker
#define SP_SCEN_BLOCK		67 //Secret Blocking Hex
#define SP_SCEN_IBLOCK		344 //Block Hex Auto Inviso

#define SP_WALL_BLOCK		622 //Wall s.t.

#define SP_MISC_SCRBLOCK	1 //!Cvet scroll block

CHexField::CHexField()
{
	view2=NULL;
	lpVBpr_tile=NULL;
	lpVBpr_roof=NULL;
	crtd=0;
	lpSM=NULL;
	ShowHex=0;
	ShowRain=0; //!Cvet

	for(int i=0;i<MAXTILEX;i++)
		for(int j=0;j<MAXTILEY;j++)
			hex_field[i][j].pos=i*MAXTILEX*10+j*10; //!Cvet вместо 2000 => MAXTILEX

	hbegin=0;
	hend=0;
	wright=0;
	wleft=0;
}

int CHexField::Init(CSpriteManager* lpsm)
{
	WriteLog("CHexField Initialization...\n");

	if(!lpsm)
	{
		ReportErrorMessage("CHexField Init","lpsm=NULL");
		return 0;
	}
	lpSM=lpsm;

	if(!fm.Init()) return 0;
	if(!fm_map.Init()) return 0;

	if(!LoadList("tiles.lst",PT_ART_TILES,&tile_fnames)) return 0;
	if(!LoadList("items.lst",PT_ART_ITEMS,&item_fnames)) return 0; //!Cvet
	if(!LoadList("items.lst",PT_PRO_ITEMS,&item_proto)) return 0; //!Cvet
	if(!LoadList("scenery.lst",PT_ART_SCENERY,&scen_fnames)) return 0;
	if(!LoadList("scenery.lst",PT_PRO_SCENERY,&scen_proto)) return 0;
	if(!LoadList("walls.lst",PT_ART_WALLS,&wall_fnames)) return 0;
//	if(!LoadList("misc.lst",PT_ART_MISC,&misc_fnames)) return 0; //!Cvet

	if(!(hex=lpSM->LoadSprite("hex.frm",PT_ART_MISC))) return 0;
	if(!(hexb=lpSM->LoadSprite("hexb.frm",PT_ART_MISC))) return 0;
	if(!(pic_drop=lpSM->LoadSprite("drop.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[0]=lpSM->LoadSprite("adrop1.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[1]=lpSM->LoadSprite("adrop2.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[2]=lpSM->LoadSprite("adrop3.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[3]=lpSM->LoadSprite("adrop4.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[4]=lpSM->LoadSprite("adrop5.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[5]=lpSM->LoadSprite("adrop6.png",PT_ART_MISC))) return 0;
	if(!(pic_adrop[6]=lpSM->LoadSprite("adrop7.png",PT_ART_MISC))) return 0;

	if(!(pic_track1=lpSM->LoadSprite("track1.png",PT_ART_MISC))) return 0;
	if(!(pic_track2=lpSM->LoadSprite("track2.png",PT_ART_MISC))) return 0;
	ShowTrack=false;

	if(!(egg=lpSM->LoadSprite("egg.png",PT_ART_MISC))) return 0;

	WriteLog("CHexField Initialization complete\n");
	crtd=1;

	MapLoaded=FALSE; //!Cvet

	return 1;
}

void CHexField::Clear()
{
	WriteLog("CHexField Clear...\n");

	fm.Clear();
	fm_map.Clear();

	for(char_map::iterator it=item_proto.begin();it!=item_proto.end();it++) //!Cvet
		delete[] (*it).second;
	item_proto.clear();

	for(char_map::iterator it=scen_proto.begin();it!=scen_proto.end();it++)
		delete[] (*it).second;
	scen_proto.clear();


	for(char_map::iterator it=item_fnames.begin();it!=item_fnames.end();it++)
		delete[] (*it).second;
	item_fnames.clear();

	for(char_map::iterator it=scen_fnames.begin();it!=scen_fnames.end();it++)
		delete[] (*it).second;
	scen_fnames.clear();

	for(char_map::iterator it=wall_fnames.begin();it!=wall_fnames.end();it++)
		delete[] (*it).second;
	wall_fnames.clear();

	for(char_map::iterator it=tile_fnames.begin();it!=tile_fnames.end();it++)
		delete[] (*it).second;
	tile_fnames.clear();

//	for(it=misc_fnames.begin();it!=misc_fnames.end();it++)
//		delete[] (*it).second;
//	misc_fnames.clear();


	for(dtree_map::iterator jt=dtree.begin();jt!=dtree.end();jt++)
		SAFEDEL((*jt).second);
	dtree.clear();

	for(dtree_map::iterator jt=dtree_roof_rain.begin();jt!=dtree_roof_rain.end();jt++) //!Cvet
		SAFEDEL((*jt).second);
	dtree_roof_rain.clear();

	for(Rain::iterator it_rain=rain.begin();it_rain!=rain.end();it_rain++) //!Cvet
		SAFEDEL((*it_rain));
	rain.clear();

	all_obj.clear(); //!Cvet

	for(int y=0;y<MAXTILEY;y++)
		for(int x=0;x<MAXTILEX;x++)
		{
			for(scen_vect::iterator si=hex_field[y][x].sc_obj.begin();si!=hex_field[y][x].sc_obj.end();si++)
				delete (*si);

			for(item_vect::iterator ti=hex_field[y][x].itm_obj.begin();ti!=hex_field[y][x].itm_obj.end();ti++)
				delete (*ti);

//			for(misc_vect::iterator mi=hex_field[y][x].msc_obj.begin();mi!=hex_field[y][x].msc_obj.end();mi++)
//				delete (*mi);
		}

	for(onesurf_vec::iterator oi=prep_vec_tile.begin();oi!=prep_vec_tile.end();oi++)
		delete (*oi);
	prep_vec_tile.clear();

	for(onesurf_vec::iterator oi=prep_vec_roof.begin();oi!=prep_vec_roof.end();oi++)
		delete (*oi);
	prep_vec_roof.clear();

	SAFEREL(lpVBpr_tile);
	SAFEREL(lpVBpr_roof);
	SAFEDELA(view2);

	MapLoaded=FALSE; //!Cvet

	crtd=0;

	WriteLog("CHexField Clear complete\n");
}

int CHexField::LoadList(char* lname,int PathType,char_map* pmap)
{
	if(!fm.LoadFile(lname,PathType))
		return 0;
	char str[1024];
	char *rec=NULL;
	int i=0;

	while(fm.GetStr(str,1023))
	{
		strlwr(str);
		char* pos=strstr(str,".");
		if(!pos)
		{	
			i++;
			WriteLog("CHexField LoadList> в list файле найдена строка без имени файла: %s\n",str);
			continue; 
		}
		pos[4]=0;//отсекаем комментарии
		rec=new char[strlen(str)+1];
		strcpy(rec,str);
		(*pmap)[i++]=rec;
	}

	fm.UnloadFile();

	return 1;
}

int CHexField::LoadMap(char* fname)
{
	WriteLog("Загрузка карты %s...\n",fname);
	TICK gc=GetTickCount();

	UnLoadMap(); //!Cvet

//	ac1=0;ac2=0;ac3=0;ac4=0;ac5=0;ac6=0;ac7=0;acres=0;

	if(!fm_map.LoadFile(fname,PT_MAPS)) return 0;

	fm_map.SetCurPos(0x20);
	uint32_t nlv=fm_map.GetDWord();//localvars
	fm_map.SetCurPos(0x30);
	uint32_t ngv=fm_map.GetDWord();//global vars

	fm_map.SetCurPos(0x28); //!Cvet размеры карты
	uint32_t count_tiles_y=fm_map.GetDWord(); //!Cvet

	fm_map.SetCurPos(0xEC+4*(nlv+ngv)); //начало секции тайлов

	uint16_t tile; //ground_tile
	uint16_t rtile; //roof_tile
	word_map::iterator it;

	for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{
			rtile=fm_map.GetWord();
			tile=fm_map.GetWord();
			if(tile>1) //1 значит пустой тайл
			{
				it=loaded_tile.find(tile);
				if(it==loaded_tile.end()) //этот тайл не загружен
				{
					uint16_t id=lpSM->LoadSprite(tile_fnames[tile],PT_ART_TILES);
					if(!id) return 0;
					loaded_tile[tile]=id;
					hex_field[2*y][2*x].tile_id=id;
				}
				else //загружен и даем ссылку на ID SpriteMngr
				{
					hex_field[2*y][2*x].tile_id=(*it).second;
				}
			}

			if(rtile>1) //!Cvet Roof
			{
				it=loaded_tile.find(rtile);
				if(it==loaded_tile.end())
				{
					uint16_t id=lpSM->LoadSprite(tile_fnames[rtile],PT_ART_TILES);
					if(!id) return 0;
					loaded_tile[rtile]=id;
					hex_field[2*y][2*x].roof_id=id;
				}
				else
				{
					hex_field[2*y][2*x].roof_id=(*it).second;
				}
			}
		}

//!Cvet ++++ перешагиваем через оставшиеся уровни
	switch (count_tiles_y)
	{
		case 0x00:
			fm_map.GoForward(80000);
			break;
		case 0x08:
			fm_map.GoForward(40000);
			break;
	}
//!Cvet ----

	if(!DropScript()) return 0; //переходим через секцию скриптов

	if(!LoadObj()) return 0;

	fm_map.UnloadFile();

	MapLoaded=TRUE;

	WriteLog("карта загружена за %d ms\n",GetTickCount()-gc);
//	WriteLog("ac1=%d ac2=%d ac3=%d ac4=%d ac5=%d ac6=%d ac7=%d acres=%d\n",ac1,ac2,ac3,ac4,ac5,ac6,ac7,acres);
	return 1;
}

int CHexField::DropScript()
{
	int cnt=0;
	int all_cnt=0; //!Cvet кол-во всех скрипты

	for(int i=0;i<5;i++)
	{
		cnt=fm_map.GetDWord();

		if(!i) continue;

		if(cnt>0)
		{
			uint32_t loop;

			for(loop=16;loop<cnt;loop+=16);
			uint32_t check=0;

			for(int j=0;j<loop;j++)
			{
				uint8_t group=fm_map.GetByte();

			//!Cvet уточнение для типов групп ++++
				switch (group)
				{
				case 1:
					fm_map.GoForward(0x47);
					break;
				case 2:
					fm_map.GoForward(0x43);
					break;
				case 3:
					fm_map.GoForward(0x3F);
					break;
				case 4:
					fm_map.GoForward(0x3F);
					break;
				default:
					fm_map.GoForward(0x3F);
					break;
				}
			//!Cvet уточнение для типов групп ----

				if((j % 16) == 15)
				{
					uint32_t v=fm_map.GetDWord();

					check+=v;

					v=fm_map.GetDWord(); //CRC
		        }

			}//script loop

			if(check!=cnt)
			{
				ReportErrorMessage("CHexField DropScript","Ошибка при прохождении секции скриптов");
				return 0;
			}

		}

		all_cnt+=cnt; //!Cvet
	}

	WriteLog("Scripts> %d\n",all_cnt); 
	return 1;
}

int CHexField::LoadObj()
{
	uint32_t cnt1=fm_map.GetDWord();
	uint32_t cnt2=fm_map.GetDWord();

	uint16_t hbmax=0; //подсчитаем максимальные размеры спрайтов
	uint16_t hemax=0; //чтобы правильно сделать оптимизцию области видимости
	uint16_t wrmax=0;
	uint16_t wlmax=0;

	uint32_t itm_cnt=0; //!Cvet кол-во итемов
	uint32_t cr_cnt=0; //!Cvet кол-во криттеров
	uint32_t sc_cnt=0; //количество scenery
	uint32_t wl_cnt=0; //количество walls

	for(int cic=0;cic<cnt2;cic++)
	{
		uint32_t buf=fm_map.GetDWord();

		uint32_t pos=fm_map.GetDWord();//позиция
		uint32_t y=pos/200;
		uint32_t x=pos%200; //координаты в файле карте (по х остаток)

		uint32_t offs_x=fm_map.GetDWord(); //!Cvet
		uint32_t offs_y=fm_map.GetDWord(); //!Cvet

		fm_map.GoForward(8); //??? что связанное со смещениями

		uint32_t frm_num=fm_map.GetDWord(); //frame number
		uint32_t ori=fm_map.GetDWord(); //ориентация

		buf=fm_map.GetDWord(); //frm pid
		uint32_t type=buf >> 0x18; //тип объекта (scenery, walls etc)
		uint32_t id=buf & 0xFFFF; //идентефикатор

		uint32_t flags=fm_map.GetDWord();

		uint32_t elev=fm_map.GetDWord();

		uint32_t proto_id=fm_map.GetDWord();

		fm_map.GoForward(4);

		uint32_t lights=fm_map.GetDWord();
		uint32_t lighti=fm_map.GetDWord();

		fm_map.GoForward(12); //проходим через Uncknown поля

		uint32_t num_obj=fm_map.GetDWord(); //кол-во объектов в контейнере

		fm_map.GoForward(12); //проходим через Uncknown поля

		switch(type)
		{
		case OBJ_ITEM: //!Cvet items
			if(!ParseItemObj(proto_id,id,x,y,&hbmax,&hemax,&wrmax,&wlmax)) return 0;
			itm_cnt++;
			break;
		case OBJ_CRIT: //!Cvet critters
			fm_map.GoForward(40);
			cr_cnt++;
			break;
		case OBJ_SCEN: //scenery
			if(!ParseScenObj(proto_id,id,x,y,&hbmax,&hemax,&wrmax,&wlmax)) return 0;
			sc_cnt++;
			break;
		case OBJ_WALL: //wall
			if(!ParseWallObj(proto_id,id,x,y,&hbmax,&hemax,&wrmax,&wlmax)) return 0;
			wl_cnt++;
			break;
		case OBJ_MISC:
			if(!ParseMiscObj(proto_id,id,x,y,&hbmax,&hemax,&wrmax,&wlmax)) return 0;
//			msc_cnt++;
			break;
		default:
			continue;
		}

		for(int o=0; o<num_obj; o++)
		{
			
			fm_map.GoForward(4);

			fm_map.GoForward(44);

			uint32_t proto_id=fm_map.GetDWord();

			fm_map.GoForward(40);

			ParseItemObjCont(proto_id,x,y);
		}
	}

	WriteLog("Critters> %d\n",cr_cnt); //!Cvet
	WriteLog("Items> %d\n",itm_cnt); //!Cvet
	WriteLog("Scenery> %d\n",sc_cnt);
	WriteLog("Walls> %d\n",wl_cnt);

	hbegin=(hbmax-24-6)/12; //верхняя граница
	hbegin+=hbegin%2;
	
	hend=(hemax-24-6)/12; //нижнаяя
	hend+=hend%2;

	wright=(wrmax-32-16)/32; //правая
	wright+=wright%2;

	wleft=(wlmax-0-16)/32; //левая
	wleft+=wleft%2+2;

	if(hbegin<0) hbegin=0;
	if(hend<0) hend=0;
	if(wright<0) wright=0;
	if(wleft<0) wleft=0;

	v2h=VIEW_HEIGHT+hbegin+hend; //высота видимой области
	v2w=VIEW_WIDTH+wright+wleft; //ширина

	v2c_x=VIEW_CX+wright; //центры
	v2c_y=VIEW_CY+hbegin;

	SAFEDELA(view2);

	view2=new ViewField[v2h*v2w]; //создаем новую видимую область

	WriteLog("hbegin=%d,hend=%d,wright=%d,wleft=%d\n",hbegin,hend,wright,wleft);

	return 1;
}

//!Cvet ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CHexField::ParseItemObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax)
{
	uint32_t sub_type;

	proto_id&=0xFFFF; //???Cvet 0xFFFFFF

	if(!fm.LoadFile(item_proto[(proto_id)-1],PT_PRO_ITEMS)) return 0;
	fm.SetCurPos(0x20); //Подтип итемов
	sub_type=fm.GetDWord();
	fm.UnloadFile();

	switch (sub_type)
	{
	case SUB_ITEM_ARMOR:
	case SUB_ITEM_CONT:
	case SUB_ITEM_DRUG:
		break;
	case SUB_ITEM_WEAPON:
		fm_map.GoForward(8);
		break;
	case SUB_ITEM_AMMO:
	case SUB_ITEM_MISC:
	case SUB_ITEM_KEY:
		fm_map.GoForward(4);
		break;
	}

	return 1; //не грузим!!!
/*
	SpriteInfo* lpinf;
	word_map::iterator it;
	uint16_t spr_id;

	it=loaded_item.find(id);
	if(it==loaded_item.end())
	{
		spr_id=lpSM->LoadSprite(item_fnames[id],PT_ART_ITEMS,&lpinf);
		if(!spr_id) return 0;

		loaded_item[id]=spr_id;
	}
	else
	{
		spr_id=(*it).second;
		lpinf=lpSM->GetSpriteInfo(spr_id);
	}

	if(lpinf->offs_y>(*hbmax)) (*hbmax)=lpinf->offs_y;
	if( (lpinf->h-lpinf->offs_y) > (*hemax)) (*hemax)=lpinf->h-lpinf->offs_y;
	if( ((lpinf->w >> 1)-lpinf->offs_x)>(*wrmax)) (*wrmax)=(lpinf->w >> 1)-lpinf->offs_x;
	if( ((lpinf->w >> 1)+lpinf->offs_x)>(*wlmax)) (*wlmax)=(lpinf->w >> 1)+lpinf->offs_x;

	ItemObj* newitm;
	newitm=new ItemObj(spr_id,x,y,&hex_field[y][x].scr_x,&hex_field[y][x].scr_y);
	hex_field[y][x].itm_obj.push_back(newitm);
*/
	return 1;
}

int CHexField::ParseItemObjCont(uint32_t proto_id,uint32_t x,uint32_t y)
{
	uint32_t sub_type;
	uint32_t frm_inv;

	proto_id&=0xFFFF; //???0xFFFFFF

	if(!fm.LoadFile(item_proto[(proto_id)-1],PT_PRO_ITEMS)) return 0;
	fm.SetCurPos(0x20); //Подтип итемов
	sub_type=fm.GetDWord();
	fm.SetCurPos(0x34); //Inv FID
	frm_inv=fm.GetDWord();
	fm.UnloadFile();

	switch (sub_type)
	{
	case SUB_ITEM_ARMOR:
	case SUB_ITEM_CONT:
	case SUB_ITEM_DRUG:
		break;
	case SUB_ITEM_WEAPON:
		fm_map.GoForward(8);
		break;
	case SUB_ITEM_AMMO:
	case SUB_ITEM_MISC:
	case SUB_ITEM_KEY:
		fm_map.GoForward(4);
		break;
	}

	return 1;
}

int CHexField::ParseMiscObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax)
{
//	uint32_t sub_type=SUB_MISC_MISC;

//	WriteLog("misc - %d - %d,%d\n",id,x,y);

	if((id>=33 && id<=48) || (id>=17 && id<=24)) fm_map.GoForward(16);

	if(x>=MAXTILEX || y>=MAXTILEY) return 1;

	if(id==SP_MISC_SCRBLOCK)
	{
		hex_field[y][x].scroll_block=TRUE;
		SETFLAG(hex_field[y][x].flags,FT_BLOCK);
		SETFLAG(hex_field[y][x].flags,FT_NOTRAKE);
	}
//	if((id>=33 && id<=48) || (id>=17 && id<=24)) sub_type=SUB_MISC_EXITGR;

//	if(sub_type==SUB_MISC_EXITGR) fm_map.GoForward(16);

//	if(sub_type==SUB_MISC_MISC) return 1;

	return 1;
/*
	SpriteInfo* lpinf;
	word_map::iterator it;
	uint16_t spr_id;

	it=loaded_misc.find(id);
	if(it==loaded_misc.end())
	{
		spr_id=lpSM->LoadSprite(misc_fnames[id],PT_ART_MISC,&lpinf);
		if(!spr_id) return 0;

		loaded_misc[id]=spr_id;
	}
	else
	{
		spr_id=(*it).second;
		lpinf=lpSM->GetSpriteInfo(spr_id);
	}

	if(lpinf->offs_y>(*hbmax)) (*hbmax)=lpinf->offs_y;
	if( (lpinf->h-lpinf->offs_y) > (*hemax)) (*hemax)=lpinf->h-lpinf->offs_y;
	if( ((lpinf->w >> 1)-lpinf->offs_x)>(*wrmax)) (*wrmax)=(lpinf->w >> 1)-lpinf->offs_x;
	if( ((lpinf->w >> 1)+lpinf->offs_x)>(*wlmax)) (*wlmax)=(lpinf->w >> 1)+lpinf->offs_x;

	MiscObj* newmsc;
	newmsc=new MiscObj(spr_id);
	hex_field[y][x].msc_obj.push_back(newmsc);

	return 1;
*/
}

int CHexField::AddObj(stat_obj* add_sobj, HexTYPE x, HexTYPE y, uint16_t tile_flags)
{
	if(!add_sobj) return 0;

	SpriteInfo* lpinf;
	word_map::iterator it;
	uint16_t spr_id;
	uint16_t pic_id=add_sobj->p[OBJ_PIC_MAP];
	uint8_t type_obj=add_sobj->type;

	if(type_obj==OBJ_TYPE_DOOR) //связано с тем что двери находяться в сценери
	{
		it=loaded_scen.find(pic_id);
		if(it==loaded_scen.end())
		{
			spr_id=lpSM->LoadSprite(scen_fnames[pic_id],PT_ART_SCENERY,&lpinf);

			if(!spr_id) return 0;

			loaded_scen[pic_id]=spr_id;
		}
		else
		{
			spr_id=(*it).second;
			lpinf=lpSM->GetSpriteInfo(spr_id);
		}
	}
	else
	{
		it=loaded_item.find(pic_id);
		if(it==loaded_item.end())
		{
			spr_id=lpSM->LoadSprite(item_fnames[pic_id],PT_ART_ITEMS,&lpinf);

			if(!spr_id) return 0;

			loaded_item[pic_id]=spr_id;
		}
		else
		{
			spr_id=(*it).second;
			lpinf=lpSM->GetSpriteInfo(spr_id);
		}
	}

	ItemObj* newitm;
	newitm=new ItemObj(add_sobj,spr_id,x,y,0,0,
		&hex_field[y][x].scr_x,&hex_field[y][x].scr_y);

	if(add_sobj->p[OBJ_ANIM_ON_MAP])
	{
		newitm->anim=new AnyFrames;
		int res=0;

		if(add_sobj->type==OBJ_TYPE_DOOR)
			res=lpSM->LoadAnyAnimation(scen_fnames[pic_id],PT_ART_SCENERY,newitm->anim);
		else
			res=lpSM->LoadAnyAnimation(item_fnames[pic_id],PT_ART_ITEMS,newitm->anim);

		if(!res)
		{
			WriteLog("Ошибка - не удалось загрузить анимацию при добавлении итема\n");
			SAFEDEL(newitm);
			return 0;
		}

		if(FLAG(tile_flags,FT_DOOR_OPEN))
		{
			newitm->SetAnimFixFromEnd();
		}
		else if(FLAG(tile_flags,FT_DOOR_CLOSE))
		{
			newitm->SetAnimFixFromStart();
		}
		else
			;//!!!!!!!!!!!!!!!!!!
	}

	hex_field[y][x].itm_obj.push_back(newitm);

	all_obj.push_back(newitm);

	if(IsVisible(x, y, spr_id))
		dtree.insert(dtree_map::value_type(hex_field[y][x].pos+DRAW_ORDER_ITEM,
			new PrepSprite(hex_field[y][x].scr_x+16,hex_field[y][x].scr_y+6,0,
			&newitm->spr_id,&newitm->scr_x,&newitm->scr_y)));

	return 1;
}

void CHexField::ChangeObj(stat_obj* chn_sobj, HexTYPE x, HexTYPE y, uint16_t tile_flags)
{
	item_vect::iterator it_i;
	for(it_i=hex_field[y][x].itm_obj.begin();it_i!=hex_field[y][x].itm_obj.end();it_i++)
		if((*it_i)->sobj->id==chn_sobj->id) break;

	if(it_i==hex_field[y][x].itm_obj.end())
	{
		WriteLog("не найден объект на карте\n");
		return;
	}

	ItemObj* iobj=(*it_i);

	switch(chn_sobj->type)
	{
	case OBJ_TYPE_DOOR:
		if(FLAG(tile_flags,FT_DOOR_CLOSE))
		{
			//if(!FLAG(hex_field[y][x].flags,FT_DOOR_OPEN)) break;
			iobj->SetAnimFromEnd();
		}
		else if(FLAG(tile_flags,FT_DOOR_OPEN))
		{
			//if(FLAG(hex_field[y][x].flags,FT_DOOR_CLOSE))
			iobj->SetAnimFromStart();
		}
		break;
	case OBJ_TYPE_CONTAINER:
		break;
	default:
		break;
	}

	hex_field[y][x].flags=tile_flags;
}

void CHexField::DelObj(stat_obj* del_sobj, HexTYPE x, HexTYPE y)
{
	for(item_vect::iterator it_i=hex_field[y][x].itm_obj.begin();it_i!=hex_field[y][x].itm_obj.end();it_i++)
		if((*it_i)->sobj->id==del_sobj->id)
		{
		  dtree_map::iterator it_dt;
			for(it_dt=dtree.find(hex_field[y][x].pos+DRAW_ORDER_ITEM);it_dt!=dtree.end();it_dt++)
				if((*it_dt).second->lp_sprid==&(*it_i)->spr_id) break;

			if(it_dt!=dtree.end())
			{
				delete (*it_dt).second;
				dtree.erase(it_dt);
			}
      
      item_vect::iterator it_i2;
			for(it_i2=all_obj.begin();it_i2!=all_obj.end();it_i2++)
				if((*it_i)==(*it_i2)) break;
			all_obj.erase(it_i2);

			delete (*it_i);
			hex_field[y][x].itm_obj.erase(it_i);

			break;
		}
}

void CHexField::ProcessObj()
{
	ItemObj* cur_obj=NULL;
	for(item_vect::iterator it_i=all_obj.begin();it_i!=all_obj.end();it_i++)
	{
		cur_obj=(*it_i);

		if(cur_obj->cur_spr==cur_obj->need_spr) continue;

		if(GetTickCount()-cur_obj->last_tick<cur_obj->anim->ticks/cur_obj->anim->cnt_frames) continue;
		
		if(cur_obj->cur_spr>cur_obj->need_spr) cur_obj->cur_spr--;
		if(cur_obj->cur_spr<cur_obj->need_spr) cur_obj->cur_spr++;

		cur_obj->SetAnimSpr(cur_obj->cur_spr);
	}

}
//Cvet --------------------------------------------------------------------

//proto_id - ищем информацию об объекте. id - это frm_id - идентификатор картинки
int CHexField::ParseScenObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax)
{
	uint32_t sub_type;

	proto_id&=0xFFFF;

	if(!fm.LoadFile(scen_proto[(proto_id)-1],PT_PRO_SCENERY)) return 0;
	fm.SetCurPos(0x20); //Scenery Subtype
	sub_type=fm.GetDWord();
	fm.UnloadFile();

	switch(sub_type)
	{
	case SUB_SCEN_PORTAL:
		fm_map.GoForward(4);
		break;
	case SUB_SCEN_MISC:
		break;
	case SUB_SCEN_STAIR:
	case SUB_SCEN_ELEV:
	case SUB_SCEN_LADDWN:
	case SUB_SCEN_LADTOP:
		fm_map.GoForward(8);
		break;
	}

	if(x>=MAXTILEX || y>=MAXTILEY) return 1; //!Cvet

	bool noload=0;
	switch(proto_id)
	{
	case SP_SCEN_BLOCK:
	case SP_SCEN_IBLOCK:
		SETFLAG(hex_field[y][x].flags,FT_BLOCK); //!Cvet проходимость
	case SP_SCEN_LIGHT:
	case SP_SCEN_MARKER:
		noload=1; //не загружаем пока спец-объекты
		break;
	default:
		SETFLAG(hex_field[y][x].flags,FT_BLOCK); //!Cvet проходимость
		break;
	}

	if(noload) return 1;

	SpriteInfo* lpinf;
	word_map::iterator it;
	uint16_t spr_id;

	it=loaded_scen.find(id);
	if(it==loaded_scen.end()) //картинка этого объекта не загружена еще
	{
		spr_id=lpSM->LoadSprite(scen_fnames[id],PT_ART_SCENERY,&lpinf);
		if(!spr_id) return 0;
			
		loaded_scen[id]=spr_id;//добавляем в список загруженных
	}
	else
	{
		spr_id=(*it).second;
		lpinf=lpSM->GetSpriteInfo(spr_id);
	}

	if(lpinf->offs_y>(*hbmax)) (*hbmax)=lpinf->offs_y;
	if( (lpinf->h-lpinf->offs_y) > (*hemax)) (*hemax)=lpinf->h-lpinf->offs_y;
	if( ((lpinf->w >> 1)-lpinf->offs_x)>(*wrmax)) (*wrmax)=(lpinf->w >> 1)-lpinf->offs_x;
	if( ((lpinf->w >> 1)+lpinf->offs_x)>(*wlmax)) (*wlmax)=(lpinf->w >> 1)+lpinf->offs_x;

	ScenObj* newsc;
	newsc=new ScenObj(spr_id);
	hex_field[y][x].sc_obj.push_back(newsc);

	return 1;
}

//proto_id - ищем информацию об объекте. id - это frm_id - идентификатор картинки
int CHexField::ParseWallObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax)
{
	proto_id&=0xFFFF;

	bool noload=0;
	switch(proto_id) 
	{
	case SP_WALL_BLOCK:
		noload=1;
		break;
	default:
		break;
	}

	if(x>=MAXTILEX || y>=MAXTILEY) return 1; //!Cvet

	SETFLAG(hex_field[y][x].flags,FT_BLOCK); //!Cvet проходимость
	SETFLAG(hex_field[y][x].flags,FT_NOTRAKE); //!Cvet простреливаемость

	if(noload) return 1;

	SpriteInfo* lpinf;
	word_map::iterator it;
	uint16_t spr_id;

	it=loaded_wall.find(id);
	if(it==loaded_wall.end()) //картинка этого объекта не загружена еще
	{
		spr_id=lpSM->LoadSprite(wall_fnames[id],PT_ART_WALLS,&lpinf);
		if(!spr_id) return 0;

		loaded_wall[id]=spr_id;//добавляем в список загруженных
	}
	else
	{
		spr_id=(*it).second;
		lpinf=lpSM->GetSpriteInfo(spr_id);
	}

//обновляем максимальные размеры
	if(lpinf->offs_y>(*hbmax)) (*hbmax)=lpinf->offs_y;
	if( (lpinf->h-lpinf->offs_y) > (*hemax)) (*hemax)=lpinf->h-lpinf->offs_y;
	if( ((lpinf->w >> 1)-lpinf->offs_x)>(*wrmax)) (*wrmax)=(lpinf->w >> 1)-lpinf->offs_x;
	if( ((lpinf->w >> 1)+lpinf->offs_x)>(*wlmax)) (*wlmax)=(lpinf->w >> 1)+lpinf->offs_x;

	for(int i=0;i<MAX_WALL_CNT;i++)
		if(!hex_field[y][x].wall_id[i])
		{
			hex_field[y][x].wall_id[i]=spr_id;//и заносим в hex id картинки
			break;
		}

	return 1;
}

void CHexField::SetCenter2(int x, int y)
{
	if(x<0 || y<0 || x>=MAXTILEX || y>=MAXTILEY) return;
	int my=y+y%2;

	InitView2(x,my);

	int ty;

	int y2=0;
	int vpos;

	//уберем флаги отрисовки со старых позиций
	for(ty=0;ty<v2h;ty++)
	{
		for(int tx=0;tx<v2w;tx++)
		{
			vpos=y2+tx;
			int mod_x=view2[vpos].cur_x;
			int mod_y=view2[vpos].cur_y;
			if(mod_x<0 || mod_y<0 || mod_x>=MAXTILEX || mod_y>=MAXTILEY) continue;
			hex_field[mod_y][mod_x].to_draw=0;
		}
		y2+=v2w;
	}

	y2=0;

	//Обрисуем флагами гексы, которые видимы на данный момент. и их координаты на экране.
	for(ty=0;ty<v2h;ty++)
	{
		for(int tx=0;tx<v2w;tx++)
		{
			vpos=y2+tx;

			int mod_x=view2[vpos].mod_x+x;
			int mod_y=view2[vpos].mod_y+y;
			view2[vpos].cur_x=mod_x;
			view2[vpos].cur_y=mod_y;
			if(mod_x<0 || mod_y<0 || mod_x>=MAXTILEX || mod_y>=MAXTILEY) continue;

			hex_field[mod_y][mod_x].to_draw=1;
			hex_field[mod_y][mod_x].scr_x=view2[vpos].scr_x;
			hex_field[mod_y][mod_x].scr_y=view2[vpos].scr_y;
		}
		y2+=v2w;
	}

	//подготовим дерево из тайлов и по нему подготовим буфер вершин, рисуемых в один вызов
	RebuildTiles();

	//очистим дерево, в котором будут объекты.
	for(dtree_map::iterator jt=dtree.begin();jt!=dtree.end();jt++)
		SAFEDEL((*jt).second);
	dtree.clear();

	for(dtree_map::iterator jt=dtree_roof_rain.begin();jt!=dtree_roof_rain.end();jt++)
		SAFEDEL((*jt).second);
	dtree_roof_rain.clear();

	for(Rain::iterator it_rain=rain.begin();it_rain!=rain.end();it_rain++)
		SAFEDEL((*it_rain));
	rain.clear();

	y2=0;
	
	for(ty=0;ty<v2h;ty++)
	{
		for(int x=0;x<v2w;x++)
		{
			vpos=y2+x;
			int ny=view2[vpos].cur_y;
			int nx=view2[vpos].cur_x;
			if(ny<0 || nx<0 || ny>=MAXTILEY || nx>=MAXTILEX) continue;

			if(ShowTrack)
			{
				if(hex_field[ny][nx].track)
					if(x>wright && x<(v2w-wleft) && ty>hbegin && ty<(v2h-hend))
						dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_HEX,
						new PrepSprite(hex_field[ny][nx].scr_x+17,hex_field[ny][nx].scr_y+14,hex_field[ny][nx].track==1?pic_track1:pic_track2)));
			}

			if(ShowHex)
			{
				if(x>wright && x<(v2w-wleft) && ty>hbegin && ty<(v2h-hend))
	//				dtree[hex_field[ny][nx].pos+DRAW_ORDER_HEX]=new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(ny==100 && nx==100)?hexb:hex);
					dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_HEX,
					new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,hex_field[ny][nx].scroll_block?hexb:hex)));
			}

//!Cvet дождь +++++
			if(ShowRain)
			{
				if(x>wright && x<(v2w-wleft) && ty>hbegin && ty<(v2h-hend))
				{

					int rofx=nx;
					int rofy=ny;
					if(rofx%2) rofx--;
					if(rofy%2) rofy--;

					Drop* new_drop;
		//			for(int i=0;i<5;i++)
					if(!hex_field[rofy][rofx].roof_id)
					{
						new_drop=new Drop(pic_drop,random(10)-random(10),(random(100))*-1,0);

						rain.push_back(new_drop);

						dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_HEX,
						new PrepSprite(hex_field[ny][nx].scr_x+16,
							hex_field[ny][nx].scr_y+6,0,
							&new_drop->id,
							&new_drop->x,
							&new_drop->y)));
					}
					else
					{
						new_drop=new Drop(pic_drop,random(10)-random(10),(100+random(100))*-1,-100);

						rain.push_back(new_drop);

						dtree_roof_rain.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_HEX,
						new PrepSprite(hex_field[ny][nx].scr_x+16,
							hex_field[ny][nx].scr_y+6,0,
							&new_drop->id,
							&new_drop->x,
							&new_drop->y)));
					}
				}
			}
//!Cvet ---------

	/*		if(!hex_field[ny][nx].msc_obj.empty() && cmn_show_misc)
				for(misc_vect::iterator mi=hex_field[ny][nx].msc_obj.begin();mi!=hex_field[ny][nx].msc_obj.end();mi++)
				{
					if(!IsVisible(nx, ny, (*mi)->spr_id)) continue;
					
					//иначе заносим его в дерево. будет больше объектов надо будет все что в цикле в отдельную процедуру вывести
					//dtree[hex_field[ny][nx].pos+DRAW_ORDER_SCEN]=new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(*si)->spr_id);
					dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_MISC, new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(*mi)->spr_id)));
				}
*/
			if(!hex_field[ny][nx].sc_obj.empty() && cmn_show_scen)
				for(scen_vect::iterator si=hex_field[ny][nx].sc_obj.begin();si!=hex_field[ny][nx].sc_obj.end();si++)
				{
					if(!IsVisible(nx, ny, (*si)->spr_id)) continue;
					
					//иначе заносим его в дерево. будет больше объектов надо будет все что в цикле в отдельную процедуру вывести
					//dtree[hex_field[ny][nx].pos+DRAW_ORDER_SCEN]=new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(*si)->spr_id);
					dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_SCEN, new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(*si)->spr_id)));
				}

			if(!hex_field[ny][nx].itm_obj.empty() && cmn_show_items)
				for(item_vect::iterator ti=hex_field[ny][nx].itm_obj.begin();ti!=hex_field[ny][nx].itm_obj.end();ti++)
				{
					if(!IsVisible(nx, ny, (*ti)->spr_id)) continue;
					
					//иначе заносим его в дерево. будет больше объектов надо будет все что в цикле в отдельную процедуру вывести
					//dtree[hex_field[ny][nx].pos+DRAW_ORDER_SCEN]=new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,(*si)->spr_id);
					dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_ITEM,
						new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,0,
						&(*ti)->spr_id,&(*ti)->scr_x,&(*ti)->scr_y)));
				}
			
			if(hex_field[ny][nx].wall_id[0] && cmn_show_walls)
			{
					for(int i=0;i<MAX_WALL_CNT;i++)
					{
						if(!hex_field[ny][nx].wall_id[i]) break;
						if(IsVisible(nx, ny, hex_field[ny][nx].wall_id[i]))
							dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_WALL, new PrepSprite(hex_field[ny][nx].scr_x+16,hex_field[ny][nx].scr_y+6,hex_field[ny][nx].wall_id[i])));
					}
			}

			if(hex_field[ny][nx].lpcrit && cmn_show_crit)
			{
				if(!IsVisible(nx, ny, hex_field[ny][nx].lpcrit->cur_id)) {hex_field[ny][nx].lpcrit->SetVisible(0);hex_field[ny][nx].lpcrit->rit=NULL;continue;} //!Cvet memory leak???!!!!!!!!!!
					hex_field[ny][nx].lpcrit->SetVisible(1);

				PrepSprite* prep=new PrepSprite(
					hex_field[ny][nx].scr_x+16,
					hex_field[ny][nx].scr_y+6,0,
					&hex_field[ny][nx].lpcrit->cur_id,
					&hex_field[ny][nx].lpcrit->cur_ox,
					&hex_field[ny][nx].lpcrit->cur_oy,
					&hex_field[ny][nx].lpcrit->alpha);

				hex_field[ny][nx].lpcrit->rit=dtree.insert(dtree_map::value_type(hex_field[ny][nx].pos+DRAW_ORDER_CRIT, prep));

					// не ясно что тут? возможна ли ошибка
				//	hex_field[ny][nx].lpcrit->rx=x;
				//	hex_field[ny][nx].lpcrit->ry=y;
				lpSM->GetDrawCntrRect(prep, &hex_field[ny][nx].lpcrit->drect);
			}
		}
		y2+=v2w;
	}

	ac1=dtree.size();

	cnt_x=x;
	cnt_y=my;
}

void CHexField::RebuildTiles() //!Cvet переделал. добавил подготовку крыши
{
	//подготовим дерево из тайлов и по нему подготовим буфер вершин, рисуемых в один вызов
	dtree_map ttree_t;
	dtree_map ttree_r; //!Cvet

	if(!cmn_show_tiles && !cmn_show_roof) //!Cvet ++++
	{
		prep_cnt_tile=0;
		prep_cnt_roof=0;
		lpSM->PrepareBuffer(&ttree_t, &lpVBpr_tile, &prep_vec_tile);
		lpSM->PrepareBuffer(&ttree_r, &lpVBpr_roof, &prep_vec_roof);
		return;
	} //!Cvet ----

	int vpos;
	int y2=(hbegin-1)*v2w;
	for(int ty=0;ty<(v2h-hend);ty++)
	{
		for(int x=wright-1;x<(v2w-wleft+1);x++)
		{
			vpos=y2+x;
			int ny=view2[vpos].cur_y;
			int nx=view2[vpos].cur_x;
			if(ny<0 || nx<0 || ny>=MAXTILEY || nx>=MAXTILEX) continue;

			if(cmn_show_tiles)
				if(hex_field[ny][nx].tile_id)
					ttree_t.insert(dtree_map::value_type(hex_field[ny][nx].pos, 
						new PrepSprite(hex_field[ny][nx].scr_x-49,hex_field[ny][nx].scr_y-3,hex_field[ny][nx].tile_id)));

			if(cmn_show_roof) //!Cvet roof
				if(hex_field[ny][nx].roof_id)
					ttree_r.insert(dtree_map::value_type(hex_field[ny][nx].pos, 
						new PrepSprite(hex_field[ny][nx].scr_x-49,hex_field[ny][nx].scr_y-3-100,hex_field[ny][nx].roof_id)));
		}
		y2+=v2w;
	}

	prep_cnt_tile=ttree_t.size();
	prep_cnt_roof=ttree_r.size(); //!Cvet
	lpSM->PrepareBuffer(&ttree_t, &lpVBpr_tile, &prep_vec_tile, NULL, TILE_ALPHA);
	lpSM->PrepareBuffer(&ttree_r, &lpVBpr_roof, &prep_vec_roof, NULL, ROOF_ALPHA); //!Cvet

	for(dtree_map::iterator jt=ttree_t.begin();jt!=ttree_t.end();jt++)
		delete (*jt).second;
	ttree_t.clear();
	for(dtree_map::iterator jt=ttree_r.begin();jt!=ttree_r.end();jt++) //!Cvet
		delete (*jt).second;
	ttree_r.clear();
}

int CHexField::IsVisible(int nx, int ny,uint16_t id)
{
	if(!hex_field[ny][nx].to_draw) return 0; //!Cvet ночь отдал чтобы эту ебню вписать...

	SpriteInfo* lpinf=lpSM->GetSpriteInfo(id);
	if(!lpinf) return 0;
	int chx=hex_field[ny][nx].scr_x+16-(lpinf->w >> 1)+lpinf->offs_x;
	int chy=hex_field[ny][nx].scr_y+6-lpinf->h+lpinf->offs_y;

	//если спрайт не попадает на экран - не рисуем его
	if(chx+lpinf->w+32<0 || chx-32>MODE_WIDTH || chy-24>MODE_HEIGHT || chy+lpinf->h+24<0) return 0;
		else return 1;
}

void CHexField::SwitchShowHex()
{
	ShowHex=ShowHex?0:1;
	SetCenter2(cnt_x,cnt_y);
}

void CHexField::SwitchShowRain() //!Cvet
{
	ShowRain=ShowRain?0:1;
	SetCenter2(cnt_x,cnt_y);
}

void CHexField::SwitchShowTrack() //!Cvet
{
	ShowTrack=ShowTrack?0:1;

	if(ShowTrack)
		for(int tx=0;tx<MAXTILEX;tx++)
			for(int ty=0;ty<MAXTILEY;ty++)
				hex_field[ty][tx].track=0;

	SetCenter2(cnt_x,cnt_y);
}

//здесь мы будем заносить в view координаты гексов
//которые они в данный момент представляют
void CHexField::InitView2(int cx,int cy)
{
	int x;
	int xa=32-((ADD_X+wright) << 5);
	int xb=16-((ADD_X+wright) << 5);
	int y=-10-12*(ADD_Y+hbegin);
	int mody;
	int modpos=cx%2;

	int y2=0;
	int vpos;

	for(int j=0;j<v2h;j++)
	{
		x=(j%2)?xa:xb;
		for(int i=0;i<v2w;i++)
		{
			vpos=y2+i;
			view2[vpos].scr_x=MODE_WIDTH-x;
			view2[vpos].scr_y=y;

			if(j>v2c_y) mody=(j-v2c_y+1)/2;
				else mody=(j-v2c_y)/2;
			int x1=i-v2c_x+mody;
			int y1=i-v2c_x+modpos-2*(j-v2c_y)+mody;
			if(y1<0) y1--;
			y1=-y1/2;

			view2[vpos].mod_x=x1;
			view2[vpos].mod_y=y1;
				
			x+=32;
		}
		y+=12;
		y2+=v2w;
	}
}

void  CHexField::DrawMap2()
{
	if(ShowRain)
	{
		Drop* cur_drop=NULL;
		for(Rain::iterator it_rain=rain.begin();it_rain!=rain.end();it_rain++)
		{
			cur_drop=(*it_rain);
			if((cur_drop->y+=2)>=cur_drop->ground_y)
			{
				if(cur_drop->adrop>6)
				{
					cur_drop->id=pic_drop;
					cur_drop->adrop=0;
					cur_drop->x=random(10)-random(10);
					cur_drop->y=(100+random(100))*-1;
				}
				else
					cur_drop->id=pic_adrop[cur_drop->adrop++];
			}
		}
	}

	//tiles
	if(cmn_show_tiles)
		lpSM->DrawPrepared(lpVBpr_tile,&prep_vec_tile,prep_cnt_tile);

	//objects
	lpSM->DrawTreeCntr(&dtree);

	//roof
	if(cmn_show_roof) //!Cvet
	{
		lpSM->DrawPrepared(lpVBpr_roof,&prep_vec_roof,prep_cnt_roof);

		if(ShowRain) lpSM->DrawTreeCntr(&dtree_roof_rain);
	}

	lpSM->Flush();
}

int offs_arrx[]={1,2,4,8,16,32};
int offs_arry[]={1,2,3,6,12,24};

int CHexField::Scroll()
{
	if(opt_scroll_delay<0) opt_scroll_delay=0;
	static TICK LastCall=GetTickCount();
	if((GetTickCount()-LastCall)<opt_scroll_delay) return 1;
		else LastCall=GetTickCount();

	int xmod=0,ymod=0;

	if(cmn_di_mleft || cmn_di_left) xmod+=1;
	if(cmn_di_mright || cmn_di_right) xmod-=1;
	if(cmn_di_mup || cmn_di_up) ymod-=1;
	if(cmn_di_mdown || cmn_di_down) ymod+=1;

	bool both_hold=xmod && ymod;

	if(!xmod && !ymod) return 0;

	if(!opt_scroll_step) opt_scroll_step=1;
	if(opt_scroll_step>32) opt_scroll_step=32;

	int step_y=1;
	for(int i=0;i<6;i++)
		if(offs_arrx[i]==opt_scroll_step)
		{
			step_y=offs_arry[i];
			break;
		}

	//до этого мы отрабатывали плавный скролинг
	cmn_scr_ox+=xmod*opt_scroll_step;
	cmn_scr_oy+=-ymod*step_y;

	//а теперь отработаем переключение CenterView
	int set=0;

	if(cmn_scr_ox>=32)
	{
		xmod=1;
		cmn_scr_ox=0;
		set=1;
	}
	else if(cmn_scr_ox<=-32)
		{
			xmod=-1;
			cmn_scr_ox=0;
			set=1;
		}

	if(set && both_hold)
		if(cmn_scr_oy)
		{
			if(cmn_scr_oy>0) cmn_scr_oy=(cmn_scr_oy>12)?24:0;
				else cmn_scr_oy=(cmn_scr_oy<-12)?-24:0;
		}

	if(cmn_scr_oy>=24)
	{
		ymod=-2;
		cmn_scr_oy=0;
		set=1;
	}
	else if(cmn_scr_oy<=-24)
		{
			ymod=2;
			cmn_scr_oy=0;
			set=1;
		}

	if(set && both_hold)
		if(cmn_scr_ox)
		{
			if(cmn_scr_ox>0) cmn_scr_ox=(cmn_scr_ox>16)?32:0;
				else cmn_scr_ox=(cmn_scr_ox<-16)?-32:0;
		}

	if(set)
	{
	//!Cvet ++++++
/*		int rt_pos=(hbegin+ymod)*v2w+wright+xmod;
		int rb_pos=(VIEW_HEIGHT+hbegin+ymod)*v2w+wright+xmod;
		int lb_pos=(VIEW_HEIGHT+hbegin+ymod)*v2w+VIEW_WIDTH+wright+xmod;
		int lt_pos=(hbegin+ymod)*v2w+VIEW_WIDTH+wright+xmod;

		//!!!пр. на грн.

		int rt_x=view2[rt_pos].cur_x;
		int rt_y=view2[rt_pos].cur_y;
		int rb_x=view2[rb_pos].cur_x;
		int rb_y=view2[rb_pos].cur_y;
		int lb_x=view2[lb_pos].cur_x;
		int lb_y=view2[lb_pos].cur_y;
		int lt_x=view2[lt_pos].cur_x;
		int lt_y=view2[lt_pos].cur_y;

		if(hex_field[rt_y][rt_x].scroll_block) return 1;
		if(hex_field[rb_y][rb_x].scroll_block) return 1;
		if(hex_field[lb_y][lb_x].scroll_block) return 1;
		if(hex_field[lt_y][lt_x].scroll_block) return 1;
*/
		int vpos=(VIEW_CY+hbegin+ymod)*v2w+VIEW_CX+wright+xmod;
		cnt_x=view2[vpos].cur_x;
		cnt_y=view2[vpos].cur_y;

		//if(cnt_x)

//WriteLog("право-верх	x=%d,y=%d\n",rt_x,rt_y);
//WriteLog("право-низ		x=%d,y=%d\n",rb_x,rb_y);
//WriteLog("лево-низ		x=%d,y=%d\n",lb_x,lb_y);
//WriteLog("лево-верх		x=%d,y=%d\n",lt_x,lt_y);
//WriteLog("центр			x=%d,y=%d\n",cnt_x,cnt_y);

	//!Cvet ------

		SetCenter2(cnt_x,cnt_y);
	} else RebuildTiles();

	return 1;
}

void CHexField::PreRestore()
{
	SAFEREL(lpVBpr_tile);
	SAFEREL(lpVBpr_roof); //!Cvet
}

void CHexField::PostRestore()
{
	SetCenter2(cnt_x,cnt_y);
}

void CHexField::SetCrit(int x,int y,CCritter* pcrit)
{
	hex_field[y][x].lpcrit=pcrit;
	SETFLAG(hex_field[y][x].flags,FT_PLAYER);

//	SpriteInfo* ii=lpSM->GetSpriteInfo(hex_field[y][x].lpcrit->cur_id);
	WriteLog("x=%d,y=%d\n",x,y);

	if(!IsVisible(x, y, hex_field[y][x].lpcrit->cur_id))
	{
WriteLog("Added not visible!\n");
		hex_field[y][x].lpcrit->SetVisible(0);
		pcrit->rit=NULL;
		return;
	}
WriteLog("Added visible!\n");

	hex_field[y][x].lpcrit->SetVisible(1);

	PrepSprite* prep=new PrepSprite(hex_field[y][x].scr_x+16,hex_field[y][x].scr_y+6,0,&hex_field[y][x].lpcrit->cur_id,&hex_field[y][x].lpcrit->cur_ox,&hex_field[y][x].lpcrit->cur_oy,&hex_field[y][x].lpcrit->alpha);
	pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[y][x].pos+DRAW_ORDER_CRIT, prep));

//	pcrit->hex_x=x; //!Cvet вынес в AddCritter // Текущие координаты Дюда 
//	pcrit->hex_y=y;
	lpSM->GetDrawCntrRect(prep, &hex_field[y][x].lpcrit->drect);
}

void CHexField::RemoveCrit(CCritter* pcrit)
{
	//hex_field[pcrit->ry][pcrit->rx].lpcrit=NULL;
	hex_field[pcrit->hex_y][pcrit->hex_x].lpcrit=NULL;
	UNSETFLAG(hex_field[pcrit->hex_y][pcrit->hex_x].flags,FT_PLAYER); //!Cvet

	if(pcrit->rit!=NULL) 
	{ //!Cvet исправил. был баг
		dtree_map::iterator it=dtree.find((*pcrit->rit).first);

		if(it!=dtree.end())
		{

	WriteLog("R1=");
			SAFEDEL((*it).second);
	WriteLog("R2...");
			dtree.erase(it);
		}

		pcrit->rit=NULL;

	}
    // отладка сетевых сообщений
    WriteLog("CritRemoved...\n");
}

//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*void CHexField::DrawTiles()
{

}

void CHexField::DrawTiles()
{

}
*/
void CHexField::TransitCritter(CCritter* pcrit, int dir, int x, int y, bool null_offs)
{
	if(null_offs==true) SetMoveOffs(pcrit,-1);

	if(hex_field[y][x].lpcrit!=NULL) return;

	UNSETFLAG(hex_field[pcrit->hex_y][pcrit->hex_x].flags,FT_PLAYER);
	SETFLAG(hex_field[y][x].flags,FT_PLAYER);

	hex_field[y][x].lpcrit=pcrit;
	hex_field[pcrit->hex_y][pcrit->hex_x].lpcrit=NULL;

	int old_x=pcrit->hex_x;
	int old_y=pcrit->hex_y;

	pcrit->hex_x=x;
	pcrit->hex_y=y;
	pcrit->cur_dir=dir;

  // FIXME[24.11.2012 alex]: by commenting these lines out I might
  //   have caused memory leakage.
  
	/*if(pcrit->rit!=NULL) 
	{ //!Cvet исправил. был баг
		dtree_map::iterator it=dtree.find((*pcrit->rit).first);

		if(it!=dtree.end())
		{
	WriteLog("R1=");
			SAFEDEL((*it).second);
	WriteLog("R2...");
			dtree.erase(it);
		}

		pcrit->rit=NULL;
	}*/
		
	if(!IsVisible(x, y, hex_field[y][x].lpcrit->cur_id))
	{
		pcrit->SetVisible(0);
		return;
	}

	pcrit->SetVisible(1);

	PrepSprite* prep=new PrepSprite(hex_field[y][x].scr_x+16,hex_field[y][x].scr_y+6,0,&hex_field[y][x].lpcrit->cur_id,&hex_field[y][x].lpcrit->cur_ox,&hex_field[y][x].lpcrit->cur_oy,&hex_field[y][x].lpcrit->alpha);
	if(null_offs==true)
		pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[y][x].pos+DRAW_ORDER_CRIT, prep));
	else
		if((old_x%2) && dir==0)
			pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[y][x].pos+11+DRAW_ORDER_CRIT, prep));
		else if(!(old_x%2) && dir==3)
			pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[old_y][old_x].pos+12+DRAW_ORDER_CRIT, prep));
		else if((old_x%2) && dir==2)
			pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[old_y][old_x].pos+DRAW_ORDER_CRIT+1, prep));
		else if(!(old_x%2) && dir==5)
			pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[old_y][old_x].pos+DRAW_ORDER_CRIT+2, prep));
		else
			pcrit->rit=dtree.insert(dtree_map::value_type(hex_field[y][x].pos+DRAW_ORDER_CRIT, prep));

	lpSM->GetDrawCntrRect(prep, &pcrit->drect);
}

void CHexField::MoveCritter(CCritter* pcrit, int move_dir, int new_x, int new_y)
{
	if(new_x<0 || new_x>=MAXTILEX || new_y<0 || new_y>=MAXTILEY) return;

	if(move_dir<0 || move_dir>5) move_dir=GetDir(pcrit->hex_x,pcrit->hex_y,new_x,new_y);

	SetMoveOffs(pcrit,move_dir);

	TransitCritter(pcrit,move_dir,new_x,new_y,false);
}

void CHexField::MoveCritter(CCritter* pcrit, int new_x, int new_y)
{
	if(new_x<0 || new_x>=MAXTILEX || new_y<0 || new_x>=MAXTILEY) return;

	int move_dir=GetDir(pcrit->hex_x,pcrit->hex_y,new_x,new_y);

	SetMoveOffs(pcrit,move_dir);

	TransitCritter(pcrit,move_dir,new_x,new_y,false);
}

void CHexField::MoveCritter(CCritter* pcrit, int move_dir)
{
	if(move_dir<0 || move_dir>5) return;

	int cur_x=pcrit->hex_x;
	int cur_y=pcrit->hex_y;

	switch(move_dir)
	{
	case 0:
		cur_x--;
		if(!(cur_x%2)) cur_y--;
		break;
	case 1:
		cur_x--;
		if(cur_x%2) cur_y++;
		break;
	case 2:
		cur_y++;
		break;
	case 3:
		cur_x++;
		if(cur_x%2) cur_y++;
		break;
	case 4:
		cur_x++;
		if(!(cur_x%2)) cur_y--;
		break;
	case 5:
		cur_y--;
		break;
	default:
		return;
	}

	SetMoveOffs(pcrit,move_dir);

	TransitCritter(pcrit,move_dir,cur_x,cur_y,false);
}

int CHexField::GetDir(int old_x, int old_y, int new_x, int new_y)
{
	int dir=0;

	if((old_x%2))
	{
		if(old_x> new_x && old_y> new_y) dir=0;
		if(old_x> new_x && old_y==new_y) dir=1;
		if(old_x==new_x && old_y< new_y) dir=2;
		if(old_x< new_x && old_y==new_y) dir=3;
		if(old_x< new_x && old_y> new_y) dir=4;
		if(old_x==new_x && old_y> new_y) dir=5;
	}
	else
	{
		if(old_x> new_x && old_y==new_y) dir=0;
		if(old_x> new_x && old_y< new_y) dir=1;
		if(old_x==new_x && old_y< new_y) dir=2;
		if(old_x< new_x && old_y< new_y) dir=3;
		if(old_x< new_x && old_y==new_y) dir=4;
		if(old_x==new_x && old_y> new_y) dir=5;
	}

	return dir;
}

int CHexField::GetFarDir(int old_x, int old_y, int new_x, int new_y)
{
	int dir=0;

	if(old_x> new_x && old_y> new_y) dir=0;
	if(old_x> new_x && old_y==new_y) dir=1;
	if(old_x> new_x && old_y< new_y) dir=2;

	//if(old_x< new_x && old_y==new_y) dir=3;
	if(old_x< new_x && old_y==new_y) dir=4;
	//if(old_x< new_x && old_y> new_y) dir=4;
	if(old_x< new_x && old_y> new_y) dir=5;
	if(old_x< new_x && old_y< new_y) dir=3;

	if(old_x==new_x && old_y< new_y) dir=2;
	if(old_x==new_x && old_y> new_y) dir=5;

	return dir;
}

void CHexField::SetMoveOffs(CCritter* pcrit,int dir)
{
	switch(dir)
	{
	case 0:
		pcrit->cur_ox=-16;
		pcrit->cur_oy=16;
		break;
	case 1:
		pcrit->cur_ox=-32;
		pcrit->cur_oy=0;
		break;
	case 2:
		pcrit->cur_ox=-16;
		pcrit->cur_oy=-16;
		break;
	case 3:
		pcrit->cur_ox=16;
		pcrit->cur_oy=-16;
		break;
	case 4:
		pcrit->cur_ox=32;
		pcrit->cur_oy=0;
		break;
	case 5:
		pcrit->cur_ox=16;
		pcrit->cur_oy=16;
		break;
	default:
		pcrit->cur_ox=0;
		pcrit->cur_oy=0;
		break;
	}
}

int CHexField::UnLoadMap()
{
	if(MapLoaded==FALSE) return 1;

	SAFEREL(lpVBpr_tile);
	SAFEREL(lpVBpr_roof);
	SAFEDELA(view2);

	lpVBpr_tile=NULL;
	lpVBpr_roof=NULL;
	view2=NULL;

	hbegin=0;
	hend=0;
	wright=0;
	wleft=0;

	v2h=0;
	v2w=0;
	v2c_x=0;
	v2c_y=0;

	cnt_x=0;
	cnt_y=0;

	all_obj.clear();
/*
	loaded_item.clear();
	loaded_scen.clear();
	loaded_wall.clear();
	loaded_tile.clear();
*/
	for(dtree_map::iterator jt=dtree.begin();jt!=dtree.end();jt++)
		SAFEDEL((*jt).second);
	dtree.clear();

	for(dtree_map::iterator jt=dtree_roof_rain.begin();jt!=dtree_roof_rain.end();jt++)
		SAFEDEL((*jt).second);
	dtree_roof_rain.clear();

	for(Rain::iterator it_rain=rain.begin();it_rain!=rain.end();it_rain++)
		SAFEDEL((*it_rain));
	rain.clear();

	for(int x=0;x<MAXTILEX;x++)
		for(int y=0;y<MAXTILEY;y++)
		{
			for(scen_vect::iterator si=hex_field[y][x].sc_obj.begin();si!=hex_field[y][x].sc_obj.end();si++)
				delete (*si);
			hex_field[y][x].sc_obj.clear();

			for(item_vect::iterator ti=hex_field[y][x].itm_obj.begin();ti!=hex_field[y][x].itm_obj.end();ti++)
				delete (*ti);
			hex_field[y][x].itm_obj.clear();

	//		for(misc_vect::iterator mi=hex_field[y][x].msc_obj.begin();mi!=hex_field[y][x].msc_obj.end();mi++)
	//			delete (*mi);
	//		hex_field[y][x].msc_obj.clear();

			hex_field[y][x].lpcrit=NULL;

			hex_field[y][x].scr_x;
			hex_field[y][x].scr_y;
			hex_field[y][x].tile_id=0;
			hex_field[y][x].roof_id=0;
			hex_field[y][x].to_draw=0;
			hex_field[y][x].flags=0;
			hex_field[y][x].scroll_block=FALSE;
			for(int i=0;i<MAX_WALL_CNT;i++) hex_field[y][x].wall_id[i]=0;
		}

	MapLoaded=FALSE;

	WriteLog("UnloadMap...OK\n");

	return 1;
}

int CHexField::GetTilePixel(int pix_x, int pix_y, HexTYPE* tile_x, HexTYPE* tile_y)
{
	int y2=0;
	int vpos=0;

	for(int ty=0;ty<v2h;ty++)
	{
		for(int tx=0;tx<v2w;tx++)
		{
			vpos=y2+tx;

			int hx=view2[vpos].scr_x;
			int hy=view2[vpos].scr_y;

			if(pix_x<=hx) continue;
			if(pix_x>hx+32) continue;
			if(pix_y<=hy) continue;
			if(pix_y>hy+16) continue;

			*tile_x=view2[vpos].cur_x;
			*tile_y=view2[vpos].cur_y;

			return 1;
		}
		y2+=v2w;
	}

	return 0;
}

ItemObj* CHexField::GetItemPixel(int pix_x, int pix_y)
{
	LONGLONG res=0;
	SpriteInfo* sprinf=NULL;

	for(item_vect::iterator it_i=all_obj.begin();it_i!=all_obj.end();it_i++)
	{
		sprinf=lpSM->GetSpriteInfo((*it_i)->spr_id);
		if(!sprinf) continue;

		if(pix_x>=(*(*it_i)->hex_scr_x+(*it_i)->scr_x+sprinf->offs_x+16+cmn_scr_ox-sprinf->w/2))
			if(pix_x<=(*(*it_i)->hex_scr_x+(*it_i)->scr_x+sprinf->offs_x+16+cmn_scr_ox+sprinf->w/2))
				if(pix_y>=(*(*it_i)->hex_scr_y+(*it_i)->scr_y+sprinf->offs_y+6+cmn_scr_oy-sprinf->h))
					if(pix_y<=(*(*it_i)->hex_scr_y+(*it_i)->scr_y+sprinf->offs_y+6+cmn_scr_oy))
					{
						return (*it_i);
					}
	}

	return NULL;
}

int CHexField::FindStep(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, uint8_t* steps)
{
//	LONGLONG fp; тест скорости
//	LONGLONG fp2;
//	QueryPerformanceCounter((PLARGE_INTEGER)&fp);

	int FindPath[MAXTILEX][MAXTILEY];
	int numindex=1; //текущий индекс
	bool indexing; // ставится флаг если хоть одна метка была нанесена
	int x1=0;
	int y1=0;
	int tx1=start_x;
	int ty1=start_y;
	int tx2=end_x;
	int ty2=end_y;
	int cur_step=0;

	//проверяем границы
	if(tx1<0 || ty1>=MAXTILEX || tx2<0 || ty2>=MAXTILEY) return FP_ERROR;

	if(tx1==tx2 && ty1==ty2) return FP_ALREADY_HERE;

	if(FLAG(hex_field[ty2][tx2].flags,FT_NOWAY)) return FP_DEADLOCK;

	int bx=start_x;
	int by=start_y;
	int ex=start_x;
	int ey=start_y;
	if((bx-=FINDPATH_MAX_PATH)<1) bx=1;
	if((by-=FINDPATH_MAX_PATH)<1) by=1;
	if((ex+=FINDPATH_MAX_PATH)>=MAXTILEX-1) ex=MAXTILEX-2;
	if((ey+=FINDPATH_MAX_PATH)>=MAXTILEY-1) ey=MAXTILEY-2;

	//очищаем массив для путя 
	for(x1=bx-1;x1<ex+1;x1++)
		for(y1=by-1;y1<ey+1;y1++) FindPath[x1][y1]=0;
	//на том месте где стоит чел индекс будет 1, от него и будем отталкиваться
	FindPath[tx1][ty1]=numindex;
	//Находим путь - наченаем раз за разом проходить массив, обозначать каждый шаг
	while(true)
	{
		//обозначаем индекс, что б узнать поменяется ли он в конце цикла
		indexing=false;
		//прочесываем карту индексов ходов
		for(x1=bx;x1<ex;x1++)
		{
			for(y1=by;y1<ey;y1++)
			{
				if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //1)нашли пункт назначения - вылазим

				if(FindPath[x1][y1]==numindex)
				{
				//а теперь метим во всех направлениях в которых дозволено
					if(!FLAG(hex_field[y1-1][x1].flags,FT_NOWAY) && !FindPath[x1][y1-1]) { FindPath[x1][y1-1]=numindex+1; indexing=true; }
					if(!FLAG(hex_field[y1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1]) { FindPath[x1+1][y1]=numindex+1; indexing=true; }
					if(!FLAG(hex_field[y1+1][x1].flags,FT_NOWAY) && !FindPath[x1][y1+1]) { FindPath[x1][y1+1]=numindex+1; indexing=true; }
					if(!FLAG(hex_field[y1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1]) { FindPath[x1-1][y1]=numindex+1; indexing=true; }
				//здесь идет поправка на чет-нечет по оси Х
					if((x1%2)) //нечет
					{
						if(!FLAG(hex_field[y1-1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1-1]) { FindPath[x1-1][y1-1]=numindex+1; indexing=true; }
						if(!FLAG(hex_field[y1-1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1-1]) { FindPath[x1+1][y1-1]=numindex+1; indexing=true; }
					}
					else //чет
					{
						if(!FLAG(hex_field[y1+1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1+1]) { FindPath[x1+1][y1+1]=numindex+1; indexing=true; }
						if(!FLAG(hex_field[y1+1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1+1]) { FindPath[x1-1][y1+1]=numindex+1; indexing=true; }
					}
				}
			}
			if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //2)продолжаем вылазить
		}
		if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //3)и последний раз
		//обозначаем индекс, что б узнать поменяется ли он в конце цикла
		if(indexing) numindex++;
		else return FP_DEADLOCK; //индекс не увеличился - значит тупик

		if(numindex >= FINDPATH_MAX_PATH) return FP_TOOFAR;
	}

	//подготавливаем массив для шагов
	if(steps) for(int i=0;i<FINDPATH_MAX_PATH;i++) steps[i]=0xFF;

	//Идем с конца в начало
	bool switcher=false;
	while(numindex>1)
	{
		switcher=!switcher;

		numindex--;

		//чет-нечет
		if(switcher==true)
		{
			if(FindPath[x1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1,y1-1,x1,y1); y1--; continue; }
			if(FindPath[x1+1][y1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1,x1,y1); x1++; continue; }
			if(FindPath[x1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1,y1+1,x1,y1); y1++; continue; }
			if(FindPath[x1-1][y1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1,x1,y1); x1--; continue; }

			if((x1%2)) //нечет
			{
				if(FindPath[x1-1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1-1,x1,y1); x1--; y1--; continue; }
				if(FindPath[x1+1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1-1,x1,y1); x1++; y1--; continue; }
			}
			else //чет
			{
				if(FindPath[x1+1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1+1,x1,y1); x1++; y1++; continue; }
				if(FindPath[x1-1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1+1,x1,y1); x1--; y1++; continue; }
			}
		}
		else
		{
			if((x1%2)) //нечет
			{
				if(FindPath[x1-1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1-1,x1,y1); x1--; y1--; continue; }
				if(FindPath[x1+1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1-1,x1,y1); x1++; y1--; continue; }
			}
			else //чет
			{
				if(FindPath[x1+1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1+1,x1,y1); x1++; y1++; continue; }
				if(FindPath[x1-1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1+1,x1,y1); x1--; y1++; continue; }
			}

			if(FindPath[x1][y1-1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1,y1-1,x1,y1); y1--; continue; }
			if(FindPath[x1+1][y1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1+1,y1,x1,y1); x1++; continue; }
			if(FindPath[x1][y1+1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1,y1+1,x1,y1); y1++; continue; }
			if(FindPath[x1-1][y1]==numindex) { if(steps) steps[numindex-1]=GetDir(x1-1,y1,x1,y1); x1--; continue; }
		}

		return FP_ERROR;
	}

	//если я стою в четной 6 позиций
/*	if(!(tx1%2))
	{
		x1=x1-tx1;
		y1=y1-ty1;
		if(x1==-1 && y1== 0) steps[0]=0;
		if(x1==-1 && y1==+1) steps[0]=1;
		if(x1== 0 && y1==+1) steps[0]=2;
		if(x1==+1 && y1==+1) steps[0]=3;
		if(x1==+1 && y1== 0) steps[0]=4;
		if(x1== 0 && y1==-1) steps[0]=5;
	}
	//если я стаю в нечетно 6 позиций
	else
	{
		x1=x1-tx1;
		y1=y1-ty1;
		if(x1==-1 && y1==-1) steps[0]=0;
		if(x1==-1 && y1== 0) steps[0]=1;
		if(x1== 0 && y1==+1) steps[0]=2;
		if(x1==+1 && y1== 0) steps[0]=3;
		if(x1==+1 && y1==-1) steps[0]=4;
		if(x1== 0 && y1==-1) steps[0]=5;
	}
*/
//	QueryPerformanceCounter((PLARGE_INTEGER)&fp2);
//	WriteLog("================================fp=%d\n",(fp2-fp)/1000);
	return FP_OK;
}

int CHexField::CutPath(HexTYPE start_x, HexTYPE start_y, HexTYPE* end_x, HexTYPE* end_y, uint8_t count_correct)
{
	int FindPath[MAXTILEX][MAXTILEY];
	int numindex=1; //текущий индекс
	bool indexing; // ставится флаг если хоть одна метка была нанесена
	int x1=0;
	int y1=0;
	int tx1=start_x;
	int ty1=start_y;
	int tx2=*end_x;
	int ty2=*end_y;

	//проверяем границы
	if(tx1<0 || ty1>=MAXTILEX || tx2<0 || ty2>=MAXTILEY) return FP_ERROR;

	if(tx1==tx2 && ty1==ty2) return 0;

	int bx=start_x;
	int by=start_y;
	int ex=start_x;
	int ey=start_y;
	if((bx-=FINDPATH_MAX_PATH)<1) bx=1;
	if((by-=FINDPATH_MAX_PATH)<1) by=1;
	if((ex+=FINDPATH_MAX_PATH)>=MAXTILEX-1) ex=MAXTILEX-2;
	if((ey+=FINDPATH_MAX_PATH)>=MAXTILEY-1) ey=MAXTILEY-2;

	//очищаем массив для путя 
	for(x1=bx-1;x1<ex+1;x1++)
		for(y1=by-1;y1<ey+1;y1++) FindPath[x1][y1]=0;
	//на том месте где стоит чел индекс будет 1, от него и будем отталкиваться
	FindPath[tx1][ty1]=numindex;
	//Находим путь - наченаем раз за разом проходить массив, обозначать каждый шаг
	while(true)
	{
		//обозначаем индекс, что б узнать поменяется ли он в конце цикла
		indexing=false;
		//прочесываем карту индексов ходов
		for(x1=bx;x1<ex;x1++)
		{
			for(y1=by;y1<ey;y1++)
			{
				if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //1)нашли пункт назначения - вылазим

				if(FindPath[x1][y1]==numindex)
				{
				//а теперь метим во всех направлениях в которых дозволено
					if(x1  ==tx2 && y1-1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
					if(!FLAG(hex_field[y1-1][x1].flags,FT_NOWAY) && !FindPath[x1][y1-1]) { FindPath[x1][y1-1]=numindex+1; indexing=true; }
					if(x1+1==tx2 && y1  ==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
					if(!FLAG(hex_field[y1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1]) { FindPath[x1+1][y1]=numindex+1; indexing=true; }
					if(x1  ==tx2 && y1+1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
					if(!FLAG(hex_field[y1+1][x1].flags,FT_NOWAY) && !FindPath[x1][y1+1]) { FindPath[x1][y1+1]=numindex+1; indexing=true; }
					if(x1-1==tx2 && y1  ==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
					if(!FLAG(hex_field[y1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1]) { FindPath[x1-1][y1]=numindex+1; indexing=true; }
				//здесь идет поправка на чет-нечет по оси Х
					if(x1%2) //нечет
					{
						if(x1-1==tx2 && y1-1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
						if(!FLAG(hex_field[y1-1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1-1]) { FindPath[x1-1][y1-1]=numindex+1; indexing=true; }
						if(x1+1==tx2 && y1-1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
						if(!FLAG(hex_field[y1-1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1-1]) { FindPath[x1+1][y1-1]=numindex+1; indexing=true; }
					}
					else //чет
					{
						if(x1+1==tx2 && y1+1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
						if(!FLAG(hex_field[y1+1][x1+1].flags,FT_NOWAY) && !FindPath[x1+1][y1+1]) { FindPath[x1+1][y1+1]=numindex+1; indexing=true; }
						if(x1-1==tx2 && y1+1==ty2) { x1=tx2; y1=ty2; FindPath[x1][y1]=++numindex; break; }
						if(!FLAG(hex_field[y1+1][x1-1].flags,FT_NOWAY) && !FindPath[x1-1][y1+1]) { FindPath[x1-1][y1+1]=numindex+1; indexing=true; }
					}
				}
			}
			if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //2)продолжаем вылазить
		}
		if(x1==tx2 && y1==ty2 && FindPath[x1][y1]==numindex) break; //3)и последний раз
		//обозначаем индекс, что б узнать поменяется ли он в конце цикла
		if(indexing) numindex++;
		else return 0; //индекс не увеличился - значит тупик

		if(numindex >= FINDPATH_MAX_PATH) return 0;
	}

	bool switcher=false;
	while(numindex>1 && count_correct)
	{
		switcher=!switcher;

		numindex--;
		count_correct--;

		//чет-нечет
		if(switcher==true)
		{
			if(FindPath[x1][y1-1]==numindex) { y1--; continue; }
			if(FindPath[x1+1][y1]==numindex) { x1++; continue; }
			if(FindPath[x1][y1+1]==numindex) { y1++; continue; }
			if(FindPath[x1-1][y1]==numindex) { x1--; continue; }

			if((x1%2)) //нечет
			{
				if(FindPath[x1-1][y1-1]==numindex) { x1--; y1--; continue; }
				if(FindPath[x1+1][y1-1]==numindex) { x1++; y1--; continue; }
			}
			else //чет
			{
				if(FindPath[x1+1][y1+1]==numindex) { x1++; y1++; continue; }
				if(FindPath[x1-1][y1+1]==numindex) { x1--; y1++; continue; }
			}
		}
		else
		{
			if((x1%2)) //нечет
			{
				if(FindPath[x1-1][y1-1]==numindex) { x1--; y1--; continue; }
				if(FindPath[x1+1][y1-1]==numindex) { x1++; y1--; continue; }
			}
			else //чет
			{
				if(FindPath[x1+1][y1+1]==numindex) { x1++; y1++; continue; }
				if(FindPath[x1-1][y1+1]==numindex) { x1--; y1++; continue; }
			}

			if(FindPath[x1][y1-1]==numindex) { y1--; continue; }
			if(FindPath[x1+1][y1]==numindex) { x1++; continue; }
			if(FindPath[x1][y1+1]==numindex) { y1++; continue; }
			if(FindPath[x1-1][y1]==numindex) { x1--; continue; }
		}

		return 0;
	}

	*end_x=x1;
	*end_y=y1;

	return 1;
}

int CHexField::FindTarget(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, uint8_t max_weapon_distance)
{
	if(ShowTrack)
		for(int tx=0;tx<MAXTILEX;tx++)
			for(int ty=0;ty<MAXTILEY;ty++)
				hex_field[ty][tx].track=0;

	if(start_x==end_x && start_y==end_y) return FINDTARGET_INVALID_TARG;

	if(max_weapon_distance==1)
	{
		int cur_step;
		char sx[6]={-1,-1,0,1,1, 0};
		char sy[6]={ 0, 1,1,1,0,-1};

		if(start_x%2)
		{
			sy[0]--;
			sy[1]--;
			sy[3]--;
			sy[4]--;
		}

		for(cur_step=0;cur_step<6;cur_step++)
			if(start_x+sx[cur_step]==end_x && start_y+sy[cur_step]==end_y) break;

		if(cur_step>5) return FINDTARGET_TOOFAR;

		return cur_step;
	}

  float dx = ::abs(end_x - start_x);
	float dy = ::abs(end_y - start_y);

	float sx1f=1.0f;
	float sy1f=1.0f;

	if(dx<dy)
		sx1f=dx/dy;
	else
		sy1f=dy/dx;

	if(end_x<start_x) sx1f*=-1;
	if(end_y<start_y) sy1f*=-1;

	float curx1f=(float)(start_x);
	float cury1f=(float)(start_y);

	int curx1i=start_x;
	int cury1i=start_y;

	int old_curx1i=curx1i;
	int old_cury1i=cury1i;

	bool right_barr=false;
	bool left_barr=false;

	int cur_dist=0;
	while(cur_dist<max_weapon_distance)
	{
		cur_dist++;

		curx1f+=sx1f;
		cury1f+=sy1f;

		old_curx1i=curx1i;
		old_cury1i=cury1i;

		curx1i=(int)(curx1f);
		if(curx1f-curx1i>=0.5f) curx1i++;
		cury1i=(int)(cury1f);
		if(cury1f-cury1i>=0.5f) cury1i++;

//WriteLog("dist:%d\n",cur_dist);
//WriteLog("x1:%d, y1:%d\n",curx1i,cury1i);

		if(!(old_curx1i%2))
		{
			if(old_curx1i-1==curx1i && old_cury1i-1==cury1i)
			{
				if(old_cury1i==end_y && old_curx1i-1==end_x)
					if(right_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i][old_curx1i-1].flags,FT_NOSHOOT)) //?
				{
					if(right_barr==true) return FINDTARGET_BARRIER;

					left_barr=true;
				}
				else
					if(left_barr==false) hex_field[old_cury1i][old_curx1i-1].track=2;

				if(old_cury1i-1==end_y && old_curx1i==end_x)
					if(left_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i-1][old_curx1i].flags,FT_NOSHOOT)) //?
				{
					if(left_barr==true) return FINDTARGET_BARRIER;

					right_barr=true;
				}
				else
					if(right_barr==false) hex_field[old_cury1i-1][old_curx1i].track=2;
			}
			
			if(old_curx1i+1==curx1i && old_cury1i-1==cury1i)
			{
				if(old_cury1i==end_y && old_curx1i+1==end_x)
					if(right_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i][old_curx1i+1].flags,FT_NOSHOOT)) //left
				{
					if(right_barr==true) return FINDTARGET_BARRIER;

					left_barr=true;
				}
				else
					if(left_barr==false) hex_field[old_cury1i][old_curx1i+1].track=2;

				if(old_cury1i-1==end_y && old_curx1i==end_x)
					if(left_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i-1][old_curx1i].flags,FT_NOSHOOT)) //right
				{
					if(left_barr==true) return FINDTARGET_BARRIER;

					right_barr=true;
				}
				else
					if(right_barr==false) hex_field[old_cury1i-1][old_curx1i].track=2;
			}
		}
		else
		{
			if(old_curx1i+1==curx1i && old_cury1i+1==cury1i)
			{
				if(old_cury1i==end_y && old_curx1i+1==end_x)
					if(right_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i][old_curx1i+1].flags,FT_NOSHOOT)) //?
				{
					if(right_barr==true) return FINDTARGET_BARRIER;

					left_barr=true;
				}
				else
					if(left_barr==false) hex_field[old_cury1i][old_curx1i+1].track=2;

				if(old_cury1i+1==end_y && old_curx1i==end_x)
					if(left_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i+1][old_curx1i].flags,FT_NOSHOOT)) //?
				{
					if(left_barr==true) return FINDTARGET_BARRIER;

					right_barr=true;
				}
				else
					if(right_barr==false) hex_field[old_cury1i+1][old_curx1i].track=2;
			}
			
			if(old_curx1i-1==curx1i && old_cury1i+1==cury1i)
			{
				if(old_cury1i==end_y && old_curx1i-1==end_x)
					if(right_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i][old_curx1i-1].flags,FT_NOSHOOT)) //left
				{
					if(right_barr==true) return FINDTARGET_BARRIER;

					left_barr=true;
				}
				else
					if(left_barr==false) hex_field[old_cury1i][old_curx1i-1].track=2;

				if(old_cury1i+1==end_y && old_curx1i==end_x)
					if(left_barr==true)
						return FINDTARGET_BARRIER;
					else
						return GetFarDir(start_x,start_y,end_x,end_y);

				if(FLAG(hex_field[old_cury1i+1][old_curx1i].flags,FT_NOSHOOT)) //right
				{
					if(left_barr==true) return FINDTARGET_BARRIER;

					right_barr=true;
				}
				else
					if(right_barr==false) hex_field[old_cury1i+1][old_curx1i].track=2;
			}
		}

		hex_field[cury1i][curx1i].track=1;

//WriteLog("x1:%d, y1:%d\n",curx1i,cury1i);

		if(curx1i==end_x && cury1i==end_y) return GetFarDir(start_x,start_y,end_x,end_y);

		if(FLAG(hex_field[cury1i][curx1i].flags,FT_NOSHOOT)) return FINDTARGET_BARRIER;
	}

	return FINDTARGET_TOOFAR;
}

void CHexField::FindSetCenter(int x, int y)
{
	int nx=x,ny=y;

	SetCenter2(x,y);

	for(int	i=0;i<47;++i) //up
		if(hex_field[ny-i][nx].scroll_block==TRUE)
		{
			ny+=50-i;
			break;
		}
	
	for(int i=0;i<25;++i) //right
		if(hex_field[ny][nx-i].scroll_block==TRUE)
		{
			nx+=25-i;
			break;
		}

	for(int i=0;i<45;++i) //up-right
		if(hex_field[ny-i/2][nx-i].scroll_block==TRUE)
		{
			nx+=45-i;
			ny+=(45-i)/2;
			break;
		}

	for(int i=0;i<40;++i) //down
		if(hex_field[ny+i][nx].scroll_block==TRUE)
		{
			ny-=40-i;
			break;
		}

	for(int i=0;i<30;++i) //left
		if(hex_field[ny][nx+i].scroll_block==TRUE)
		{
			ny-=30-i;
			break;
		}

	for(int i=0;i<45;++i) //down-left
		if(hex_field[ny+i/2][nx+i].scroll_block==TRUE)
		{
			nx-=45-i;
			ny-=(45-i)/2;
			break;
		}

	SetCenter2(nx,ny);
}

//!Cvet ---------------------------------------------------------------------------------