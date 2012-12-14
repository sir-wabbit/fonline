/********************************************************************
	author:	Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"
#include "FOserv.h"

void CServer::SetVisibleObj(CCritter* acl)
{
	acl->GenLook();

	int min_x;
	if((min_x=acl->info.x-acl->info.look-1)<0) min_x=0;

	int min_y;
	if((min_y=acl->info.y-acl->info.look-1)<0) min_y=0;

	int max_x;
	if((max_x=acl->info.x+acl->info.look+1)>=MAXHEXX) max_x=MAXHEXX-1;

	int max_y;
	if((max_y=acl->info.y+acl->info.look+1)>=MAXHEXY) max_y=MAXHEXY-1;

//	DHexTYPE min_dhex=(min_x<<16)|min_y;
//	DHexTYPE max_dhex=(max_x<<16)|max_y;

	int hx;
	int hy;
	int dist;

	dyn_map::iterator it2;
	for(dyn_map_map::iterator it=objects_map[acl->info.map].begin();it!=objects_map[acl->info.map].end();it++)
	{
		//if((*it).first<min_dhex) continue;
		//if((*it).first>max_dhex) break;

		hx=(*it).first >> 16;
		hy=(*it).first & 0xFFFF;

		if(hx<min_x || hy<min_y) continue;
		if(hx>max_x || hy>max_y) continue;
//		if(hx>(min_x+3) && hy>(min_y+3) && hx<(max_x-3) && hy<(max_y-3)) continue;
		if(hx>max_x && hy>max_y) break;

		dist=(int) sqrt(pow(acl->info.x-hx,2.0)+pow(acl->info.y-hy,2.0));

		if(acl->info.look>=dist)
		{
			for(it2=(*it).second->begin();it2!=(*it).second->end();it2++)
				if(AddObjIntoListInd(acl,(*it2).second->id)) Send_AddObjOnMap(acl,(*it2).second);
		}
		else
		{
			for(it2=(*it).second->begin();it2!=(*it).second->end();it2++)
				if(DelObjFromListInd(acl,(*it2).second->id)) Send_RemObjFromMap(acl,(*it2).second);
		}
	}
}

int CServer::AddObjIntoListInd(CCritter* acl, uint32_t add_ind)
{
	if(!acl->info.obj_id.count(add_ind))
	{
		acl->info.obj_id.insert(add_ind);
		return 1;
	}
	return 0;
}

int CServer::DelObjFromListInd(CCritter* acl, uint32_t del_ind)
{
	if(acl->info.obj_id.count(del_ind))
	{
		acl->info.obj_id.erase(del_ind);
		return 1;
	}
	return 0;
}

void CServer::ClearStaticObjects() {
  for(stat_map::iterator it=all_s_obj.begin(); it!=all_s_obj.end(); it++) {
    SAFEDEL((*it).second);
  }
  
  all_s_obj.clear();
}

int CServer::LoadAllStaticObjects()
{
	LogExecStr("Загрузка статических объектов...");

	FILE *cf;
	FILE *cf2;
	params_map::iterator it_o;

	ClearStaticObjects();

	if((cf=fopen("objects\\all_obj.st","rt"))==NULL)
	{
		LogExecStr("Файл all_obj.st не найден\n");
		return 0;
	}

	int cnt_obj=0;
	int tmpi=0;
	char tmpc[120];

	while(!feof(cf))
	{
		tmpi=0;
		fscanf(cf,"%d",&tmpi);
		if(!tmpi) break;

		sprintf(tmpc,"objects\\%d.st",tmpi);
		if((cf2=fopen(tmpc,"rt"))==NULL)
		{
			LogExecStr("Файл |%s| не найден\n",tmpc);
			return 0;
		}

		stat_obj* new_obj;
		new_obj= new stat_obj;

		new_obj->id=tmpi;
		fscanf(cf2,"%s",&tmpc);
		it_o=object_map.find(tmpc);
		if(it_o==object_map.end())
		{
		  delete new_obj;
			LogExecStr("Параметр |%s| не найден",tmpc);
			return 0;
		}
		new_obj->type=(*it_o).second;

		while(!feof(cf2))
		{
			fscanf(cf2,"%s%d",&tmpc,&tmpi);

			it_o=object_map.find(tmpc);
			if(it_o==object_map.end())
			{
			  delete new_obj;
				LogExecStr("Параметр |%s| не найден",tmpc);
				return 0;
			}
			new_obj->p[(*it_o).second]=tmpi;
		}

		all_s_obj[new_obj->id]=new_obj;

		fclose(cf2);
		cnt_obj++;
	}
	
	fclose(cf);

	LogExecStr("OK (%d объектов)\n",cnt_obj);

	return 1;
}

int CServer::LoadAllObj() //загрузка динамических объектов из mySQL
{
/*	LogExecStr("Загрузка динамических объектов\n");
	//узнаем кол-во записей всего
	int find_obj=sql.CountTable("objects","id");

	int add_obj=0;
	int num_obj=1;
	if(find_obj)
		while(add_obj<find_obj)
		{
			if(num_obj==sql.GetInt("objects","id","id",num_obj))
			{
				dyn_obj* new_obj=new dyn_obj;
				
				new_obj->id=num_obj;
				sql.LoadDataObject(new_obj);
				new_obj->object=all_s_obj[sql.GetInt("objects","num_st","id",new_obj->id)];

				//присваеваем тайлу ссылку на динамический объект
				if(new_obj->map) InsertObjOnMap(new_obj,new_obj->map,new_obj->x,new_obj->y);
				//присваеваем игроку ссылку на динамический объект
				else if(new_obj->player)
				{
					for(cl_map::iterator it=cl.begin(); it!=cl.end(); it++)
						if((*it).second->info.id==new_obj->player)
						{
							(*it).second->info.obj[num_obj]=new_obj;

							if(new_obj->active)
								if(!new_obj->object->type==OBJ_TYPE_ARMOR)
									(*it).second->info.a_obj_arm=new_obj;
								else
									(*it).second->info.a_obj=new_obj;
							break;
						}
				}
				//присваеваем общему списку ссылку на динамический объект
				all_obj[num_obj]=new_obj;

				add_obj++;
			}
			num_obj++;
		}

	LogExecStr("Загрузка динамических объектов прошла успешно\n");
*/
	return 1;
}

