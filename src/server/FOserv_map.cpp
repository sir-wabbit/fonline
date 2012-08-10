/********************************************************************
	author:	Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"
#include "FOserv.h"

int CServer::RefreshZoneMasks()
{
	LogExecStr("Обновление карт зон...");
	int zx,zy;
	bool not_all=false;

	if(!fm.LoadFile("maps_westland.bmp",PT_MASKS)) not_all=true;
	else
	{
		for(zx=0;zx<GM_MAXZONEX;++zx)
			for(zy=0;zy<GM_MAXZONEY;++zy)
				if(GetZoneColor(&fm,zx,zy)==0xFF0000)
					SETFLAG(gm_zone[zx][zy].district,DISTRICT_WESTLAND);
				else
					UNSETFLAG(gm_zone[zx][zy].district,DISTRICT_WESTLAND);
	}

	if(!fm.LoadFile("maps_mountain.bmp",PT_MASKS)) not_all=true;
	else
	{
		for(zx=0;zx<GM_MAXZONEX;++zx)
			for(zy=0;zy<GM_MAXZONEY;++zy)
				if(GetZoneColor(&fm,zx,zy)==0xFF0000)
					SETFLAG(gm_zone[zx][zy].district,DISTRICT_MOUNTAINS);
				else
					UNSETFLAG(gm_zone[zx][zy].district,DISTRICT_MOUNTAINS);
	}

	if(!fm.LoadFile("maps_forest.bmp",PT_MASKS)) not_all=true;
	else
	{
		for(zx=0;zx<GM_MAXZONEX;++zx)
			for(zy=0;zy<GM_MAXZONEY;++zy)
				if(GetZoneColor(&fm,zx,zy)==0xFF0000)
					SETFLAG(gm_zone[zx][zy].district,DISTRICT_FOREST);
				else
					UNSETFLAG(gm_zone[zx][zy].district,DISTRICT_FOREST);
	}

	if(!fm.LoadFile("maps_ocean.bmp",PT_MASKS)) not_all=true;
	else
	{
		for(zx=0;zx<GM_MAXZONEX;++zx)
			for(zy=0;zy<GM_MAXZONEY;++zy)
				if(GetZoneColor(&fm,zx,zy)==0xFF0000)
					SETFLAG(gm_zone[zx][zy].district,DISTRICT_OCEAN);
				else
					UNSETFLAG(gm_zone[zx][zy].district,DISTRICT_OCEAN);
	}

	if(!fm.LoadFile("mobs_power.bmp",PT_MASKS)) not_all=true;
	else
	{
		for(zx=0;zx<GM_MAXZONEX;++zx)
			for(zy=0;zy<GM_MAXZONEY;++zy)
				switch(GetZoneColor(&fm,zx,zy))
				{
				case 0xFF: //2
					gm_zone[zx][zy].mobs_power=2;
					break;
				case 0xFF0000: //3
					gm_zone[zx][zy].mobs_power=3;
					break;
				case 0xFF00: //1
				default:
					gm_zone[zx][zy].mobs_power=1;
					break;
				}
	}

	fm.UnloadFile();

	if(not_all==true)
	{
		LogExecStr("прошло со сбоями\n");
		return 0;
	}

	LogExecStr("OK\n");
	return 1;
}

DWORD CServer::GetZoneColor(FileManager* fmngr, int zx, int zy)
{
	static const DWORD HBMP_LEN=54;
	static const DWORD LINE_LEN=(GM_MAXZONEX*3)+((GM_MAXZONEX*3)%4);

	fmngr->SetCurPos(HBMP_LEN+(zx*3)+(GM_MAXZONEY-1-zy)*LINE_LEN);

	return fmngr->GetRDWord();
}

int CServer::LoadAllMaps()
{
	if(!RefreshZoneMasks()) return 0;

	LogExecStr("Загрузка карт\n");

	FILE *cf;

	if(!(cf=fopen("maps\\data_maps.txt","rt")))
	{
		LogExecStr("\tФайл data_maps не найден\n");
		return 0;
	}

	LogExecStr("\tГлобальная карта...");

	char ch;

	WORD tmp_wrd;
	char tmp_str[32];
	int tmp_int=0,city_num=0,encaunter_num=0;
	
	while(!feof(cf))
	{
		fscanf(cf,"%c",&ch);

		if(ch=='@')
		{
			fscanf(cf,"%d",&city_num); //citynum
			if(city_num<0 || city_num>=MAX_CITIES) { LogExecStr("error - неверный номер города\n"); return 0; }
			city[city_num].num=city_num;

			fscanf(cf,"%d",&tmp_int); //x
			if(tmp_int<0 || tmp_int>=GM_MAXX) { LogExecStr("error - неверная координата Х города\n"); return 0; }
			city[city_num].wx=tmp_int;

			fscanf(cf,"%d",&tmp_int); //y
			if(tmp_int<0 || tmp_int>=GM_MAXY) { LogExecStr("error - неверная координата Y города\n"); return 0; }
			city[city_num].wy=tmp_int;

			fscanf(cf,"%d",&tmp_int); //radius
			if(tmp_int<=0 || tmp_int>=GM_ZONE_LEN) { LogExecStr("error - неверный радиус города\n"); return 0; }
			city[city_num].radius=tmp_int;

			while(!feof(cf))
			{
				fscanf(cf,"%c",&ch);
				if(ch=='@') break;
				if(ch!='#') continue;

				fscanf(cf,"%d%s",&tmp_wrd,&tmp_str);
				if(tmp_wrd==0 || tmp_wrd>=MAX_MAPS) { LogExecStr("error - данные о локальной карте неверны. номер карты.\n"); return 0; }
				map[tmp_wrd].num=tmp_wrd;
				map_info* cur_map=&map[tmp_wrd];

				map_str.insert(map_str_map::value_type(tmp_wrd,string(tmp_str)));

				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=MAXHEXX) { LogExecStr("error - данные о локальной карте неверны. старт Х.\n"); return 0; }
				cur_map->start_hex_x=tmp_wrd;

				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=MAXHEXY) { LogExecStr("error - данные о локальной карте неверны. старт Y.\n"); return 0; }
				cur_map->start_hex_y=tmp_wrd;

				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=6) { LogExecStr("error - данные о локальной карте неверны. старт Ori.\n"); return 0; }
				cur_map->start_ori=tmp_wrd;

				cur_map->city=&city[city_num];

				city[city_num].maps.push_back(cur_map);
			}
			if(feof(cf)) { LogExecStr("error - неожиданный конец файла\n"); return 0; }

			gm_zone[GM_ZONE(city[city_num].wx)][GM_ZONE(city[city_num].wy)].cities.push_back(&city[city_num]);
		}
		else if(ch=='%')
		{
			fscanf(cf,"%d",&encaunter_num); //encaunter num
			if(encaunter_num<0 || encaunter_num>=MAX_ENCAUNTERS)
				{ LogExecStr("error - неверный номер энкаунтера\n"); return 0; }
			encaunter[encaunter_num].num=encaunter_num;

			fscanf(cf,"%d",&tmp_wrd); //district
			if(tmp_wrd==0 || tmp_wrd>(DISTRICT_WESTLAND|DISTRICT_MOUNTAINS|DISTRICT_FOREST|DISTRICT_OCEAN))
				{ LogExecStr("error - неверная местность энкаунтера\n"); return 0; }
			encaunter[encaunter_num].district=tmp_wrd;

			fscanf(cf,"%d",&tmp_wrd); //max_groups
			if(tmp_wrd==0 || tmp_wrd>=ENCAUNTERS_MAX_GROUPS)
				{ LogExecStr("error - неверные данные по максимальныйм группам в энкаунтере.\n"); return 0; }
			encaunter[encaunter_num].max_groups=tmp_wrd;

			fscanf(cf,"%d%s",&tmp_wrd,&tmp_str);
				if(tmp_wrd==0 || tmp_wrd>=MAX_MAPS) { LogExecStr("error - данные о локальной карте неверны. номер карты.\n"); return 0; }

			map_str.insert(map_str_map::value_type(tmp_wrd,string(tmp_str)));
			map[tmp_wrd].num=tmp_wrd;
			map_info* cur_map=&map[tmp_wrd];
			cur_map->encaunter=&encaunter[encaunter_num];
			encaunter[encaunter_num].emap=cur_map;

			for(int i=0;i<encaunter[encaunter_num].max_groups;++i)
			{
				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=MAXHEXX) { LogExecStr("error - данные о энкаунтере неверны. старт Х.\n"); return 0; }
				encaunter[encaunter_num].start_hx[i]=tmp_wrd;

				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=MAXHEXY) { LogExecStr("error - данные о энкаунтере неверны. старт Y.\n"); return 0; }
				encaunter[encaunter_num].start_hy[i]=tmp_wrd;

				fscanf(cf,"%d",&tmp_wrd);
				if(tmp_wrd>=6) { LogExecStr("error - данные о энкаунтере неверны. старт Ori.\n"); return 0; }
				encaunter[encaunter_num].start_ori[i]=tmp_wrd;
			}

			while(!feof(cf))
			{
				fscanf(cf,"%c",&ch);
				if(ch=='%') break;
			}

			encaunters_free.insert(encaunter_num);

			if(ch!='%') { LogExecStr("error - неожиданный конец файла2\n"); return 0; }
		}
	}

	LogExecStr("ОК\n");
	LogExecStr("\tЛокальные:\n");

	char fnam[32];

	for(map_str_map::iterator it_m=map_str.begin();it_m!=map_str.end();it_m++)
	{
		strcpy(fnam,(*it_m).second.c_str());
		LogExecStr("\t%s...",fnam);

		if(fm.LoadFile(fnam,PT_MAPS))
		{
			
		}
		else
		{
			LogExecStr("FALSE\n");
			return 0;
		}

		LogExecStr("OK\n");
	}

	LogExecStr("Загрузка карт прошла успешно\n");

	/*
	transit_map[11][0x10006D]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x6D].flags,FT_TRANSIT);
	transit_map[11][0x10006E]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x6E].flags,FT_TRANSIT);
	transit_map[11][0x10006F]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x6F].flags,FT_TRANSIT);
	transit_map[11][0x100070]=0x0C01008C00057;
	SETFLAG(tile[11][0x10][0x70].flags,FT_TRANSIT);
	transit_map[11][0x100071]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x71].flags,FT_TRANSIT);
	transit_map[11][0x100072]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x72].flags,FT_TRANSIT);
	transit_map[11][0x100073]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x73].flags,FT_TRANSIT);
	transit_map[11][0x100074]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x74].flags,FT_TRANSIT);
	transit_map[11][0x100075]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x75].flags,FT_TRANSIT);
	transit_map[11][0x100076]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x76].flags,FT_TRANSIT);
	transit_map[11][0x100077]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x77].flags,FT_TRANSIT);
	transit_map[11][0x100078]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x78].flags,FT_TRANSIT);
	transit_map[11][0x100079]=0x0C01008C0057;
	SETFLAG(tile[11][0x10][0x79].flags,FT_TRANSIT);
*/

	transit_map[11][0x10006D]=0;
	SETFLAG(tile[11][0x10][0x6D].flags,FT_TRANSIT);
	transit_map[11][0x10006E]=0;
	SETFLAG(tile[11][0x10][0x6E].flags,FT_TRANSIT);
	transit_map[11][0x10006F]=0;
	SETFLAG(tile[11][0x10][0x6F].flags,FT_TRANSIT);
	transit_map[11][0x100070]=0;
	SETFLAG(tile[11][0x10][0x70].flags,FT_TRANSIT);
	transit_map[11][0x100071]=0;
	SETFLAG(tile[11][0x10][0x71].flags,FT_TRANSIT);
	transit_map[11][0x100072]=0;
	SETFLAG(tile[11][0x10][0x72].flags,FT_TRANSIT);
	transit_map[11][0x100073]=0;
	SETFLAG(tile[11][0x10][0x73].flags,FT_TRANSIT);
	transit_map[11][0x100074]=0;
	SETFLAG(tile[11][0x10][0x74].flags,FT_TRANSIT);
	transit_map[11][0x100075]=0;
	SETFLAG(tile[11][0x10][0x75].flags,FT_TRANSIT);
	transit_map[11][0x100076]=0;
	SETFLAG(tile[11][0x10][0x76].flags,FT_TRANSIT);
	transit_map[11][0x100077]=0;
	SETFLAG(tile[11][0x10][0x77].flags,FT_TRANSIT);
	transit_map[11][0x100078]=0;
	SETFLAG(tile[11][0x10][0x78].flags,FT_TRANSIT);
	transit_map[11][0x100079]=0;
	SETFLAG(tile[11][0x10][0x79].flags,FT_TRANSIT);

	transit_map[12][0x8E0054]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x54].flags,FT_TRANSIT);
	transit_map[12][0x8E0055]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x55].flags,FT_TRANSIT);
	transit_map[12][0x8E0056]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x56].flags,FT_TRANSIT);
	transit_map[12][0x8E0057]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x57].flags,FT_TRANSIT);
	transit_map[12][0x8E0058]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x58].flags,FT_TRANSIT);
	transit_map[12][0x8E0059]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x59].flags,FT_TRANSIT);
	transit_map[12][0x8E005A]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x5A].flags,FT_TRANSIT);
	transit_map[12][0x8E005B]=0x0B0300160073;
	SETFLAG(tile[12][0x8E][0x5B].flags,FT_TRANSIT);

	transit_map[21][0xA0007B]=0;
	SETFLAG(tile[21][0xA0][0x7B].flags,FT_TRANSIT);
	transit_map[21][0xA0007D]=0;
	SETFLAG(tile[21][0xA0][0x7D].flags,FT_TRANSIT);
	transit_map[21][0xA0007E]=0;
	SETFLAG(tile[21][0xA0][0x7E].flags,FT_TRANSIT);
	transit_map[21][0xA0007F]=0;
	SETFLAG(tile[21][0xA0][0x7F].flags,FT_TRANSIT);
	transit_map[21][0xA00080]=0;
	SETFLAG(tile[21][0xA0][0x80].flags,FT_TRANSIT);
	transit_map[21][0xA00081]=0;
	SETFLAG(tile[21][0xA0][0x81].flags,FT_TRANSIT);
	transit_map[21][0xA00082]=0;
	SETFLAG(tile[21][0xA0][0x82].flags,FT_TRANSIT);
	transit_map[21][0xA00083]=0;
	SETFLAG(tile[21][0xA0][0x83].flags,FT_TRANSIT);

	proc_zone_x=0;
	proc_zone_y=0;

	return 1;
}

