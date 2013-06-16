/********************************************************************
	author:	Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"
#include "FOServ.h"

void CServer::Send_AddCritter(CCritter* acl, crit_info* pinfo) //Oleg + Cvet edit
{
    MessageType msg=NETMSG_ADDCRITTER;
//	char mname[MAX_NAME+1];
	acl->bout << msg;
	acl->bout << pinfo->id;
	acl->bout << pinfo->base_type;
	acl->bout << pinfo->a_obj->object->id;
	acl->bout << pinfo->a_obj_arm->object->id;
	acl->bout << pinfo->x;
	acl->bout << pinfo->y;
	acl->bout << pinfo->ori;
	acl->bout << pinfo->st[ST_GENDER];
	acl->bout << pinfo->cond;
	acl->bout << pinfo->cond_ext;
	acl->bout << pinfo->flags;
//	bout.Write(MakeName(pinfo->name,mname),MAX_NAME);
	acl->bout.Write(pinfo->name,MAX_NAME);
	for(int i=0;i<5;i++)
		acl->bout.Write(pinfo->cases[i],MAX_NAME);

//	LogExecStr("Посылаю данные id=%d о обноружении id=%d\n", acl->info.id, pinfo->id);
}

void CServer::Send_RemoveCritter(CCritter* acl, CritterID remid) //Oleg
{
	MessageType msg=NETMSG_REMOVECRITTER;

	acl->bout << msg;
	acl->bout << remid;
//LogExecStr("Посылаю данные id=%d о скрывании id=%d\n", acl->info.id, remid);
}

void CServer::Send_LoadMap(CCritter* acl)
{
	MessageType msg=NETMSG_LOADMAP;

	acl->state=STATE_LOGINOK;

	acl->bout << msg;
	acl->bout << acl->info.map;
}

void CServer::Send_LoginMsg(CCritter* acl, uint8_t LogMsg)
{
	MessageType msg=NETMSG_LOGMSG;
	acl->bout << msg;
	acl->bout << LogMsg;
}

void CServer::SendA_Move(CCritter* acl, uint16_t move_params)
{
	acl->info.ori=FLAG(move_params,BIN8(00000111));

	if(FLAG(move_params,0x38)==0x38) //проверка по окончании пути
		if(FLAG(acl->info.flags,FCRIT_PLAYER)) Send_XY(acl);

	if(!acl->vis_cl.empty())
	{
		MessageType msg=NETMSG_CRITTER_MOVE;
		CCritter* c=NULL;

		for(cl_map::iterator it_cl=acl->vis_cl.begin();it_cl!=acl->vis_cl.end();)
		{
			c=(*it_cl).second;
			it_cl++;

			if(!c || acl->info.map!=c->info.map)
			{
				DelClFromVisVec(acl,c);
				continue;
			}

			c->bout << msg;
			c->bout << acl->info.id;
			c->bout << move_params;

			c->bout << acl->info.x;
			c->bout << acl->info.y;

//LogExecStr("Посылаю данные id=%d о ходе игроком id=%d\n",c->info.id, acl->info.id);
		}
	}
}

void CServer::SendA_Action(CCritter* acl, uint8_t num_action, uint8_t rate_action)
{
//LogExecStr("Send_Action - BEGIN %d\n",acl->info.id);
	if(!acl->vis_cl.empty())
	{
		MessageType msg=NETMSG_CRITTER_ACTION;
		CCritter* c=NULL;

		for(cl_map::iterator it_cl=acl->vis_cl.begin();it_cl!=acl->vis_cl.end();)
		{
			c=(*it_cl).second;
			it_cl++;

			if(!c || acl->info.map!=c->info.map)
			{
				DelClFromVisVec(acl,c);
				continue;
			}

			c->bout << msg;
			c->bout << acl->info.id;
			c->bout << num_action;
			c->bout << acl->info.a_obj->object->id;
			c->bout << acl->info.a_obj_arm->object->id;
			c->bout << rate_action;
			c->bout << acl->info.ori;

//LogExecStr("Посылаю данные id=%d о действии id=%d\n", c->info.id, acl->info.id);
		}
	}
//LogExecStr("Send_Action - END\n");
}

void CServer::Send_AddObjOnMap(CCritter* acl, dyn_obj* o)
{
	MessageType msg=NETMSG_ADD_OBJECT_ON_MAP;

	acl->bout << msg;
	acl->bout << o->ACC_HEX.x;
	acl->bout << o->ACC_HEX.y;
	acl->bout << o->object->id;
	acl->bout << GetTileFlags(o->ACC_HEX.map,o->ACC_HEX.x,o->ACC_HEX.y);
}

void CServer::SendA_AddObjOnMap(CCritter* acl, dyn_obj* o)
{
	if(!acl->vis_cl.empty())
	{
		MessageType msg=NETMSG_ADD_OBJECT_ON_MAP;
		uint16_t tile_flags=GetTileFlags(o->ACC_HEX.map,o->ACC_HEX.x,o->ACC_HEX.y);
		CCritter* c=NULL;

		for(cl_map::iterator it_cl=acl->vis_cl.begin();it_cl!=acl->vis_cl.end();)
		{
			c=(*it_cl).second;
			it_cl++;

			if(!c || acl->info.map!=c->info.map)
			{
				DelClFromVisVec(acl,c);
				continue;
			}

			c->bout << msg;
			c->bout << o->ACC_HEX.x;
			c->bout << o->ACC_HEX.y;
			c->bout << o->object->id;
			c->bout << tile_flags;
		}
	}
}

void CServer::Send_ChangeObjOnMap(CCritter* acl, dyn_obj* o)
{
	MessageType msg=NETMSG_CHANGE_OBJECT_ON_MAP;

	acl->bout << msg;
	acl->bout << o->ACC_HEX.x;
	acl->bout << o->ACC_HEX.y;
	acl->bout << o->object->id;
	acl->bout << GetTileFlags(o->ACC_HEX.map,o->ACC_HEX.x,o->ACC_HEX.y);
}

void CServer::SendA_ChangeObjOnMap(CCritter* acl, dyn_obj* o)
{
	if(!acl->vis_cl.empty())
	{
		MessageType msg=NETMSG_CHANGE_OBJECT_ON_MAP;
		uint16_t tile_flags=GetTileFlags(o->ACC_HEX.map,o->ACC_HEX.x,o->ACC_HEX.y);
		CCritter* c=NULL;

		for(cl_map::iterator it_cl=acl->vis_cl.begin();it_cl!=acl->vis_cl.end();)
		{
			c=(*it_cl).second;
			it_cl++;

			if(!c || acl->info.map!=c->info.map)
			{
				DelClFromVisVec(acl,c);
				continue;
			}

			c->bout << msg;
			c->bout << o->ACC_HEX.x;
			c->bout << o->ACC_HEX.y;
			c->bout << o->object->id;
			c->bout << tile_flags;
		}
	}
}

void CServer::Send_RemObjFromMap(CCritter* acl, dyn_obj* o)
{
	MessageType msg=NETMSG_REMOVE_OBJECT_FROM_MAP;

	acl->bout << msg;
	acl->bout << o->ACC_HEX.x;
	acl->bout << o->ACC_HEX.y;
	acl->bout << o->object->id;
}

void CServer::SendA_RemObjFromMap(CCritter* acl, dyn_obj* o)
{
	if(!acl->vis_cl.empty())
	{
		MessageType msg=NETMSG_REMOVE_OBJECT_FROM_MAP;
		CCritter* c=NULL;

		for(cl_map::iterator it_cl=acl->vis_cl.begin();it_cl!=acl->vis_cl.end();)
		{
			c=(*it_cl).second;
			it_cl++;

			if(!c || acl->info.map!=c->info.map)
			{
				DelClFromVisVec(acl,c);
				continue;
			}

			c->bout << msg;
			c->bout << o->ACC_HEX.x;
			c->bout << o->ACC_HEX.y;
			c->bout << o->object->id;
		}
	}
}

void CServer::Send_AddObject(CCritter* acl, dyn_obj* send_obj)
{
	//отсылаем объект игроку
	MessageType msg=NETMSG_ADD_OBJECT;

	acl->bout << msg;
	//ид динамического и статического объекта
	acl->bout << send_obj->id;
	acl->bout << send_obj->object->id;
	//активный или в инвентаре
	acl->bout << send_obj->ACC_CRITTER.slot;
	//динамические показатели
	acl->bout << send_obj->time_wear;
	acl->bout << send_obj->broken_info;
}

void CServer::Send_RemObject(CCritter* acl, dyn_obj* send_obj)
{
	//отсылаем запрос игроку на удаление объекта
	MessageType msg=NETMSG_REMOVE_OBJECT;

	acl->bout << msg;
	//ид динамического
	acl->bout << send_obj->id;
}

void CServer::Send_WearObject(CCritter* acl, dyn_obj* send_obj)
{
	//извещаем игрока о износе объекта
	MessageType msg=NETMSG_WEAR_OBJECT;

	acl->bout << msg;
	acl->bout << send_obj->id;
	acl->bout << send_obj->time_wear;
	acl->bout << send_obj->broken_info;
}

void CServer::Send_Map(CCritter* acl, uint16_t map_num)
{
	LogExecStr("Отправка карты №%d игроку ID №%d...",map_num,acl->info.id);

	//acl->bout << map_size;
	//acl->bout.Write(

	LogExecStr("OK\n");
}

void CServer::Send_XY(CCritter* acl)
{
    MessageType msg=NETMSG_XY;

	acl->bout << msg;
	acl->bout << acl->info.x;
	acl->bout << acl->info.y;
	acl->bout << acl->info.ori;
//LogExecStr("Try connect! Send_XY id=%d\n", acl->info.id);
}

void CServer::Send_AllParams(CCritter* acl, uint8_t type_param)
{
	//отсылаем ствты которые не равны 0
	uint8_t all_send_params=0;
	uint8_t param=0;

	switch (type_param)
	{
	case TYPE_STAT:
		for(param=0; param<ALL_STATS; param++)
			if(acl->info.st[param]) all_send_params++;
		break;
	case TYPE_SKILL:
		for(param=0; param<ALL_SKILLS; param++)
			if(acl->info.sk[param]) all_send_params++;
		break;
	case TYPE_PERK:
		for(param=0; param<ALL_PERKS; param++)
			if(acl->info.pe[param]) all_send_params++;
		break;
	}

	if(all_send_params)
	{
		MessageType msg=NETMSG_ALL_PARAMS;
		acl->bout << msg;
		acl->bout << type_param;
		acl->bout << all_send_params;

		switch (type_param)
		{
		case TYPE_STAT:
			for(param=0; param<ALL_STATS; param++)
				if(acl->info.st[param])
				{
					acl->bout << param;
					acl->bout << acl->info.st[param];
				}
			break;
		case TYPE_SKILL:
			for(param=0; param<ALL_SKILLS; param++)
				if(acl->info.sk[param])
				{
					acl->bout << param;
					acl->bout << acl->info.sk[param];
				}
			break;
		case TYPE_PERK:
			for(param=0; param<ALL_PERKS; param++)
				if(acl->info.pe[param])
				{
					acl->bout << param;
					acl->bout << acl->info.pe[param];
				}
			break;
		}
	}
}

void CServer::Send_Param(CCritter* acl, uint8_t type_param, uint8_t num_param)
{
	MessageType msg=NETMSG_PARAM;
	acl->bout << msg;
	acl->bout << type_param;
	acl->bout << num_param;

	switch (type_param)
	{
	case TYPE_STAT:
		acl->bout << acl->info.st[num_param];
		break;
	case TYPE_SKILL:
		acl->bout << acl->info.sk[num_param];
		break;
	case TYPE_PERK:
		acl->bout << acl->info.pe[num_param];
		break;
	}
}

void CServer::Send_Talk(CCritter* acl, npc_dialog* send_dialog)
{
	MessageType msg=NETMSG_TALK_NPC;
	acl->bout << msg;
	if(send_dialog==NULL)
	{
		const uint8_t zero_byte=0;
		acl->bout << zero_byte;
		return;
	}
//всего вариантов ответа
	uint8_t all_answers=send_dialog->answers.size();
	acl->bout << all_answers;
	if(!all_answers) return;
//основной текст
	acl->bout << send_dialog->id_text;
//варианты ответов
	for(answers_list::iterator it_a=send_dialog->answers.begin(); it_a!=send_dialog->answers.end(); it_a++)
		acl->bout << (*it_a)->id_text;
}

void CServer::Send_GlobalInfo(CCritter* acl, uint8_t info_flags)
{
	LogExecStr("Посылаю данные о глобале...");

	MessageType msg=NETMSG_GLOBAL_INFO;

	if(acl->info.map || !acl->group_move)
	{
		Send_LoadMap(acl);
		return;
	}

	acl->bout << msg;
	acl->bout << info_flags;

	if(FLAG(info_flags,GM_INFO_CITIES))
	{
		uint16_t count_cities=acl->known_cities.size();
		acl->bout << count_cities;

		city_info* cur_city=NULL;
		for(word_set::iterator it_num=acl->known_cities.begin();it_num!=acl->known_cities.end();++it_num)
		{
			cur_city=&city[(*it_num)];

			acl->bout << cur_city->num;
			acl->bout << cur_city->wx;
			acl->bout << cur_city->wy;
			acl->bout << cur_city->radius;
		}
	}

	if(FLAG(info_flags,GM_INFO_CRITS))
	{
		uint8_t count_group=acl->group_move->crit_move.size();

		acl->bout << count_group;

		CCritter* c=NULL;
		for(cl_map::iterator it_cr=acl->group_move->crit_move.begin();it_cr!=acl->group_move->crit_move.end();++it_cr)
		{
			c=(*it_cr).second;

			acl->bout << c->info.id;

			acl->bout.Write(c->info.name,MAX_NAME);

			if(acl==c) SETFLAG(c->info.flags,FCRIT_CHOSEN);

			acl->bout << c->info.flags;

			UNSETFLAG(c->info.flags,FCRIT_CHOSEN);
		}
	}

	if(FLAG(info_flags,GM_INFO_PARAM))
	{
		int speed_x=(acl->group_move->speedx*1000000);
		int speed_y=(acl->group_move->speedy*1000000);

		acl->bout << (uint16_t)(acl->group_move->xi);
		acl->bout << (uint16_t)(acl->group_move->yi);
		acl->bout << (uint16_t)(acl->group_move->move_x);
		acl->bout << (uint16_t)(acl->group_move->move_y);
		acl->bout << acl->group_move->speed;
		acl->bout << speed_x;
		acl->bout << speed_y;
	}

	acl->bout << (uint8_t)(0xAA);

	LogExecStr("OK\n");
}

void CServer::SendA_GlobalInfo(gmap_group* group, uint8_t info_flags)
{
	CCritter* c=NULL;
	for(cl_map::iterator it_cr=group->crit_move.begin();it_cr!=group->crit_move.end();++it_cr)
	{
		c=(*it_cr).second;

		if(!FLAG(c->info.flags,FCRIT_PLAYER) || c->state!=STATE_GAME) continue;

		Send_GlobalInfo(c,info_flags);
	}
}

void CServer::Send_GameTime(CCritter* acl)
{
	MessageType msg=NETMSG_GAME_TIME;

	GetSystemTime(&sys_time);
	Game_Time=(sys_time.wHour*60+sys_time.wMinute)*TIME_MULTIPLER;

	Game_Day=15;
	Game_Month=5;
	Game_Year=2155;

	acl->bout << msg;
	acl->bout << Game_Time;
	acl->bout << Game_Day;
	acl->bout << Game_Month;
	acl->bout << Game_Year;
}

void CServer::Send_Text(CCritter* to_acl, char* s_str, uint8_t say_param)
{
	if(!s_str) return;

	MessageType msg=NETMSG_CRITTERTEXT;

	uint16_t s_len=strlen(s_str);

	to_acl->bout << msg;
	to_acl->bout << to_acl->info.id;
	to_acl->bout << say_param;
	to_acl->bout << s_len;
	to_acl->bout.Write(s_str,s_len);
}

void CServer::SendA_Text(CCritter* from_acl, cl_map* to_cr, char* s_str, char* o_str, uint8_t say_param)
{
	if(!to_cr->empty())
	{
		MessageType msg=NETMSG_CRITTERTEXT;
		uint16_t o_len=strlen(o_str);

		CCritter* c=NULL;

		for(cl_map::iterator it_cr=to_cr->begin();it_cr!=to_cr->end();)
		{
			c=(*it_cr).second;
			it_cr++;

			if(!c) continue;

			if(c==from_acl) continue;

			if(!FLAG(c->info.flags,FCRIT_PLAYER) || FLAG(c->info.flags,FCRIT_DISCONNECT)) continue;

			c->bout << msg;
			c->bout << from_acl->info.id;
			c->bout << say_param;
			c->bout << o_len;
			c->bout.Write(o_str,o_len);
		}
	}

	if(from_acl && s_str) Send_Text(from_acl,s_str,say_param);
}
