#include "stdafx.h"
#include "FOserv.h"

int CServer::LoadAllPlayers()
{
	LogExecStr("Загрузка игроков\n");
	//узнаем кол-во записей всего
	int find_cl=sql.CountTable("users","id")-1;
		
	sql.PrintTableInLog("users","*");

	char str_login[MAX_LOGIN+1];
	char str_pass[MAX_LOGIN+1];

	int add_cl=0;
	int num_cl=10001;
	if(find_cl)
		while(add_cl<find_cl)
		{
			if(num_cl==sql.GetInt("users","id","id",num_cl))
			{
				CCritter* ncl=new CCritter;
				ncl->s=NULL;
				ncl->info.id=num_cl;
				ncl->state=STATE_DISCONNECT;
				sql.GetChar("users","login","id",num_cl,&str_login[0]);
				sql.GetChar("users","pass","id",num_cl,&str_pass[0]);
				strcpy(ncl->info.login,&str_login[0]);
				strcpy(ncl->info.pass,&str_pass[0]);
				sql.LoadDataPlayer(&ncl->info);

				ncl->InitClient();

				ncl->info.cond=COND_LIFE;
				ncl->info.cond_ext=COND_LIFE_NONE;
				ncl->info.st[ST_CURRENT_HP]=ncl->info.st[ST_MAX_LIFE]; //!!!!!!!!
				
				ncl->info.a_obj=&ncl->info.def_obj1;
				ncl->info.a_obj_arm=&ncl->info.def_obj2;

				ncl->info.a_obj->object=all_s_obj[ncl->info.base_type];
				ncl->info.a_obj_arm->object=all_s_obj[ncl->info.base_type+200];
				//добавляем в список карты
				if(AddCrToMap(ncl,ncl->info.map,ncl->info.x,ncl->info.y)!=TR_OK) return 0;
				//добовляем в общий список
				cr[ncl->info.id]=ncl;
				add_cl++;

//				NumCritters++;
			}
			num_cl++;
		}

	LogExecStr("Загрузка игроков прошло успешно\n");
	return 1;
}

void CServer::SaveAllDataPlayers()
{
	cl_map::iterator it;
//сохраняем клиентов
	for(it=cl.begin();it!=cl.end();it++)
	{
		sql.SaveDataPlayer(&(*it).second->info);
	}
//сохраняем нпц
	for(it=pc.begin();it!=pc.end();it++)
	{
		sql.SaveDataNPC(&(*it).second->info);
	}
}

void CServer::GenWear(dyn_obj* wear_obj, bool use_obj)
{
//	wear_obj->tick-=GetTickCount()-wear_obj->last_tick;
//	if(use_obj)
//		wear_obj->tick-=wear_obj->object->p[17]/2000; //штраф за использование
//	wear_obj->last_tick=wear_obj->tick;
}

