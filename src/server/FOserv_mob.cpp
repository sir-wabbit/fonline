/********************************************************************
	author:	Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"
#include "FOserv.h"

int CServer::MOBs_LoadAllGroups()
{

	LogExecStr("Загрузка групп мобов\n");

	char ch1;
	int num_group=0;
	int count_group=0;
	char data_name[256];
	char mob_name[256];
	FILE* cf1;

	mobs_group* cur_group=NULL;
	CCritter* mob=NULL;

	sprintf(data_name,"%smobs_groups.txt",PATH_MOB);

	if(!(cf1=fopen(data_name, "rt")))
	{
		LogExecStr("Файл не найден |%s|\n",data_name);
		return 0;
	}

	int cur_mob_id=MOB_MIN_ID;

	while(!feof(cf1))
	{
		fscanf(cf1,"%c",&ch1);

		if(ch1!='@') continue;

		LogExecStr("\tНайдена группа, номер:");

		fscanf(cf1,"%d",&num_group);
		if(num_group<=0 || num_group>=MOBS_MAX_GROUPS)
		{
			LogExecStr("неправильный номер группы\n");
			return 0;
		}
		LogExecStr("%d...",num_group);

		cur_group=&mobs_gr[num_group];
		if(cur_group->num)
		{
			LogExecStr("дублирование группы\n");
			return 0;
		}
		cur_group->num=num_group;

		count_group=0;

		while(!feof(cf1))
		{
			fscanf(cf1,"%c",&ch1);

			if(ch1=='@') break;
			if(ch1!='#') continue;

			mob_name[0]=NULL;
			fscanf(cf1,"%s",&mob_name[0]);
			if(!mob_name[0])
			{
				LogExecStr("не удалось прочитать имя моба\n");
				return 0;
			}

			LogExecStr("%s...",mob_name);

/*
	FILE* cf;
	

	
	if(!(cf=fopen(mob_path, "rt")))
	{
		LogExecStr("Файл не найден |%s|\n",mob_path);
		return 0;
	}

	fclose(cf);
*/

			prep_mob_map::iterator it_fm=prep_mob.find(string(mob_name));
			if(it_fm==prep_mob.end())
			{
				LogExecStr("Загрузка прототипа...");

				CCritter* pmob=new CCritter;
				pmob->i_mob=new mob_info;

				char mob_path[256];
				sprintf(mob_path,"%s%s.mob",PATH_MOB,mob_name);

				bool Err_load=false;

				//основные параметры
				pmob->info.map=0;
				pmob->info.x=0;
				pmob->info.y=0;
				pmob->info.ori=0;

				if((pmob->info.base_type=GetPrivateProfileInt("info","base_type",-1,mob_path))==-1) Err_load=true;

				if((pmob->i_mob->base_cond=GetPrivateProfileInt("info","mob_info",-1,mob_path))==-1) Err_load=true;
				pmob->i_mob->cond=pmob->i_mob->base_cond;
				pmob->i_mob->name=string(mob_name);

				GetPrivateProfileString("info","name"	,"e",pmob->info.name		,MAX_NAME,mob_path);
				if(pmob->info.name[0]		=='e') Err_load=true;
				GetPrivateProfileString("info","cases0"	,"e",pmob->info.cases[0]	,MAX_NAME,mob_path);
				if(pmob->info.cases[0][0]	=='e') Err_load=true;
				GetPrivateProfileString("info","cases1"	,"e",pmob->info.cases[1]	,MAX_NAME,mob_path);
				if(pmob->info.cases[1][0]	=='e') Err_load=true;
				GetPrivateProfileString("info","cases2"	,"e",pmob->info.cases[2]	,MAX_NAME,mob_path);
				if(pmob->info.cases[2][0]	=='e') Err_load=true;
				GetPrivateProfileString("info","cases3"	,"e",pmob->info.cases[3]	,MAX_NAME,mob_path);
				if(pmob->info.cases[3][0]	=='e') Err_load=true;
				GetPrivateProfileString("info","cases4"	,"e",pmob->info.cases[4]	,MAX_NAME,mob_path);
				if(pmob->info.cases[4][0]	=='e') Err_load=true;

			//объекты
				UseDefObj(pmob,DOBJ_SLOT_HAND1);
				UseDefObj(pmob,DOBJ_SLOT_ARMOR);

			//статы, скиллы, перки
				int go=0;
				for(go=0; go<ALL_STATS ; go++) pmob->info.st[go]=5;
				for(go=0; go<ALL_SKILLS; go++) pmob->info.sk[go]=5;
				for(go=0; go<ALL_PERKS ; go++) pmob->info.pe[go]=0;
				pmob->info.st[ST_GENDER]=0;
				pmob->info.st[ST_AGE]=50;

				pmob->info.st[ST_CURRENT_HP]=50;
				pmob->info.st[ST_MAX_LIFE]=50;

				pmob->InitMOB();

			//состояния
				pmob->info.cond=COND_LIFE;
				pmob->info.cond_ext=COND_LIFE_NONE;
				pmob->info.flags=FCRIT_MOB;
				pmob->state=STATE_GAME;

				if(Err_load==true)
				{
					LogExecStr("Ошибка при загрузке основных параметров из файла\n");
					SAFEDEL(pmob);
					return 0;
				}

				prep_mob.insert(prep_mob_map::value_type(string(mob_name),pmob));

				LogExecStr("OK...");
			}

			it_fm=prep_mob.find(string(mob_name));
			if(it_fm==prep_mob.end())
			{
				LogExecStr("Ошибка - не найден прототип\n");
				return 0;
			}

			LogExecStr("прототип найден...");

			mob=new CCritter;
			mob->i_mob=new mob_info;

			mob->info.id=cur_mob_id;
			mob->i_mob->num_group=num_group;
			mob->state=STATE_GAME;

			MOB_Update((*it_fm).second,mob);

			cr[cur_mob_id]=mob;
			mb[cur_mob_id]=mob;

			cur_group->mobs[cur_mob_id]=mob;

			cur_mob_id++;
			count_group++;

			LogExecStr("OK...");
		}

		if(ch1!='@')
		{
			LogExecStr("оибка чтения\n");
			return 0;
		}

		if(!count_group || count_group>GM_MAX_GROUP)
		{
			LogExecStr("нулевая группа или превышает лимит\n");
			return 0;
		}

		LogExecStr("группа загружена\n");

		cur_group->mobs_count=count_group;
		cur_group->mobs_level=0;

		mobs_group_free.insert(cur_group->num);
	}

	fclose(cf1);

	LogExecStr("Загрузка групп мобов прошла успешно\n");

	return 1;
}