//==============================================================================================================================
//*****  LOCAL MAP  ************************************************************************************************************
//==============================================================================================================================

int CServer::TransitCr(CCritter* acl, int to_map, int to_x, int to_y, int to_ori)
{
	WORD old_map=acl->info.map;
	if(old_map==to_map) return TR_FALSE;

	HexTYPE old_x=acl->info.x;
	HexTYPE old_y=acl->info.y;

	if(AddCrToMap(acl,to_map,to_x,to_y)!=TR_OK) return TR_FALSE;

	EraseCrFromMap(acl,old_map,old_x,old_y);

	acl->info.ori=to_ori;

	if(FLAG(acl->info.flags,FCRIT_PLAYER)) sql.SaveDataPlayer(&acl->info);
	if(FLAG(acl->info.flags,FCRIT_NPC	)) sql.SaveDataNPC(&acl->info);

	return TR_OK;
}

int CServer::MoveToTile(CCritter* acl, HexTYPE mx, HexTYPE my)
{
	if(mx>=MAXHEXX || my>=MAXHEXY) return MR_FALSE;

	if(FLAG(tile[acl->info.map][mx][my].flags,FT_NOWAY)) return MR_FALSE;

	UNSETFLAG(tile[acl->info.map][acl->info.x][acl->info.y].flags,FT_PLAYER);

	if(FLAG(tile[acl->info.map][mx][my].flags,FT_TRANSIT))
	{
		LONGLONG find_transit=FindTransit(acl->info.map,mx,my);

		if(find_transit!=0xFFFFFFFFFFFFFFFF)
		{
			WORD new_map= (find_transit & 0xFFFF0000000000) >> 40;
			BYTE new_ori= (find_transit & 0x0000FF00000000) >> 32;
			HexTYPE new_x=(find_transit & 0x000000FFFF0000) >> 16;
			HexTYPE new_y=(find_transit & 0x0000000000FFFF);

			if(TransitCr(acl,new_map,new_x,new_y,new_ori)==TR_OK)
			{
				if(!new_map)
				{
					GM_GroupStartMove(acl);

					return MR_TOGLOBAL;
				}

				if(FLAG(acl->info.flags,FT_PLAYER)) Send_LoadMap(acl);

				return MR_TRANSIT;
			}
		}
	}

	acl->info.x=mx;
	acl->info.y=my;

	SETFLAG(tile[acl->info.map][mx][my].flags,FT_PLAYER);

	//LogExecStr("x=%d,y=%d\n",acl->info.x,acl->info.y);

	return MR_STEP;
}