void CServer::CreateParamsMaps()
{
	stats_map.insert(params_map::value_type("ST_STRENGHT",				ST_STRENGHT));
	stats_map.insert(params_map::value_type("ST_PERCEPTION",			ST_PERCEPTION));
	stats_map.insert(params_map::value_type("ST_ENDURANCE",				ST_ENDURANCE));
	stats_map.insert(params_map::value_type("ST_CHARISMA",				ST_CHARISMA));
	stats_map.insert(params_map::value_type("ST_INTELLECT",				ST_INTELLECT));
	stats_map.insert(params_map::value_type("ST_AGILITY",				ST_AGILITY));
	stats_map.insert(params_map::value_type("ST_LUCK",					ST_LUCK));
	stats_map.insert(params_map::value_type("ST_MAX_LIFE",				ST_MAX_LIFE));
	stats_map.insert(params_map::value_type("ST_MAX_COND",				ST_MAX_COND));
	stats_map.insert(params_map::value_type("ST_ARMOR_CLASS",			ST_ARMOR_CLASS));
	stats_map.insert(params_map::value_type("ST_MELEE_DAMAGE",			ST_MELEE_DAMAGE));
	stats_map.insert(params_map::value_type("ST_WEAPON_DAMAGE",			ST_WEAPON_DAMAGE));
	stats_map.insert(params_map::value_type("ST_CARRY_WEIGHT",			ST_CARRY_WEIGHT));
	stats_map.insert(params_map::value_type("ST_SEQUENCE",				ST_SEQUENCE));
	stats_map.insert(params_map::value_type("ST_HEALING_RATE",			ST_HEALING_RATE));
	stats_map.insert(params_map::value_type("ST_CRITICAL_CHANCE",		ST_CRITICAL_CHANCE));
	stats_map.insert(params_map::value_type("ST_MAX_CRITICAL",			ST_MAX_CRITICAL));
	stats_map.insert(params_map::value_type("ST_INGURE_ABSORB",			ST_INGURE_ABSORB));
	stats_map.insert(params_map::value_type("ST_LASER_ABSORB",			ST_LASER_ABSORB));
	stats_map.insert(params_map::value_type("ST_FIRE_ABSORB",			ST_FIRE_ABSORB));
	stats_map.insert(params_map::value_type("ST_PLASMA_ABSORB",			ST_PLASMA_ABSORB));
	stats_map.insert(params_map::value_type("ST_ELECTRO_ABSORB",		ST_ELECTRO_ABSORB));
	stats_map.insert(params_map::value_type("ST_EMP_ABSORB",			ST_EMP_ABSORB));
	stats_map.insert(params_map::value_type("ST_BLAST_ABSORB",			ST_BLAST_ABSORB));
	stats_map.insert(params_map::value_type("ST_INGURE_RESIST",			ST_INGURE_RESIST));
	stats_map.insert(params_map::value_type("ST_LASER_RESIST",			ST_LASER_RESIST));
	stats_map.insert(params_map::value_type("ST_FIRE_RESIST",			ST_FIRE_RESIST));
	stats_map.insert(params_map::value_type("ST_PLASMA_RESIST",			ST_PLASMA_RESIST));
	stats_map.insert(params_map::value_type("ST_ELECTRO_RESIST",		ST_ELECTRO_RESIST));
	stats_map.insert(params_map::value_type("ST_EMP_RESIST",			ST_EMP_RESIST));
	stats_map.insert(params_map::value_type("ST_BLAST_RESIST",			ST_BLAST_RESIST));
	stats_map.insert(params_map::value_type("ST_RADIATION_RESISTANCE",	ST_RADIATION_RESISTANCE));
	stats_map.insert(params_map::value_type("ST_POISON_RESISTANCE",		ST_POISON_RESISTANCE));
	stats_map.insert(params_map::value_type("ST_AGE",					ST_AGE));
	stats_map.insert(params_map::value_type("ST_GENDER",				ST_GENDER));
	stats_map.insert(params_map::value_type("ST_CURRENT_HP",			ST_CURRENT_HP));
	stats_map.insert(params_map::value_type("ST_POISONING_LEVEL",		ST_POISONING_LEVEL));
	stats_map.insert(params_map::value_type("ST_RADIATION_LEVEL",		ST_RADIATION_LEVEL));
	stats_map.insert(params_map::value_type("ST_CURRENT_STANDART",		ST_CURRENT_STANDART));

	skills_map.insert(params_map::value_type("SK_SMALL_GUNS",			SK_SMALL_GUNS));
	skills_map.insert(params_map::value_type("SK_BIG_GUNS",				SK_BIG_GUNS));
	skills_map.insert(params_map::value_type("SK_ENERGY_WEAPONS",		SK_ENERGY_WEAPONS));
	skills_map.insert(params_map::value_type("SK_UNARMED",				SK_UNARMED));
	skills_map.insert(params_map::value_type("SK_MELEE_WEAPONS",		SK_MELEE_WEAPONS));
	skills_map.insert(params_map::value_type("SK_THROWING",				SK_THROWING));
	skills_map.insert(params_map::value_type("SK_FIRST_AID",			SK_FIRST_AID));
	skills_map.insert(params_map::value_type("SK_DOCTOR",				SK_DOCTOR));
	skills_map.insert(params_map::value_type("SK_SNEAK",				SK_SNEAK));
	skills_map.insert(params_map::value_type("SK_LOCKPICK",				SK_LOCKPICK));
	skills_map.insert(params_map::value_type("SK_STEAL",				SK_STEAL));
	skills_map.insert(params_map::value_type("SK_TRAPS",				SK_TRAPS));
	skills_map.insert(params_map::value_type("SK_SCIENCE",				SK_SCIENCE));
	skills_map.insert(params_map::value_type("SK_REPAIR",				SK_REPAIR));
	skills_map.insert(params_map::value_type("SK_SPEECH",				SK_SPEECH));
	skills_map.insert(params_map::value_type("SK_BARTER",				SK_BARTER));
	skills_map.insert(params_map::value_type("SK_GAMBLING",				SK_GAMBLING));
	skills_map.insert(params_map::value_type("SK_OUTDOORSMAN",			SK_OUTDOORSMAN));

	perks_map.insert(params_map::value_type("PE_FAST_METABOLISM",		PE_FAST_METABOLISM));
	perks_map.insert(params_map::value_type("PE_BRUISER",				PE_BRUISER));
	perks_map.insert(params_map::value_type("PE_SMALL_FRAME",			PE_SMALL_FRAME));
	perks_map.insert(params_map::value_type("PE_ONE_HANDER",			PE_ONE_HANDER));
	perks_map.insert(params_map::value_type("PE_FINESSE",				PE_FINESSE));
	perks_map.insert(params_map::value_type("PE_KAMIKAZE",				PE_KAMIKAZE));
	perks_map.insert(params_map::value_type("PE_HEAVY_HANDED",			PE_HEAVY_HANDED));
	perks_map.insert(params_map::value_type("PE_FAST_SHOT",				PE_FAST_SHOT));
	perks_map.insert(params_map::value_type("PE_BLOODY_MESS",			PE_BLOODY_MESS));
	perks_map.insert(params_map::value_type("PE_JINXED",				PE_JINXED));
	perks_map.insert(params_map::value_type("PE_GOOD_NATURED",			PE_GOOD_NATURED));
	perks_map.insert(params_map::value_type("PE_CHEM_RELIANT",			PE_CHEM_RELIANT));
	perks_map.insert(params_map::value_type("PE_CHEM_RESISTANT",		PE_CHEM_RESISTANT));
	perks_map.insert(params_map::value_type("PE_NIGHT_PERSON",			PE_NIGHT_PERSON));
	perks_map.insert(params_map::value_type("PE_SKILLED",				PE_SKILLED));
	perks_map.insert(params_map::value_type("PE_GIFTED",				PE_GIFTED));
	perks_map.insert(params_map::value_type("PE_AWARENESS",				PE_AWARENESS));
	perks_map.insert(params_map::value_type("PE_A_MELEE_ATT",			PE_A_MELEE_ATT));
	perks_map.insert(params_map::value_type("PE_A_MELEE_DAM",			PE_A_MELEE_DAM));
	perks_map.insert(params_map::value_type("PE_A_MOVE",				PE_A_MOVE));
	perks_map.insert(params_map::value_type("PE_A_DAM",					PE_A_DAM));
	perks_map.insert(params_map::value_type("PE_A_SPEED",				PE_A_SPEED));
	perks_map.insert(params_map::value_type("PE_PASS_FRONT",			PE_PASS_FRONT));
	perks_map.insert(params_map::value_type("PE_RAPID_HEAL",			PE_RAPID_HEAL));
	perks_map.insert(params_map::value_type("PE_MORE_CRIT_DAM",			PE_MORE_CRIT_DAM));
	perks_map.insert(params_map::value_type("PE_NIGHT_SIGHT",			PE_NIGHT_SIGHT));
	perks_map.insert(params_map::value_type("PE_PRESENCE",				PE_PRESENCE));
	perks_map.insert(params_map::value_type("PE_RES_NUKLEAR",			PE_RES_NUKLEAR));
	perks_map.insert(params_map::value_type("PE_ENDURENCE",				PE_ENDURENCE));
	perks_map.insert(params_map::value_type("PE_STR_BACK",				PE_STR_BACK));
	perks_map.insert(params_map::value_type("PE_MARKSMAN",				PE_MARKSMAN));
	perks_map.insert(params_map::value_type("PE_STEALHING",				PE_STEALHING));
	perks_map.insert(params_map::value_type("PE_LIFEFULL",				PE_LIFEFULL));
	perks_map.insert(params_map::value_type("PE_MERCHANT",				PE_MERCHANT));
	perks_map.insert(params_map::value_type("PE_FORMED",				PE_FORMED));
	perks_map.insert(params_map::value_type("PE_HEALER",				PE_HEALER));
	perks_map.insert(params_map::value_type("PE_TR_DIGGER",				PE_TR_DIGGER));
	perks_map.insert(params_map::value_type("PE_BEST_HITS",				PE_BEST_HITS));
	perks_map.insert(params_map::value_type("PE_COMPASION",				PE_COMPASION));
	perks_map.insert(params_map::value_type("PE_KILLER",				PE_KILLER));
	perks_map.insert(params_map::value_type("PE_SNIPER",				PE_SNIPER));
	perks_map.insert(params_map::value_type("PE_SILENT_DEATH",			PE_SILENT_DEATH));
	perks_map.insert(params_map::value_type("PE_C_FIGHTER",				PE_C_FIGHTER));
	perks_map.insert(params_map::value_type("PE_MIND_BLOCK",			PE_MIND_BLOCK));
	perks_map.insert(params_map::value_type("PE_PROLONGATION_LIFE",		PE_PROLONGATION_LIFE));
	perks_map.insert(params_map::value_type("PE_RECOURCEFULNESS",		PE_RECOURCEFULNESS));
	perks_map.insert(params_map::value_type("PE_SNAKE_EATER",			PE_SNAKE_EATER));
	perks_map.insert(params_map::value_type("PE_REPEARER",				PE_REPEARER));
	perks_map.insert(params_map::value_type("PE_MEDIC",					PE_MEDIC));
	perks_map.insert(params_map::value_type("PE_SKILLED_THIEF",			PE_SKILLED_THIEF));
	perks_map.insert(params_map::value_type("PE_SPEAKER",				PE_SPEAKER));
	perks_map.insert(params_map::value_type("PE_GUTCHER",				PE_GUTCHER));
	perks_map.insert(params_map::value_type("PE_UNKNOWN_1",				PE_UNKNOWN_1));
	perks_map.insert(params_map::value_type("PE_PICK_POCKER",			PE_PICK_POCKER));
	perks_map.insert(params_map::value_type("PE_GHOST",					PE_GHOST));
	perks_map.insert(params_map::value_type("PE_CHAR_CULT",				PE_CHAR_CULT));
	perks_map.insert(params_map::value_type("PE_THIFER",				PE_THIFER));
	perks_map.insert(params_map::value_type("PE_DISCOVER",				PE_DISCOVER));
	perks_map.insert(params_map::value_type("PE_OVERROAD",				PE_OVERROAD));
	perks_map.insert(params_map::value_type("PE_ANIMAL_FRIENDSHIP",		PE_ANIMAL_FRIENDSHIP));
	perks_map.insert(params_map::value_type("PE_SCOUT",					PE_SCOUT));
	perks_map.insert(params_map::value_type("PE_MIST_CHAR",				PE_MIST_CHAR));
	perks_map.insert(params_map::value_type("PE_RANGER",				PE_RANGER));
	perks_map.insert(params_map::value_type("PE_PICK_POCKET_2",			PE_PICK_POCKET_2));
	perks_map.insert(params_map::value_type("PE_INTERLOCUTER",			PE_INTERLOCUTER));
	perks_map.insert(params_map::value_type("PE_NOVICE",				PE_NOVICE));
	perks_map.insert(params_map::value_type("PE_PRIME_SKILL",			PE_PRIME_SKILL));
	perks_map.insert(params_map::value_type("PE_MUTATION",				PE_MUTATION));
	perks_map.insert(params_map::value_type("PE_NARC_NUKACOLA",			PE_NARC_NUKACOLA));
	perks_map.insert(params_map::value_type("PE_NARC_BUFFOUT",			PE_NARC_BUFFOUT));
	perks_map.insert(params_map::value_type("PE_NARC_MENTAT",			PE_NARC_MENTAT));
	perks_map.insert(params_map::value_type("PE_NARC_PSYHO",			PE_NARC_PSYHO));
	perks_map.insert(params_map::value_type("PE_NARC_RADAWAY",			PE_NARC_RADAWAY));
	perks_map.insert(params_map::value_type("PE_DISTANT_WEAP",			PE_DISTANT_WEAP));
	perks_map.insert(params_map::value_type("PE_ACCURARY_WEAP",			PE_ACCURARY_WEAP));
	perks_map.insert(params_map::value_type("PE_PENETRATION_WEAP",		PE_PENETRATION_WEAP));
	perks_map.insert(params_map::value_type("PE_KILLER_WEAP",			PE_KILLER_WEAP));
	perks_map.insert(params_map::value_type("PE_ENERGY_ARMOR",			PE_ENERGY_ARMOR));
	perks_map.insert(params_map::value_type("PE_BATTLE_ARMOR",			PE_BATTLE_ARMOR));
	perks_map.insert(params_map::value_type("PE_WEAP_RANGE",			PE_WEAP_RANGE));
	perks_map.insert(params_map::value_type("PE_RAPID_RELOAD",			PE_RAPID_RELOAD));
	perks_map.insert(params_map::value_type("PE_NIGHT_SPYGLASS",		PE_NIGHT_SPYGLASS));
	perks_map.insert(params_map::value_type("PE_FLAMER",				PE_FLAMER));
	perks_map.insert(params_map::value_type("PE_APA_I",					PE_APA_I));
	perks_map.insert(params_map::value_type("PE_APA_II",				PE_APA_II));
	perks_map.insert(params_map::value_type("PE_FORCEAGE",				PE_FORCEAGE));
	perks_map.insert(params_map::value_type("PE_DEADLY_NARC",			PE_DEADLY_NARC));
	perks_map.insert(params_map::value_type("PE_CHARMOLEANCE",			PE_CHARMOLEANCE));
	perks_map.insert(params_map::value_type("PE_GEKK_SKINER",			PE_GEKK_SKINER));
	perks_map.insert(params_map::value_type("PE_SKIN_ARMOR",			PE_SKIN_ARMOR));
	perks_map.insert(params_map::value_type("PE_A_SKIN_ARMOR",			PE_A_SKIN_ARMOR));
	perks_map.insert(params_map::value_type("PE_SUPER_ARMOR",			PE_SUPER_ARMOR));
	perks_map.insert(params_map::value_type("PE_A_SUPER_ARMOR",			PE_A_SUPER_ARMOR));
	perks_map.insert(params_map::value_type("PE_VAULT_INOCUL",			PE_VAULT_INOCUL));
	perks_map.insert(params_map::value_type("PE_ADRENALIN_RUSH",		PE_ADRENALIN_RUSH));
	perks_map.insert(params_map::value_type("PE_CAREFULL",				PE_CAREFULL));
	perks_map.insert(params_map::value_type("PE_INTELEGENCE",			PE_INTELEGENCE));
	perks_map.insert(params_map::value_type("PE_PYROKASY",				PE_PYROKASY));
	perks_map.insert(params_map::value_type("PE_DUDE",					PE_DUDE));
	perks_map.insert(params_map::value_type("PE_A_STR",					PE_A_STR));
	perks_map.insert(params_map::value_type("PE_A_PER",					PE_A_PER));
	perks_map.insert(params_map::value_type("PE_A_END",					PE_A_END));
	perks_map.insert(params_map::value_type("PE_A_CHA",					PE_A_CHA));
	perks_map.insert(params_map::value_type("PE_A_INT",					PE_A_INT));
	perks_map.insert(params_map::value_type("PE_A_AGL",					PE_A_AGL));
	perks_map.insert(params_map::value_type("PE_A_LUC",					PE_A_LUC));
	perks_map.insert(params_map::value_type("PE_PURERER",				PE_PURERER));
	perks_map.insert(params_map::value_type("PE_IMAG",					PE_IMAG));
	perks_map.insert(params_map::value_type("PE_EVASION",				PE_EVASION));
	perks_map.insert(params_map::value_type("PE_DROSHKADRAT",			PE_DROSHKADRAT));
	perks_map.insert(params_map::value_type("PE_KARMA_GLOW",			PE_KARMA_GLOW));
	perks_map.insert(params_map::value_type("PE_SILENT_STEPS",			PE_SILENT_STEPS));
	perks_map.insert(params_map::value_type("PE_ANATOMY",				PE_ANATOMY));
	perks_map.insert(params_map::value_type("PE_CHAMER",				PE_CHAMER));
	perks_map.insert(params_map::value_type("PE_ORATOR",				PE_ORATOR));
	perks_map.insert(params_map::value_type("PE_PACKER",				PE_PACKER));
	perks_map.insert(params_map::value_type("PE_EDD_GAYAN_MANIAC",		PE_EDD_GAYAN_MANIAC));
	perks_map.insert(params_map::value_type("PE_FAST_REGENERATION",		PE_FAST_REGENERATION));
	perks_map.insert(params_map::value_type("PE_VENDOR",				PE_VENDOR));
	perks_map.insert(params_map::value_type("PE_STONE_WALL",			PE_STONE_WALL));
	perks_map.insert(params_map::value_type("PE_THIEF_AGAIN",			PE_THIEF_AGAIN));
	perks_map.insert(params_map::value_type("PE_WEAPON_SKILL",			PE_WEAPON_SKILL));
	perks_map.insert(params_map::value_type("PE_MAKE_VAULT",			PE_MAKE_VAULT));
	perks_map.insert(params_map::value_type("PE_ALC_BUFF_1",			PE_ALC_BUFF_1));
	perks_map.insert(params_map::value_type("PE_ALC_BUFF_2",			PE_ALC_BUFF_2));
/*	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
	perks_map.insert(params_map::value_type("!",!));
*/
	perks_map.insert(params_map::value_type("PE_HIDE_MODE",				PE_HIDE_MODE));

	object_map.insert(params_map::value_type("OBJ_TYPE_ARMOR",			OBJ_TYPE_ARMOR));
	object_map.insert(params_map::value_type("OBJ_TYPE_CONTAINER",		OBJ_TYPE_CONTAINER));
	object_map.insert(params_map::value_type("OBJ_TYPE_DRUG",			OBJ_TYPE_DRUG));
	object_map.insert(params_map::value_type("OBJ_TYPE_WEAPON",			OBJ_TYPE_WEAPON));
	object_map.insert(params_map::value_type("OBJ_TYPE_AMMO",			OBJ_TYPE_AMMO));
	object_map.insert(params_map::value_type("OBJ_TYPE_MISC",			OBJ_TYPE_MISC));
	object_map.insert(params_map::value_type("OBJ_TYPE_KEY",			OBJ_TYPE_KEY));
	object_map.insert(params_map::value_type("OBJ_TYPE_DOOR",			OBJ_TYPE_DOOR));
	object_map.insert(params_map::value_type("OBJ_NAME",				OBJ_NAME));
	object_map.insert(params_map::value_type("OBJ_INFO",				OBJ_INFO));
	object_map.insert(params_map::value_type("OBJ_TIME_SHOW",			OBJ_TIME_SHOW));
	object_map.insert(params_map::value_type("OBJ_TIME_HIDE",			OBJ_TIME_HIDE));
	object_map.insert(params_map::value_type("OBJ_DISTANCE_LIGHT",		OBJ_DISTANCE_LIGHT));
	object_map.insert(params_map::value_type("OBJ_INTENSITY_LIGHT",		OBJ_INTENSITY_LIGHT));
	object_map.insert(params_map::value_type("OBJ_PASSED",				OBJ_PASSED));
	object_map.insert(params_map::value_type("OBJ_RAKED",				OBJ_RAKED));
	object_map.insert(params_map::value_type("OBJ_TRANSPARENT",			OBJ_TRANSPARENT));
	object_map.insert(params_map::value_type("OBJ_CAN_USE",				OBJ_CAN_USE));
	object_map.insert(params_map::value_type("OBJ_CAN_PICK_UP",			OBJ_CAN_PICK_UP));
	object_map.insert(params_map::value_type("OBJ_CAN_USE_ON_SMTH",		OBJ_CAN_USE_ON_SMTH));
	object_map.insert(params_map::value_type("OBJ_HIDDEN",				OBJ_HIDDEN));
	object_map.insert(params_map::value_type("OBJ_WEIGHT",				OBJ_WEIGHT));
	object_map.insert(params_map::value_type("OBJ_SIZE",				OBJ_SIZE));
	object_map.insert(params_map::value_type("OBJ_TWOHANDS",			OBJ_TWOHANDS));
	object_map.insert(params_map::value_type("OBJ_PIC_MAP",				OBJ_PIC_MAP));
	object_map.insert(params_map::value_type("OBJ_ANIM_ON_MAP",			OBJ_ANIM_ON_MAP));
	object_map.insert(params_map::value_type("OBJ_PIC_INV",				OBJ_PIC_INV));
	object_map.insert(params_map::value_type("OBJ_SOUND",				OBJ_SOUND));
	object_map.insert(params_map::value_type("OBJ_LIVETIME",			OBJ_LIVETIME));
	object_map.insert(params_map::value_type("OBJ_COST",				OBJ_COST));
	object_map.insert(params_map::value_type("OBJ_MATERIAL",			OBJ_MATERIAL));
	object_map.insert(params_map::value_type("OBJ_ARM_ANIM0_MALE",		OBJ_ARM_ANIM0_MALE));
	object_map.insert(params_map::value_type("OBJ_ARM_ANIM0_MALE2",		OBJ_ARM_ANIM0_MALE2));
	object_map.insert(params_map::value_type("OBJ_ARM_ANIM0_FEMALE",	OBJ_ARM_ANIM0_FEMALE));
	object_map.insert(params_map::value_type("OBJ_ARM_ANIM0_FEMALE2",	OBJ_ARM_ANIM0_FEMALE2));
	object_map.insert(params_map::value_type("OBJ_ARM_AC",				OBJ_ARM_AC));
	object_map.insert(params_map::value_type("OBJ_ARM_PERK",			OBJ_ARM_PERK));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_NORMAL",		OBJ_ARM_DR_NORMAL));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_LASER",		OBJ_ARM_DR_LASER));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_FIRE",			OBJ_ARM_DR_FIRE));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_PLASMA",		OBJ_ARM_DR_PLASMA));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_ELECTR",		OBJ_ARM_DR_ELECTR));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_EMP",			OBJ_ARM_DR_EMP));
	object_map.insert(params_map::value_type("OBJ_ARM_DR_EXPLODE",		OBJ_ARM_DR_EXPLODE));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_NORMAL",		OBJ_ARM_DT_NORMAL));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_LASER",		OBJ_ARM_DT_LASER));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_FIRE",			OBJ_ARM_DT_FIRE));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_PLASMA",		OBJ_ARM_DT_PLASMA));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_ELECTR",		OBJ_ARM_DT_ELECTR));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_EMP",			OBJ_ARM_DT_EMP));
	object_map.insert(params_map::value_type("OBJ_ARM_DT_EXPLODE",		OBJ_ARM_DT_EXPLODE));
	object_map.insert(params_map::value_type("OBJ_CONT_SIZE",			OBJ_CONT_SIZE));
	object_map.insert(params_map::value_type("OBJ_CONT_FLAG",			OBJ_CONT_FLAG));
	object_map.insert(params_map::value_type("OBJ_DRUG_STAT0",			OBJ_DRUG_STAT0));
	object_map.insert(params_map::value_type("OBJ_DRUG_STAT1",			OBJ_DRUG_STAT1));
	object_map.insert(params_map::value_type("OBJ_DRUG_STAT2",			OBJ_DRUG_STAT2));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT0_S0",		OBJ_DRUG_AMOUNT0_S0));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT0_S1",		OBJ_DRUG_AMOUNT0_S1));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT0_S2",		OBJ_DRUG_AMOUNT0_S2));
	object_map.insert(params_map::value_type("OBJ_DRUG_DURATION1",		OBJ_DRUG_DURATION1));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT1_S0",		OBJ_DRUG_AMOUNT1_S0));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT1_S1",		OBJ_DRUG_AMOUNT1_S1));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT1_S2",		OBJ_DRUG_AMOUNT1_S2));
	object_map.insert(params_map::value_type("OBJ_DRUG_DURATION2",		OBJ_DRUG_DURATION2));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT2_S0",		OBJ_DRUG_AMOUNT2_S0));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT2_S1",		OBJ_DRUG_AMOUNT2_S1));
	object_map.insert(params_map::value_type("OBJ_DRUG_AMOUNT2_S2",		OBJ_DRUG_AMOUNT2_S2));
	object_map.insert(params_map::value_type("OBJ_DRUG_ADDICTION",		OBJ_DRUG_ADDICTION));
	object_map.insert(params_map::value_type("OBJ_DRUG_W_EFFECT",		OBJ_DRUG_W_EFFECT));
	object_map.insert(params_map::value_type("OBJ_DRUG_W_ONSET",		OBJ_DRUG_W_ONSET));
	object_map.insert(params_map::value_type("OBJ_WEAP_ANIM1",			OBJ_WEAP_ANIM1));
	object_map.insert(params_map::value_type("OBJ_WEAP_TIME_ACTIV",		OBJ_WEAP_TIME_ACTIV));
	object_map.insert(params_map::value_type("OBJ_WEAP_TIME_UNACTIV",	OBJ_WEAP_TIME_UNACTIV));
	object_map.insert(params_map::value_type("OBJ_WEAP_VOL_HOLDER",		OBJ_WEAP_VOL_HOLDER));
	object_map.insert(params_map::value_type("OBJ_WEAP_CALIBER",		OBJ_WEAP_CALIBER));
	object_map.insert(params_map::value_type("OBJ_WEAP_VOL_HOLDER_E",	OBJ_WEAP_VOL_HOLDER_E));
	object_map.insert(params_map::value_type("OBJ_WEAP_CALIBER_E",		OBJ_WEAP_CALIBER_E));
	object_map.insert(params_map::value_type("OBJ_WEAP_CR_FAILTURE",	OBJ_WEAP_CR_FAILTURE));
	object_map.insert(params_map::value_type("OBJ_WEAP_TYPE_ATTACK",	OBJ_WEAP_TYPE_ATTACK));
	object_map.insert(params_map::value_type("OBJ_WEAP_COUNT_ATTACK",	OBJ_WEAP_COUNT_ATTACK));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_SKILL",		OBJ_WEAP_PA_SKILL));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_HOLDER",		OBJ_WEAP_PA_HOLDER));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_PIC",			OBJ_WEAP_PA_PIC));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_DMG_MIN",		OBJ_WEAP_PA_DMG_MIN));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_DMG_MAX",		OBJ_WEAP_PA_DMG_MAX));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_MAX_DIST",	OBJ_WEAP_PA_MAX_DIST));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_EFF_DIST",	OBJ_WEAP_PA_EFF_DIST));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_ANIM2",		OBJ_WEAP_PA_ANIM2));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_TIME",		OBJ_WEAP_PA_TIME));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_AIM",			OBJ_WEAP_PA_AIM));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_ROUND",		OBJ_WEAP_PA_ROUND));
	object_map.insert(params_map::value_type("OBJ_WEAP_PA_REMOVE",		OBJ_WEAP_PA_REMOVE));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_SKILL",		OBJ_WEAP_SA_SKILL));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_HOLDER",		OBJ_WEAP_SA_HOLDER));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_PIC",			OBJ_WEAP_SA_PIC));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_DMG_MIN",		OBJ_WEAP_SA_DMG_MIN));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_DMG_MAX",		OBJ_WEAP_SA_DMG_MAX));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_MAX_DIST",	OBJ_WEAP_SA_MAX_DIST));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_EFF_DIST",	OBJ_WEAP_SA_EFF_DIST));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_ANIM2",		OBJ_WEAP_SA_ANIM2));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_TIME",		OBJ_WEAP_SA_TIME));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_AIM",			OBJ_WEAP_SA_AIM));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_ROUND",		OBJ_WEAP_SA_ROUND));
	object_map.insert(params_map::value_type("OBJ_WEAP_SA_REMOVE",		OBJ_WEAP_SA_REMOVE));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_SKILL",		OBJ_WEAP_TA_SKILL));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_HOLDER",		OBJ_WEAP_TA_HOLDER));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_PIC",			OBJ_WEAP_TA_PIC));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_DMG_MIN",		OBJ_WEAP_TA_DMG_MIN));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_DMG_MAX",		OBJ_WEAP_TA_DMG_MAX));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_MAX_DIST",	OBJ_WEAP_TA_MAX_DIST));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_EFF_DIST",	OBJ_WEAP_TA_EFF_DIST));	
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_ANIM2",		OBJ_WEAP_TA_ANIM2));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_TIME",		OBJ_WEAP_TA_TIME));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_AIM",			OBJ_WEAP_TA_AIM));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_ROUND",		OBJ_WEAP_TA_ROUND));
	object_map.insert(params_map::value_type("OBJ_WEAP_TA_REMOVE",		OBJ_WEAP_TA_REMOVE));
	object_map.insert(params_map::value_type("OBJ_AMMO_CALIBER",		OBJ_AMMO_CALIBER));
	object_map.insert(params_map::value_type("OBJ_AMMO_TYPE_DAMAGE",	OBJ_AMMO_TYPE_DAMAGE));
	object_map.insert(params_map::value_type("OBJ_AMMO_QUANTITY",		OBJ_AMMO_QUANTITY));
	object_map.insert(params_map::value_type("OBJ_AMMO_AC",				OBJ_AMMO_AC));
	object_map.insert(params_map::value_type("OBJ_AMMO_DR",				OBJ_AMMO_DR));
	object_map.insert(params_map::value_type("OBJ_AMMO_DM",				OBJ_AMMO_DM));
	object_map.insert(params_map::value_type("OBJ_AMMO_DD",				OBJ_AMMO_DD));
}

