#pragma once

/********************************************************************
  created:  end of 2006; updated: begin of 2007

  author:   Anton Tsvetinsky and Evgeny Barabanov (binyan)

  purpose:
*********************************************************************/

#include "main.h"
#include "netproto.h"
#include "Critter.h"

#include <mysql/mysql.h>

//Переменные НПЦ
const uint8_t NPC_VAR_LOCAL =0;
const uint8_t NPC_VAR_GLOBAL  =1;

class SQL {
public:
  SQL();
  ~SQL();

  bool Initialize();
  void Terminate();

  bool Query(const char* query, ...);
  bool Check(char* str);

  int GetInt(const char* table, const char* find_row, const char* row, const char* row_vol);
  int GetInt(const char* table, const char* find_row, const char* row, int row_vol);
  bool GetChar(const char* table, const char* find_row, const char* row, const char* row_vol, char* result);
  bool GetChar(const char* table, const char* find_row, const char* row, int row_vol, char* result);

  bool CheckUser(const char* name);
  int CheckUserPass(const char* name, const char* pass);

  int CountRows(const char* table, const char* column, const char* count_vol);
  int CountRows(const char* table, const char* column, int count_vol);

  //игроки
  int NewPlayer(crit_info* info);
  int SaveDataPlayer(crit_info* info);
  int LoadDataPlayer(crit_info* info);
  void DeleteDataPlayer(crit_info* info);

  //нпц
  int NewNPC(crit_info* info);
  int SaveDataNPC(crit_info* info);
  int LoadDataNPC(crit_info* info);
  void DeleteDataNPC(crit_info* info);

  //переменные нпц
  int CheckVarNPC(CritterID npc_id, const std::string& var_name, CritterID player_id, char oper, int count);
  void ChangeVarNPC(CritterID npc_id, const std::string& var_name, CritterID player_id, char oper, int count);

  //переменные игроков
  int CheckVar(CritterID crid, uint16_t var_num, char oper, int count);
  void ChangeVar(CritterID crid, uint16_t var_num, char oper, int count);

  //квесты
  int CheckQuest(CritterID crid, uint16_t quest_num, char choose, int count);
  void ChangeQuest(CritterID crid, uint16_t quest_num, char choose, int count);

  //гвары
  //  int CheckGVar(uint16_t num_gvar, char oper, int value);
  //  void ChangeGVar(uint16_t num_gvar, char oper, int value);

  //объекты
  int NewObject(dyn_obj* obj, uint32_t obj_id);
  void SaveDataObject(dyn_obj* obj);
  int LoadDataObject(dyn_obj* obj, uint8_t obj_type);
  void DeleteDataObject(dyn_obj* obj);

  //работа с таблицей читов
  void AddCheat(CritterID user_id, const char* text_cheat);

  //сервис
  void WriteLog(const char* str);

  int CountTable(const char* table, const char* row);
  void PrintTableInLog(const char* table, const char* rows);

private:
  bool initialized;
  std::set<char> true_char;

  MYSQL* mySQL;
  MYSQL_RES* mySQL_res;
  MYSQL_ROW mySQL_row;

  int CodeParams(char* stats, char* skills, char* perks, crit_info* info);
  int DecodeParams(const char* stats, const char* skills, const char* perks, crit_info* info);
};