int CServer::AddCrToMap(CCritter* acl, WORD tmap, HexTYPE tx, HexTYPE ty)
{
	if(!tmap)
	{
		cl_map::iterator it_cr=map_cr[tmap].find(acl->info.id);
		if(it_cr!=map_cr[tmap].end())
		{
			if(!acl->group_move)
			{
				return TR_FALSE;
		//		GM_GroupStartMove(acl);
			}
			else
			{
				CCritter* er_acl=(*it_cr).second;
				cl_map::iterator it_cr2=acl->group_move->crit_move.find(er_acl->info.id);
				if(it_cr2!=acl->group_move->crit_move.end())
				{
					acl->group_move->crit_move.erase(it_cr2);
					acl->group_move->crit_move.insert(cl_map::value_type(acl->info.id,acl));
				}

				if(er_acl->group_move==&er_acl->group)
				{
					int zone_x=GM_ZONE(acl->group_move->xi);
					int zone_y=GM_ZONE(acl->group_move->yi);
					for(gmap_group_vec::iterator it_gr=gm_zone[zone_x][zone_y].groups.begin();it_gr!=gm_zone[zone_x][zone_y].groups.end();++it_gr)
						if((*it_gr)==er_acl->group_move)
						{
							gm_zone[zone_x][zone_y].groups.erase(it_gr);
							gm_zone[zone_x][zone_y].groups.push_back(acl->group_move);
							break;
						}

					for(cl_map::iterator it_cr3=acl->group_move->crit_move.begin();it_cr3!=acl->group_move->crit_move.end();++it_cr3)
						(*it_cr3).second->group_move=acl->group_move;
				}
			}

LogExecStr(".1.");
			SAFEDEL((*it_cr).second);
LogExecStr(".2.");
			map_cr[tmap].erase(it_cr);
		}
		else
		{
			if(map[acl->info.map].city)
			{
				acl->info.world_x=map[acl->info.map].city->wx;
				acl->info.world_y=map[acl->info.map].city->wy;
			}
		}

		map_cr[tmap].insert(cl_map::value_type(acl->info.id,acl));
		acl->info.map=tmap;

		return TR_OK;
	}

	if(tx>=MAXHEXX || ty>=MAXHEXY) return TR_FALSE;

	cl_map::iterator it_cr=map_cr[tmap].find(acl->info.id);
	if(it_cr!=map_cr[tmap].end())
	{
		UNSETFLAG(tile[tmap][(*it_cr).second->info.x][(*it_cr).second->info.y].flags,FT_PLAYER);
		DelClFromAllVisVec((*it_cr).second,tmap);
		delete (*it_cr).second;
		map_cr[tmap].erase(it_cr);
	}

	bool find_place=false;

	if(!FLAG(tile[tmap][tx][ty].flags,FT_NOWAY)) find_place=true;

	if(find_place==false)
	{
		int i;
		char sx[6]={-1,-1,0,1,1, 0};
		char sy[6]={ 0, 1,1,1,0,-1};

		if(tx%2)
		{
			sy[0]--;
			sy[1]--;
			sy[3]--;
			sy[4]--;
		}

		for(i=0;i<6;i++)
		{
			if(tx+(sx[i])<0 || tx+(sx[i])>=MAXHEXX) continue;
			if(ty+(sy[i])<0 || ty+(sy[i])>=MAXHEXY) continue;

			if(!FLAG(tile[tmap][tx+(sx[i])][ty+(sy[i])].flags,FT_NOWAY))
			{
				find_place=true;
				break;
			}
		}

		if(find_place==true)
		{
			tx+=(sx[i]);
			ty+=(sy[i]);
		}
		else return TR_FALSE;
	}

	SETFLAG(tile[tmap][tx][ty].flags,FT_PLAYER);

	map_cr[tmap].insert(cl_map::value_type(acl->info.id,acl));

	acl->info.map=tmap;
	acl->info.x=tx;
	acl->info.y=ty;

	acl->vis_cl.clear();

	return TR_OK;
}