int CServer::UpdateVarsTemplate()
{
	LogExecStr("Обновление шаблонов переменных игроков\n");

	FILE *cf;

	if(!(cf=fopen("data\\server\\data\\vars.txt","rt")))
	{
		LogExecStr("\tФайл vars не найден\n");
		return 1;
	}

	char ch;
	int tmp_int1=0,tmp_int2=0,tmp_int3=0,tmp_int4=0;
	int err_update=0;

	while(!feof(cf))
	{
		fscanf(cf,"%c",&ch);

		if(ch=='#')
		{
			LogExecStr("\tНовая переменная...");
			if(!fscanf(cf,"%d",&tmp_int1))
			{
				err_update++;
				LogExecStr("ошибка чтения\n");
				continue;
			}
			LogExecStr("номер=%d...",tmp_int1);

			if(sql.CountRows("player_vars_templates","var_num",tmp_int1))
			{
				LogExecStr("уже создана\n");
				continue;
			}

			if(!fscanf(cf,"%d",&tmp_int2))
			{
				err_update++;
				LogExecStr("ошибка чтения\n");
				continue;
			}
			LogExecStr("стартовое значение=%d...",tmp_int2);

			if(!fscanf(cf,"%d",&tmp_int3))
			{
				err_update++;
				LogExecStr("ошибка чтения\n");
				continue;
			}
			LogExecStr("min=%d...",tmp_int3);

			if(!fscanf(cf,"%d",&tmp_int4))
			{
				err_update++;
				LogExecStr("ошибка чтения\n");
				continue;
			}
			LogExecStr("max=%d...",tmp_int4);

			if(tmp_int3>tmp_int4 || tmp_int2<tmp_int3 || tmp_int2>tmp_int4)
			{
				err_update++;
				LogExecStr("несоответствие в данных\n");
				continue;
			}
			
			sql.Query("INSERT INTO `player_vars_templates` (var_num,count,min,max) "
			"VALUES ('%d','%d','%d','%d');",tmp_int1,tmp_int2,tmp_int3,tmp_int4);
			LogExecStr("добавлена\n");
		}
	}

	LogExecStr("Обновление шаблонов переменных игроков прошло успешно\n");
	return err_update;
}

