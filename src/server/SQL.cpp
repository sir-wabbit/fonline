/********************************************************************
	created:	end of 2006; updated: begin of 2007

	author:		Anton Tsvetinsky and Evgeny Barabanov binyan
	
	purpose:	
*********************************************************************/

#include "stdafx.h"
#include <IniFile/IniFile.hpp>
#include "SQL.h"

SQL::SQL()
{
	Active=false;
}

SQL::~SQL()
{

}

int SQL::Init_mySQL()
{
	if(Active==true) return 1;

	LogExecStr("mySQL init begin\n");

  std::string load_char;

  using namespace IniFile;

  RecordMap ini;
  LoadINI("./data/server.ini", ini);
  load_char = GetValue<std::string>(ini, "SQL.true_char", "@");

	//if(load_char[0]=='@')
	//{
	//	LogExecStr("mySQL true_char failed. see foserv.cfg!\nmySQL init FALSE\n");
	//	return 0;
	//}

	LogExecStr("Разрешенные символы:|%s|. Все остальные будут заменяться на пробел.\n",load_char.c_str());

	for (size_t i = 0; i < load_char.size(); i++) {
	  true_char.insert(true_char_set::value_type(load_char[i]));
	}

	mySQL=mysql_init(NULL);
	if(!mySQL)
	{
		LogExecStr("ошибка инициализации\n");
		return 0;
	}

  this->user = GetValue<std::string>(ini, "SQL.user", "root");
  this->passwd = GetValue<std::string>(ini, "SQL.passwd", "");
  this->host = GetValue<std::string>(ini, "SQL.host", "localhost");
  this->db = GetValue<std::string>(ini, "SQL.db", "");
  this->unix_socket = GetValue<std::string>(ini, "SQL.unix_socket", "");
  
  this->port = GetValue<int>(ini, "SQL.port", 3306);
  this->clientflag = GetValue<int>(ini, "SQL.clientflag", 0);
  
  LogExecStr("Accepted characters : %s\n", load_char.c_str());
  LogExecStr("User : %s\n", user.c_str());
  LogExecStr("Password : %s\n", passwd.c_str());
  LogExecStr("Host : %s\n", host.c_str());
  LogExecStr("DB : %s\n", db.c_str());
  LogExecStr("Socket : %s\n", unix_socket.c_str());

	if(!mysql_real_connect(mySQL, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, unix_socket.c_str(), clientflag))
	{
		LogExecStr("%s\n",mysql_error(mySQL));
		return 0;
	}

	Query("DROP TABLE IF EXISTS `log`");

	Query("DROP TABLE IF EXISTS `users`");
	Query("DROP TABLE IF EXISTS `npc`");

	Query("DROP TABLE IF EXISTS `obj_armor`");
	Query("DROP TABLE IF EXISTS `obj_container`");
	Query("DROP TABLE IF EXISTS `obj_drug`");
	Query("DROP TABLE IF EXISTS `obj_weapon`");
	Query("DROP TABLE IF EXISTS `obj_ammo`");
	Query("DROP TABLE IF EXISTS `obj_misc`");
	Query("DROP TABLE IF EXISTS `obj_key`");
	Query("DROP TABLE IF EXISTS `obj_door`");
	Query("DROP TABLE IF EXISTS `obj_crafting`");

	Query("DROP TABLE IF EXISTS `npc_vars_templates`");
	Query("DROP TABLE IF EXISTS `npc_vars`");

	Query("DROP TABLE IF EXISTS `player_vars_templates`");
	Query("DROP TABLE IF EXISTS `player_vars`");

	Query("DROP TABLE IF EXISTS `quests`");

	Query("DROP TABLE IF EXISTS `users_cheat`");

//Логгинг
	Query("CREATE TABLE IF NOT EXISTS `log` ("
	"`id`			int			unsigned	NOT NULL	auto_increment,"
	"`log`			varchar(2048)						default NULL,"
	"PRIMARY KEY  (`id`)"
	");");

//Игроки
	Query("CREATE TABLE IF NOT EXISTS `users` ("
	"`id`			int			unsigned	NOT NULL	auto_increment,"
	"`login`		varchar(20)				NOT NULL	default '0',"
	"`pass`			varchar(20)				NOT NULL	default '0',"
	"`reg`			tinyint		unsigned	NOT NULL	default '0',"
	"`base_type`	tinyint		unsigned	NOT NULL	default '0',"
	"`map`			smallint	unsigned	NOT NULL	default '0',"
	"`x`			smallint	unsigned	NOT NULL	default '0',"
	"`y`			smallint	unsigned	NOT NULL	default '0',"
	"`ori`			tinyint		unsigned	NOT NULL	default '0',"
	"`name`			varchar(20)				NOT NULL	default '0',"
	"`cases0`		varchar(20)				NOT NULL	default '0',"
	"`cases1`		varchar(20)				NOT NULL	default '0',"
	"`cases2`		varchar(20)				NOT NULL	default '0',"
	"`cases3`		varchar(20)				NOT NULL	default '0',"
	"`cases4`		varchar(20)				NOT NULL	default '0',"
	"`stats`		char(%d)				NOT NULL	default '0',"
	"`skills`		char(%d)				NOT NULL	default '0',"
	"`perks`		char(%d)				NOT NULL	default '0',"
	"PRIMARY KEY  (`id`),"
	"UNIQUE KEY `login` (`login`)"
	");",
	(ALL_STATS*4+1), (ALL_SKILLS*3+1), (ALL_PERKS+1));

	if(!CountRows("users","id",USERS_START_ID))
		Query("INSERT INTO users (id) VALUES ('%d');",USERS_START_ID-1);

//НПЦ
	Query("CREATE TABLE IF NOT EXISTS `npc` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`base_type`	smallint	unsigned	NOT NULL	default '0',"
	"`map`			smallint	unsigned	NOT NULL	default '0',"
	"`x`			smallint	unsigned	NOT NULL	default '0',"
	"`y`			smallint	unsigned	NOT NULL	default '0',"
	"`ori`			tinyint		unsigned	NOT NULL	default '0',"
	"`name`			varchar(20)				NOT NULL	default '0',"
	"`cases0`		varchar(20)				NOT NULL	default '0',"
	"`cases1`		varchar(20)				NOT NULL	default '0',"
	"`cases2`		varchar(20)				NOT NULL	default '0',"
	"`cases3`		varchar(20)				NOT NULL	default '0',"
	"`cases4`		varchar(20)				NOT NULL	default '0',"
	"`stats`		char(%d)				NOT NULL	default '0',"
	"`skills`		char(%d)				NOT NULL	default '0',"
	"`perks`		char(%d)				NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");",
	(ALL_STATS*4+1), (ALL_SKILLS*3+1), (ALL_PERKS+1));