LONGLONG CServer::FindTransit(WORD num_map, HexTYPE num_x, HexTYPE num_y)
{
	LONGLONG find_transit=0;

	find_transit+=num_x << 16;
	find_transit+=num_y;

	longlong_map::iterator it=transit_map[num_map].find(find_transit);

	if(it==transit_map[num_map].end()) return 0xFFFFFFFFFF;

	if(!(*it).second) return (*it).second; //global

	if((((*it).second >> 40) & 0xFFFF)	>= MAX_MAPS) return 0xFFFFFFFFFFFFFFFF;
	if((((*it).second >> 32) & 0xFF)	> 5)		 return 0xFFFFFFFFFFFFFFFF;
	if((((*it).second >> 16) & 0xFFFF)	>= MAXHEXX) return 0xFFFFFFFFFFFFFFFF;
	if(((*it).second		 & 0xFFFF)	>= MAXHEXY) return 0xFFFFFFFFFFFFFFFF;

	return (*it).second; //local
}

void CServer::EraseCrFromMap(CCritter* acl, int num_map, int hex_x, int hex_y)
{
	cl_map::iterator it_cr=NULL;

	if(!num_map)
	{
		it_cr=map_cr[num_map].find(acl->info.id);
		if(it_cr!=map_cr[num_map].end()) map_cr[num_map].erase(it_cr);
		return;
	}

	it_cr=map_cr[num_map].find(acl->info.id);
	if(it_cr!=map_cr[num_map].end()) map_cr[num_map].erase(it_cr);

	UNSETFLAG(tile[num_map][hex_x][hex_y].flags,FT_PLAYER);

	DelClFromAllVisVec(acl,num_map);

	LMapGarbager(num_map);

//	acl->vis_cl.clear();
}

void CServer::LMapGarbager(WORD num_map)
{
	if(map[num_map].encaunter)
	{
	  cl_map::iterator it_cr;
		for(it_cr=map_cr[num_map].begin();it_cr!=map_cr[num_map].end();++it_cr)
			if(FLAG((*it_cr).second->info.flags,FCRIT_PLAYER) || FLAG((*it_cr).second->info.flags,FCRIT_NPC)) break;

		if(it_cr==map_cr[num_map].end())
		{
			//GM_EncaunterFree(map[num_map].encaunter);
			MOBs_EraseFromMap(num_map);
			if(!map_cr[num_map].empty()) LogExecStr("Ошибка - Локация не очистилась после энкаунтера!!!\n");

			map[num_map].encaunter->count_groups=0;
			encaunters_busy.erase(map[num_map].encaunter->num);
			encaunters_free.insert(map[num_map].encaunter->num);
		}
	}
}