void CServer::Skill_Sneak_Change(CCritter* acl)
{
	acl->info.pe[PE_HIDE_MODE]=acl->info.pe[PE_HIDE_MODE]?FALSE:TRUE;
	Send_Param(acl,TYPE_PERK,PE_HIDE_MODE);
	SetVisCr(acl);
}

void CServer::Skill_Sneak_Set(CCritter* acl)
{
	if(acl->info.pe[PE_HIDE_MODE]) return;

	acl->info.pe[PE_HIDE_MODE]=TRUE;
	Send_Param(acl,TYPE_PERK,PE_HIDE_MODE);
	SetVisCr(acl);
}

void CServer::Skill_Sneak_UnSet(CCritter* acl)
{
	if(!acl->info.pe[PE_HIDE_MODE]) return;

	acl->info.pe[PE_HIDE_MODE]=FALSE;
	Send_Param(acl,TYPE_PERK,PE_HIDE_MODE);
	SetVisCr(acl);
}

int CServer::Act_Attack(CCritter* acl, uint8_t rate_object, CrID target_id)
{
//LogExecStr("Выполняется действие №1: АТАКА...");
	//находим атакуемого
	if(acl->info.id==target_id) return 0;
	cl_map::iterator it=map_cr[acl->info.map].find(target_id);
	if(it==map_cr[acl->info.map].end()) return 0;
	CCritter* t_acl=(*it).second;
	//проверяем не мертв ли атакуемый
	if(t_acl->info.cond==COND_DEAD) return 0;
	//находим дистанцию
	uint16_t dist=(uint16_t) sqrt(pow(acl->info.x-t_acl->info.x,2.0)+pow(acl->info.y-t_acl->info.y,2.0));
//LogExecStr("дистанция до цели =%d...",dist);
	//проверяем дистанцию
	int wpn_max_dist=0;
	int wpn_eff_dist=0;
	int dmg_max=0;
	int dmg_min=0;
	switch(rate_object)
	{
	case 1:
		wpn_max_dist=acl->info.a_obj->object->p[OBJ_WEAP_PA_MAX_DIST];
		wpn_eff_dist=acl->info.a_obj->object->p[OBJ_WEAP_PA_EFF_DIST];
		dmg_max=acl->info.a_obj->object->p[OBJ_WEAP_PA_DMG_MAX];
		dmg_min=acl->info.a_obj->object->p[OBJ_WEAP_PA_DMG_MIN];
		break;
	case 2:
		wpn_max_dist=acl->info.a_obj->object->p[OBJ_WEAP_SA_MAX_DIST];
		wpn_eff_dist=acl->info.a_obj->object->p[OBJ_WEAP_SA_EFF_DIST];
		dmg_max=acl->info.a_obj->object->p[OBJ_WEAP_SA_DMG_MAX];
		dmg_min=acl->info.a_obj->object->p[OBJ_WEAP_SA_DMG_MIN];
		break;
	case 3:
		wpn_max_dist=acl->info.a_obj->object->p[OBJ_WEAP_TA_MAX_DIST];
		wpn_eff_dist=acl->info.a_obj->object->p[OBJ_WEAP_TA_EFF_DIST];
		dmg_max=acl->info.a_obj->object->p[OBJ_WEAP_TA_DMG_MAX];
		dmg_min=acl->info.a_obj->object->p[OBJ_WEAP_TA_DMG_MIN];
		break;
	default: return 0;
	}

	if(dist>wpn_max_dist) return 0;

	bool eff_attack=true;
	if(dist>wpn_eff_dist) eff_attack=false;

	//рассчитываем вероятность попадания
	//Skill+ammo_mod-AC_target-range
//	uint16_t ammo_mod=0;
//	uint16_t ammo_DR=0;
//	if(acl->info.a_obj->object->p[25])
//	{
//		ammo_mod=1;
//		ammo_DR=1;
//	}
//	uint16_t Skill=acl->info.s[acl->info.a_obj->object->p[act+1]];
//	uint16_t AC_target=t_acl->info.s[9];
//	uint16_t min_dmg=acl->info.a_obj->object->p[act+17];
//	uint16_t max_dmg=acl->info.a_obj->object->p[act+16];
//	uint16_t luck=acl->info.s[6];
//	uint16_t DR=acl->info.s[acl->info.a_obj->object->p[act+15]+24];

//	int hit=Skill+ammo_mod-AC_target-dist;
//	if(hit<10) hit=(100-hit-luck)/10;

//	int dmg=max_dmg*((100-DR-ammo_DR)/100);
//	if(dmg<min_dmg) dmg=min_dmg;

//LogExecStr("лифе олд =%d...",t_acl->info.st[ST_CURRENT_HP]);
	int t_cur_hp=t_acl->info.st[ST_CURRENT_HP];
	int t_max_hp=t_acl->info.st[ST_MAX_LIFE];

	int dmg=(dmg_max+dmg_min)/2;

	t_cur_hp-=dmg;
	if(t_cur_hp<0) t_cur_hp=0;

	//t_acl->info.st[ST_CURRENT_HP]=t_cur_hp;
	CHANGE_STAT(t_acl,ST_CURRENT_HP,=,t_cur_hp);
//LogExecStr("лифе нев =%d...",t_acl->info.st[ST_CURRENT_HP]);

	//обновляем ориентацию атакуемого
//	uint8_t new_ori=acl->info.ori+3;
//	if(new_ori>5) new_ori-=6;

	bool attack_miss=false;

	bool attack_front=false;
	if(int(acl->info.ori-t_acl->info.ori)>2 || int(acl->info.ori-t_acl->info.ori)<-1) attack_front=true;
//	t_acl->info.ori=new_ori;

	//рассылаем всем итоги стычки
	MSGTYPE msg;
	//отсылает наподавший инфу о атаке
	SendA_Action(acl,ACT_USE_OBJ,rate_object);
	if(!t_acl->info.st[ST_CURRENT_HP]) //отыгрываем смерть
	{
//LogExecStr("смерть\n",dmg);
		//устанавливаем флаг смерти игроку
		t_acl->info.cond=COND_DEAD;
		if(attack_front==true)
			t_acl->info.cond_ext=COND_DEAD_NORMAL_UP;
		else
			t_acl->info.cond_ext=COND_DEAD_NORMAL_DOWN;

		//отылаем ему результаты
//		Send_Stat(t_acl,50,t_acl->info.s[50]); //смерть
//		Send_Stat(t_acl,ST_CURRENT_HP,t_acl->info.st[ST_CURRENT_HP]); //жизнь
		msg=NETMSG_CRITTER_ACTION;
		t_acl->bout << msg;
		t_acl->bout << t_acl->info.id;
		t_acl->bout << ACT_DEAD;
		t_acl->bout << t_acl->info.a_obj->object->id;
		t_acl->bout << t_acl->info.a_obj_arm->object->id;
		t_acl->bout << t_acl->info.cond_ext;
		t_acl->bout << t_acl->info.ori;
		//отсылаем результаты смерти всем кого он видит
		SendA_Action(t_acl,ACT_DEAD,t_acl->info.cond_ext);
	}
	else //отыгрываем повреждение
	{
		uint8_t defeat_type=0;

		if(attack_front==true)
			defeat_type=ACT_DEFEAT_FRONT;
		else
			defeat_type=ACT_DEFEAT_REAR;

		if(attack_miss==true) defeat_type=ACT_DEFEAT_MISS;

		//отсылаем поврежденному инфу
		//Send_Stat(t_acl,ST_CURRENT_HP,t_acl->info.s[ST_CURRENT_HP]); //жизнь

		msg=NETMSG_CRITTER_ACTION;
		t_acl->bout << msg;
		t_acl->bout << t_acl->info.id;
		t_acl->bout << ACT_DEFEAT;
		t_acl->bout << t_acl->info.a_obj->object->id;
		t_acl->bout << t_acl->info.a_obj_arm->object->id;
		t_acl->bout << defeat_type;
		t_acl->bout << t_acl->info.ori;
		//отсылает атакуемумый инфу о приниятии повреждения
		SendA_Action(t_acl,ACT_DEFEAT,defeat_type);
	}

	return 1;
}