void CServer::MOB_Update(CCritter* prep_mob, CCritter* mob)
{
	if(!prep_mob || !mob) return;

	DWORD mob_id=mob->info.id;
	WORD mob_group=mob->i_mob->num_group;

	memcpy(mob->i_mob,prep_mob->i_mob,sizeof(mob_info));
	memcpy(&mob->info,&prep_mob->info,sizeof(crit_info));

	mob->info.id=mob_id;
	mob->i_mob->num_group=mob_group;

	UseDefObj(mob,DOBJ_SLOT_HAND1);
	UseDefObj(mob,DOBJ_SLOT_ARMOR);
}

void CServer::MOBs_Proccess()
{
	mobs_group* cur_mgroup=NULL;
	CCritter* mob=NULL;
	for(word_set::iterator it_mgr=mobs_group_busy.begin();it_mgr!=mobs_group_busy.end();++it_mgr)
	{
		cur_mgroup=&mobs_gr[(*it_mgr)];

		for(cl_map::iterator it_m=cur_mgroup->mobs.begin();it_m!=cur_mgroup->mobs.end();++it_m)
		{
			mob=(*it_m).second;

			if(mob->IsBusy()) continue;

			if(mob->info.cond==COND_LIFE)
			{

			}
			else if(mob->info.cond==COND_KNOCK_OUT)
			{

			}
			else if(mob->info.cond==COND_DEAD)
			{

			}
			else
			{
				LogExecStr("!!!!MOB COND ERROR |%d|\n",mob->info.cond);
				continue;
			}

	
		//действия моба
			//Crit_MoveRandom(mob);
			// XXX[31.7.2012 alex]: should probably do it with Lua
			/*asIScriptContext *ctx=NULL;

			if(!(ctx=ss->CreateContext()))
			{
				LogExecStr("e1\n");
				ctx->Release();
				continue;
			}

			asIScriptFunction* mainFunc = ss->GetGlobalFunctionByDecl("void main2(int mob)");
			
			if(!mainFunc)
			{
				LogExecStr("e2\n");
				ctx->Release();
				continue;
			}

			if(ctx->Prepare(mainFunc)<0)
			{
				LogExecStr("e3\n");
				ctx->Release();
				continue;
			}

			ctx->SetArgDWord(0,(DWORD)(mob));

			if(ctx->Execute()<0)
			{
				LogExecStr("e4\n");
				ctx->Release();
				continue;
			}

			ctx->Release();*/
		}
	}
}