void CServer::SaveAllObj() //запись динамических объектов в mySQL
{
	for(dyn_map::iterator it=all_obj.begin(); it!=all_obj.end(); it++)
		sql.SaveDataObject((*it).second);
}

void CServer::CreateObjToTile(MapTYPE c_map, HexTYPE c_x, HexTYPE c_y, uint16_t num_st_obj)
{
	if(!c_map || c_map>MAX_MAPS) return;
	if(c_x>MAXHEXX || c_y>MAXHEXY) return;

	dyn_obj* new_obj=new dyn_obj;

	stat_map::iterator it_st=all_s_obj.find(num_st_obj);
	if(it_st==all_s_obj.end())
	{
		delete new_obj;
		return;
	}

	new_obj->object=(*it_st).second;
	new_obj->type=new_obj->object->type;
	new_obj->accessory=DOBJ_ACCESSORY_HEX;

	new_obj->ACC_HEX.map=c_map;
	new_obj->ACC_HEX.x=c_x;
	new_obj->ACC_HEX.y=c_y;

	new_obj->time_wear=new_obj->object->p[OBJ_LIVETIME]*1000000;
	new_obj->last_tick=GetTickCount();

	if(!sql.NewObject(new_obj,cur_obj_id))
	{
		delete new_obj;
		return;
	}

	all_obj[cur_obj_id]=new_obj;

	InsertObjOnMap(new_obj,c_map,c_x,c_y);

	cur_obj_id++;

LogExecStr("Item Create to TL =%d\n",cur_obj_id);
}