void CServer::Process_CreateClient(CCritter* acl)
{
	LogExecStr("РЕГИСТРАЦИЯ ИГРОКА\n");
	TICK tickStart=GetTickCount();

	int bi;

//ПРИЕМ ДАННЫХ
	//логин
	acl->bin.Read(acl->info.login,MAX_LOGIN);
	acl->info.login[MAX_LOGIN]=0;
	//пасс
	acl->bin.Read(acl->info.pass,MAX_LOGIN);
	acl->info.pass[MAX_LOGIN]=0;
	//имя
	acl->bin.Read(acl->info.name,MAX_NAME);

	acl->info.name[MAX_NAME]=0;

//	my_strlwr(acl->info.name);
	sql.Check(acl->info.name);

	//cases
	for(bi=0; bi<5; bi++)
	{
		acl->bin.Read(acl->info.cases[bi],MAX_NAME);

		acl->info.cases[bi][MAX_NAME]=0;
//		my_strlwr(acl->info.cases[bi]);
		sql.Check(acl->info.cases[bi]);
	}
	//Обнуляем все статы, скиллы, перки
	for(bi=0; bi<ALL_STATS;  bi++) acl->info.st[bi]=0;
	for(bi=0; bi<ALL_SKILLS; bi++) acl->info.sk[bi]=0;
	for(bi=0; bi<ALL_PERKS;  bi++) acl->info.pe[bi]=0;
	//SPECIAL
	acl->bin >> acl->info.st[ST_STRENGHT	];
	acl->bin >> acl->info.st[ST_PERCEPTION	];
	acl->bin >> acl->info.st[ST_ENDURANCE	];
	acl->bin >> acl->info.st[ST_CHARISMA	];
	acl->bin >> acl->info.st[ST_INTELLECT	];
	acl->bin >> acl->info.st[ST_AGILITY		];
	acl->bin >> acl->info.st[ST_LUCK		];
	//возраст
	acl->bin >> acl->info.st[ST_AGE];
	//пол
	acl->bin >> acl->info.st[ST_GENDER];

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data forProcess_CreateCCritter from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,8);
		return;
	}

