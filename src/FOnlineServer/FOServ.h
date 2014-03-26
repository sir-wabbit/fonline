#pragma once

/********************************************************************
  created:  18:08:2004   23:48; edited: 2006,2007

  author:   Oleg Mareskin
  add/edit: Denis Balihin, Anton Tsvetinsky

  purpose:
*********************************************************************/

#include "stdafx.h"
#include "main.h"
#include "netproto.h"

//!Cvet ++++
#include "SQL.h"
#include "Critter.h"
#include <FOnlineFileManager/FileManager.hpp>

#define PATH_OBJECTS ".\\objects\\"
#define PATH_NPC ".\\npc\\"
#define PATH_MOB ".\\mob\\"
#define PATH_SCRIPTS ".\\scripts\\"
//!Cvet ----

//==============================================================================================================================
//*****  MAPS DATA by Cvet  *** begin ******************************************************************************************
//==============================================================================================================================

#define MAX_CITIES    50
#define MAX_ENCAUNTERS  100
#define MAX_MAPS    200
#define MAXHEXX 400
#define MAXHEXY 400

//Mobs
#define MOBS_MAX_GROUPS   (100+1) // 0 не производиться


//Move Results
#define MR_FALSE    0
#define MR_STEP     1
#define MR_TRANSIT    2
#define MR_TOGLOBAL   3

//Transit Results
#define TR_FALSE    0
#define TR_OK     1

//Encaunters
#define ENCAUNTERS_MAX_INGAME 100
#define ENCAUNTERS_MAX_GROUPS 10


#define ENCAUNTERS_TIME     10000

#define ENCAUNTERS_MIN_TIME   40000

#define ENCAUNTERS_GEN_TIME(x)  (300000/(x))
#define ENCAUNTERS_GROUPS(x)  ((x)/4+1)

//Encaunters District
#define DISTRICT_WESTLAND 0x1
#define DISTRICT_MOUNTAINS  0x2
#define DISTRICT_FOREST   0x4
#define DISTRICT_OCEAN    0x8


struct tile_info
{
  uint16_t flags;

  tile_info():flags(0){};
};

struct city_info;
struct encaunter_info;

struct map_info
{
  uint16_t num;
  uint16_t flags[MAXHEXX][MAXHEXY];

  uint16_t wx;
  uint16_t wy;

  HexTYPE start_hex_x;
  HexTYPE start_hex_y;
  uint8_t start_ori;

  city_info* city;
  encaunter_info* encaunter;

  map_info():num(0),wx(0),wy(0),city(NULL),encaunter(NULL),start_hex_x(0),start_hex_y(0),start_ori(0)
    {for(int i=0;i<MAXHEXX;++i)for(int i2=0;i2<MAXHEXY;++i2)flags[i][i2]=0;};
//  map_info(uint16_t _num, uint16_t _wx,uint16_t _wy,city_info* _city,HexTYPE _start_hex_x,HexTYPE _start_hex_y,uint8_t _start_ori):
//    num(_num),wx(_wx),wy(_wy),city(_city),start_hex_x(_start_hex_x),start_hex_y(_start_hex_y),start_ori(_start_ori)
//    {for(int i=0;i<MAXHEXX;++i)for(int i2=0;i2<MAXHEXY;++i2)flags[i][i2]=0;};
};

typedef std::vector<map_info*> map_vec;

struct gmap_zone;

struct city_info
{
  //for city
  uint16_t num;
  uint16_t wx;
  uint16_t wy;
  uint8_t radius;
  uint16_t steal;

  int population;
  int life_level;

  map_vec maps;

  city_info():num(0),wx(0),wy(0),radius(0),population(0),life_level(0),steal(0){};
//  city_info(uint16_t _num,uint16_t _wx,uint16_t _wy,uint8_t _radius,int _population,int _life_level):num(_num),wx(_wx),wy(_wy),radius(_radius),
//    population(_population),life_level(_life_level){};
  ~city_info(){maps.clear();};//!!!del for maps
};

typedef std::vector<city_info*> city_vec;

struct gmap_zone
{
  gmap_group_vec groups;

  city_vec cities;

  uint16_t district;
  uint16_t mobs_power;

  uint32_t last_tick;

  int worn_lev;
  uint16_t zx;
  uint16_t zy;

  gmap_zone():last_tick(0),worn_lev(0),zx(0),zy(0),district(DISTRICT_WESTLAND),mobs_power(1){};
  ~gmap_zone(){groups.clear();cities.clear();};
};

struct encaunter_info
{
  uint16_t num;
  uint16_t district;
  uint16_t max_groups;
  uint16_t count_groups;

