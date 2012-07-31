/********************************************************************
	created:	17:03:2007   08:32;

	author:		Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

#ifndef __CCRITTER_H__
#define __CCRITTER_H__

#include "netproto.h"
#include <zlib.h>
#include "BufMngr.h"
//#include "sql.h"


#define TALK_MAX_TIME 60000

const BYTE NOT_ANSWER_CLOSE_DIALOG	=0;
const BYTE NOT_ANSWER_BEGIN_BATTLE	=1;

//Выборка
const char CHOOSE_PLAYER		=0;
const char CHOOSE_ALL			=1;
const char CHOOSE_ALL_NOT_PLAYER=2;

//Условие
const BYTE DEMAND_NONE		=0;
const BYTE DEMAND_STAT		=1;
const BYTE DEMAND_SKILL		=2;
const BYTE DEMAND_PERK		=3;
const BYTE DEMAND_QUEST		=4;
const BYTE DEMAND_ITEM		=5;
const BYTE DEMAND_VAR		=6;
const BYTE DEMAND_PVAR		=7;

struct demand
{
	demand():type(DEMAND_NONE),param(0),var_num(0),oper('>'),count(0){};

	BYTE type;

	BYTE param;
	WORD var_num; //quest_num
	string var_name;
	char oper; //choose
	int count;
};

typedef vector<demand*> demand_list;

//Результат
const BYTE RESULT_NONE		=0;
const BYTE RESULT_STAT		=1;
const BYTE RESULT_SKILL		=2;
const BYTE RESULT_PERK		=3;
const BYTE RESULT_QUEST		=4;
const BYTE RESULT_ITEM		=5;
const BYTE RESULT_VAR		=6;
const BYTE RESULT_PVAR		=7;
const BYTE RESULT_LOCK		=8;

struct result
{
	result():type(RESULT_NONE),param(0),var_num(0),oper('+'),count(0){};

	BYTE type;

	BYTE param;
	WORD var_num;
	string var_name;
	char oper;
	int count;
};

typedef vector<result*> result_list;

struct answer
{
	answer():link(0),id_text(0){};
//	~answer(){SAFEDELA(demands);SAFEDELA(results);};

	DWORD link;
	DWORD id_text;

	demand_list demands;

	result_list results;
};

typedef vector<answer*> answers_list;

struct npc_dialog
{
	npc_dialog():id(0),id_text(0),time_break(TALK_MAX_TIME),not_answer(NOT_ANSWER_CLOSE_DIALOG){};
//	~npc_dialog(){SAFEDELA(answers);};

	DWORD id;

	DWORD id_text;
	answers_list answers;

	UINT time_break; //время на прочтение диалога игроком
	BYTE not_answer; //что делать если нет ответа
};

typedef map<DWORD, npc_dialog*, less<DWORD> > dialogs_map;

struct npc_info
{
//ид игрока с которым он сейчас разговаривает
	CrID talking;
//диалоги
	dialogs_map dialogs;
//текущий скампонованный диалог
	npc_dialog compiled_dialog;

	npc_info():talking(0){};
	//~npc_info(){SAFEDELA(dialogs);};
};

const BYTE MOB_COND_PASSIVE			=0;
const BYTE MOB_COND_AGGRESSIVE		=1;
const BYTE MOB_COND_DEFENCE			=2;

struct mob_info
{
	string name;
	WORD num_group;
	BYTE base_cond;
	BYTE cond;

	mob_info():name(""),num_group(0),base_cond(0),cond(0){};
};

class CCritter;
//typedef vector<CCritter*> cl_vec;
typedef map<CrID, CCritter*, less<CrID> > cl_map;
//typedef list<CCritter*> cl_list;

typedef set<CrID> crid_set;
typedef set<WORD> word_set;

struct mobs_group
{
	WORD num;

	cl_map mobs;

	BYTE mobs_level;
	BYTE mobs_count;

	mobs_group():num(0),mobs_level(0),mobs_count(0){};
};

typedef list<mobs_group*> mobs_group_list;

struct gmap_group
{
	crid_set prep_crit;

	cl_map crit_move;
	BYTE players;

	CCritter* rule;

	int crits_luck;
	int crits_pathfind;

	int xi;
	int yi;
	float xf;
	float yf;
	int move_x;
	int move_y;
	BYTE speed;
	float speedx;
	float speedy;

	DWORD last_encaunter;

	gmap_group():crits_luck(0),crits_pathfind(0),xi(0),yi(0),xf(0),yf(0),players(0),
	move_x(0),move_y(0),speed(0),speedx(0),speedy(0),rule(0),last_encaunter(0){};
	~gmap_group(){prep_crit.clear();crit_move.clear();rule=NULL;};

	//void SaveGroup();
	//void LoadGroup();
};

struct gmap_group;
typedef vector<gmap_group*> gmap_group_vec;

class CCritter
{
public:
	int InitClient();
	int InitNPC();
	int InitMOB();

	SOCKET s; // Socket id
	SOCKADDR_IN from;

	CBufMngr bin; // буфер входящий
	CBufMngr bout; // буфер исходящий

	WORD state; // состояние
	z_stream zstrm; // поток

	crit_info info; //Общая инфа криттера
	npc_info* i_npc; //инфа НПЦ
	mob_info* i_mob; //инфа моба

	cl_map vis_cl; //клиенты ктором мы отпровляем наши действия

	gmap_group group; //группа клиента
	gmap_group* group_move; //текущая группа на гм
	word_set known_cities;

	CCritter* target;

	inline void GenParam();
	inline void GenLook();

	int CheckKnownCity(WORD city_num){if(known_cities.count(city_num)) return 1; return 0;};
	int AddKnownCity(WORD city_num){if(known_cities.count(city_num)) return 0; known_cities.insert(city_num); return 1;};
	int DelKnownCity(WORD city_num){if(!known_cities.count(city_num)) return 0; known_cities.erase(city_num); return 1;};
	//int LoadAllKnownCitySQL(); //при старте сервера
	//int SaveKnownCitySQL(WORD city_num);
	//int EraseKnownCitySQL(WORD city_num);

	int IsFree(){if(GetTickCount()-info.start_bt>=info.break_time) return 1; return 0;};
	int IsBusy(){if(GetTickCount()-info.start_bt>=info.break_time) return 0; return 1;};
	void BreakTime(int ms){info.break_time=ms; info.start_bt=GetTickCount();};

//	inline void Skill_Sneak_Change();
//	inline void Skill_Sneak_Set();
//	inline void Skill_Sneak_UnSet();

	void CopyParamsTo(CCritter* acl);

	CCritter();
	~CCritter();
private:

};

#endif //__CCRITTER_H__