//ПРОВЕРКА ДАННЫХ
	if(sql.Check(acl->info.login) || sql.Check(acl->info.pass))
	{
		LogExecStr("Запрещенные символы при регистрации игрока: LOGIN или PASSWORD\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,1);
		return;
	}
	//проверка на длинну логина
	if(strlen(acl->info.login)<MIN_LOGIN || strlen(acl->info.login)>MAX_LOGIN)
	{
		LogExecStr("Неправильные данные при регистрации игрока: LOGIN\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,1);
		return;
	}

//	LogExecStr("logn:|%s|\n",acl->info.login);
//	LogExecStr("pass:|%s|\n",acl->info.pass);

//проверка на наличие созданного по такому логину игрока
	if(sql.CheckUser(acl->info.login))
	{
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,3);
		return;
	}
//проверка на длинну пасса
	if(strlen(acl->info.pass)<MIN_LOGIN || strlen(acl->info.pass)>MAX_LOGIN)
	{
		LogExecStr("Неправильные данные при регистрации игрока: PASS\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,2);
		return;
	}
//проверка на длинну имени
	if(strlen(acl->info.name)<MIN_NAME || strlen(acl->info.name)>MAX_NAME)
	{
		LogExecStr("Неправильные данные при регистрации игрока: NAME\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,13);
		return;
	}
//проверка на длинну cases
	for(bi=0; bi<5; bi++)
		if(strlen(acl->info.cases[bi])<MIN_NAME || strlen(acl->info.cases[bi])>MAX_NAME)
		{
			LogExecStr("Неправильные данные при регистрации игрока: CASES%d\n",bi);
			acl->state=STATE_DISCONNECT;
			Send_LoginMsg(acl,14);
			return;
		}
//проверка пола
	if(acl->info.st[ST_GENDER]<0 || acl->info.st[ST_GENDER]>1) 
	{ 
		LogExecStr("Неправильные данные при регистрации игрока: ПОЛ\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,15);
		return;
	}
//проверка возраста
	if(acl->info.st[ST_AGE]<14 || acl->info.st[ST_AGE]>80) 
	{ 
		LogExecStr("Неправильные данные при регистрации игрока: ВОЗРАСТ\n");
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,16);
		return;
	}
//проверка SPECIAL
	if( (acl->info.st[ST_STRENGHT	]<1)||(acl->info.st[ST_STRENGHT		]>10)||
		(acl->info.st[ST_PERCEPTION	]<1)||(acl->info.st[ST_PERCEPTION	]>10)||
		(acl->info.st[ST_ENDURANCE	]<1)||(acl->info.st[ST_ENDURANCE	]>10)||
		(acl->info.st[ST_CHARISMA	]<1)||(acl->info.st[ST_CHARISMA		]>10)||
		(acl->info.st[ST_INTELLECT	]<1)||(acl->info.st[ST_INTELLECT	]>10)||
		(acl->info.st[ST_AGILITY	]<1)||(acl->info.st[ST_AGILITY		]>10)||
		(acl->info.st[ST_LUCK		]<1)||(acl->info.st[ST_LUCK			]>10))
		{
			LogExecStr("Неправильные данные при регистрации игрока: SPECIAL №%d\n", bi);
			acl->state=STATE_DISCONNECT;
			Send_LoginMsg(acl,5);
			return;
		}
	if((acl->info.st[ST_STRENGHT]+acl->info.st[ST_PERCEPTION]+acl->info.st[ST_ENDURANCE]+
		acl->info.st[ST_CHARISMA]+acl->info.st[ST_INTELLECT]+
		acl->info.st[ST_AGILITY]+acl->info.st[ST_LUCK])!=40)
		{
			LogExecStr("Неправильные данные при регистрации игрока: SPECIAL сумма\n");
			acl->state=STATE_DISCONNECT;
			Send_LoginMsg(acl,5);
			return;
		}

//СОЗДАНИЕ ПЕРСОНАЖА
	//инфа по игроку
	//stats,skills,start_perks,perks
	acl->info.sk[SK_SMALL_GUNS		]=5	+4*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_BIG_GUNS		]=0	+2*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_ENERGY_WEAPONS	]=0	+2*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_UNARMED			]=30+2*(acl->info.st[ST_AGILITY]+acl->info.st[ST_AGILITY]);
	acl->info.sk[SK_MELEE_WEAPONS	]=20+2*(acl->info.st[ST_AGILITY]+acl->info.st[ST_AGILITY]);
	acl->info.sk[SK_THROWING		]=0	+4*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_FIRST_AID		]=0	+2*(acl->info.st[ST_PERCEPTION]+acl->info.st[ST_INTELLECT]);
	acl->info.sk[SK_DOCTOR			]=5	+acl->info.st[ST_PERCEPTION]+acl->info.st[ST_INTELLECT];
	acl->info.sk[SK_SNEAK			]=5	+3*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_LOCKPICK		]=10+acl->info.st[ST_PERCEPTION]+acl->info.st[ST_AGILITY];
	acl->info.sk[SK_STEAL			]=0	+3*acl->info.st[ST_AGILITY];
	acl->info.sk[SK_TRAPS			]=10+acl->info.st[ST_PERCEPTION]+acl->info.st[ST_AGILITY];
	acl->info.sk[SK_SCIENCE			]=0	+4*acl->info.st[ST_INTELLECT];
	acl->info.sk[SK_REPAIR			]=0	+3*acl->info.st[ST_INTELLECT];
	acl->info.sk[SK_SPEECH			]=0	+5*acl->info.st[ST_CHARISMA];
	acl->info.sk[SK_BARTER			]=0	+4*acl->info.st[ST_CHARISMA];
	acl->info.sk[SK_GAMBLING		]=0	+5*acl->info.st[ST_LUCK];
	acl->info.sk[SK_OUTDOORSMAN		]=0	+2*(acl->info.st[ST_ENDURANCE]+acl->info.st[ST_INTELLECT]);

	acl->InitClient();

	acl->info.st[ST_CURRENT_HP]=acl->info.st[ST_MAX_LIFE];
	acl->info.st[ST_POISONING_LEVEL]=0;
	acl->info.st[ST_RADIATION_LEVEL]=0;
	acl->info.st[ST_CURRENT_STANDART]=acl->info.st[ST_MAX_COND];

	//map,x,y,base_type,ori
	acl->info.map=11;
	acl->info.x=64;
	acl->info.y=107;
	acl->info.ori=4;
	acl->info.base_type=10;

	if(!sql.NewPlayer(&acl->info))
	{
		LogExecStr("!!!WORNING!!!: траблы с mySQL - неудалось сохранить игрока\n");
		sql.DeleteDataPlayer(&acl->info);
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,8);
		return;
	}

	//вывод инфы в лог
	//sql.PrintTableInLog("users");
	//посылка подтверждения
	Send_LoginMsg(acl,6);
	//отключение клиента
	acl->state=STATE_DISCONNECT;

	LogExecStr("РЕГИСТРАЦИЯ ИГРОКА ПРОШЛА УСПЕШНО ЗА %d МСЕК\n", GetTickCount()-tickStart);
}

void CServer::Process_GetLogIn(CCritter* acl)
{
	LogExecStr("Клиент логиниться...");

	acl->bin.Read(acl->info.login,MAX_LOGIN);
	acl->bin.Read(acl->info.pass,MAX_LOGIN);
	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_GetLogIn from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,8);
		return;
	}

	if(acl->state!=STATE_CONN)
	{
		LogExecStr("CCritter not STATE_CONN for Process_GetLogIn from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,8);
		return;
	}

	//проверка на длинну логина и пасса
	if(strlen(acl->info.login)<MIN_LOGIN || strlen(acl->info.login)>MAX_LOGIN)
	{
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,1);
		return;
	}
	if(strlen(acl->info.pass)<MIN_LOGIN || strlen(acl->info.pass)>MAX_LOGIN)
	{
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,2);
		return;
	}
	//проверка запрещенных символов
	if(sql.Check(acl->info.login) || sql.Check(acl->info.pass))
	{
		LogExecStr("Запрещенные символы при логине игрока: LOGIN или PASSWORD\n");
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!(acl->info.id=sql.CheckUserPass(acl->info.login,acl->info.pass)))
	{
		LogExecStr("Неправильный логин|%s| или пароль|%s|\n",acl->info.login,acl->info.pass);
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,9);
		return;
	}

	int map=0;
	if((map=sql.GetInt("users","map","id",acl->info.id))==0xFFFFFFFF)
	{
		acl->state=STATE_DISCONNECT;
		Send_LoginMsg(acl,8);
		return;
	}
	acl->info.map=map;

	MSGTYPE msg=NETMSG_LOGINOK;
	acl->bout << msg;

//	acl->state=STATE_LOGINOK;

	Send_LoadMap(acl);

	LogExecStr("OK\n");
}

void CServer::Process_MapLoaded(CCritter* acl)
{
	LogExecStr("Карта загружена. Отправка данных игроку...");

	BREAK_BEGIN
		cl_map::iterator it_cr=cr.find(acl->info.id);
		if(it_cr!=cr.end())
		{
			CCritter* crit=(*it_cr).second;

			if(crit==acl) break;
		//	{
			//	UNSETFLAG(acl->info.flags,FCRIT_DISCONNECT);
			//	acl->info.flags=FCRIT_PLAYER;
		//		break;
		//	}
	//		EraseCrFromMap(crit,crit->info.map,crit->info.x,crit->info.y);

			crit->CopyParamsTo(acl);

		//	delete (*it_cr).second;

			cr.erase(it_cr);
		} 
		else if(!sql.LoadDataPlayer(&acl->info))
		{
			acl->state=STATE_DISCONNECT;
			Send_LoginMsg(acl,8);
			return;
		}
		else
		{
			acl->info.cond=COND_LIFE;
			acl->info.cond_ext=COND_LIFE_NONE;
		}

		if(AddCrToMap(acl,acl->info.map,acl->info.x,acl->info.y)!=TR_OK) //чтоб друг другу на головы не высаживались
		{
			Send_LoginMsg(acl,11);
			acl->state=STATE_DISCONNECT;
			return;
		}

		cr.insert(cl_map::value_type(acl->info.id,acl));

		SETFLAG(acl->info.flags,FCRIT_PLAYER);
		UNSETFLAG(acl->info.flags,FCRIT_DISCONNECT);

		UseDefObj(acl,DOBJ_SLOT_HAND1);
		UseDefObj(acl,DOBJ_SLOT_ARMOR);

		for(dyn_map::iterator it2=acl->info.obj.begin();it2!=acl->info.obj.end();it2++)
			switch((*it2).second->ACC_CRITTER.slot)
			{
			case DOBJ_SLOT_INV: continue;
			case DOBJ_SLOT_HAND1:
				if((*it2).second->type!=OBJ_TYPE_ARMOR)
					acl->info.a_obj=(*it2).second;
				continue;
			case DOBJ_SLOT_HAND2: continue;
			case DOBJ_SLOT_TWOHAND: continue;
			case DOBJ_SLOT_ARMOR:
				if((*it2).second->type==OBJ_TYPE_ARMOR)
					acl->info.a_obj_arm=(*it2).second;
				continue;
			default: continue;
			}
					

	BREAK_END;

	if(!acl->info.map)
	{
		Send_GlobalInfo(acl,GM_INFO_ALL);

		acl->state=STATE_GAME;
		LogExecStr("Global OK\n");
		return;
	}

//	EraseCrFromMap(acl,acl->info.map);

//	if(AddCrToMap(acl,acl->info.map,acl->info.x,acl->info.y)!=TR_OK)
//	{
//		Send_LoginMsg(acl,11);
//		acl->state=STATE_DISCONNECT;
//		return;
//	}

	Send_GameTime(acl);

	SETFLAG(acl->info.flags,FCRIT_CHOSEN);
	Send_AddCritter(acl,&acl->info);
	UNSETFLAG(acl->info.flags,FCRIT_CHOSEN);

	Send_AllParams(acl,TYPE_STAT ); //отправка всех статов игроку
	Send_AllParams(acl,TYPE_SKILL); //отправка всех скиллов игроку
	Send_AllParams(acl,TYPE_PERK ); //отправка всех перков игроку

	if(!acl->info.obj.size())
	{
		CreateObjToPl(acl->info.idchannel,1301);
		CreateObjToPl(acl->info.idchannel,2016);
	}

	//отправка объектов игроку
	for(dyn_map::iterator it_o=acl->info.obj.begin();it_o!=acl->info.obj.end();it_o++)
	{
//		GenWear((*it_o).second,0);
//		if(!(*it_o).second->tick) трабла с итератором. исправить!!!!!!!!!!!!!!!!
//		{
//			DeleteObj((*it_o).second->id);
//			continue;
//		}
		Send_AddObject(acl,(*it_o).second);
	}

	acl->info.obj_id.clear();

	SetVisCr(acl);
	SetVisibleObj(acl);

	acl->state=STATE_GAME;

	LogExecStr("Local OK\n");
}

