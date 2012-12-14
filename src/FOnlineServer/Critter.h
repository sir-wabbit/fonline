/********************************************************************
	created:	17:03:2007   08:32;

	author:		Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

#ifndef __CCRITTER_H__
#define __CCRITTER_H__

#include <vector>
#include <list>
#include <map>

#include "netproto.h"
#include <zlib/zlib.h>
#include <FOnlineCommon/buffer.hpp>
//#include "sql.h"


#define TALK_MAX_TIME 60000

const uint8_t NOT_ANSWER_CLOSE_DIALOG	=0;
const uint8_t NOT_ANSWER_BEGIN_BATTLE	=1;

//Выборка
const char CHOOSE_PLAYER		=0;
const char CHOOSE_ALL			=1;
const char CHOOSE_ALL_NOT_PLAYER=2;

//Условие
const uint8_t DEMAND_NONE		=0;
const uint8_t DEMAND_STAT		=1;
const uint8_t DEMAND_SKILL		=2;
const uint8_t DEMAND_PERK		=3;
const uint8_t DEMAND_QUEST		=4;
const uint8_t DEMAND_ITEM		=5;
const uint8_t DEMAND_VAR		=6;
const uint8_t DEMAND_PVAR		=7;

struct demand
{
	demand():type(DEMAND_NONE),param(0),var_num(0),oper('>'),count(0){};

	uint8_t type;

	uint8_t param;
	uint16_t var_num; //quest_num
	std::string var_name;
	char oper; //choose
	int count;
};

typedef std::vector<demand*> demand_list;

//Результат
const uint8_t RESULT_NONE		=0;
const uint8_t RESULT_STAT		=1;
const uint8_t RESULT_SKILL		=2;
const uint8_t RESULT_PERK		=3;
const uint8_t RESULT_QUEST		=4;
const uint8_t RESULT_ITEM		=5;
const uint8_t RESULT_VAR		=6;
const uint8_t RESULT_PVAR		=7;
const uint8_t RESULT_LOCK		=8;

struct result
{
	result():type(RESULT_NONE),param(0),var_num(0),oper('+'),count(0){};

	uint8_t type;

	uint8_t param;
	uint16_t var_num;
	std::string var_name;
	char oper;
	int count;
};

typedef std::vector<result*> result_list;

struct answer
{
	answer():link(0),id_text(0){};
//	~answer(){SAFEDELA(demands);SAFEDELA(results);};

	uint32_t link;
	uint32_t id_text;

	demand_list demands;

	result_list results;
};

typedef std::vector<answer*> answers_list;

struct npc_dialog
{
	npc_dialog():id(0),id_text(0),time_break(TALK_MAX_TIME),not_answer(NOT_ANSWER_CLOSE_DIALOG){};
//	~npc_dialog(){SAFEDELA(answers);};

	uint32_t id;

	uint32_t id_text;
	answers_list answers;

	UINT time_break; //время на прочтение диалога игроком
	uint8_t not_answer; //что делать если нет ответа
};

typedef std::map<uint32_t, npc_dialog*, std::less<uint32_t> > dialogs_map;

struct npc_info
{
//ид игрока с которым он сейчас разговаривает
	CritterID talking;
//диалоги
	dialogs_map dialogs;
//текущий скампонованный диалог
	npc_dialog compiled_dialog;

	npc_info():talking(0){};
	//~npc_info(){SAFEDELA(dialogs);};
};

const uint8_t MOB_COND_PASSIVE			=0;
const uint8_t MOB_COND_AGGRESSIVE		=1;
const uint8_t MOB_COND_DEFENCE			=2;

struct mob_info
{
  std::string name;
	uint16_t num_group;
	uint8_t base_cond;
	uint8_t cond;

	mob_info():name(""),num_group(0),base_cond(0),cond(0){};
};

class CCritter;
//typedef vector<CCritter*> cl_vec;
typedef std::map<CritterID, CCritter*, std::less<CritterID> > cl_map;
//typedef list<CCritter*> cl_list;

typedef std::set<CritterID> crid_set;
typedef std::set<uint16_t> word_set;

struct mobs_group
{
	uint16_t num;

	cl_map mobs;

	uint8_t mobs_level;
	uint8_t mobs_count;

	mobs_group():num(0),mobs_level(0),mobs_count(0){};
};

typedef std::list<mobs_group*> mobs_group_list;

struct gmap_group
{
	crid_set prep_crit;

	cl_map crit_move;
	uint8_t players;

	CCritter* rule;

	int crits_luck;
	int crits_pathfind;

	int xi;
	int yi;
	float xf;
	float yf;
	int move_x;
	int move_y;
	uint8_t speed;
	float speedx;
	float speedy;

	uint32_t last_encaunter;

	gmap_group():crits_luck(0),crits_pathfind(0),xi(0),yi(0),xf(0),yf(0),players(0),
	move_x(0),move_y(0),speed(0),speedx(0),speedy(0),rule(0),last_encaunter(0){};
	~gmap_group(){prep_crit.clear();crit_move.clear();rule=NULL;};

	//void SaveGroup();
	//void LoadGroup();
};

struct gmap_group;
typedef std::vector<gmap_group*> gmap_group_vec;

class CCritter
{
public:
	int InitClient();
	int InitNPC();
	int InitMOB();

	SOCKET s; // Socket id
	SOCKADDR_IN from;

  fonline::Buffer bin; // буфер входящий
	fonline::Buffer bout; // буфер исходящий

	uint16_t state; // состояние
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

	int CheckKnownCity(uint16_t city_num){if(known_cities.count(city_num)) return 1; return 0;};
	int AddKnownCity(uint16_t city_num){if(known_cities.count(city_num)) return 0; known_cities.insert(city_num); return 1;};
	int DelKnownCity(uint16_t city_num){if(!known_cities.count(city_num)) return 0; known_cities.erase(city_num); return 1;};
	//int LoadAllKnownCitySQL(); //при старте сервера
	//int SaveKnownCitySQL(uint16_t city_num);
	//int EraseKnownCitySQL(uint16_t city_num);

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