int CServer::MOBs_AddToEncaunter(WORD num_encaunter, BYTE mobs_level)
{
LogExecStr("Высаживаем мобов==========================================\n");
	encaunter_info* cur_encaunter=&encaunter[num_encaunter];
	WORD num_map=cur_encaunter->emap->num;

	if(!encaunters_busy.count(num_encaunter)) return 0;
	if(cur_encaunter->count_groups>=cur_encaunter->max_groups) return 0;

	mobs_group* cur_mobs=NULL;
	word_set::iterator it_mgr;
	for(it_mgr=mobs_group_free.begin();it_mgr!=mobs_group_free.end();++it_mgr)
		if(mobs_gr[(*it_mgr)].mobs_level==mobs_level)
		{
			cur_mobs=&mobs_gr[(*it_mgr)];
			break;
		}

	if(it_mgr==mobs_group_free.end()) return 0;

	CCritter* mob=NULL;
	for(cl_map::iterator it_m=cur_mobs->mobs.begin();it_m!=cur_mobs->mobs.end();++it_m)
	{
		mob=(*it_m).second;

		if(AddCrToMap(mob,
			cur_encaunter->emap->num,
			cur_encaunter->start_hx[cur_encaunter->count_groups],
			cur_encaunter->start_hx[cur_encaunter->count_groups])!=TR_OK)
		{
			LogExecStr("Не удалось высадить моба!!!\n");
			continue;
		}

		mob->info.ori=cur_encaunter->start_ori[cur_encaunter->count_groups];
	}

	cur_encaunter->count_groups++;

	mobs_group_busy.insert(cur_mobs->num);
	mobs_group_free.erase(cur_mobs->num);

LogExecStr("x=%d,y=%d\n",cur_encaunter->start_hx[cur_encaunter->count_groups-1],cur_encaunter->start_hx[cur_encaunter->count_groups-1]);
LogExecStr("Высадили мобов++++++++++++++++++++++++++++++++++++++++++++\n");
	return 1;
}

void CServer::MOBs_EraseFromMap(WORD num_map)
{
	if(!num_map && num_map>=MAX_MAPS)
	{
		LogExecStr("Ошибка - Попытка удалить мобов на несуществующей карте |%d|\n",num_map);
		return;
	}

	CCritter* cur_mob=NULL;
	for(cl_map::iterator it_m=map_cr[num_map].begin();it_m!=map_cr[num_map].end();)
	{
		cur_mob=(*it_m).second;
		it_m++;

		if(!FLAG(cur_mob->info.flags,FCRIT_MOB)) continue;

		map_cr[num_map].erase(cur_mob->info.id);
		cur_mob->info.map=0;
		UNSETFLAG(tile[num_map][cur_mob->info.x][cur_mob->info.y].flags,FT_PLAYER);
	}

	MOBs_RefreshGroup(cur_mob->i_mob->num_group);

	mobs_group_busy.erase(cur_mob->i_mob->num_group);
	mobs_group_free.insert(cur_mob->i_mob->num_group);

}

void CServer::MOBs_RefreshGroup(WORD num_group)
{
	if(!num_group && num_group>=MOBS_MAX_GROUPS)
	{
		LogExecStr("Ошибка - Попытка обновить несуществующую группу |%d|\n",num_group);
		return;
	}

	mobs_group* cur_group=&mobs_gr[num_group];

	CCritter* cur_mob=NULL;
	prep_mob_map::iterator it_pm=NULL;
	for(cl_map::iterator it_m=cur_group->mobs.begin();it_m!=cur_group->mobs.end();++it_m)
	{
		cur_mob=(*it_m).second;
		if(cur_mob->info.map) LogExecStr("Ошибка - Не удаленный моб после очистки карты от мобов\n");

		it_pm=prep_mob.find(cur_mob->i_mob->name);
		if(it_pm==prep_mob.end()) continue;

		MOB_Update((*it_pm).second,cur_mob);
	}
}