void CServer::UseDefObj(CCritter* acl, uint8_t slot)
{
	switch(slot)
	{
	case DOBJ_SLOT_INV:
		break;
	case DOBJ_SLOT_HAND1:
		acl->info.a_obj=&acl->info.def_obj1;
		acl->info.a_obj->object=all_s_obj[acl->info.base_type];
		acl->info.a_obj->accessory=DOBJ_ACCESSORY_CRIT;
		acl->info.a_obj->ACC_CRITTER.id=0;
		acl->info.a_obj->ACC_CRITTER.slot=DOBJ_SLOT_HAND1;
		acl->info.a_obj->broken_info=0;
		acl->info.a_obj->id=0;
		acl->info.a_obj->last_tick=0;
		acl->info.a_obj->time_wear=0;
		acl->info.a_obj->type=acl->info.a_obj->object->type;
		break;
	case DOBJ_SLOT_HAND2:
		break;
	case DOBJ_SLOT_TWOHAND:
		break;
	case DOBJ_SLOT_ARMOR:
		acl->info.a_obj_arm=&acl->info.def_obj2;
		acl->info.a_obj_arm->object=all_s_obj[acl->info.base_type+200];
		acl->info.a_obj->ACC_CRITTER.id=0;
		acl->info.a_obj_arm->accessory=DOBJ_ACCESSORY_CRIT;
		acl->info.a_obj->ACC_CRITTER.slot=DOBJ_SLOT_HAND1;
		acl->info.a_obj_arm->broken_info=0;
		acl->info.a_obj_arm->id=0;
		acl->info.a_obj_arm->last_tick=0;
		acl->info.a_obj_arm->time_wear=0;
		acl->info.a_obj_arm->type=acl->info.a_obj_arm->object->type;
		break;
	}
}

void CServer::Process_Move(CCritter* acl)
{
	uint16_t move_params;

	acl->bin >> move_params;

	if(!acl->info.map) return;

	uint8_t dir=FLAG(move_params,BIN8(00000111));

//	LogExecStr("Process_Move move_params=%d, dir=%d, how_move=%d\n",move_params,dir,how_move);

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_Move from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(dir>PMOVE_5)
	{
		SetCheat(acl,"Process_Move - неправильное направление движения");
		Send_XY(acl);
		return;
	}

	uint16_t old_map=acl->info.map;

	uint8_t move_res=0;

	switch(dir)
	{
	case PMOVE_0:
		if(!(acl->info.x%2))
			move_res=MoveToTile(acl,acl->info.x-1,acl->info.y);
		else
			move_res=MoveToTile(acl,acl->info.x-1,acl->info.y-1);
		break;
	case PMOVE_1:
		if(acl->info.x%2)
			move_res=MoveToTile(acl,acl->info.x-1,acl->info.y);
		else
			move_res=MoveToTile(acl,acl->info.x-1,acl->info.y+1);
		break;
	case PMOVE_2:
		move_res=MoveToTile(acl,acl->info.x,acl->info.y+1);
		break;
	case PMOVE_3:
		if(acl->info.x%2)
			move_res=MoveToTile(acl,acl->info.x+1,acl->info.y);
		else
			move_res=MoveToTile(acl,acl->info.x+1,acl->info.y+1);
		break;
	case PMOVE_4:
		if(!(acl->info.x%2))
			move_res=MoveToTile(acl,acl->info.x+1,acl->info.y);
		else
			move_res=MoveToTile(acl,acl->info.x+1,acl->info.y-1);
		break;
	case PMOVE_5:
		move_res=MoveToTile(acl,acl->info.x,acl->info.y-1);
		break;
	}

	switch(move_res)
	{
	case MR_STEP:
		if(FLAG(PMOVE_RUN,move_params)) Skill_Sneak_UnSet(acl); //!!!!!! двойной вызов SetVisCr

		SetVisCr(acl); //!!!!!!!
		SetVisibleObj(acl);
		SendA_Move(acl,move_params);
		break;
	case MR_TRANSIT:
		//acl->Send_LoadMap();
	//	acl->state=STATE_LOGINOK;
		break;
	case MR_TOGLOBAL:
		//GM_GroupStartMove(acl);
	//	acl->state=STATE_LOGINOK;
		break;
	default:
	case MR_FALSE:
		SetCheat(acl,"Process_Move - попытка походить в занятую клетку");
		Send_XY(acl);
		break;
	}
}

void CServer::Process_ChangeObject(CCritter* acl)
{
	uint32_t idobj;
	uint8_t num_slot;
	//num_slot
	//1 - работаем с предметом в руке
	//2 - с слотом армор
	//3 - кидаем на землю
	
	acl->bin >> idobj;
	acl->bin >> num_slot;

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_ChangeObject from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!acl->info.map) return;

//LogExecStr("Process_ChangeObject id=%d, num_slot=%d\n", idobj, num_slot);

	Skill_Sneak_UnSet(acl);

//деактивируем предмет в активном слоте
	if(!idobj && num_slot==1)
	{
		SendA_Action(acl,ACT_HIDE_OBJ,0);
		acl->info.a_obj->ACC_CRITTER.slot=DOBJ_SLOT_INV;
		UseDefObj(acl,DOBJ_SLOT_HAND1);
		return;
	}
//активируем предмет в активном слоте
	if(idobj && num_slot==1)
	{
		dyn_map::iterator it=acl->info.obj.find(idobj);
		if(it!=acl->info.obj.end())
			if((*it).second->type!=OBJ_TYPE_ARMOR && (*it).second->ACC_CRITTER.slot==DOBJ_SLOT_INV)
			{
				(*it).second->ACC_CRITTER.slot=DOBJ_SLOT_HAND1;
				acl->info.a_obj=(*it).second;
				SendA_Action(acl,ACT_SHOW_OBJ,0);
				return;
			}
	}
//деактивируем предмет в слоте брони
	if(!idobj && num_slot==2)
	{
		acl->info.a_obj_arm->ACC_CRITTER.slot=DOBJ_SLOT_INV;
		UseDefObj(acl,DOBJ_SLOT_ARMOR);
		SendA_Action(acl,ACT_CHANGE_ARM,0);
		return;
	}
//активируем предмет в слоте брони
	if(idobj && num_slot==2)
	{
		dyn_map::iterator it=acl->info.obj.find(idobj);
		if(it!=acl->info.obj.end())
			if((*it).second->type==OBJ_TYPE_ARMOR && (*it).second->ACC_CRITTER.slot==DOBJ_SLOT_INV)
			{
				(*it).second->ACC_CRITTER.slot=DOBJ_SLOT_ARMOR;
				acl->info.a_obj_arm=(*it).second;
				SendA_Action(acl,ACT_CHANGE_ARM,0);
				return;
			}
	}
//кидаем объект на землю
	if(idobj && num_slot==3)
	{
		dyn_map::iterator it=acl->info.obj.find(idobj);
		if(it!=acl->info.obj.end())
		{
			TransferDynObjPlTl(acl,(*it).second);
			return;
		}
	}
	//здесь надо обновлять игрока при неправильных мессагах
	//обнуление
	acl->info.a_obj=&acl->info.def_obj1;
	acl->info.a_obj_arm=&acl->info.def_obj2;
	MSGTYPE msg=NETMSG_CRITTER_ACTION;
	acl->bout << msg;
	acl->bout << acl->info.id;
	acl->bout << ACT_NULL;
	acl->bout << acl->info.a_obj->object->id;
	acl->bout << acl->info.a_obj_arm->object->id;
	acl->bout << uint8_t(0);
	acl->bout << acl->info.ori;
}