void CServer::CreateObjToPl(CritterID c_pl_idchannel, uint16_t num_st_obj)
{
	dyn_obj* new_obj=new dyn_obj;

	stat_map::iterator it_st=all_s_obj.find(num_st_obj);
	if(it_st==all_s_obj.end())
	{
		delete new_obj;
		return;
	}

	new_obj->object=(*it_st).second;
	new_obj->type=new_obj->object->type;

	new_obj->time_wear=new_obj->object->p[OBJ_LIVETIME]*1000000;
	new_obj->last_tick=GetTickCount();

	cl_map::iterator it=cl.find(c_pl_idchannel);
	if(it==cl.end())
	{
		delete new_obj;
		return;
	}

	new_obj->accessory=DOBJ_ACCESSORY_CRIT;
	new_obj->ACC_CRITTER.id=(*it).second->info.id;
	new_obj->ACC_CRITTER.slot=DOBJ_SLOT_INV;
//	new_obj->ACC_CRITTER.active=0;

	if(!sql.NewObject(new_obj,cur_obj_id))
	{
		delete new_obj;
		return;
	}

	(*it).second->info.obj[cur_obj_id]=new_obj;
	all_obj[cur_obj_id]=new_obj;

	cur_obj_id++;

LogExecStr("Item Create to PL =%d\n",cur_obj_id);
}

void CServer::DeleteObj(uint32_t id_obj)
{
	dyn_map::iterator it=all_obj.find(id_obj);
	if(it==all_obj.end()) return;

	sql.DeleteDataObject((*it).second);

	if((*it).second->ACC_HEX.map)
	{
	//	EraseObject((*it).second,&it);
		EraseObjFromMap((*it).second);
	}
	else
	{
		cl_map::iterator it2=cr.find((*it).second->ACC_CRITTER.id);
		if(it2==cl.end()) return;
		(*it2).second->info.obj.erase(it);
	}

	delete (*it).second;
	all_obj.erase(it);
}

void CServer::TransferDynObjPlPl(CCritter* from_acl, CCritter* to_acl, dyn_obj* obj)
{

}

void CServer::TransferDynObjTlPl(CCritter* acl, dyn_obj* obj)
{
	SendA_Action(acl,ACT_PICK_OBJ_DOWN,0);

	SendA_RemObjFromMap(acl,obj);
	Send_RemObjFromMap(acl,obj);

	EraseObjFromMap(obj);
	obj->ACC_HEX.map=0;
	obj->ACC_HEX.x=0;
	obj->ACC_HEX.y=0;

	DelObjFromListInd(acl,obj->id);

	acl->info.obj.insert(dyn_map::value_type(obj->id,obj));

	obj->accessory=DOBJ_ACCESSORY_CRIT;
	obj->ACC_CRITTER.id=acl->info.id;
	obj->ACC_CRITTER.slot=DOBJ_SLOT_INV;

	sql.SaveDataObject(obj);

	Send_AddObject(acl,obj);
}

void CServer::TransferDynObjPlTl(CCritter* acl, dyn_obj* obj)
{
	AddObjIntoListInd(acl,obj->id);

	obj->ACC_CRITTER.id=NULL;

	if(obj->ACC_CRITTER.slot==DOBJ_SLOT_HAND1) UseDefObj(acl,DOBJ_SLOT_HAND1);
	if(obj->ACC_CRITTER.slot==DOBJ_SLOT_ARMOR) UseDefObj(acl,DOBJ_SLOT_ARMOR);

	InsertObjOnMap(obj,acl->info.map,acl->info.x,acl->info.y);

	obj->accessory=DOBJ_ACCESSORY_HEX;
	obj->ACC_HEX.map=acl->info.map;
	obj->ACC_HEX.x=acl->info.x;
	obj->ACC_HEX.y=acl->info.y;
	
	sql.SaveDataObject(obj);

	SendA_Action(acl,ACT_DROP_OBJ,0);
	SendA_AddObjOnMap(acl,obj);

	acl->info.obj.erase(obj->id);
}