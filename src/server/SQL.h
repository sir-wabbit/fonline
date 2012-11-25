#ifndef __SQL_H__
#define __SQL_H__

/********************************************************************
	created:	end of 2006; updated: begin of 2007

	author:		Anton Tsvetinsky and Evgeny Barabanov (binyan)
	
	purpose:	
*********************************************************************/

#include "main.h"
#include "netproto.h"
#include "Critter.h"

//Переменные НПЦ
const uint8_t NPC_VAR_LOCAL	=0;
const uint8_t NPC_VAR_GLOBAL	=1;

typedef std::set<char> true_char_set;

class SQL  
{
public:

	SQL();
	~SQL();

	MYSQL* mySQL;
	MYSQL_RES* mySQL_res;
	MYSQL_ROW mySQL_row;

	int Init_mySQL();
	void Finish_mySQL();

	int Query(char* query, ...);
	int Check(char* str);

	int GetInt(char* table, char* find_row, char* row, char* row_vol);
	int GetInt(char* table, char* find_row, char* row, int row_vol);
	BOOL GetChar(char* table, char* find_row, char* row, char* row_vol, char* result);
	BOOL GetChar(char* table, char* find_row, char* row, int row_vol, char* result);

	BOOL CheckUser(char* name);
	int CheckUserPass(char* name, char* pass);

	int CountRows(char* table, char* column, char* count_vol);
	int CountRows(char* table, char* column, int count_vol);

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
	int CheckVarNPC(CritterID npc_id, std::string var_name, CritterID player_id, char oper, int count);
	void ChangeVarNPC(CritterID npc_id, std::string var_name, CritterID player_id, char oper, int count);

//переменные игроков
	int CheckVar(CritterID crid, uint16_t var_num, char oper, int count);
	void ChangeVar(CritterID crid, uint16_t var_num, char oper, int count);

//квесты
	int CheckQuest(CritterID crid, uint16_t quest_num, char choose, int count);
	void ChangeQuest(CritterID crid, uint16_t quest_num, char choose, int count);

//гвары
//	int CheckGVar(uint16_t num_gvar, char oper, int value);
//	void ChangeGVar(uint16_t num_gvar, char oper, int value);

//объекты
	int NewObject(dyn_obj* obj, uint32_t obj_id);
	void SaveDataObject(dyn_obj* obj);
	int LoadDataObject(dyn_obj* obj, uint8_t obj_type);
	void DeleteDataObject(dyn_obj* obj);

//работа с таблицей читов
	void AddCheat(CritterID user_id, char* text_cheat);

//сервис
	void WriteLog(char* str);


	int CountTable(char* table, char* row);
	void PrintTableInLog(char* table, char* rows);

private:

	int CodeParams(char* stats, char* skills, char* perks, crit_info* info);
	int DecodeParams(char* stats, char* skills, char* perks, crit_info* info);

	bool Active;
	true_char_set true_char;

	char stats[ALL_STATS*4+1];
	char skills[ALL_SKILLS*3+1];
	char perks[ALL_PERKS+1];

  std::string user;
  std::string passwd;
  std::string host;
  std::string db;
	UINT port;
	
	std::string unix_socket;
	ULONG clientflag;
};

#endif //__SQL_H__