void CServer::Process_UseObject(CCritter* acl)
{
//LogExecStr("Process_UseObject...");

	uint8_t c_type_target;
	uint32_t t_id;
	uint8_t c_ori;
	uint8_t c_action;
	uint8_t c_rate_object;

	acl->bin >> c_type_target;
	acl->bin >> t_id;
	acl->bin >> c_ori;
	acl->bin >> c_action;
	acl->bin >> c_rate_object;

//LogExecStr("Цель ID=%d, Тайл Х=%d, Тайл У=%d, Ориентация=%d, Режим использования=%d...",t_id,t_x,t_y,c_ori,t_action);

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_UseObject from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!acl->info.map) return;

	if(c_ori>5) return;

	Skill_Sneak_UnSet(acl);

	acl->info.ori=c_ori;

	if(acl->info.cond!=COND_LIFE)
	{
		LogExecStr("!WORNING! - читерство - попытка применить объект мертвым игроком. ид=|%d|\n",acl->info.id);
		return;
	}
//Оружие
	if(c_type_target==USE_OBJ_ON_CRITTER)
	{
		switch(acl->info.a_obj->type)
		{
		case OBJ_TYPE_WEAPON:
		//использование оружия
			if(c_action==ACT_USE_OBJ)
				if(acl->info.a_obj->object->p[OBJ_WEAP_TIME_ACTIV])
				{
					if(acl->info.cond_ext==COND_LIFE_ACTWEAP)
					{
						Act_Attack(acl,c_rate_object,t_id);
						return;
					}
					else c_action=ACT_ACTIVATE_OBJ;
				}
				else
				{
					Act_Attack(acl,c_rate_object,t_id);
					return;
				}
		//активация оружия
			if(c_action==ACT_ACTIVATE_OBJ && acl->info.a_obj->object->p[OBJ_WEAP_TIME_ACTIV])
			{
				acl->info.cond_ext=COND_LIFE_ACTWEAP;
				SendA_Action(acl,c_action,0);
				return;
			}
		//деактивация оружия
			if(c_action==ACT_DACTIVATE_OBJ)
			{
				acl->info.cond_ext=COND_LIFE_NONE;
				SendA_Action(acl,c_action,0);
				return;
			}

		//не выходит ли изза границ режим использования
		//	if(c_action>acl->info.a_obj->object->p[129])
		//		{ LogExecStr("ОШИБКА - режим использования выходит изза границ\n"); return; }

			break;
		case OBJ_TYPE_DRUG:
			break;
		case OBJ_TYPE_AMMO:
			break;
		case OBJ_TYPE_MISC:
			break;
		case OBJ_TYPE_KEY:
			break;
		case OBJ_TYPE_CONTAINER:
			break;
		}
	}
	else if(c_type_target==USE_OBJ_ON_OBJ)
	{

	}
	else
		LogExecStr("!!!!!!!");
}

void CServer::Process_PickObject(CCritter* acl)
{
	HexTYPE targ_x;
	HexTYPE targ_y;
	uint16_t id_sobj;

	acl->bin >> targ_x;
	acl->bin >> targ_y;
	acl->bin >> id_sobj;

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_PickObject from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!acl->info.map) return;

	if(targ_x>=MAXHEXX || targ_y>=MAXHEXY) return;

	int cur_dir;
	char sx[6]={-1,-1,0,1,1, 0};
	char sy[6]={ 0, 1,1,1,0,-1};

	if(acl->info.x%2)
	{
		sy[0]--;
		sy[1]--;
		sy[3]--;
		sy[4]--;
	}

	for(cur_dir=0;cur_dir<6;cur_dir++)
		if(acl->info.x+sx[cur_dir]==targ_x && acl->info.y+sy[cur_dir]==targ_y) break;

	if(cur_dir>5)
	{
		SetCheat(acl,"Попытка пикнуть предмет с дистанции не равной 1");
		return;
	}

	dyn_obj* pick_obj=GetDynObj(id_sobj,acl->info.map,targ_x,targ_y);

	if(!pick_obj)
	{
		//???? ченить сообщить игроку
		return;
	}

	Skill_Sneak_UnSet(acl);

	switch(pick_obj->object->type)
	{
	case OBJ_TYPE_DOOR:
		if(FLAG(GetTileFlags(acl->info.map,targ_x,targ_y),FT_PLAYER)) break;

		if(FLAG(GetTileFlags(acl->info.map,targ_x,targ_y),FT_DOOR_CLOSE))
		{
			UnSetTileFlag(acl->info.map,targ_x,targ_y,FT_DOOR_CLOSE);
			SetTileFlag(acl->info.map,targ_x,targ_y,FT_DOOR_OPEN);
		}
		else if(FLAG(GetTileFlags(acl->info.map,targ_x,targ_y),FT_DOOR_OPEN))
		{
			UnSetTileFlag(acl->info.map,targ_x,targ_y,FT_DOOR_OPEN);
			SetTileFlag(acl->info.map,targ_x,targ_y,FT_DOOR_CLOSE);
		}
		else
			return;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		SendA_Action(acl,ACT_PICK_OBJ_UP,0);
		SendA_ChangeObjOnMap(acl,pick_obj);
		Send_ChangeObjOnMap(acl,pick_obj);
		break;
	case OBJ_TYPE_CONTAINER:
		break;
	default:
		TransferDynObjTlPl(acl,pick_obj);
		break;
	}
}

void CServer::Process_UseSkill(CCritter* acl)
{
	uint8_t skill;
	uint32_t targ_id;
	uint8_t ori;

	acl->bin >> skill;
	acl->bin >> targ_id;
	acl->bin >> ori;

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data for Process_UseSkill from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!acl->info.map) return;

	if(ori>5) return;

	acl->info.ori=ori;

	switch(skill)
	{
	case SK_FIRST_AID: //{Помощь}
		break;
	case SK_DOCTOR: //{Врач}
		break;
	case SK_SNEAK: //{Крадучесть}
		if(acl->info.cond==COND_LIFE && acl->info.cond_ext==COND_LIFE_NONE)
			Skill_Sneak_Change(acl);
		else
			Skill_Sneak_UnSet(acl);
		break;
	case SK_LOCKPICK: //{Взломщик}
		break;
	case SK_STEAL: //{Красть}
		break;
	case SK_TRAPS: //{Ловушки}
		break;
	case SK_SCIENCE: //{Наука}
		break;
	case SK_REPAIR: //{Ремонт}
		break;
	default:
		SetCheat(acl,"Process_UseSkill - неверный номер скилла на использование");
		break;
	}
}

void CServer::Process_Dir(CCritter* acl) //!Cvet переделал
{
	uint8_t new_dir;
	acl->bin >> new_dir;

	if(acl->bin.IsError())
	{
		LogExecStr("Wrong MSG data forProcess_Dir from SockID %d!\n",acl->s);
		acl->state=STATE_DISCONNECT;
		return;
	}

	if(!acl->info.map) return;

	if(new_dir>5) return;
	if(acl->info.ori==new_dir) return;

	acl->info.ori=new_dir;

	if(!acl->vis_cl.empty())
	{
		MSGTYPE msg=NETMSG_CRITTER_DIR;
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
			c->bout << new_dir;

//LogExecStr("Посылаю данные id=%d о действии id=%d\n", c->info.id, acl->info.id);
		}
	}
}

int CServer::Crit_FindTarget(CCritter* acl, uint8_t flags)
{
	if(!acl) return 0;
	return 1;
}

int CServer::Crit_FreeTarget(CCritter* acl)
{
	if(!acl) return 0;

	acl->target=NULL;

	return 1;
}

int CServer::Crit_IsTarget(CCritter* acl)
{
	if(!acl) return 0;

	if(acl->target) return 1;

	return 0;
}

int CServer::Crit_IsSelfTarget(CCritter* acl)
{
	if(!acl) return 0;

	int count=0;
	for(cl_map::iterator it_cr=acl->vis_cl.begin();it_cr!=acl->vis_cl.end();++it_cr)
		if((*it_cr).second->target==acl) ++count;

	return count;
}

int CServer::Crit_MoveToTarget(CCritter* acl)
{
	if(!acl) return 0;
	if(!acl->target) return 0;

	return 0;
}

int CServer::Crit_PossibleToAttack(CCritter* acl)
{
	if(!acl) return 0;
	if(!acl->target) return 0;

	return 0;
}

int CServer::Crit_AttackTarget(CCritter* acl)
{
	if(!acl) return 0;
	return 1;
}

int CServer::Crit_IsBusy(CCritter* acl)
{
	if(!acl) return 1;

	if(acl->IsBusy()) return 1;

	return 0;
}

int CServer::Crit_Wait(CCritter* acl, int ms)
{
	if(!acl) return 0;

	acl->BreakTime(ms);

	return 1;
}

int CServer::Crit_MoveRandom(CCritter* acl)
{
	if(!acl) return 0;

	static uint8_t dir_move=0;
	HexTYPE new_x=0,new_y=0;
	if(!self->LMapGetCoords(dir_move,acl->info.x,acl->info.y,new_x,new_y)) return 0;
	if(++dir_move>5) dir_move=0;

	if(self->MoveToTile(acl,new_x,new_y)!=TR_OK) return 0;

	uint16_t move_flags=BIN16(00000000,00111100);

	self->SetVisCr(acl);
	self->SendA_Move(acl,move_flags);

	acl->BreakTime(4000);

	return 1;
}

int CServer::Crit_GetStat(CCritter* acl, uint8_t num_stat)
{
	if(!acl) return -1;
	if(num_stat>=ALL_STATS) return -1;

	return acl->info.st[num_stat];
}

int CServer::Crit_GetSkill(CCritter* acl, uint8_t num_skill)
{
	if(!acl) return -1;
	if(num_skill>=ALL_SKILLS) return -1;

	return acl->info.sk[num_skill];
}

int CServer::Crit_GetPerk(CCritter* acl, uint8_t num_perk)
{
	if(!acl) return -1;
	if(num_perk>=ALL_PERKS) return -1;

	return acl->info.pe[num_perk];
}