void CServer::SetVisCr(CCritter* acl)
{
	int vis;
	int dist;

	acl->GenLook();

	CCritter* c=NULL;
	for(cl_map::iterator it=map_cr[acl->info.map].begin();it!=map_cr[acl->info.map].end();++it)
	{
		if((*it).second==acl) continue;

		c=(*it).second;

		if(!c->info.map)
		{
			LogExecStr("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			if(DelClFromVisVec(acl,c)) Send_RemoveCritter(c,acl->info.id);
			continue;
		}

		//dist=DISTANCE(acl,c);
		dist=DistFast(acl->info.x-c->info.x,acl->info.y-c->info.y);

	//для себя
		BREAK_BEGIN

			if(!FLAG(acl->info.flags,FCRIT_PLAYER)) break;

			if(c->info.pe[PE_HIDE_MODE])
			{
				vis=acl->info.look-(c->info.sk[SK_SNEAK]-acl->info.sk[SK_SNEAK]/4)/10;
				if(vis<8) vis=8;
				//if(vis<MIN_VISIBLE_CRIT) vis=MIN_VISIBLE_CRIT;
			}
			else
				vis=acl->info.look;

			if(vis>=dist)
			{
				if(AddClIntoVisVec(c,acl)) Send_AddCritter(acl,&c->info);
			}
			else
			{
				if(DelClFromVisVec(c,acl)) Send_RemoveCritter(acl,c->info.id);
			}

		BREAK_END;

	//для оппонента
		if(!FLAG(c->info.flags,FCRIT_PLAYER)) continue;

		c->GenLook();

		if(acl->info.pe[PE_HIDE_MODE])
		{
			vis=c->info.look-(acl->info.sk[SK_SNEAK]-c->info.sk[SK_SNEAK]/4)/10;
			if(vis<8) vis=8;
			//if(vis<MIN_VISIBLE_CRIT) vis=MIN_VISIBLE_CRIT;
		}
		else
			vis=c->info.look;

		if(vis>=dist)
		{
			if(AddClIntoVisVec(acl,c)) Send_AddCritter(c,&acl->info);
		}
		else
		{
			if(DelClFromVisVec(acl,c)) Send_RemoveCritter(c,acl->info.id);
		}
	}
}

int CServer::AddClIntoVisVec(CCritter* acl, CCritter* add_cl)
{
	if(acl->vis_cl.count(add_cl->info.id)) return 0;

	acl->vis_cl.insert(cl_map::value_type(add_cl->info.id,add_cl));

	return 1;
}

int CServer::DelClFromVisVec(CCritter* acl, CCritter* del_cl)
{
	if(!acl->vis_cl.count(del_cl->info.id)) return 0;

	acl->vis_cl.erase(del_cl->info.id);

	return 1;
}

int CServer::DelClFromAllVisVec(CCritter* del_cl, WORD num_map)
{
	for(cl_map::iterator it=map_cr[num_map].begin();it!=map_cr[num_map].end();it++)
	{
		if((*it).second==del_cl) continue;

		if(DelClFromVisVec((*it).second,del_cl))
			if(FLAG((*it).second->info.flags,FCRIT_PLAYER) && !FLAG((*it).second->info.flags,FCRIT_DISCONNECT))
				Send_RemoveCritter((*it).second,del_cl->info.id);
	}

	return 1;
}

int CServer::LMapGetCoords(BYTE dir_move, HexTYPE cur_hx, HexTYPE cur_hy, HexTYPE& to_hx, HexTYPE& to_hy)
{
	switch(dir_move)
	{
	case 0:
		cur_hx--;
		if(!(cur_hx%2)) cur_hy--;
		break;
	case 1:
		cur_hx--;
		if(cur_hx%2) cur_hy++;
		break;
	case 2:
		cur_hy++;
		break;
	case 3:
		cur_hx++;
		if(cur_hx%2) cur_hy++;
		break;
	case 4:
		cur_hx++;
		if(!(cur_hx%2)) cur_hy--;
		break;
	case 5:
		cur_hy--;
		break;
	default:
		return 0;
	}

	if(cur_hx>=MAXHEXX || cur_hy>=MAXHEXY) return 0;

	to_hx=cur_hx;
	to_hy=cur_hy;

	return 1;
}

//==============================================================================================================================
//*****  OBJECTS ON MAP  *******************************************************************************************************
//==============================================================================================================================

void CServer::InsertObjOnMap(dyn_obj* ins_obj, MapTYPE omap, HexTYPE ox, HexTYPE oy)
{
	if(!omap || omap>=MAX_MAPS || ox>=MAXHEXX || oy>=MAXHEXY)
	{
		ins_obj->ACC_HEX.map=0;
		ins_obj->ACC_HEX.x=0;
		ins_obj->ACC_HEX.y=0;
		return;
	}

	ins_obj->ACC_HEX.map=omap;
	ins_obj->ACC_HEX.x=ox;
	ins_obj->ACC_HEX.y=oy;

	DHexTYPE hex_ind;
	hex_ind=ins_obj->ACC_HEX.x<<16;
	hex_ind+=ins_obj->ACC_HEX.y;

	dyn_map* obj_cont;
	dyn_map_map::iterator it_tile_objects=objects_map[omap].find(hex_ind);
	if(it_tile_objects==objects_map[omap].end())
	{
		obj_cont=new dyn_map;
		objects_map[omap].insert(dyn_map_map::value_type(hex_ind,obj_cont));
	}
	else
		obj_cont=(*it_tile_objects).second;

	obj_cont->insert(dyn_map::value_type(ins_obj->id,ins_obj));

	switch(ins_obj->object->type)
	{
	case OBJ_TYPE_DOOR:
		SETFLAG(tile[omap][ox][oy].flags,FT_DOOR_CLOSE);
		break;
	case OBJ_TYPE_CONTAINER:
		break;
	default:
		SETFLAG(tile[omap][ox][oy].flags,FT_TILECONT);
		break;
	}
/*
	word_set* pics;
	word_set_map::iterator it_pics_cont=pic_on_ground[omap].find(hex_ind);
	if(it_pics_cont==pic_on_ground[omap].end())
	{
		word_set new_pics;
		pic_on_ground[omap].insert(word_set_map::value_type(hex_ind,new_pics));
		pics=&pic_on_ground[omap][hex_ind];
	}
	else
		pics=&(*it_pics_cont).second;

	if(!pics->count(ins_obj->object->p111[OBJ_PIC_MAP]))
		pics->insert(ins_obj->object->p111[OBJ_PIC_MAP]);
*/
}

void CServer::EraseObjFromMap(dyn_obj* ers_obj)
{
	DHexTYPE hex_ind;
	hex_ind=ers_obj->ACC_HEX.x<<16;
	hex_ind+=ers_obj->ACC_HEX.y;

	dyn_map_map::iterator it_tile_objects=objects_map[ers_obj->ACC_HEX.map].find(hex_ind);
	if(it_tile_objects==objects_map[ers_obj->ACC_HEX.map].end())
	{
		UNSETFLAG(tile[ers_obj->ACC_HEX.map][ers_obj->ACC_HEX.x][ers_obj->ACC_HEX.y].flags,FT_TILECONT);
		ers_obj->ACC_HEX.map=0;
		ers_obj->ACC_HEX.x=0;
		ers_obj->ACC_HEX.y=0;
		return;
	}
	dyn_map* tile_objects=(*it_tile_objects).second;

	tile_objects->erase(ers_obj->id);

	if(tile_objects->empty())
	{
		UNSETFLAG(tile[ers_obj->ACC_HEX.map][ers_obj->ACC_HEX.x][ers_obj->ACC_HEX.y].flags,FT_TILECONT);

		objects_map[ers_obj->ACC_HEX.map].erase(hex_ind);
		delete tile_objects;
	}

	ers_obj->ACC_HEX.map=0;
	ers_obj->ACC_HEX.x=0;
	ers_obj->ACC_HEX.y=0;
}
/*
void CServer::EraseObject(dyn_obj* ers_obj, dyn_map::iterator* it_obj)
{
//	delete (**it_obj).second;

//	tile[ers_obj->map][ers_obj->x][ers_obj->y].obj.erase((*it_obj));
}
*/

dyn_obj* CServer::GetDynObj(WORD id_sobj, MapTYPE num_map, HexTYPE num_x, HexTYPE num_y)
{
	DHexTYPE hex_ind;
	hex_ind=num_x<<16;
	hex_ind+=num_y;

	dyn_map_map::iterator it_tile_objects=objects_map[num_map].find(hex_ind);
	if(it_tile_objects==objects_map[num_map].end()) return NULL;

	dyn_map* tile_objects=(*it_tile_objects).second;

	dyn_map::iterator it_obj;
	for(it_obj=tile_objects->begin();it_obj!=tile_objects->end();it_obj++)
		if((*it_obj).second->object->id==id_sobj) break;

	if(it_obj==tile_objects->end()) return NULL;

	return (*it_obj).second;
}

//==============================================================================================================================
//*****  GLOBAL MAP  ***********************************************************************************************************
//==============================================================================================================================

void CServer::Process_GiveGlobalInfo(CCritter* acl)
{
	BYTE info_flags;
	acl->bin >> info_flags;
	Send_GlobalInfo(acl,info_flags);
}

void CServer::Process_RuleGlobal(CCritter* acl)
{
	BYTE command;
	DWORD param1;
	DWORD param2;

	acl->bin >> command;
	acl->bin >> param1;
	acl->bin >> param2;

//	if(!acl->group_move || acl->info.map)
//	{
//		SetCheat(acl,"Попытка управлять группой, когда ее нет или с локальной карты");
//		return;
//	}

	switch(command)
	{
	case GM_RULE_COMMAND_ADDPREPCRIT:
		{
			if(!param1) break;

			cl_map::iterator it_cr=cr.find(param1);
			if(it_cr==cr.end()) break;

			GM_GroupAddPrepCrit((*it_cr).second,acl->info.id);
		}
		break;
	case GM_RULE_COMMAND_DELPREPCRIT:
		GM_GroupDelPrepCrit(acl,param1);
		break;
	case GM_RULE_COMMAND_SETMOVE:
		GM_GroupSetMove(acl,param1,param2);
		break;
	case GM_RULE_COMMAND_STOP:
		GM_GroupStopMove(acl);
		break;
	case GM_RULE_COMMAND_TOLOCAL:
		if(!param1)
			GM_GroupToEncaunter(acl,NULL);
		else
			GM_GroupToCity(acl,param1,param2);
		break;
	case GM_RULE_COMMAND_SETSPEED:
		GM_GroupSetSpeed(acl,param1);
		break;
	default:
		SetCheat(acl,"Неизвестная команда упровлением группой");
		break;
	}
}

void CServer::GM_Process(int max_time)
{
	if(max_time<=0) return;

	DWORD cur_time=GetTickCount();
	int cur_zone=0;

	while(true)
	{
		int time=cur_time-gm_zone[proc_zone_x][proc_zone_y].last_tick;

		if(time>=GM_ZONE_TIME_PROC)
		{
			gmap_zone* cur_zone=&gm_zone[proc_zone_x][proc_zone_y];
			gmap_group* cur_group=NULL;

			for(gmap_group_vec::iterator it_g=cur_zone->groups.begin();it_g!=cur_zone->groups.end();++it_g)
			{
				cur_group=(*it_g);

				if(GM_GroupMove(cur_group,time)) cur_zone->groups.erase(it_g);
				else
				{
					//encaunter
					if(cur_time-cur_group->last_encaunter>=ENCAUNTERS_TIME)
					{
						LogExecStr("gen enc+\n");
						int num_encaunter=0;
						if(!(num_encaunter=GM_GroupToEncaunter(cur_group->rule,NULL))) cur_group->last_encaunter=GetTickCount();
						else
						{
							MOBs_AddToEncaunter(num_encaunter,0);
							//GM_GroupToEncaunter(cur_group->rule,NULL)
						}
						LogExecStr("gen enc-\n");
					}
				}

				if(it_g==cur_zone->groups.end()) break;
			}

			gm_zone[proc_zone_x][proc_zone_y].last_tick=cur_time;
		}

		proc_zone_x++;
		if(proc_zone_x>=GM_MAXZONEX)
		{
			proc_zone_x=0;
			proc_zone_y++;
			if(proc_zone_y>=GM_MAXZONEY) proc_zone_y=0;
		}

		cur_zone++;
		if(cur_zone>=GM_MAXZONEX*GM_MAXZONEY || GetTickCount()-cur_time>max_time)
		{
			//LogExecStr("cur_zone=%d,time=%d(max:%d)\n",cur_zone,GetTickCount()-cur_time,max_time);
			break;
		}
	}
}

int CServer::GM_GroupAddPrepCrit(CCritter* rule_acl, CrID id)
{
	if(rule_acl->info.id==id) return 0;

	if(rule_acl->group.prep_crit.count(id)) return 1;

	rule_acl->group.prep_crit.insert(id);

	//различные расчеты над параметрами группы !!!!
//SaveGroup
	return 1;
}

int CServer::GM_GroupDelPrepCrit(CCritter* rule_acl, CrID id)
{
	if(rule_acl->info.id==id) return 0; //нельзя удалять проводника

	if(rule_acl->group.prep_crit.empty()) return 1; //группы нет

	if(!rule_acl->group.prep_crit.count(id)) return 1;

	rule_acl->group.prep_crit.erase(id);
//различные расчеты над параметрами группы !!!!
//SaveGroup
	return 1;
}

int CServer::GM_GroupMove(gmap_group* group, DWORD time)
{
	if(group->rule->info.map) return 0; //разрулить!!!

	if(group->speedx || group->speedy)
	{
	//	if(cur_group->xi==cur_group->move_x && cur_group->yi==cur_group->move_y) continue;

	//	float relief=(float)(gm_dot[group->xi][group->yi].relief)/100;
	//	group->xf+=group->speedx/relief*((float)(time)/GM_ZONE_TIME_PROC);
	//	group->yf+=group->speedy/relief*((float)(time)/GM_ZONE_TIME_PROC);

		group->xf+=group->speedx*((float)(time)/GM_ZONE_TIME_PROC);
		group->yf+=group->speedy*((float)(time)/GM_ZONE_TIME_PROC);

		if(group->xf>GM_MAXX-1 || group->yf>GM_MAXY-1 || group->xf<0 || group->yf<0)
		{
			group->xf=group->xi;
			group->yf=group->yi;

			group->speedx=0;
			group->speedy=0;
		}

		int old_xi=group->xi;
		int old_yi=group->yi;

		group->xi=group->xf;
		group->yi=group->yf;

		if(old_xi!=group->xi || old_yi!=group->yi)
		{
			int old_zone_x=GM_ZONE(old_xi);
			int old_zone_y=GM_ZONE(old_yi);
			int cur_zone_x=GM_ZONE(group->xi);
			int cur_zone_y=GM_ZONE(group->yi);
			if(old_zone_x!=cur_zone_x || old_zone_y!=cur_zone_y)
			{
				GM_GroupAddToZone(group,cur_zone_x,cur_zone_y);
				SendA_GlobalInfo(group,GM_INFO_PARAM);

				return 1;
			}

		//	SendA_GlobalInfo(group,GM_INFO_PARAM);
		}
	}

	if(group->xi==group->move_x && group->yi==group->move_y)
	{
		group->speedx=0;
		group->speedy=0;

	//	if(!FLAG(cur_group->rule->info.flags,FCRIT_PLAYER) && GM_GroupEndMove(cur_group->rule))
	//	{
	//		cur_zone->groups.erase(it_g);
	//		if(it_g==cur_zone->groups.end()) break;
	//		continue;
	//	}
	}

	return 0;
}

void CServer::GM_GroupAddToZone(gmap_group* group, int zx, int zy)
{
	gmap_zone* cur_zone=&gm_zone[zx][zy];
	cur_zone->groups.push_back(group);

	if(cur_zone->cities.empty()) return;

	CCritter* cr=NULL;
	for(cl_map::iterator it_cr=group->crit_move.begin();it_cr!=group->crit_move.end();++it_cr)
	{
		cr=(*it_cr).second;
	//	if(!FLAG((*it_cr)->info.flags,FCRIT_PLAYER)) continue;

		for(city_vec::iterator it_ci=cur_zone->cities.begin();it_ci!=cur_zone->cities.end();++it_ci)
		{
			if(cr->AddKnownCity((*it_ci)->num))
				if(!FLAG(cr->info.flags,FCRIT_DISCONNECT))
					Send_GlobalInfo(cr,GM_INFO_CITIES);
		}
	}

//	SendA_GlobalInfo(group,GM_INFO_PARAM);
//	if(!cur_zone->cities.empty()) SendA_GlobalInfo(group,GM_INFO_CITIES);
}

void CServer::GM_GroupZeroParams(gmap_group* group)
{
	group->crit_move.clear();
	group->players=0;
	group->crits_luck=0;
	group->crits_pathfind=0;
	group->move_x=0;
	group->move_y=0;
	group->speedx=0;
	group->speedy=0;
	group->xf=0;
	group->yf=0;
	group->xi=0;
	group->yi=0;
}

void CServer::GM_GroupStartMove(CCritter* rule_acl)
{
	if(rule_acl->info.map) return;

	GM_GroupZeroParams(&rule_acl->group);

	rule_acl->group_move=&rule_acl->group;
	rule_acl->group_move->crit_move.insert(cl_map::value_type(rule_acl->info.id,rule_acl));
	rule_acl->group_move->xi=rule_acl->info.world_x;
	rule_acl->group_move->yi=rule_acl->info.world_y;
	rule_acl->group_move->xf=rule_acl->info.world_x;
	rule_acl->group_move->yf=rule_acl->info.world_y;
	rule_acl->group_move->move_x=rule_acl->info.world_x;
	rule_acl->group_move->move_y=rule_acl->info.world_y;

	rule_acl->group_move->speed=GM_SPEED_NORM;
	rule_acl->group_move->speedx=0;
	rule_acl->group_move->speedy=0;

	rule_acl->group_move->last_encaunter=GetTickCount();

	rule_acl->group_move->rule=rule_acl;
	SETFLAG(rule_acl->info.flags,FCRIT_RULEGROUP);

	if(FLAG(rule_acl->info.flags,FCRIT_PLAYER))
	{
		rule_acl->group_move->players++;
		if(rule_acl->state==STATE_GAME) Send_LoadMap(rule_acl);
	}

	if(!rule_acl->group.prep_crit.empty())
	{
		CCritter* cr=NULL;
		cl_map::iterator it_cr=NULL;
		for(crid_set::iterator it_id=rule_acl->group.prep_crit.begin();it_id!=rule_acl->group.prep_crit.end();++it_id)
		{
			it_cr=rule_acl->vis_cl.find((*it_id));
			if(it_cr==rule_acl->vis_cl.end()) continue;

			cr=(*it_cr).second;

			if(TransitCr(cr,0,0,0,0)!=TR_OK){ LogExecStr("Tr1 FALSE\n"); continue;}

			cr->group_move=&rule_acl->group;

			cr->info.world_x=cr->group_move->xi;
			cr->info.world_x=cr->group_move->yi;

			UNSETFLAG(cr->info.flags,FCRIT_RULEGROUP);

			if(FLAG(cr->info.flags,FCRIT_PLAYER))
			{
				cr->group_move->players++;
				if(cr->state==STATE_GAME) Send_LoadMap(cr);
			}

			rule_acl->group.crit_move.insert(cl_map::value_type(cr->info.id,cr));
		}
	}

	GM_GroupAddToZone(rule_acl->group_move,GM_ZONE(rule_acl->group_move->xi),GM_ZONE(rule_acl->group_move->yi));

	//если нпц, то выбираем точку прибытия
}

int CServer::GM_GroupToMap(CCritter* rule_acl, WORD map_num, HexTYPE shx, HexTYPE shy, BYTE sori)
{
	if(!map_num) return 0;

	rule_acl->info.world_x=rule_acl->group.xi;
	rule_acl->info.world_y=rule_acl->group.yi;

	if(rule_acl->state!=STATE_GAME || TransitCr(rule_acl,map_num,shx,shy,sori)!=TR_OK) return 0;

	CCritter* cr=NULL;
	for(cl_map::iterator it_cr=rule_acl->group.crit_move.begin();it_cr!=rule_acl->group.crit_move.end();)
	{
		cr=(*it_cr).second;
		it_cr++;
		if(cr==rule_acl) continue;

		cr->info.world_x=rule_acl->group.xi;
		cr->info.world_y=rule_acl->group.yi;

		//x,y,ori !!!!

		if(cr->state!=STATE_GAME || TransitCr(cr,map_num,shx,shy,sori)!=TR_OK)
		{
			GM_GroupToGemmate(cr);
		}

		if(FLAG(cr->info.flags,FCRIT_PLAYER))
		{
			Send_LoadMap(cr);
		}

		if(FLAG(cr->info.flags,FCRIT_NPC))
		{
			SetVisCr(cr);
		}

		if(FLAG(cr->info.flags,FCRIT_MOB))
		{
			SetVisCr(cr);
		}
	}

	if(FLAG(rule_acl->info.flags,FCRIT_PLAYER))
	{
		Send_LoadMap(rule_acl);
	}

	int zone_x=GM_ZONE(rule_acl->group_move->xi);
	int zone_y=GM_ZONE(rule_acl->group_move->yi);
	for(gmap_group_vec::iterator it_gr=gm_zone[zone_x][zone_y].groups.begin();it_gr!=gm_zone[zone_x][zone_y].groups.end();++it_gr)
		if(rule_acl->group_move==(*it_gr))
		{
			gm_zone[zone_x][zone_y].groups.erase(it_gr);
			break;
		}

	rule_acl->group.crit_move.clear();
	rule_acl->group_move=NULL;

	return 1;
}

int CServer::GM_GroupToCity(CCritter* rule_acl, WORD city_num, WORD map_count)
{
	if(rule_acl->info.map) return 0;
	if(!rule_acl->group_move) return 0;

	if(rule_acl!=rule_acl->group_move->rule)
	{
		SetCheat(rule_acl,"Попытка управления группой не рулевым. Проникновение в город.");
		return 0;
	}

	if(rule_acl->state!=STATE_GAME) return 0;

	if(!city_num)
	{
	//	if(GM_GroupToEncaunter(CCritter* rule_acl,NULL)) return 1;
		return 0;
	}

	if(!rule_acl->CheckKnownCity(city_num))
	{
		SetCheat(rule_acl,"Попытка проникновения в город, о котором не известно");
		return 0;
	}

	city_info* cinf=&city[city_num];

	if(DistSqrt(rule_acl->group.xi-cinf->wx,rule_acl->group.yi-cinf->wy)>cinf->radius)
	{
		SetCheat(rule_acl,"Попытка проникновения в город, который дальше группы");
		return 0;
	}

	if(cinf->maps.empty()) return 0;

	map_info* mapi=*cinf->maps.begin();

	if(!mapi->num) return 0;
//	if(!(minf=))
//	{
//		SetCheat(rule_acl,"Попытка проникновения в город, на неизвестную карту");
//		return 0;
//	}

	if(!GM_GroupToMap(rule_acl,mapi->num,mapi->start_hex_x,mapi->start_hex_y,mapi->start_ori)) return 0;

	return 1;
}

int CServer::GM_GroupToEncaunter(CCritter* rule_acl, WORD num_encaunter)
{
	if(rule_acl->info.map) return 0;
	if(!rule_acl->group_move) return 0;

	if(rule_acl!=rule_acl->group_move->rule)
	{
		SetCheat(rule_acl,"Попытка управления группой не рулевым. Проникновение в энкаунтер.");
		return 0;
	}

	if(rule_acl->state!=STATE_GAME) return 0;

//создаем новый
	if(!num_encaunter)
	{
		if(encaunters_free.empty()) return 0;

		WORD district=gm_zone[GM_ZONE(rule_acl->group_move->xi)][GM_ZONE(rule_acl->group_move->yi)].district;
		word_set::iterator it_en;
		for(it_en=encaunters_free.begin();it_en!=encaunters_free.end();++it_en)
			if(encaunter[(*it_en)].district & district)
			{
				num_encaunter=(*it_en);
				break;
			}

		if(!num_encaunter || num_encaunter>=MAX_ENCAUNTERS || it_en==encaunters_free.end()) return 0;

		encaunter_info* cur_encaunter=&encaunter[num_encaunter];

		if(!GM_GroupToMap(rule_acl,
			cur_encaunter->emap->num,
			cur_encaunter->start_hx[0],
			cur_encaunter->start_hx[0],
			cur_encaunter->start_ori[0])) return 0;

		cur_encaunter->count_groups=1;

		encaunters_busy.insert(num_encaunter);
		encaunters_free.erase(it_en);

		return num_encaunter;
	}

//присоединяемся
	if(!encaunters_busy.count(num_encaunter)) return 0;
	encaunter_info* cur_encaunter=&encaunter[num_encaunter];

	if(cur_encaunter->count_groups>=cur_encaunter->max_groups) return 0;

	if(!GM_GroupToMap(rule_acl,
		cur_encaunter->emap->num,
		cur_encaunter->start_hx[cur_encaunter->count_groups],
		cur_encaunter->start_hx[cur_encaunter->count_groups],
		cur_encaunter->start_ori[cur_encaunter->count_groups])) return 0;

	cur_encaunter->count_groups++;

	return 1;
}

int CServer::GM_GroupToGemmate(CCritter* rule_acl)
{
	if(rule_acl->info.map) return 0;
	if(!rule_acl->group_move) return 0;

	if(FLAG(rule_acl->info.flags,FCRIT_PLAYER))
		rule_acl->group_move->players--;
	cl_map::iterator it_cr=rule_acl->group_move->crit_move.find(rule_acl->info.id);
	if(it_cr!=rule_acl->group_move->crit_move.end())
		rule_acl->group_move->crit_move.erase(it_cr);

	SendA_GlobalInfo(rule_acl->group_move,GM_INFO_CRITS);

	GM_GroupZeroParams(&rule_acl->group);

	rule_acl->group_move=&rule_acl->group;
	rule_acl->group_move->crit_move.insert(cl_map::value_type(rule_acl->info.id,rule_acl));
	rule_acl->group_move->xi=rule_acl->info.world_x;
	rule_acl->group_move->yi=rule_acl->info.world_y;
	rule_acl->group_move->xf=rule_acl->info.world_x;
	rule_acl->group_move->yf=rule_acl->info.world_y;
	rule_acl->group_move->move_x=rule_acl->info.world_x;
	rule_acl->group_move->move_y=rule_acl->info.world_y;

	rule_acl->group_move->speed=GM_SPEED_NORM;
	rule_acl->group_move->speedx=0;
	rule_acl->group_move->speedy=0;

	rule_acl->group_move->rule=rule_acl;
	SETFLAG(rule_acl->info.flags,FCRIT_RULEGROUP);

	GM_GroupAddToZone(rule_acl->group_move,GM_ZONE(rule_acl->group_move->xi),GM_ZONE(rule_acl->group_move->yi));

	SendA_GlobalInfo(rule_acl->group_move,GM_INFO_ALL);

	return 1;
}

//void CMapMngr::GM_EraseGroup(gmap_zone* zone, gmap_group* group)
//{
	//if(!cur_group->crit.size()) cur_zone->groups.erase(it_g);
//}

void CServer::GM_GroupSetSpeed(CCritter* rule_acl, BYTE speed)
{
	if(rule_acl->info.map) return;
	if(!rule_acl->group_move) return;
	if(rule_acl!=rule_acl->group_move->rule) return; //!!!set_cheat
	if(speed!=GM_SPEED_SLOW && speed!=GM_SPEED_NORM && speed!=GM_SPEED_FAST) return;

	rule_acl->group_move->speed=speed;

	float k_speed=0;
	switch(rule_acl->group_move->speed)
	{
	case 0: k_speed=0.5f; break;
	case 1: k_speed=1.0f; break;
	case 2: k_speed=2.0f; break;
	default: rule_acl->group_move->speed=1; k_speed=1.0f; break;
	}

	int dist=(int) sqrt(pow(rule_acl->group_move->move_x-rule_acl->group_move->xi,2.0)+pow(rule_acl->group_move->move_y-rule_acl->group_move->yi,2.0));
	float time=(((GM_MOVE_1KM_TIME/k_speed)/TIME_MULTIPLER)*dist)/GM_ZONE_TIME_PROC;

	rule_acl->group_move->speedx=(float)(rule_acl->group_move->move_x-rule_acl->group_move->xi)/time;
	rule_acl->group_move->speedy=(float)(rule_acl->group_move->move_y-rule_acl->group_move->yi)/time;

	SendA_GlobalInfo(rule_acl->group_move,GM_INFO_PARAM);
}

void CServer::GM_GroupSetMove(CCritter* rule_acl, int gx, int gy)
{
	if(rule_acl->info.map) return;
	if(!rule_acl->group_move) return;
	if(rule_acl!=rule_acl->group_move->rule) return; //!!!set_cheat
	if(gx<0 || gx>=GM_MAXX || gy<0 || gy>=GM_MAXY) return;

	rule_acl->group_move->move_x=gx;
	rule_acl->group_move->move_y=gy;

	float k_speed=0;
	switch(rule_acl->group_move->speed)
	{
	case 0: k_speed=0.5f; break;
	case 1: k_speed=1.0f; break;
	case 2: k_speed=2.0f; break;
	default: rule_acl->group_move->speed=1; k_speed=1.0f; break;
	}

	int dist= (int) sqrt(pow(rule_acl->group_move->move_x-rule_acl->group_move->xi,2.0)+pow(rule_acl->group_move->move_y-rule_acl->group_move->yi,2.0));
	float time=(((GM_MOVE_1KM_TIME/k_speed)/TIME_MULTIPLER)*dist)/GM_ZONE_TIME_PROC;

	rule_acl->group_move->speedx=(float)(rule_acl->group_move->move_x-rule_acl->group_move->xi)/time;
	rule_acl->group_move->speedy=(float)(rule_acl->group_move->move_y-rule_acl->group_move->yi)/time;

	SendA_GlobalInfo(rule_acl->group_move,GM_INFO_PARAM);
}

void CServer::GM_GroupStopMove(CCritter* rule_acl)
{
	if(rule_acl->info.map) return;
	if(!rule_acl->group_move) return;
	if(rule_acl!=rule_acl->group_move->rule) return; //!!!set_cheat

	rule_acl->group_move->move_x=rule_acl->group_move->xi;
	rule_acl->group_move->move_y=rule_acl->group_move->yi;
	rule_acl->group_move->xf=rule_acl->group_move->xi;
	rule_acl->group_move->yf=rule_acl->group_move->yi;
	rule_acl->group_move->speedx=0;
	rule_acl->group_move->speedy=0;

	SendA_GlobalInfo(rule_acl->group_move,GM_INFO_PARAM);
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================