  HexTYPE start_hx[ENCAUNTERS_MAX_GROUPS];
  HexTYPE start_hy[ENCAUNTERS_MAX_GROUPS];
  uint8_t start_ori[ENCAUNTERS_MAX_GROUPS];

  map_info* emap;

  encaunter_info():num(0),district(DISTRICT_WESTLAND),max_groups(0),count_groups(0),emap(NULL){};
  ~encaunter_info(){
    // Apparently map info is not owned by encAunter_info,
    // thus no need to free it.

    //if (emap != NULL) {
    //  delete emap;
    //  emap = NULL;
    //}
  };

};

struct gmap_dot
{
  uint8_t relief;

  gmap_dot():relief(100){};
};

typedef std::map<uint16_t, std::string> map_str_map;

typedef std::map<DHexTYPE, int64_t> longlong_map;

typedef std::map<DHexTYPE, dyn_map*> dyn_map_map; //контейнер контейнеров

//==============================================================================================================================
//*****  MAPS DATA by Cvet  *** end ********************************************************************************************
//==============================================================================================================================

//!Cvet +++++++++++++++++++++++++++++++++++++++++++
//макросы для работы с параметрами
#define CHANGE_STAT(CCritter,stat,oper,count) \
  { \
    CCritter -> info.st[(stat)] oper (count); \
    if (CCritter -> info.st[(stat)] > 9999) CCritter -> info.st[(stat)] = 9999;\
    if (GetBits(CCritter -> info.flags, FCRIT_PLAYER) && !GetBits(CCritter -> info.flags, FCRIT_DISCONNECT)) { \
      Send_Param(CCritter,TYPE_STAT,(stat)); \
    } \
  }