//Объекты
	Query("CREATE TABLE IF NOT EXISTS `obj_armor` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_container` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_drug` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_weapon` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_ammo` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_misc` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_key` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_door` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_crafting` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `obj_grid` ("
	"`id`			int			unsigned	NOT NULL	default '0',"
	"`proto_id`		smallint	unsigned	NOT NULL	default '0',"
	"`accessory`	tinyint		unsigned	NOT NULL	default '0',"
	"`acc1`			int			unsigned	NOT NULL	default '0',"
	"`acc2`			int			unsigned	NOT NULL	default '0',"
	"`time_wear`	int			unsigned	NOT NULL	default '0',"
	"`last_tick`	int			unsigned	NOT NULL	default '0',"
	"`broken_info`	int			unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

//Переменные НПЦ
	Query("CREATE TABLE IF NOT EXISTS `npc_vars_templates` ("
	"`id`			int			unsigned	NOT NULL	auto_increment,"
	"`npc_id`		int			unsigned	NOT NULL	default '0',"
	"`name`			varchar(20)			NOT NULL	default 'none',"
	"`count`		int					NOT NULL	default '0',"
	"`min`			int					NOT NULL	default '0',"
	"`max`			int					NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `npc_vars` ("
	"`id`			int		unsigned	NOT NULL	auto_increment,"
	"`uniq_name`	varchar(40)			NOT NULL    default 'error',"
	"`npc_id`		int		unsigned	NOT NULL	default '0',"
	"`name`			varchar(20)			NOT NULL	default 'none',"
	"`player_id`	int		unsigned	NOT NULL	default '0',"
	"`count`		int					NOT NULL	default '0',"
	"`min`			int					NOT NULL	default '0',"
	"`max`			int					NOT NULL	default '0',"
	"PRIMARY KEY  (`id`),"
	"UNIQUE KEY `uniq_name` (`uniq_name`)"
	");");

	Query("CREATE TRIGGER `var_change` BEFORE UPDATE ON `npc_vars` "
	"FOR EACH ROW BEGIN "
	"IF NEW.count > NEW.max THEN SET NEW.count = NEW.max; END IF;"
	"IF NEW.count < NEW.min THEN SET NEW.count = NEW.min; END IF;"
	"END;");

//	Query("CREATE TRIGGER `var_change` BEFORE UPDATE ON `npc_vars` "
//	"forEACH ROW BEGIN "
//	"ifNEW.count > NEW.max THEN SET NEW.count = NEW.max; END IF;"
//	"ifNEW.count < NEW.min THEN SET NEW.count = NEW.min; END IF;"
//	"END;");

//Переменные игроков
	Query("CREATE TABLE IF NOT EXISTS `player_vars_templates` ("
	"`id`			int		unsigned	NOT NULL	auto_increment,"
	"`var_num`		int		unsigned	NOT NULL	default '0',"
	"`count`		int					NOT NULL	default '0',"
	"`min`			int					NOT NULL	default '0',"
	"`max`			int					NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TABLE IF NOT EXISTS `player_vars` ("
	"`id`			int		unsigned	NOT NULL	auto_increment,"
	"`var_num`		int		unsigned	NOT NULL	default '0',"
	"`crid`			int		unsigned	NOT NULL	default '0',"
	"`count`		int					NOT NULL	default '0',"
	"`min`			int					NOT NULL	default '0',"
	"`max`			int					NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

	Query("CREATE TRIGGER `var_change2` BEFORE UPDATE ON `player_vars` "
	"FOR EACH ROW BEGIN "
	"IF NEW.count > NEW.max THEN SET NEW.count = NEW.max; END IF;"
	"IF NEW.count < NEW.min THEN SET NEW.count = NEW.min; END IF;"
	"END;");

//Квесты игроков
	Query("CREATE TABLE IF NOT EXISTS `quests` ("
	"`id`			int		unsigned	NOT NULL	auto_increment,"
	"`quest_num`	int		unsigned	NOT NULL	default '0',"
	"`crid`			int		unsigned	NOT NULL	default '0',"
	"`value`		tinyint	unsigned	NOT NULL	default '0',"
	"PRIMARY KEY  (`id`)"
	");");

//Читерство
	Query("CREATE TABLE IF NOT EXISTS `users_cheat` ("
	"`id`			int		unsigned	NOT NULL	auto_increment,"
	"`user_id`		int		unsigned	NOT NULL	default '0',"
	"`text_cheat`	varchar(1024)		NOT NULL    default 'err',"
	"PRIMARY KEY  (`id`)"
	");");

	Active=true;

	LogExecStr("mySQL init OK\n");

	return 1;
}

void SQL::Finish_mySQL()
{
	mysql_close(mySQL);
	mySQL=NULL;

	Active=false;
}

int SQL::Query(char* query, ...)
{
	char str[2048];

	va_list list;
	va_start(list, query);
	wvsprintf(str, query, list);
	va_end(list);

	if(mysql_query(mySQL, str))
	{
		LogExecStr("mySQL Query |%s| error: %s\n", str, mysql_error(mySQL));
		return 0;
	}
    return 1;
}

int SQL::Check(char* str)
{
	int i=0;
	bool cheat_fix=false;

	while(str[i])
	{
		if(!true_char.count(str[i]))
		{
			str[i]=0x20;
			cheat_fix=true;
		}
		i++;
	}

	if(cheat_fix==true) return 1;

	return 0;
}

int SQL::GetInt(char* table, char* find_row, char* row, char* row_vol)
{
	char stradd[256];
	sprintf(stradd,"SELECT `%s` FROM `%s` WHERE `%s`='%s'", find_row, table, row, row_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL GetInt1 error: %s\n", mysql_error(mySQL));
		return 0xFFFFFFFF;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0xFFFFFFFF;

	if(!mysql_num_rows(mySQL_res)) return 0xFFFFFFFF;

	mySQL_row=mysql_fetch_row(mySQL_res);

	return atoi(mySQL_row[0]);
}

int SQL::GetInt(char* table, char* find_row, char* row, int row_vol)
{
	char stradd[256];
	sprintf(stradd,"SELECT `%s` FROM `%s` WHERE `%s`='%d'", find_row, table, row, row_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL GetInt2 error: %s\n", mysql_error(mySQL));
		return 0xFFFFFFFF;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0xFFFFFFFF;

	if(!mysql_num_rows(mySQL_res)) return 0xFFFFFFFF;

	mySQL_row=mysql_fetch_row(mySQL_res);

	return atoi(mySQL_row[0]);
}

BOOL SQL::GetChar(char* table, char* find_row, char* row, char* row_vol, char* result)
{
	char stradd[256];
	sprintf(stradd,"SELECT `%s` FROM `%s` WHERE `%s`='%s'", find_row, table, row, row_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL GetChar1 error: %s\n", mysql_error(mySQL));
		return FALSE;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return FALSE;

	if(!mysql_num_rows(mySQL_res)) return FALSE;

	mySQL_row=mysql_fetch_row(mySQL_res);

	strcpy(result,mySQL_row[0]);

	return TRUE;
}

BOOL SQL::GetChar(char* table, char* find_row, char* row, int row_vol, char* result)
{
	char stradd[256];
	sprintf(stradd,"SELECT `%s` FROM `%s` WHERE `%s`='%d'", find_row, table, row, row_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL GetChar2 error: %s\n", mysql_error(mySQL));
		return FALSE;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return FALSE;

	if(!mysql_num_rows(mySQL_res)) return FALSE;

	mySQL_row=mysql_fetch_row(mySQL_res);

	strcpy(result,mySQL_row[0]);

	return TRUE;
}

BOOL SQL::CheckUser(char* name)
{
	char stradd[256];
	sprintf(stradd,"SELECT `id` FROM `users` WHERE `login`='%s'", name);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CheckUser error: %s\n", mysql_error(mySQL));
		return FALSE;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return FALSE;

	if(!mysql_num_rows(mySQL_res)) return FALSE;

	return TRUE;
}

int SQL::CheckUserPass(char* name, char* pass)
{
	char stradd[256];
	sprintf(stradd,"SELECT `id` FROM `users` WHERE `login`='%s' AND `pass`='%s'", name, pass);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CheckUserPass error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;

	if(!mysql_num_rows(mySQL_res)) return 0;

	mySQL_row=mysql_fetch_row(mySQL_res);

	return atoi(mySQL_row[0]);
}

int SQL::CountRows(char* table, char* column, char* count_vol)
{
	char stradd[256];
	sprintf(stradd,"SELECT * FROM `%s` WHERE `%s`='%s'", table, column, count_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CountRows error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;

	return mysql_num_rows(mySQL_res);
}

int SQL::CountRows(char* table, char* column, int count_vol)
{
	char stradd[256];
	sprintf(stradd,"SELECT * FROM `%s` WHERE `%s`='%d'", table, column, count_vol);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CountRows error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;

	return mysql_num_rows(mySQL_res);
}

int SQL::NewPlayer(crit_info* info)
{
	Query("INSERT INTO `users` (login) VALUES ('%s')", info->login);

	if(!(info->id=mysql_insert_id(mySQL))) return 0;

	if(!SaveDataPlayer(info)) return 0;

	return 1;
}

int SQL::SaveDataPlayer(crit_info* info)
{
	if(!CodeParams(stats,skills,perks,info))
	{
		LogExecStr("!!!WORNING!!! SQL. Ошибка кодировки данных криттера\n");
		return 0;
	}

	Query("UPDATE users SET login='%s',pass='%s',base_type='%d',map='%d',x='%d',y='%d',ori='%d',"
	"name='%s',cases0='%s',cases1='%s',cases2='%s',cases3='%s',cases4='%s',"
	"stats='%s',skills='%s',perks='%s' WHERE id='%d'",
	info->login,info->pass,info->base_type,info->map,info->x,info->y,info->ori,info->name,
	info->cases[0],info->cases[1],info->cases[2],info->cases[3],info->cases[4],
	stats,skills,perks,info->id);

	return 1;
}

int SQL::LoadDataPlayer(crit_info* info)
{
	char str[256];

	sprintf(str, "SELECT * FROM users WHERE login='%s'", info->login);
	if(mysql_query(mySQL, str))
	{
		LogExecStr("mySQL LoadDataPlayer error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;
	if(!mysql_num_rows(mySQL_res)) return 0;
	mySQL_row=mysql_fetch_row(mySQL_res);

	info->id=atoi(mySQL_row[0]);
	info->base_type=atoi(mySQL_row[4]);
	info->map=atoi(mySQL_row[5]);
	info->x=atoi(mySQL_row[6]);
	info->y=atoi(mySQL_row[7]);
	info->ori=atoi(mySQL_row[8]);
	strcpy(info->name,mySQL_row[9]);
	strcpy(info->cases[0],mySQL_row[10]);
	strcpy(info->cases[1],mySQL_row[11]);
	strcpy(info->cases[2],mySQL_row[12]);
	strcpy(info->cases[3],mySQL_row[13]);
	strcpy(info->cases[4],mySQL_row[14]);
	strcpy(stats,mySQL_row[15]);
	strcpy(skills,mySQL_row[16]);
	strcpy(perks,mySQL_row[17]);

	if(!info->name) return 0;
	if(!info->cases[0]) return 0;
	if(!info->cases[1]) return 0;
	if(!info->cases[2]) return 0;
	if(!info->cases[3]) return 0;
	if(!info->cases[4]) return 0;

	if(!DecodeParams(stats,skills,perks,info)) return 0;

	return 1;
}

void SQL::DeleteDataPlayer(crit_info* info)
{
	Query("DELETE FROM users WHERE id='%d'",info->id);
}

int SQL::NewNPC(crit_info* info)
{
	Query("INSERT INTO `npc` (id) VALUES ('%d')", info->id);

//	if(!(info->id=mysql_insert_id(mySQL))) return 0;

	if(!SaveDataNPC(info)) return 0;

	return 1;
}

int SQL::SaveDataNPC(crit_info* info)
{
	if(!CodeParams(stats,skills,perks,info))
	{
		LogExecStr("!!!WORNING!!! SQL. Ошибка кодировки данных криттера\n");
		return 0;
	}

	Query("UPDATE npc SET base_type='%d',map='%d',x='%d',y='%d',ori='%d',"
	"name='%s',cases0='%s',cases1='%s',cases2='%s',cases3='%s',cases4='%s',"
	"stats='%s',skills='%s',perks='%s' WHERE id='%d'",
	info->base_type,info->map,info->x,info->y,info->ori,info->name,
	info->cases[0],info->cases[1],info->cases[2],info->cases[3],info->cases[4],
	stats,skills,perks,info->id);

	return 1;
}

int SQL::LoadDataNPC(crit_info* info)
{
	char str[64];

	sprintf(str, "SELECT * FROM npc WHERE id='%d'", info->id);
	if(mysql_query(mySQL, str))
	{
		LogExecStr("mySQL LoadDataNPC error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;
	if(!mysql_num_rows(mySQL_res)) return 0;
	mySQL_row=mysql_fetch_row(mySQL_res);

	info->id=atoi(mySQL_row[0]);
	info->base_type=atoi(mySQL_row[1]);
	info->map=atoi(mySQL_row[2]);
	info->x=atoi(mySQL_row[3]);
	info->y=atoi(mySQL_row[4]);
	info->ori=atoi(mySQL_row[5]);
	strcpy(info->name,mySQL_row[6]);
	strcpy(info->cases[0],mySQL_row[7]);
	strcpy(info->cases[1],mySQL_row[8]);
	strcpy(info->cases[2],mySQL_row[9]);
	strcpy(info->cases[3],mySQL_row[10]);
	strcpy(info->cases[4],mySQL_row[11]);
	strcpy(stats,mySQL_row[12]);
	strcpy(skills,mySQL_row[13]);
	strcpy(perks,mySQL_row[14]);

	if(!info->name) return 0;
	if(!info->cases[0]) return 0;
	if(!info->cases[1]) return 0;
	if(!info->cases[2]) return 0;
	if(!info->cases[3]) return 0;
	if(!info->cases[4]) return 0;

	if(!DecodeParams(stats,skills,perks,info)) return 0;

	return 1;
}

void SQL::DeleteDataNPC(crit_info* info)
{
	Query("DELETE FROM npc WHERE id='%d'",info->id);
}

int SQL::CheckVarNPC(CrID npc_id, string var_name, CrID player_id, char oper, int count)
{
	char uniq_name[80];

	if(var_name[0]=='l')
		sprintf(uniq_name,"%d_%s_%d",npc_id,var_name.c_str(),player_id);
	else if(var_name[0]=='g')
		sprintf(uniq_name,"%d_%s",npc_id,var_name.c_str());
	else
	{
		LogExecStr("Неизвестная переменная %s у %d при попытке проверки\n",var_name.c_str(),npc_id);
		return 0;
	}

	int get_count=GetInt("npc_vars","count","uniq_name",uniq_name);

	if(get_count==0xFFFFFFFF)
	{
		char str_f[256];

		sprintf(str_f, "SELECT * FROM `npc_vars_templates` WHERE `npc_id`='%d' AND `name`='%s'", npc_id, var_name.c_str());
		if(mysql_query(mySQL, str_f))
		{
			LogExecStr("mySQL CheckVarNPC error: %s\n", mysql_error(mySQL));
			return 0;
		}

		if(!(mySQL_res=mysql_store_result(mySQL))) { LogExecStr("Проверка - Ошибка в записи переменной\n"); return 0; }
		if(!mysql_num_rows(mySQL_res)) { LogExecStr("Проверка - Шаблон переменной не найден\n"); return 0; }
		mySQL_row=mysql_fetch_row(mySQL_res);

		int var_count=atoi(mySQL_row[3]);
		int var_min=atoi(mySQL_row[4]);
		int var_max=atoi(mySQL_row[5]);

		Query("INSERT INTO `npc_vars` (uniq_name,npc_id,name,player_id,count,min,max) "
		"VALUES ('%s','%d','%s','%d','%d','%d','%d');",uniq_name,npc_id,var_name.c_str(),player_id,var_count,var_min,var_max);

		get_count=var_count;
	}

	if(oper=='>') { if(get_count>count) return 1; }
	else if(oper=='<') { if(get_count<count) return 1; }
	else if(oper=='=') { if(get_count==count) return 1; }

	return 0;
}

void SQL::ChangeVarNPC(CrID npc_id, string var_name, CrID player_id, char oper, int count)
{
	char uniq_name[80];

	if(var_name[0]=='l')
		sprintf(uniq_name,"%d_%s_%d",npc_id,var_name.c_str(),player_id);
	else if(var_name[0]=='g')
		sprintf(uniq_name,"%d_%s",npc_id,var_name.c_str());
	else
	{
		LogExecStr("Неизвестная переменная %s у %d при попытке изменения\n",var_name.c_str(),npc_id);
		return;
	}

	if(CountRows("npc_vars","uniq_name",uniq_name))
	{
		if(oper=='=')
			Query("UPDATE `npc_vars` SET `count`='%d' WHERE uniq_name='%s';",count,uniq_name);
		else
			Query("UPDATE `npc_vars` SET `count`=count%c'%d' WHERE uniq_name='%s';",oper,count,uniq_name);
	}
	else
	{
		char str_f[256];

		sprintf(str_f, "SELECT * FROM npc_vars_templates WHERE npc_id='%d' AND name='%s'", npc_id, var_name.c_str());
		if(mysql_query(mySQL, str_f))
		{
			LogExecStr("mySQL ChangeVarNPC error: %s\n", mysql_error(mySQL));
			return;
		}

		if(!(mySQL_res=mysql_store_result(mySQL))) { LogExecStr("Изменение - Ошибка в записи переменной\n"); return; }
		if(!mysql_num_rows(mySQL_res)) { LogExecStr("Изменение - Шаблон переменной не найден\n"); return; }
		mySQL_row=mysql_fetch_row(mySQL_res);

		int var_count=atoi(mySQL_row[3]);
		int var_min=atoi(mySQL_row[4]);
		int var_max=atoi(mySQL_row[5]);

		if(oper=='+') var_count+=count;
		else if(oper=='-') var_count-=count;
		else if(oper=='=') var_count=count;
		else if(oper=='*') var_count*=count;
		else if(oper=='/') var_count/=count;

		Query("INSERT INTO `npc_vars` (uniq_name,npc_id,name,player_id,count,min,max) "
		"VALUES ('%s','%d','%s','%d','%d','%d','%d');",uniq_name,npc_id,var_name.c_str(),player_id,var_count,var_min,var_max);
	}
}

int SQL::CheckVar(CrID crid, WORD var_num, char oper, int count)
{
	char stradd[256];

	if(!var_num) return 0;

	if(var_num<=10000) //global
		sprintf(stradd,"SELECT `count` FROM `player_vars` WHERE `var_num`='%d'",var_num);
	else //local
		sprintf(stradd,"SELECT `count` FROM `player_vars` WHERE `crid`='%d' AND `var_num`='%d'",crid,var_num);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CheckVar error: %s\n", mysql_error(mySQL));
		return 0;
	}

	int get_count=0xFFFFFFFF;

	if((mySQL_res=mysql_store_result(mySQL)))
	{
		if(mysql_num_rows(mySQL_res))
		{
			mySQL_row=mysql_fetch_row(mySQL_res);

			get_count=atoi(mySQL_row[0]);
		}
	}

	if(get_count==0xFFFFFFFF)
	{
		char str_f[256];

		sprintf(str_f, "SELECT * FROM `player_vars_templates` WHERE `var_num`='%d'", var_num);
		if(mysql_query(mySQL, str_f))
		{
			LogExecStr("mySQL CheckVar2 error: %s\n", mysql_error(mySQL));
			return 0;
		}

		if(!(mySQL_res=mysql_store_result(mySQL))) { LogExecStr("Проверка2 - Ошибка в записи переменной\n"); return 0; }
		if(!mysql_num_rows(mySQL_res)) { LogExecStr("Проверка2 - Шаблон переменной не найден\n"); return 0; }
		mySQL_row=mysql_fetch_row(mySQL_res);

		int var_count=atoi(mySQL_row[2]);
		int var_min=atoi(mySQL_row[3]);
		int var_max=atoi(mySQL_row[4]);

		if(var_num<=10000)
			Query("INSERT INTO `player_vars` (var_num,crid,count,min,max) "
			"VALUES ('%d','%d','%d','%d','%d');",var_num,0,var_count,var_min,var_max);
		else
			Query("INSERT INTO `player_vars` (var_num,crid,count,min,max) "
			"VALUES ('%d','%d','%d','%d','%d');",var_num,crid,var_count,var_min,var_max);

		get_count=var_count;
	}

	if(oper=='>') { if(get_count>count) return 1; }
	else if(oper=='<') { if(get_count<count) return 1; }
	else if(oper=='=') { if(get_count==count) return 1; }

	return 0;
}

void SQL::ChangeVar(CrID crid, WORD var_num, char oper, int count)
{
	char stradd[256];

	if(!var_num) return;

	if(var_num<=10000) //global
		sprintf(stradd,"SELECT `count` FROM `player_vars` WHERE `var_num`='%d'",var_num);
	else //local
		sprintf(stradd,"SELECT `count` FROM `player_vars` WHERE `crid`='%d' AND `var_num`='%d'",crid,var_num);

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL ChangeVar error: %s\n", mysql_error(mySQL));
		return;
	}

	if((mySQL_res=mysql_store_result(mySQL)))
	{
		if(!(mysql_num_rows(mySQL_res)))
		{
			char str_f[256];

			sprintf(str_f, "SELECT * FROM `player_vars_templates` WHERE `var_num`='%d'", var_num);
			if(mysql_query(mySQL, str_f))
			{
				LogExecStr("mySQL ChangeVar2 error: %s\n", mysql_error(mySQL));
				return;
			}

			if(!(mySQL_res=mysql_store_result(mySQL))) { LogExecStr("Установка вары - Ошибка в записи переменной\n"); return; }
			if(!mysql_num_rows(mySQL_res)) { LogExecStr("Установка вары - Шаблон переменной не найден\n"); return; }
			mySQL_row=mysql_fetch_row(mySQL_res);

			int var_count=atoi(mySQL_row[2]);
			int var_min=atoi(mySQL_row[3]);
			int var_max=atoi(mySQL_row[4]);

			if(oper=='+') var_count+=count;
			else if(oper=='-') var_count-=count;
			else if(oper=='=') var_count=count;
			else if(oper=='*') var_count*=count;
			else if(oper=='/') var_count/=count;

			if(var_num<=10000)
				Query("INSERT INTO `player_vars` (var_num,crid,count,min,max) "
				"VALUES ('%d','%d','%d','%d','%d');",var_num,0,var_count,var_min,var_max);
			else
				Query("INSERT INTO `player_vars` (var_num,crid,count,min,max) "
				"VALUES ('%d','%d','%d','%d','%d');",var_num,crid,var_count,var_min,var_max);

			return;
		}
		
		if(oper=='=')
		{
			if(var_num<=10000)
				Query("UPDATE `player_vars` SET `count`='%d' WHERE `var_num`='%d'",var_num);
			else
				Query("UPDATE `player_vars` SET `count`='%d' WHERE `var_num`='%d' AND `crid`='%d'",var_num,crid);
		}
		else
		{
			if(var_num<=10000)
				Query("UPDATE `player_vars` SET `count`=`count`%c'%d' WHERE `var_num`='%d'",oper,var_num);
			else
				Query("UPDATE `player_vars` SET `count`=`count`%c'%d' WHERE `var_num`='%d' AND `crid`='%d'",oper,var_num,crid);
		}

		return;
	}

	LogExecStr("mySQL ChangeVar error mysql_store_result\n");
}

int SQL::CheckQuest(CrID crid, WORD quest_num, char choose, int count)
{
	char stradd[256];

	switch(choose)
	{
	case CHOOSE_PLAYER:
		sprintf(stradd,"SELECT `value` FROM `quests` WHERE `quest_num`='%d' AND `crid`='%d' AND `value`='%d'",quest_num,crid,count);
		break;
	case CHOOSE_ALL:
		sprintf(stradd,"SELECT `value` FROM `quests` WHERE `quest_num`='%d' AND `value`='%d'",quest_num,count);
		break;
	case CHOOSE_ALL_NOT_PLAYER:
		sprintf(stradd,"SELECT `value` FROM `quests` WHERE `quest_num`='%d' AND `crid`<>'%d' AND `value`='%d'",quest_num,crid,count);
		break;
	default:
		return 0;
	}

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL CheckQuest error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;
	if(!(mysql_num_rows(mySQL_res))) return 0;

	return 1;
}

void SQL::ChangeQuest(CrID crid, WORD quest_num, char choose, int count)
{
	char stradd[256];

	int value1=count/10;
	int value2=count%10;
	if(count<10) value2=0;

	switch(choose)
	{
	case CHOOSE_PLAYER:
		if(value2)
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d' AND `crid`='%d' AND `value`='%d'",quest_num,crid,value2);
		else
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d' AND `crid`='%d'",quest_num,crid);
		break;
	case CHOOSE_ALL:
		if(value2)
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d' AND `value`='%d'",quest_num,value2);
		else
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d'",quest_num);
		break;
	case CHOOSE_ALL_NOT_PLAYER:
		if(value2)
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d' AND `crid`<>'%d' AND `value`='%d'",quest_num,crid,value2);
		else
			sprintf(stradd,"SELECT `crid` FROM `quests` WHERE `quest_num`='%d' AND `crid`<>'%d'",quest_num,crid);
		break;
	default:
		return;
	}

	if(mysql_query(mySQL,stradd))
	{
		LogExecStr("mySQL ChangeQuest error: %s\n", mysql_error(mySQL));
		return;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return;
	int num_rows=0;
	if(!(num_rows=mysql_num_rows(mySQL_res))) return;

	mySQL_row=mysql_fetch_row(mySQL_res);

	for(int i=0;i<num_rows;++i) //!!!требуется оптимизация
	{
		Query("UPDATE `quests` SET `value`='%d' WHERE `quest_num`='%d' AND `crid`='%s'",value1,quest_num,mySQL_row[i]);
		//!!! отправлять данные игрокам о квестах
		//дублирование?
	}
}

int SQL::CodeParams(char* stats, char* skills, char* perks, crit_info* info)
{
	int cc;
	char tmpstr[6];

	stats[0]=0;
	for(cc=0; cc<ALL_STATS; cc++)
	{
		if(info->st[cc]>9999) info->st[cc]=0;

		sprintf(tmpstr,"%04d",info->st[cc]);

		strcat(stats,tmpstr);
	}
//	LogExecStr("stats:%s\n",stats);

	skills[0]=0;
	for(cc=0; cc<ALL_SKILLS; cc++)
	{
		if(info->sk[cc]>300) info->sk[cc]=299;

		sprintf(tmpstr,"%03d",info->sk[cc]);

		strcat(skills,tmpstr);
	}
//	LogExecStr("skills:%s\n",skills);

	perks[0]=0;
	for(cc=0; cc<ALL_PERKS; cc++)
	{
		if(info->pe[cc]>9) info->pe[cc]=0;

		sprintf(tmpstr,"%d",info->pe[cc]);

		strcat(perks,tmpstr);
	}
//	LogExecStr("perks:%s\n",perks);

	return 1;
}

int SQL::DecodeParams(char* stats, char* skills, char* perks, crit_info* info)
{
	int cc;
	char tmpstr[6];

	if(strlen(stats)!=(ALL_STATS*4)) return 0;
	else
	{
		for(cc=0; cc<ALL_STATS; cc++)
		{
			sprintf(tmpstr,"%c%c%c%c",stats[cc*4],stats[cc*4+1],stats[cc*4+2],stats[cc*4+3]);
			info->st[cc]=atoi(tmpstr);
		}
	}

	if(strlen(skills)!=(ALL_SKILLS*3)) return 0;
	else
	{
		for(cc=0; cc<ALL_SKILLS; cc++)
		{
			sprintf(tmpstr,"%c%c%c",skills[cc*3],skills[cc*3+1],skills[cc*3+2]);
			info->sk[cc]=atoi(tmpstr);
		}
	}

	if(strlen(perks)!=ALL_PERKS) return 0;
	else
	{
		for(cc=0; cc<ALL_PERKS; cc++)
		{
			sprintf(tmpstr,"%c",perks[cc]);
			info->pe[cc]=atoi(tmpstr);
		}
	}

	return 1;
}

int SQL::NewObject(dyn_obj* obj, DWORD obj_id)
{
	if(!obj) return 0;

	switch(obj->type)
	{
	case OBJ_TYPE_ARMOR:
		if(!Query("INSERT INTO `obj_armor` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_CONTAINER:
		if(!Query("INSERT INTO `obj_container` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_DRUG:
		if(!Query("INSERT INTO `obj_drug` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_WEAPON:
		if(!Query("INSERT INTO `obj_weapon` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_AMMO:
		if(!Query("INSERT INTO `obj_ammo` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_MISC:
		if(!Query("INSERT INTO `obj_misc` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_KEY:
		if(!Query("INSERT INTO `obj_key` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_DOOR:
		if(!Query("INSERT INTO `obj_door` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_CRAFTING:
		if(!Query("INSERT INTO `obj_crafting` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	case OBJ_TYPE_GRID:
		if(!Query("INSERT INTO `obj_grid` (id) VALUES ('%d')",obj_id)) return 0;
		break;
	default:
		return 0;
	}

	obj->id=obj_id;

	SaveDataObject(obj);

	return 1;
}

void SQL::SaveDataObject(dyn_obj* obj)
{
	DWORD acc1=0;
	DWORD acc2=0;

	switch(obj->accessory)
	{
	case DOBJ_ACCESSORY_CRIT:
		acc1=obj->ACC_CRITTER.id;
	//	acc2=(obj->ACC_CRITTER.slot<<8) | obj->ACC_CRITTER.active;
		acc2=obj->ACC_CRITTER.slot;
		break;
	case DOBJ_ACCESSORY_HEX:
		acc1=obj->ACC_HEX.map;
		acc2=(obj->ACC_HEX.x<<16) | obj->ACC_HEX.y;
		break;
	case DOBJ_ACCESSORY_CONT:
		acc1=obj->ACC_CONTAINER.id;
		acc2=0;
		break;
	default:
		return;
	}

	switch(obj->type)
	{
	case OBJ_TYPE_ARMOR:
		Query("UPDATE obj_armor SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_CONTAINER:
		Query("UPDATE obj_container SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_DRUG:
		Query("UPDATE obj_drug SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_WEAPON:
		Query("UPDATE obj_weapon SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_AMMO:
		Query("UPDATE obj_ammo SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_MISC:
		Query("UPDATE obj_misc SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_KEY:
		Query("UPDATE obj_key SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_DOOR:
		Query("UPDATE obj_door SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_CRAFTING:
		Query("UPDATE obj_crafting SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	case OBJ_TYPE_GRID:
		Query("UPDATE obj_grid SET proto_id='%d',accessory='%d',acc1='%d',acc2='%d',"
		"time_wear='%d',last_tick='%d',broken_info='%d' WHERE id='%d';",
		obj->object->id,obj->accessory,acc1,acc2,
		obj->time_wear,obj->last_tick,obj->broken_info,
		obj->id);
		break;
	default:
		return;
	}
}

int SQL::LoadDataObject(dyn_obj* obj, BYTE obj_type)
{
//	obj->object->p[0]=GetInt("objects","num_st","id",obj->id);
	char str[256];

	switch(obj_type)
	{
	case OBJ_TYPE_ARMOR:
		sprintf(str, "SELECT * FROM obj_armor WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_CONTAINER:
		sprintf(str, "SELECT * FROM obj_container WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_DRUG:
		sprintf(str, "SELECT * FROM obj_drug WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_WEAPON:
		sprintf(str, "SELECT * FROM obj_weapon WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_AMMO:
		sprintf(str, "SELECT * FROM obj_ammo WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_MISC:
		sprintf(str, "SELECT * FROM obj_misc WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_KEY:
		sprintf(str, "SELECT * FROM obj_key WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_DOOR:
		sprintf(str, "SELECT * FROM obj_door WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_CRAFTING:
		sprintf(str, "SELECT * FROM obj_crafting WHERE id='%d'", obj->id);
		break;
	case OBJ_TYPE_GRID:
		sprintf(str, "SELECT * FROM obj_grid WHERE id='%d'", obj->id);
		break;
	default:
		return 0;
	}

	if(mysql_query(mySQL, str))
	{
		LogExecStr("mySQL LoadDataObject error: %s\n", mysql_error(mySQL));
		return 0;
	}

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;
	if(!mysql_num_rows(mySQL_res)) return 0;
	mySQL_row=mysql_fetch_row(mySQL_res);

	obj->accessory=atoi(mySQL_row[2]);
	DWORD acc1=atoi(mySQL_row[3]);
	DWORD acc2=atoi(mySQL_row[4]);
	switch(obj->accessory)
	{
	case DOBJ_ACCESSORY_CRIT:
		obj->ACC_CRITTER.id=acc1;
		obj->ACC_CRITTER.slot=acc2;
	//	obj->ACC_CRITTER.slot=acc2>>24;
	//	obj->ACC_CRITTER.active=acc2&0xFF;
		break;
	case DOBJ_ACCESSORY_HEX:
		obj->ACC_HEX.map=acc1;
		obj->ACC_HEX.x=acc2>>16;
		obj->ACC_HEX.y=acc2&0xFFFF;
		break;
	case DOBJ_ACCESSORY_CONT:
		obj->ACC_CONTAINER.id=acc1;
		break;
	default:
		return 0;
	}

	obj->time_wear=atoi(mySQL_row[5]);
	obj->last_tick=atoi(mySQL_row[6]);
	obj->broken_info=atoi(mySQL_row[7]);

	return 1;
}

void SQL::DeleteDataObject(dyn_obj* obj)
{
	if(!obj) return;

	switch(obj->type)
	{
	case OBJ_TYPE_ARMOR:
		Query("DELETE FROM `obj_armor` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_CONTAINER:
		Query("DELETE FROM `obj_container` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_DRUG:
		Query("DELETE FROM `obj_drug` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_WEAPON:
		Query("DELETE FROM `obj_weapon` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_AMMO:
		Query("DELETE FROM `obj_ammo` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_MISC:
		Query("DELETE FROM `obj_misc` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_KEY:
		Query("DELETE FROM `obj_key` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_DOOR:
		Query("DELETE FROM `obj_door` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_CRAFTING:
		Query("DELETE FROM `obj_crafting` WHERE id='%d'",obj->id);
		break;
	case OBJ_TYPE_GRID:
		Query("DELETE FROM `obj_grid` WHERE id='%d'",obj->id);
		break;
	default:
		return;
	}
}

void SQL::AddCheat(CrID user_id, char* text_cheat)
{
	Query("INSERT INTO `users_cheat` (user_id,text_cheat) VALUES ('%d','%s');",user_id,text_cheat);
}

int SQL::CountTable(char* table, char* row)
{
	if(!Query("SELECT %s FROM %s", row, table)) return 0;

	if(!(mySQL_res=mysql_store_result(mySQL))) return 0;

	return mysql_num_rows(mySQL_res);
}


void SQL::PrintTableInLog(char* table, char* rows)
{
	LogExecStr("Выводим таблицу: %s\n", table);

	if(!Query("SELECT %s FROM %s",rows,table)) return;
	
	if(!(mySQL_res=mysql_store_result(mySQL))) return;

	LogExecStr("Всего записей: %d \n",mysql_num_rows(mySQL_res));

	while((mySQL_row=mysql_fetch_row(mySQL_res)))
	{
		for(int pt=0; pt<mysql_num_fields(mySQL_res); pt++)
			LogExecStr("%s - ",mySQL_row[pt]); 
		LogExecStr(" | \n ");
	}

}

void SQL::WriteLog(char* str)
{
	char stradd[2176];
	sprintf(stradd,"INSERT INTO `log` (log) VALUES ('%s');", str);

	if(mysql_query(mySQL,stradd))
	{
		logging=1;
		LogExecStr("mySQL WriteLog error: %s\n", mysql_error(mySQL));
	}
}