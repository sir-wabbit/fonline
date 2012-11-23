#include "stdafx.h"

/********************************************************************
	created:	18:08:2004   23:48; edited: 2006,2007

	author:		Oleg Mareskin
	add/edit:	Denis Balihin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/


#include "FOServ.h"

#include "main.h"
#include "socials.h"


void *zlib_alloc(void *opaque, unsigned int items, unsigned int size);
void zlib_free(void *opaque, void *address);

#define MAX_CCritterS 100

CrID busy[MAX_CCritterS];

//!Cvet изменил концепцию ++++
//команда без префикса ~
#define CMD_SAY			0

//команды имеют префикс ~
#define CMD_EXIT		1 //выход ~exit
#define CMD_CRITID		2 //узнать ИД криттера по его имени ~id name -> crid/"false"
#define CMD_MOVECRIT	3 //двигать криттера ~move id x y -> "ok"/"false"
#define CMD_KILLCRIT	4 //убить криттера ~kill id -> "ok"/"false"
#define CMD_DISCONCRIT	5 //отсоединить криттера ~disconnect id -> "ok"/"false"
#define CMD_TOGLOBAL	6 //выход на глобал ~toglobal -> toglobal/"false"

//уровни доступа
#define ACCESS_ALL		0
#define ACCESS_MODER	1
#define ACCESS_ADMIN	2
#define ACCESS_GOD		3

struct cmdlist_def
{
	char cmd[30];
	int id;
	uint8_t access;
};

const int CMN_LIST_COUNT=12;
const cmdlist_def cmdlist[]=
{
	{"конец",CMD_EXIT,ACCESS_ALL},
	{"exit",CMD_EXIT,ACCESS_ALL},

	{"ид",CMD_CRITID,ACCESS_MODER},
	{"id",CMD_CRITID,ACCESS_MODER},

	{"двигать",CMD_MOVECRIT,ACCESS_MODER},
	{"move",CMD_MOVECRIT,ACCESS_MODER},

	{"убить",CMD_KILLCRIT,ACCESS_ADMIN},
	{"kill",CMD_KILLCRIT,ACCESS_ADMIN},

	{"отсоединить",CMD_DISCONCRIT,ACCESS_MODER},
	{"disconnect",CMD_DISCONCRIT,ACCESS_MODER},

	{"наглобал",CMD_TOGLOBAL,ACCESS_ALL},
	{"toglobal",CMD_TOGLOBAL,ACCESS_ALL},
};
//!Cvet ----

HANDLE hDump;

CServer* CServer::self=NULL; //!Cvet

CServer::CServer()
{
	Active=0;
	sql.mySQL=NULL;
	outLEN=4096;
	outBUF=new char[outLEN];
	last_id=0; // Никто не присоединился
	for(int i=0;i<MAX_CCritterS;i++) busy[i]=0;

	cur_obj_id=1;

	self=this; //!Cvet
}

CServer::~CServer()
{
	Finish();
	SAFEDELA(outBUF);
	CloseHandle(hDump);

	self=NULL; //!Cvet
}

void CServer::ClearClients() //!Cvet edit
{
	//!Cvet сохраняем данные объектов !!!!!!!!!!!!!!!!!!!dest
	SaveAllObj();

	//!Cvet сохраняем данные клиентов !!!!!!!!!!!!!!!!dest
	SaveAllDataPlayers();
	
	//!Cvet удаляем объекты
	for(dyn_map::iterator it2=all_obj.begin();it2!=all_obj.end();it2++)
	{
		delete (*it2).second;
	}
	all_obj.clear();

	//!Cvet удаляем клиентов
	cl_map::iterator it;
	for(it=cl.begin();it!=cl.end();it++)
	{
		if((*it).second->s!=NULL)
		{
			closesocket((*it).second->s);
			deflateEnd(&(*it).second->zstrm);
		}
		delete (*it).second;
	}
	cl.clear();

	//!Cvet удаляем НПЦ !!!!!!!!!!!!!!!!!!!!
	for(it=pc.begin();it!=pc.end();it++)
	{
//		delete (*it).second;
	}
	pc.clear();

	NumClients=0;
#ifndef FOSERVICE_VERSION
	SetEvent(hUpdateEvent);
#endif
}

//!Cvet ++++ изменил много чего
int CServer::ConnectClient(SOCKET serv)
{
	LogExecStr("Попытка соеденить нового клиента...");

    SOCKADDR_IN from;
	int addrsize=sizeof(from);
	
	SOCKET NewCl=accept(serv,(sockaddr*)&from,&addrsize);

	if(NewCl==INVALID_SOCKET) { LogExecStr("INVALID_SOCKET №%d\n",NewCl); return 0; }

	CCritter* ncl=new CCritter;
	ncl->s=NewCl;
	ncl->from=from;

    ncl->zstrm.zalloc = zlib_alloc;
    ncl->zstrm.zfree = zlib_free;
    ncl->zstrm.opaque = NULL;

	if(deflateInit(&ncl->zstrm,Z_DEFAULT_COMPRESSION)!=Z_OK)
	{
		LogExecStr("DeflateInit error forSockID=%d\n",NewCl);
		ncl->state=STATE_DISCONNECT; //!!!!!
		return 0;
	}

	int free_place=-1;
	for(int i=0;i<MAX_CCritterS;i++) //проверяем есть ли свободный канал Для Игрока
	{
		if(!busy[i])
		{
			free_place=i; //опре-ся не занятый номер канала
			ncl->info.idchannel=i;
			busy[ncl->info.idchannel]=1;
			break;
		}
	}

	if(free_place==-1)
	{
		LogExecStr("Нет свободного канала\n",NewCl);
		ncl->state=STATE_DISCONNECT;
		return 0;
	}

	ncl->state=STATE_CONN;
	
	cl.insert(cl_map::value_type(ncl->info.idchannel,ncl));

   	NumClients++; //инкремент кол-ва подключенных клиентов

	LogExecStr("OK. Канал=%d. Всего клиентов в игре: %d\n",ncl->info.idchannel,NumClients);

#ifndef FOSERVICE_VERSION
	SetEvent(hUpdateEvent);
#endif

	return 1;
}

void CServer::DisconnectClient(CrID idchannel)
{
	LogExecStr("Disconnecting a client with the channel id =  %d...", idchannel);

	cl_map::iterator it_ds=cl.find(idchannel);
	if(it_ds==cl.end())
	{
		LogExecStr("WARNING: Could not find the client.\n");
		return;
	}

	closesocket((*it_ds).second->s);
	deflateEnd(&(*it_ds).second->zstrm);

	//Освобождение канала
	busy[idchannel]=0;

	if((*it_ds).second->info.cond!=COND_NOT_IN_GAME)
	{
		SETFLAG((*it_ds).second->info.flags,FCRIT_DISCONNECT);

		if((*it_ds).second->info.map)
			SendA_Action((*it_ds).second,ACT_DISCONNECT,NULL);
		else
			SendA_GlobalInfo((*it_ds).second->group_move,GM_INFO_CRITS);

		sql.SaveDataPlayer(&(*it_ds).second->info);
	}
	else
	{
		LogExecStr(".1.");
		SAFEDEL((*it_ds).second); //!!!!!!!!BUG??? ВАЙ???!!!!
		LogExecStr(".2.");
	}

	//Удаление клиента из списка
	cl.erase(it_ds);

	NumClients--;

	LogExecStr("Отсоединение завершено. Всего клиентов в игре: %d\n",NumClients);
}

void CServer::RemoveCritter(CrID id)
{
	LogExecStr("Удаляем криттера id=%d\n",id);

	cl_map::iterator it=cr.find(id);
	if(it==cr.end()) { LogExecStr("!!!WORNING!!! RemoveCritter - клиент не найден id=%d\n",id); return; } // Значит не нашел обьекта на карте

	if((*it).second->info.map)
	{
		//Удаляем с тайла
		EraseCrFromMap((*it).second,(*it).second->info.map,(*it).second->info.x,(*it).second->info.y);
	}

	delete (*it).second;
	cr.erase(it);

//	NumCritters--;

	LogExecStr("Криттер удален\n");

#ifndef FOSERVICE_VERSION
	SetEvent(hUpdateEvent);
#endif
}
//!Cvet ----

void CServer::RunGameLoop()
{
	if(!Active) return;

	TICK ticks;
	int delta;
	timeval tv;
	tv.tv_sec=0;
	tv.tv_usec=0;
	CCritter* c;

	ticks=GetTickCount();

	LogExecStr("***   Starting Game loop   ***\n");

//!Cvet сбор статистики +++
	loop_time=0;
	loop_cycles=0;
	loop_min=100;
	loop_max=0;

	lt_FDsel=0;
	lt_conn=0;
	lt_input=0;
	lt_proc_cl=0;
	lt_proc_pc=0;
	lt_output=0;
	lt_discon=0;

	lt_FDsel_max=0;
	lt_conn_max=0;
	lt_input_max=0;
	lt_proc_cl_max=0;
	lt_proc_pc_max=0;
	lt_output_max=0;
	lt_discon_max=0;

	lags_count=0;

	TICK lt_ticks,lt_ticks2;
//!Cvet ---

	while(!FOQuit)
	{
		ticks=GetTickCount()+100;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&exc_set);
		FD_SET(s,&read_set);
	
		for(cl_map::iterator it=cl.begin();it!=cl.end();it++)
		{
			c=(*it).second;
			if(c->state!=STATE_DISCONNECT)
			{
				FD_SET(c->s,&read_set);
				FD_SET(c->s,&write_set);
				FD_SET(c->s,&exc_set);
			}
		}

		select(0,&read_set,&write_set,&exc_set,&tv);

		lt_ticks=GetTickCount();
		lt_FDsel+=lt_ticks-(ticks-100);

	//Новое подключение клиента
		if(FD_ISSET(s,&read_set))
		{
			ConnectClient(s);
		}

		lt_ticks2=lt_ticks;
		lt_ticks=GetTickCount();
		lt_conn+=lt_ticks-lt_ticks2;

	//!Cvet Прием данных от клиентов
		for(cl_map::iterator it=cl.begin();it!=cl.end();++it)
		{
			c=(*it).second;
			if((FD_ISSET(c->s,&read_set))&&(c->state!=STATE_DISCONNECT)) {
				if(!Input(c)) {
				  LogExecStr("Could not recieve data from a client.\n");
				  c->state=STATE_DISCONNECT;
				}
		  }
		}

		lt_ticks2=lt_ticks;
		lt_ticks=GetTickCount();
		lt_input+=lt_ticks-lt_ticks2;

	//Обработка данных клиентов
		for(cl_map::iterator it=cl.begin();it!=cl.end();it++)
		{
			c=(*it).second;
			if(c->state==STATE_DISCONNECT) continue;

			if(c->bin.writePosition) Process(c);
		}

		lt_ticks2=lt_ticks;
		lt_ticks=GetTickCount();
		lt_proc_cl+=lt_ticks-lt_ticks2;

	//Обработка НПЦ
		for(cl_map::iterator it=pc.begin();it!=pc.end();it++)
		{
			c=(*it).second;
			NPC_Process(c);
		}

		lt_ticks2=lt_ticks;
		lt_ticks=GetTickCount();
		lt_proc_pc+=lt_ticks-lt_ticks2;

	//Обработка Мобов
		MOBs_Proccess();

	//	lt_ticks2=lt_ticks;
	//	lt_ticks=GetTickCount();
	//	lt_proc_pc+=lt_ticks-lt_ticks2;

	//Посылка данных клиентов
		for(cl_map::iterator it=cl.begin();it!=cl.end();it++)
		{
			c=(*it).second;
			if(FD_ISSET(c->s,&write_set)) Output(c);
		}

		lt_ticks2=lt_ticks;
		lt_ticks=GetTickCount();
		lt_output+=lt_ticks-lt_ticks2;

	//Убирание отключенных клиентов 
		for(cl_map::iterator it=cl.begin();it!=cl.end();)
		{
			c=(*it).second;
			it++;
			if(c->state==STATE_DISCONNECT) 
			{
				DisconnectClient(c->info.idchannel);
				continue; 
			}
		}

		GM_Process(ticks-GetTickCount());

		lt_discon+=GetTickCount()-lt_ticks;

	//!Cvet сбор статистики
		uint32_t loop_cur=GetTickCount()-(ticks-100);
		loop_time+=loop_cur;
		loop_cycles++;
		if(loop_cur > loop_max) loop_max=loop_cur;
		if(loop_cur < loop_min) loop_min=loop_cur;

	//если быстро справились, то спим
		delta=ticks-GetTickCount();
		if(delta>0)
		{
			Sleep(delta);
		}
		else lags_count++;//LogExecStr("\nLag for%d ms\n",-delta);
	}

	//LogExecStr("***   Finishing Game loop   ***\n\n");
}

int CServer::Input(CCritter* acl)
{
	UINT len=recv(acl->s,inBUF,2048,0);
	if(len==SOCKET_ERROR || !len) // если клиент отвалился
	{
		LogExecStr("SOCKET_ERROR forSockID=%d\n",acl->s);
		return 0;
	}

	if(len==2048 || (acl->bin.writePosition+len>=acl->bin.capacity))
	{
		LogExecStr("FLOOD_CONTROL forSockID=%d\n",acl->s);
		return 0; // если флудит игрок
	}

	acl->bin.Write(inBUF,len);

	return 1;
}

void CServer::Process(CCritter* acl) // Лист Событий
{
	MSGTYPE msg;

	if(acl->state==STATE_CONN) //!Cvet ++++
	{
		if(acl->bin.NeedProcess())
		{
			acl->bin >> msg;
		
			switch(msg) 
			{
			case NETMSG_LOGIN:
				Process_GetLogIn(acl);
				break;
			case NETMSG_CREATE_CLIENT:
				Process_CreateClient(acl);
				break;
			default:
				LogExecStr("Неправильное MSG: %d от SockID %d при приеме LOGIN или CREATE_CCritter!\n",msg,acl->s);
				acl->state=STATE_DISCONNECT;
				Send_LoginMsg(acl,8);
				acl->bin.Reset(); //!Cvet при неправильном пакете данных  - удаляеться весь список
				return;
			}
		}
		acl->bin.Reset();
		return;
	} //!Cvet ----

	if(acl->state==STATE_LOGINOK) //!Cvet ++++
	{
		while(acl->bin.NeedProcess())
		{
			acl->bin >> msg;
		
			switch(msg) 
			{
			case NETMSG_SEND_GIVE_ME_MAP:
				Send_Map(acl,acl->info.map);
				break;
			case NETMSG_SEND_LOAD_MAP_OK:
				Process_MapLoaded(acl);
				break;
			default:
				LogExecStr("Неправильное MSG: %d от SockID %d при STATE_LOGINOK!\n",msg,acl->s);
		//		acl->state=STATE_DISCONNECT;
		//		Send_LoginMsg(acl,8);
		//		acl->bin.Reset(); //!Cvet при неправильном пакете данных  - удаляеться весь список
				continue;
			}
		}
		acl->bin.Reset();
		return;
	} //!Cvet ----

	//!Cvet если игрок мертв
	if(acl->info.cond!=COND_LIFE)
	{
		acl->bin.Reset();
		return;
	}

	while(acl->bin.NeedProcess())
	{
		acl->bin >> msg;
		
		switch(msg) 
		{
		case NETMSG_TEXT:
			Process_GetText(acl);
			break;
		case NETMSG_DIR:
			Process_Dir(acl);
			break;
		case NETMSG_SEND_MOVE:
			Process_Move(acl);
			break;
		case NETMSG_SEND_USE_OBJECT: //!Cvet
			Process_UseObject(acl);
			break;
		case NETMSG_SEND_PICK_OBJECT: //!Cvet
			Process_PickObject(acl);
			break;
		case NETMSG_SEND_CHANGE_OBJECT: //!Cvet
			Process_ChangeObject(acl);
			break;
		case NETMSG_SEND_USE_SKILL: //!Cvet
			Process_UseSkill(acl);
			break;
		case NETMSG_SEND_TALK_NPC: //!Cvet
			Process_Talk_NPC(acl);
			break;
		case NETMSG_SEND_GET_TIME: //!Cvet
			Send_GameTime(acl);
			break;
//		case NETMSG_SEND_GIVE_ME_MAP: //!Cvet
//			Send_Map(acl,acl->info.map);
//			break;
//		case NETMSG_SEND_LOAD_MAP_OK: //!Cvet
//			Process_MapLoaded(acl);
//			break;
		case NETMSG_SEND_GIVE_GLOBAL_INFO:
			Process_GiveGlobalInfo(acl);
			break;
		case NETMSG_SEND_RULE_GLOBAL:
			Process_RuleGlobal(acl);
			break;
		default:
			LogExecStr("Wrong MSG: %d from SockID %d при приеме игровых сообщений!\n",msg,acl->s);
			//acl->state=STATE_DISCONNECT;
			acl->bin.Reset(); //!Cvet при неправильном пакете данных  - удаляеться весь список
			return;
		}
	}
	acl->bin.Reset();
}

void CServer::Process_GetText(CCritter* acl)
{
	uint16_t len;
	char str[MAX_TEXT+1];

	acl->bin >> len;

// 	if(acl->state!=STATE_GAME)
	if(acl->bin.IsError() || len>MAX_TEXT)
	{
		LogExecStr("Wrong MSG data forProcess_GetText from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	acl->bin.Read(str,len);
	str[len]=0;

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data forProcess_GetText - partial recv from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

//	LogExecStr("GetText: %s\n",str);

	char* param;
	char* next;

	uint16_t self_len=0;
	uint16_t o_len=0;
	char self_str[MAX_TEXT+255+1]="";
	char o_str[MAX_TEXT+255+1]="";
	char mname[MAX_NAME+1];

//!Cvet переделал концепцию +++++++++++++++++++++++++
	uint16_t cmd=CMD_SAY;
	uint8_t say_param=SAY_NORM;

	if(str[0]=='~')
	{
		param=GetParam(&str[1],&next);
		if(!param)
		{
			strcpy(self_str,"Write command");
			cmd=0xFFFF;
		}
		else if(!(cmd=GetCmdNum(param,acl->info.access)))
		{
			strcpy(self_str,"wrong command or access denied");
			cmd=0xFFFF;
		}
	}
	else
		next=str;

	switch(cmd)
	{
	case CMD_SAY:
		if(next[0]=='/' || next[0]=='.') //??? next[0]=='!'
		{
			next++;
			if(!next)
			{
				strcpy(self_str, "эээ?!");
				break;
			}

			if(next[0]=='к' || next[0]=='К' || next[0]=='s' || next[0]=='S') say_param=SAY_SHOUT;
//			else if(next[0]=='о' || next[0]=='О' || next[0]=='m' || next[0]=='M') say_param=SAY_MSHOUT;
			else if(next[0]=='э' || next[0]=='Э' || next[0]=='e' || next[0]=='E') say_param=SAY_EMOTE;
			else if(next[0]=='ш' || next[0]=='Ш' || next[0]=='w' || next[0]=='W') say_param=SAY_WHISP;
			else if(next[0]=='с' || next[0]=='С' || next[0]=='$') say_param=SAY_SOCIAL;
		}

		if(say_param!=SAY_NORM)
		{
			next++;
			if(next[0]==' ') next++;
		}

		switch(say_param)
		{
		case SAY_NORM:
			if(!next)
				strcpy(self_str, "А чего сказать то?!");
			else
			{
				sprintf(self_str, "Вы: %s",next);
				sprintf(o_str, "%s: %s",MakeName(acl->info.name,mname),next);
			//	sprintf(o_str, "%s",next);
			}

			if(acl->info.map)
				SendA_Text(acl,&acl->vis_cl,self_str,o_str,say_param);
			else
				SendA_Text(acl,&acl->group_move->crit_move,self_str,o_str,say_param);
			break;
		case SAY_SHOUT:
			if(!next)
				strcpy(self_str, "Покричу, только скажи что?!");
			else
			{
				sprintf(self_str, "Вы закричали: !!!%s!!!",my_strupr(next));
				sprintf(o_str, "%s закричал%s: !!!%s!!!",MakeName(acl->info.name,mname),(acl->info.st[ST_GENDER]==0)?"":"а",my_strupr(next));
			//	sprintf(self_str, "!!!%s!!!",my_strupr(next));
			//	sprintf(o_str, "!!!%s!!!",next);
			}

			if(acl->info.map)
				SendA_Text(acl,&map_cr[acl->info.map],self_str,o_str,say_param);
			else
				SendA_Text(acl,&acl->group_move->crit_move,self_str,o_str,say_param);
			break;
//		case SAY_MSHOUT:
//			if(!next)
//				strcpy(self_str, "Что орем?!");
//			else
//			{
//				sprintf(self_str, "Вы заорали: !!!%s!!!",my_strupr(next));							//!Cvet изм. .gender=='m'
//				sprintf(o_str, "%s заорал%s: !!!%s!!!",MakeName(acl->info.name,mname),(acl->info.st[ST_GENDER]==0)?"":"а",next);
//			}
//			break;
		case SAY_EMOTE:
			if(!next)
				strcpy(self_str, "Никаких эмоций!");
			else
			{
				sprintf(self_str, "**%s %s**",MakeName(acl->info.name,mname),next);
				sprintf(o_str, "**%s %s**",mname,next);
			}

			if(acl->info.map)
				SendA_Text(acl,&acl->vis_cl,self_str,o_str,say_param);
			else
				SendA_Text(acl,&acl->group_move->crit_move,self_str,o_str,say_param);
			break;
		case SAY_WHISP: //добавил шепет
			if(!next)
				strcpy(self_str, "Че шептать будем?...");
			else
			{
				sprintf(self_str, "Вы прошептали: ...%s...",my_strlwr(next));							//!Cvet изм. .gender=='m'
				sprintf(o_str, "%s прошептал%s: ...%s...",MakeName(acl->info.name,mname),(acl->info.st[ST_GENDER]==0)?"":"а",my_strlwr(next));
			//	sprintf(self_str, "...%s...",my_strlwr(next));
			//	sprintf(o_str, "...%s...",next);
			}

			if(acl->info.map)
				SendA_Text(acl,&acl->vis_cl,self_str,o_str,say_param);
			else
				SendA_Text(acl,&acl->group_move->crit_move,self_str,o_str,say_param);
			break;
		case SAY_SOCIAL:
			int socid=GetSocialId(next);
			if(socid>=0)
			{
				ProcessSocial(acl,socid,next);
				return;
			}
			else
				strcpy(self_str, "Хмм?!");

			if(acl->info.map)
				SendA_Text(acl,&acl->vis_cl,self_str,o_str,say_param);
			else
				SendA_Text(acl,&acl->group_move->crit_move,self_str,o_str,say_param);
			break;
		}
		break;
	case CMD_EXIT: //выход ~exit
		LogExecStr("CMD_EXIT for %s\n",acl->info.name);
		acl->state=STATE_DISCONNECT;
		break;
	case CMD_CRITID: //узнать ИД криттера по его имени ~id name -> crid/"false"
		//эту функцию надо перенести в клиента!
		if(next) strcpy(self_str,next);
		break;
	case CMD_MOVECRIT: //двигать криттера ~move id x y -> "ok"/"false"
		if(next) strcpy(self_str,next);
		break;
	case CMD_KILLCRIT: //убить криттера ~kill id -> "ok"/"false"
		if(next) strcpy(self_str,next);
		break;
	case CMD_DISCONCRIT: //отсоединить криттера ~disconnect id -> "ok"/"false"
		if(next) strcpy(self_str,next);
		break;
	case CMD_TOGLOBAL: //отсоединить криттера ~disconnect id -> "ok"/"false"
		if(TransitCr(acl,0,0,0,0)==TR_OK)
		{
			GM_GroupStartMove(acl);
			strcpy(self_str,"To Global - OK");
		}	
		else
			strcpy(self_str,"To Global - FALSE");

		Send_Text(acl,self_str,SAY_NORM);
		break;
	case 0xFFFF:
		break;
	default:
		return;
	}
//!Cvet ------------------------------------------

//	LogExecStr("self: %s\not: %s\n",self_str,o_str);
}

void CServer::ProcessSocial(CCritter* sender,uint16_t socid,char* aparam)
{
	char* param;
	char* next;

	uint16_t self_len=0;
	uint16_t vic_len=0;
	uint16_t all_len=0;

	char SelfStr[MAX_TEXT+255+1]="";
	char VicStr[MAX_TEXT+255+1]="";
	char AllStr[MAX_TEXT+255+1]="";
	
	CCritter* victim=NULL;
	param=GetParam(aparam,&next);

//	LogExecStr("ProcessSocial: %s\n",param?param:"NULL");
	
	if(param && param[0] && GetPossParams(socid)!=SOC_NOPARAMS)
	{
		my_strlwr(param);
		if(!strcmp(param,"я") && GetPossParams(socid)!=SOC_NOSELF)
		{
			GetSocSelfStr(socid,SelfStr,AllStr,&sender->info);
		}
		else
			{
				victim=LocateByPartName(param);
				if(!victim) 
					GetSocVicErrStr(socid,SelfStr,&sender->info);
				else
					GetSocVicStr(socid,SelfStr,VicStr,AllStr,&sender->info,&victim->info);
			}
	}
	else
		GetSocNoStr(socid,SelfStr,AllStr,&sender->info);

//	LogExecStr("self: %s\nvic: %s\nall: %s\n",SelfStr,VicStr,AllStr);
	self_len=strlen(SelfStr);
	vic_len=strlen(VicStr);
	all_len=strlen(AllStr);

	MSGTYPE msg=NETMSG_CRITTERTEXT;

	CCritter* c;
	for(cl_map::iterator it=cl.begin();it!=cl.end();it++)
	{
		c=(*it).second;

		if(c==sender && self_len)
		{
			c->bout << msg;
			c->bout << sender->info.id;
			c->bout << (uint8_t)(SAY_SOCIAL);
			c->bout << self_len;
			c->bout.Write(SelfStr,self_len);
		}
		else if(c==victim && vic_len)
		{
			c->bout << msg;
			c->bout << sender->info.id;
			c->bout << (uint8_t)(SAY_SOCIAL);
			c->bout << vic_len;
			c->bout.Write(VicStr,vic_len);
		}
		else if(all_len)
		{
			c->bout << msg;
			c->bout << sender->info.id;
			c->bout << (uint8_t)(SAY_SOCIAL);
			c->bout << all_len;
			c->bout.Write(AllStr,all_len);
		}
	}
}

CCritter* CServer::LocateByPartName(char* name)
{
	bool found=0;
	CCritter* c;
	for(cl_map::iterator it=cl.begin();it!=cl.end();it++)
	{
		c=(*it).second;

		if(PartialRight(name,c->info.name)) 
		{
			found=1;
			break;
		}
	}

	return found?c:NULL;
}

int CServer::Output(CCritter* acl)
{

	if(!acl->bout.writePosition) return 1;

	if(acl->bout.capacity>=outLEN)
	{
		while(acl->bout.capacity>=outLEN) outLEN<<=1;
		SAFEDELA(outBUF);
		outBUF=new char[outLEN];
	}

	acl->zstrm.next_in=(UCHAR*)acl->bout.data;
	acl->zstrm.avail_in=acl->bout.writePosition;
	acl->zstrm.next_out=(UCHAR*)outBUF;
	acl->zstrm.avail_out=outLEN;

	DWORD br;
	WriteFile(hDump,acl->bout.data,acl->bout.writePosition,&br,NULL);

	if(deflate(&acl->zstrm,Z_SYNC_FLUSH)!=Z_OK)
	{
		LogExecStr("Deflate error forSockID=%d\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return 0;
	}

	int tosend=acl->zstrm.next_out-(UCHAR*)outBUF;
	LogExecStr("idchannel=%d, send %d->%d bytes\n",acl->info.idchannel,acl->bout.writePosition,tosend);
	int sendpos=0;
	while(sendpos<tosend)
	{
		int bsent=send(acl->s,outBUF+sendpos,tosend-sendpos,0);
		sendpos+=bsent;
		if(bsent==SOCKET_ERROR)
		{
			LogExecStr("SOCKET_ERROR whilesend forSockID=%d\n",acl->s);
			acl->state=STATE_DISCONNECT;
			return 0;
		}
	}

	acl->bout.Reset();

	return 1;
}

int CServer::Init()
{
	if(Active) return 1;

	Active=0;

	LogExecStr("***   Starting initialization   ****\n");

	hDump=CreateFile(".\\dump.dat",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);
	WSADATA WsaData;
	if(WSAStartup(0x0101,&WsaData))
	{
		LogExecStr("WSAStartup error!");
		goto SockEND;
	}
	s=socket(AF_INET,SOCK_STREAM,0);

	UINT port;
	port=GetPrivateProfileInt("server","port",4000,".\\foserv.cfg");
	
	SOCKADDR_IN sin;
	sin.sin_family=AF_INET;
	sin.sin_port=htons(port);
	sin.sin_addr.s_addr=INADDR_ANY;

	LogExecStr("Starting local server on port %d: ",port);

	if(bind(s,(sockaddr*)&sin,sizeof(sin))==SOCKET_ERROR)
	{
		LogExecStr("Bind error!");
		goto SockEND;
	}

	LogExecStr("OK\n");

	if(listen(s,5)==SOCKET_ERROR)
	{
		LogExecStr("listen error!");
		goto SockEND;
	}

	if(!sql.Init_mySQL())
		goto SockEND;

	LoadSocials(sql.mySQL);

//!Cvet ++++++++++++++++++++++++++++++++++++++++
	LogExecStr("cr=%d\n",sizeof(CCritter));
	LogExecStr("ci=%d\n",sizeof(crit_info));
	LogExecStr("mi=%d\n",sizeof(mob_info));

	//if(!InitScriptSystem())
	//{
	//	LogExecStr("Script System Init FALSE\n");
	//	goto SockEND;
	//}

	CreateParamsMaps();

	//шаблоны варов игроков
	if(UpdateVarsTemplate()) goto SockEND;

	//файл-менеджер
	if(!fm.Init()) goto SockEND;

	//карты
	if(!LoadAllMaps()) goto SockEND;

	//загрузка объектов
	if(!LoadAllStaticObjects())
	{
		LogExecStr("Загрузка статических объектов прошла со сбоями!!!\n");
		goto SockEND;
	}
	//создаем всех клиентов
	if(!LoadAllPlayers())
	{
		LogExecStr("Создание игроков прошли со сбоями!!!\n");
		goto SockEND;
	}
	//создаем всю динамику
	if(!LoadAllObj())
	{
		LogExecStr("Создание динамических объектов прошла со сбоями!!!\n");
		goto SockEND;
	}
	//загружаем НПЦ
	if(!NPC_LoadAll())
	{
		LogExecStr("Загрузка НПЦ прошла со сбоями!!!\n");
		goto SockEND;
	}
	//загружаем мобов
	if(!MOBs_LoadAllGroups())
	{
		LogExecStr("Загрузка Мобов прошла со сбоями!!!\n");
		goto SockEND;
	}

//	LogExecStr("Создаем объекты\n");
	
//	CreateObjToPl(101,1200);
//	CreateObjToPl(102,1100);
//	CreateObjToPl(102,1200);
//	CreateObjToPl(103,1100);
//	CreateObjToPl(103,1200);
//	CreateObjToPl(104,1100);
//	CreateObjToPl(104,1200);

//	CreateObjToTile(11,61,112,1301);
//	CreateObjToTile(11,61,112,1301);
//	CreateObjToTile(11,61,112,1301);
	CreateObjToTile(11,61,112,1301);

	CreateObjToTile(11,61,113,2016);
//	CreateObjToTile(11,61,113,1301);

//	CreateObjToTile(11,58,114,1301);
	CreateObjToTile(11,64,115,1301);

	CreateObjToTile(11,58,157,7001);

//!Cvet ---------------------------------------

//	sql.PrintTableInLog("objects","*");

	Active=1;
	LogExecStr("***   Initializing complete   ****\n\n");
	return 1;

SockEND:
	closesocket(s);
	ClearClients();
	return 0;
	
}

void CServer::Finish()
{
	if(!Active) return;

	closesocket(s);

	//FinishScriptSystem();
	ClearClients();
	UnloadSocials();
	sql.Finish_mySQL();

	LogExecStr("Server stopped\n");

	Active=0;
}

int CServer::GetCmdNum(char* cmd, uint8_t access_level)
{
//	my_strlwr(cmd);

//	if(!strcmp(cmd,cmdlist[CMD_EXIT].cmd))
//		return CMD_EXIT;

	if(strlen(cmd)>=14) return 0;

	for(int cur_cmd=0;cur_cmd<CMN_LIST_COUNT;cur_cmd++)
		if(!stricmp(cmd,cmdlist[cur_cmd].cmd))
		{
			if(access_level>=cmdlist[cur_cmd].access)
				return cmdlist[cur_cmd].id;
			else
				return 0;
		}

//	for(int i=CMD_EXIT+1;cmdlist[i].cmd[0];i++)
//		if(PartialRight(cmd,(char*)cmdlist[i].cmd))
//			return i;

	return 0;
}

char* CServer::GetParam(char* cmdstr,char** next)
{
	if(!cmdstr)
	{
	 *next=NULL;
	 return NULL;
	}
	
	char* ret=NULL;
	int stop=0;
	int i;
	for(i=0;cmdstr[i];i++)
		if(cmdstr[i]!=' ') break;
	if(!cmdstr[i]) //нет первого параметра
	{
		*next=NULL;
		return ret;
	}
	ret=cmdstr+i;
	stop=i+1;
	for(i=stop;cmdstr[i];i++)
		if(cmdstr[i]==' ') break;
	if(!cmdstr[i]) //нет следующего параметра
	{
		*next=NULL;
		return ret;
	}
	cmdstr[i]=0;
	stop=i+1;
	for(i=stop;cmdstr[i];i++)
		if(cmdstr[i]!=' ') break;
	
	*next=cmdstr[i]?cmdstr+i:NULL;
	return ret;
}

char* CServer::my_strlwr(char* str)
{
	strlwr(str);
	for(int i=0;str[i];i++)
		if(str[i]>='А' && str[i]<='Я') str[i]+=0x20;
	return str;
}

char* CServer::my_strupr(char* str)
{
	strupr(str);
	for(int i=0;str[i];i++)
		if(str[i]>='а' && str[i]<='я') str[i]-=0x20;
	return str;
}


int CServer::PartialRight(char* str,char* et)
{
	int res=1;

	for(int i=0;str[i];i++)
		if(!et[i] || str[i]!=et[i]) return 0;
		
	return res;
}

char* CServer::MakeName(char* str,char* res)
{
	strcpy(res,str);
	res[0]-=0x20;
	return res;
}


void *zlib_alloc(void *opaque, unsigned int items, unsigned int size)
{
    return calloc(items, size);
}

void zlib_free(void *opaque, void *address)
{
    free(address);
}

//!Cvet ++++++++++++++++++++++++++++++++++++
int CServer::DistFast(int dx, int dy)
{
	if(dx<0) dx=-dx;
	if(dy<0) dy=-dy;
	if(dx<dy) return (123*dy+51*dx)/128;  
	else return (123*dx+51*dy)/128;
}

int CServer::DistSqrt(int dx, int dy)
{
	return (int) sqrt((double) dx*dx + (double) dy*dy);
}

void CServer::SetCheat(CCritter* acl, char* cheat_message)
{
	sql.AddCheat(acl->info.id,cheat_message);
}

//!Cvet ------------------------------------