#define CHANGE_SKILL(CCritter,skill,oper,count) {CCritter##->info.sk[(skill)]##oper##(count);\
if(CCritter##->info.sk[(skill)]>999) CCritter##->info.sk[(skill)]=999;\
if(GetBits(CCritter##->info.flags,FCRIT_PLAYER)&&!GetBits(CCritter##->info.flags,FCRIT_DISCONNECT))\
Send_Param(CCritter,TYPE_SKILL,(skill));}
#define CHANGE_PERK(CCritter,perk,oper,count) {CCritter##->info.pe[(perk)]##oper##(count);\
if(CCritter##->info.pe[(perk)]>9) CCritter##->info.pe[(perk)]=9;\
if(GetBits(CCritter##->info.flags,FCRIT_PLAYER)&&!GetBits(CCritter##->info.flags,FCRIT_DISCONNECT))\
Send_Param(CCritter,TYPE_PERK,(perk));}

//дистанция
#define DISTANCE(cl1,cl2) (sqrt(((cl1##->info.x-cl2##->info.x)*(cl1##->info.x-cl2##->info.x))+\
((cl1##->info.y-cl2##->info.y)*(cl1##->info.y-cl2##->info.y))))

//!Cvet -------------------------------------------

class CServer
{
//==============================================================================================================================
//*****  SCRIPTING  by Cvet (using AngelScript)  *******************************************************************************
//==============================================================================================================================

  //asIScriptEngine* ss;
//  asCModule ss_mod;

  //static HANDLE hSSLogFile;

  //int InitScriptSystem();
  //void FinishScriptSystem();

  //int SS_LoadScript(char* sname);

  //int Test(lua_State* pLua);

  //int SS_StartLog();
  //void SS_EndLog();
  //static void SS_WriteLog(string& str);

  //static void SS_Callback(const asSMessageInfo *msg, void *param);


//==============================================================================================================================
//*****  CLIENTS  by Cvet  *****************************************************************************************************
//==============================================================================================================================

//Списки криттеров
  cl_map cl; //Карта клиентов
  cl_map cr; //Карта всех криттеров + в картах по картам

//Net proccess
  void Process_MapLoaded(CCritter* acl);
  void Process_Move(CCritter* acl);
  void Process_CreateClient(CCritter* acl);
  void Process_GetLogIn(CCritter* acl);
  void Process_Dir(CCritter* acl);
  void Process_ChangeObject(CCritter* acl);
  void Process_UseObject(CCritter* acl); //заявка на использование объекта
  void Process_PickObject(CCritter* acl);
  void Process_UseSkill(CCritter* acl); //заявка на использование скилла

//Работа с данными
  int LoadAllPlayers(); //загрузка всех клиентов (при запуске сервера)
  void SaveAllDataPlayers(); //сохранение всех данных всех игроков

//Параметры
  void CreateParamsMaps();
  params_map stats_map;
  params_map skills_map;
  params_map perks_map;
  params_map object_map;

  int UpdateVarsTemplate();

//Работа с параметрами
  void GenWear(dyn_obj* wear_obj, bool use_obj);

  void Skill_Sneak_Change(CCritter* acl);
  void Skill_Sneak_Set(CCritter* acl);
  void Skill_Sneak_UnSet(CCritter* acl);

//Действия
  int Act_Attack(CCritter* acl, uint8_t rate_object, CritterID target_id);

  void UseDefObj(CCritter* acl, uint8_t slot);

//Работа с криттерами, чтобы было нормально обращение со скриптов должны быть static
  static int Crit_FindTarget(CCritter* acl, uint8_t flags);
  static int Crit_FreeTarget(CCritter* acl);
  static int Crit_IsTarget(CCritter* acl);
  static int Crit_IsSelfTarget(CCritter* acl);
  static int Crit_MoveToTarget(CCritter* acl);
  static int Crit_PossibleToAttack(CCritter* acl);
  static int Crit_AttackTarget(CCritter* acl);
  static int Crit_IsBusy(CCritter* acl);
  static int Crit_Wait(CCritter* acl, int ms);
  static int Crit_MoveRandom(CCritter* acl);

  static int Crit_GetStat(CCritter* acl, uint8_t num_stat);
  static int Crit_GetSkill(CCritter* acl, uint8_t num_skill);
  static int Crit_GetPerk(CCritter* acl, uint8_t num_perk);


//==============================================================================================================================
//*****  OBJECTS  by Cvet  *****************************************************************************************************
//==============================================================================================================================

//списки
  stat_map all_s_obj; //вся статика в игре
  dyn_map all_obj; //вся динамика объектов в игре

//Текущий ИД
  uint32_t cur_obj_id;

//статика
  void ClearStaticObjects();
  int LoadAllStaticObjects();

//динамика
  int LoadAllObj(); //загрузка динамических объектов из mySQL
  void SaveAllObj(); //запись динамических объектов в mySQL

  void DeleteObj(uint32_t id_obj); //удаление объекта
  void CreateObjToPl(CritterID c_pl_idchannel, uint16_t num_st_obj); //создание объекта на тайле
  void CreateObjToTile(MapTYPE c_map, HexTYPE c_x, HexTYPE c_y, uint16_t num_st_obj); //создание объекта у игрока

  void SetVisibleObj(CCritter* acl);
  int AddObjIntoListInd(CCritter* acl, uint32_t add_ind);
  int DelObjFromListInd(CCritter* acl, uint32_t del_ind);

//обмен объектами
  void TransferDynObjPlPl(CCritter* from_acl, CCritter* to_acl, dyn_obj* obj);
  void TransferDynObjTlPl(CCritter* acl, dyn_obj* obj);
  void TransferDynObjPlTl(CCritter* acl, dyn_obj* obj);

//==============================================================================================================================
//*****  SENDS  by Cvet  *******************************************************************************************************
//==============================================================================================================================

  void Send_AddCritter(CCritter* acl, crit_info* pinfo); //Oleg + Cvet edit
  void Send_RemoveCritter(CCritter* acl, CritterID id); //Oleg

  void Send_LoadMap(CCritter* acl);
  void Send_LoginMsg(CCritter* acl, uint8_t LogMsg);

  void Send_XY(CCritter* acl);
  void SendA_Move(CCritter* acl, uint16_t move_params);

  void SendA_Action(CCritter* acl, uint8_t num_action, uint8_t rate_object);

  void Send_AddObjOnMap(CCritter* acl, dyn_obj* o);
  void SendA_AddObjOnMap(CCritter* acl, dyn_obj* o);
  void Send_ChangeObjOnMap(CCritter* acl, dyn_obj* o);
  void SendA_ChangeObjOnMap(CCritter* acl, dyn_obj* o);
  void Send_RemObjFromMap(CCritter* acl, dyn_obj* o);
  void SendA_RemObjFromMap(CCritter* acl, dyn_obj* o);

  void Send_AddObject(CCritter* acl, dyn_obj* send_obj);
  void Send_RemObject(CCritter* acl, dyn_obj* send_obj);
  void Send_WearObject(CCritter* acl, dyn_obj* send_obj);

  void Send_GlobalInfo(CCritter* acl, uint8_t info_flags);
  void SendA_GlobalInfo(gmap_group* group, uint8_t info_flags);

  void Send_AllParams(CCritter* acl, uint8_t type_param);
  void Send_Param(CCritter* acl, uint8_t type_param, uint8_t num_param);

  void Send_Talk(CCritter* acl, npc_dialog* send_dialog);

  void Send_Map(CCritter* acl, uint16_t map_num);

  void Send_GameTime(CCritter* acl);

  void Send_Text(CCritter* to_acl, char* s_str, uint8_t say_param);
  void SendA_Text(CCritter* from_acl, cl_map* to_cr, char* s_str, char* o_str, uint8_t say_param);

// просто - список видимых клиентов
// крик - список карты
// глобал - список группы
//==============================================================================================================================
//*****  NPC by Cvet  **********************************************************************************************************
//==============================================================================================================================

  cl_map pc; //Карта НПЦ

  void NPC_ClearAll();
  int  NPC_LoadAll();
  void NPC_Remove(CCritter* npc);

//Net Proccess
  void Process_Talk_NPC(CCritter* acl); //разговор с НПЦ

  void NPC_Process(CCritter* npc);

  void NPC_Dialog_Close(CCritter* npc, CCritter* acl, uint8_t onhead_say);
  int  NPC_Dialog_Compile(CCritter* npc, CCritter* acl, npc_dialog* new_dlg);
  int  NPC_Check_demand(CCritter* npc, CCritter* acl, answer* check_answ);
  void NPC_Use_result(CCritter* npc, CCritter* acl, answer* use_answ);

//==============================================================================================================================
//*****  MOBs by Cvet  *********************************************************************************************************
//==============================================================================================================================

  cl_map mb; //Карта мобов

  word_set mobs_group_free;
  word_set mobs_group_busy;

  typedef std::map<std::string, CCritter*> prep_mob_map;
  prep_mob_map prep_mob;

  void MOB_Update(CCritter* prep_mob, CCritter* mob);

  int MOBs_LoadAllGroups();

  void MOBs_Proccess();
  int MOBs_AddToEncaunter(uint16_t num_encaunter, uint8_t mobs_level);
  void MOBs_EraseFromMap(uint16_t num_map);
  void MOBs_RefreshGroup(uint16_t num_group);

  mobs_group mobs_gr[MOBS_MAX_GROUPS];

//==============================================================================================================================
//*****  MAP by Cvet  **********************************************************************************************************
//==============================================================================================================================

//Init LMAP,GMAP
  int RefreshZoneMasks();
  uint32_t GetZoneColor(FileManager* fmngr, int zx, int zy);
  int LoadAllMaps();

//Net proccess
  void Process_GiveGlobalInfo(CCritter* acl);
  void Process_RuleGlobal(CCritter* acl);

//GMAP
  void GM_Process(int max_time);
  void GM_GroupStartMove(CCritter* rule_acl);
  int  GM_GroupToMap(CCritter* rule_acl, uint16_t map_num, HexTYPE shx, HexTYPE shy, uint8_t sori);
  int  GM_GroupToCity(CCritter* rule_acl, uint16_t city_num, uint16_t map_count);
  int  GM_GroupToEncaunter(CCritter* rule_acl, uint16_t num_encaunter);
  int  GM_GroupAddPrepCrit(CCritter* rule_acl, CritterID id);
  int  GM_GroupDelPrepCrit(CCritter* rule_acl, CritterID id);
  void GM_GroupSetMove(CCritter* rule_acl, int gx, int gy);
  void GM_GroupStopMove(CCritter* rule_acl);
  void GM_GroupSetSpeed(CCritter* rule_acl, uint8_t speed);
  int  GM_GroupToGemmate(CCritter* rule_acl);
  void GM_GroupZeroParams(gmap_group* group);
  int  GM_GroupMove(gmap_group* group, uint32_t time);
  void GM_GroupAddToZone(gmap_group* group, int zx, int zy);
//  void GM_GroupScanZone(gmap_group* group);
  map_info* GM_GetMap(uint16_t cur_map){return &map[cur_map];};

  city_info city[MAX_CITIES];
  encaunter_info encaunter[MAX_ENCAUNTERS];
  map_info map[MAX_MAPS];

  gmap_dot gm_dot[GM_MAXX][GM_MAXY];
  gmap_zone gm_zone[GM_MAXZONEX][GM_MAXZONEY];
  int proc_zone_x,proc_zone_y;

  word_set encaunters_busy;
  word_set encaunters_free;

//LMAP CRITS
  int MoveToTile(CCritter* acl, HexTYPE mx, HexTYPE my);
  int AddCrToMap(CCritter* acl, uint16_t tmap, HexTYPE tx, HexTYPE ty);
  void EraseCrFromMap(CCritter* acl, int num_map, int hex_x, int hex_y);

  tile_info* GetTile(MapTYPE num_map, HexTYPE num_x, HexTYPE num_y){return &tile[num_map][num_x][num_y];}
  uint16_t GetTileFlags(MapTYPE num_map, HexTYPE num_x, HexTYPE num_y){return tile[num_map][num_x][num_y].flags;};

  void SetTileFlag(MapTYPE num_map, HexTYPE num_x, HexTYPE num_y, uint16_t flag) {
    SetBits(tile[num_map][num_x][num_y].flags, flag);
  }
  void UnSetTileFlag(MapTYPE num_map, HexTYPE num_x, HexTYPE num_y, uint16_t flag) {
    ClearBits(tile[num_map][num_x][num_y].flags,flag);
  }

  void SetVisCr(CCritter* acl);
  int AddClIntoVisVec(CCritter* acl, CCritter* add_cl);
  int DelClFromVisVec(CCritter* acl, CCritter* del_cl);
  int DelClFromAllVisVec(CCritter* del_cl, uint16_t num_map);

  cl_map map_cr[MAX_MAPS];

  int TransitCr(CCritter* acl, int to_map, int to_x, int to_y, int to_ori);

  uint64_t FindTransit(uint16_t num_map, HexTYPE num_x, HexTYPE num_y);
  longlong_map transit_map[MAX_MAPS];

  map_str_map map_str;

  tile_info tile[MAX_MAPS][MAXHEXX][MAXHEXY];

  void LMapGarbager(uint16_t num_map);

  int LMapGetCoords(uint8_t dir_move, HexTYPE cur_hx, HexTYPE cur_hy, HexTYPE& to_hx, HexTYPE& to_hy);

//LMAP OBJECTS
  void InsertObjOnMap(dyn_obj* ins_obj, MapTYPE omap, HexTYPE ox, HexTYPE oy);
  void EraseObjFromMap(dyn_obj* ers_obj);
//  void EraseObject(dyn_obj* ers_obj, dyn_map::iterator* it_obj);

  dyn_map_map objects_map[MAX_MAPS];

  dyn_obj* GetDynObj(uint16_t id_sobj, MapTYPE num_map, HexTYPE num_x, HexTYPE num_y);

//==============================================================================================================================
//*****  SERVER by Oleg && Cvet  ***********************************************************************************************
//==============================================================================================================================

  static CServer* self;

  bool Active; // включен сервер

  int s; // socket

  void ClearClients(); //Del all
//  void RemoveCCritter(CritterID id); //Del by id

  void RemoveCritter(CritterID id);
  void DisconnectClient(CritterID idchannel); //!Cvet Disconnect Client by id
  int ConnectClient(int serv); //add new Client

  int Input(CCritter* acl);
  int Output(CCritter* acl);
  void Process(CCritter* acl);

//  void Process_GetName(CCritter* acl); !Cvet убрал

  void Process_GetText(CCritter* acl);

  int GetCmdNum(char* cmd, uint8_t access_level); //!Cvet + int access_level

  char* GetParam(char* cmdstr,char** next);
  char* my_strlwr(char* str);
  char* my_strupr(char* str);
  int PartialRight(char* str,char* et);
  char* MakeName(char* str,char* str2);

  void ProcessSocial(CCritter* sender,uint16_t socid,char* aparam);
  CCritter* LocateByPartName(char* name);

  char inBUF[2048];
  char *outBUF;
  uint32_t outLEN;

  // Для того чтобы исключить повторный сигнал о подключение уже
  // присутствующего игрока на сервере, ид посл-го подключенного.
  CritterID last_id;

//!Cvet ++++++++++++++++++++++++
  int DistFast(int dx, int dy); //Accuracy:95%  Speed:2.5x
  int DistSqrt(int dx, int dy); //Accuracy:100% Speed:1.0x

  FileManager fm;

//Игровое время
  //SYSTEMTIME sys_time;
  uint16_t Game_Time;
  uint8_t Game_Day;
  uint8_t Game_Month;
  uint16_t Game_Year;

//Слежение за читерством
  void SetCheat(CCritter* acl, const char* cheat_message);
//!Cvet ------------------------

public:

  SQL sql; //!Cvet

//!Cvet запись времени выполнения +++
  int loop_time;
  int loop_cycles;
  int loop_min;
  int loop_max;

  int lt_FDsel;
  int lt_conn;
  int lt_input;
  int lt_proc_cl;
  int lt_proc_pc;
  int lt_output;
  int lt_discon;

  int lt_FDsel_max;
  int lt_conn_max;
  int lt_input_max;
  int lt_proc_cl_max;
  int lt_proc_pc_max;
  int lt_output_max;
  int lt_discon_max;

  int lags_count;
//!Cvet ---

  int Init();
  void Finish();

  void RunGameLoop(); // серверная петля

   CServer();
   ~CServer();
};
