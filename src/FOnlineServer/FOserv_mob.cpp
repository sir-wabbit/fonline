/********************************************************************
  author: Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"

#include <string.h>
#include <stdio.h>

#include <IniFile/IniFile.hpp>

#include "FOServ.h"

int CServer::MOBs_LoadAllGroups()
{

  FONLINE_LOG("Загрузка групп мобов");

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
    FONLINE_LOG("Файл не найден |%s|",data_name);
    return 0;
  }

  int cur_mob_id=MOB_MIN_ID;

  while(!feof(cf1))
  {
    fscanf(cf1,"%c",&ch1);

    if(ch1!='@') continue;

    FONLINE_LOG("\tНайдена группа, номер:");

    fscanf(cf1,"%d",&num_group);
    if(num_group<=0 || num_group>=MOBS_MAX_GROUPS)
    {
      FONLINE_LOG("неправильный номер группы");
      return 0;
    }
    FONLINE_LOG("%d...",num_group);

    cur_group=&mobs_gr[num_group];
    if(cur_group->num)
    {
      FONLINE_LOG("дублирование группы");
      return 0;
    }
    cur_group->num=num_group;

    count_group=0;

    while(!feof(cf1))
    {
      fscanf(cf1,"%c",&ch1);

      if(ch1=='@') break;
      if(ch1!='#') continue;

      mob_name[0] = 0;
      fscanf(cf1,"%s",&mob_name[0]);
      if(!mob_name[0])
      {
        FONLINE_LOG("не удалось прочитать имя моба");
        return 0;
      }

      FONLINE_LOG("%s...",mob_name);

/*
  FILE* cf;



  if(!(cf=fopen(mob_path, "rt")))
  {
    FONLINE_LOG("Файл не найден |%s|",mob_path);
    return 0;
  }

  fclose(cf);
*/

      prep_mob_map::iterator it_fm=prep_mob.find(std::string(mob_name));
      if(it_fm==prep_mob.end())
      {
        FONLINE_LOG("Загрузка прототипа...");

        CCritter* pmob=new CCritter;
        pmob->i_mob=new mob_info;

        char mob_path[256];
        sprintf(mob_path,"%s%s.mob",PATH_MOB,mob_name);

        using namespace IniFile;

        RecordMap mobInfo;
        LoadINI(mob_path, mobInfo);

        bool Err_load=false;

        //основные параметры
        pmob->info.map=0;
        pmob->info.x=0;
        pmob->info.y=0;
        pmob->info.ori=0;

        if ((pmob->info.base_type = GetValue<int>(mobInfo, "info.base_type", -1)) == -1) {
          Err_load = true;
        }

        if ((pmob->i_mob->base_cond = GetValue<int>(mobInfo, "info.mob_info", -1)) == -1) {
          Err_load = true;
        }

        pmob->i_mob->cond = pmob->i_mob->base_cond;
        pmob->i_mob->name = std::string(mob_name);

        std::string tmp;

        tmp = GetValue<std::string>(mobInfo, "info.name", "e");
        tmp.copy(pmob->info.name, MAX_NAME, 0);
        if (pmob->info.name[0] == 'e') {
          Err_load = true;
        }

        tmp = GetValue<std::string>(mobInfo, "info.cases0", "e");
        tmp.copy(pmob->info.cases[0], MAX_NAME, 0);
        if (pmob->info.cases[0][0] == 'e') {
          Err_load = true;
        }

        tmp = GetValue<std::string>(mobInfo, "info.cases1", "e");
        tmp.copy(pmob->info.cases[1], MAX_NAME, 0);
        if (pmob->info.cases[1][0] == 'e') {
          Err_load = true;
        }

        tmp = GetValue<std::string>(mobInfo, "info.cases2", "e");
        tmp.copy(pmob->info.cases[2], MAX_NAME, 0);
        if (pmob->info.cases[2][0] == 'e') {
          Err_load = true;
        }

        tmp = GetValue<std::string>(mobInfo, "info.cases3", "e");
        tmp.copy(pmob->info.cases[3], MAX_NAME, 0);
        if (pmob->info.cases[3][0] == 'e') {
          Err_load = true;
        }

        tmp = GetValue<std::string>(mobInfo, "info.cases4", "e");
        tmp.copy(pmob->info.cases[4], MAX_NAME, 0);
        if (pmob->info.cases[4][0] == 'e') {
          Err_load = true;
        }

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
          FONLINE_LOG("Ошибка при загрузке основных параметров из файла");
          SafeDelete(pmob);
          return 0;
        }

        prep_mob.insert(prep_mob_map::value_type(std::string(mob_name),pmob));

        FONLINE_LOG("OK...");
      }

      it_fm=prep_mob.find(std::string(mob_name));
      if(it_fm==prep_mob.end())
      {
        FONLINE_LOG("Ошибка - не найден прототип");
        return 0;
      }

      FONLINE_LOG("прототип найден...");

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

      FONLINE_LOG("OK...");
    }

    if(ch1!='@')
    {
      FONLINE_LOG("Read error.");
      return 0;
    }

    if(!count_group || count_group>GM_MAX_GROUP)
    {
      FONLINE_LOG("нулевая группа или превышает лимит");
      return 0;
    }

    FONLINE_LOG("группа загружена");

    cur_group->mobs_count=count_group;
    cur_group->mobs_level=0;

    mobs_group_free.insert(cur_group->num);
  }

  fclose(cf1);

  FONLINE_LOG("Загрузка групп мобов прошла успешно");

  return 1;
}

void CServer::MOB_Update(CCritter* prep_mob, CCritter* mob)
{
  if(!prep_mob || !mob) return;

  uint32_t mob_id=mob->info.id;
  uint16_t mob_group=mob->i_mob->num_group;

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
        FONLINE_LOG("!!!!MOB COND ERROR |%d|",mob->info.cond);
        continue;
      }


    //действия моба
      //Crit_MoveRandom(mob);
      // XXX[31.7.2012 alex]: should probably do it with Lua
      /*asIScriptContext *ctx=NULL;

      if(!(ctx=ss->CreateContext()))
      {
        FONLINE_LOG("e1");
        ctx->Release();
        continue;
      }

      asIScriptFunction* mainFunc = ss->GetGlobalFunctionByDecl("void main2(int mob)");

      if(!mainFunc)
      {
        FONLINE_LOG("e2");
        ctx->Release();
        continue;
      }

      if(ctx->Prepare(mainFunc)<0)
      {
        FONLINE_LOG("e3");
        ctx->Release();
        continue;
      }

      ctx->SetArgDWord(0,(uint32_t)(mob));

      if(ctx->Execute()<0)
      {
        FONLINE_LOG("e4");
        ctx->Release();
        continue;
      }

      ctx->Release();*/
    }
  }
}

int CServer::MOBs_AddToEncaunter(uint16_t num_encaunter, uint8_t mobs_level)
{
FONLINE_LOG("Высаживаем мобов==========================================");
  encaunter_info* cur_encaunter=&encaunter[num_encaunter];
  uint16_t num_map=cur_encaunter->emap->num;

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
      FONLINE_LOG("Не удалось высадить моба!!!");
      continue;
    }

    mob->info.ori=cur_encaunter->start_ori[cur_encaunter->count_groups];
  }

  cur_encaunter->count_groups++;

  mobs_group_busy.insert(cur_mobs->num);
  mobs_group_free.erase(cur_mobs->num);

FONLINE_LOG("x=%d,y=%d",cur_encaunter->start_hx[cur_encaunter->count_groups-1],cur_encaunter->start_hx[cur_encaunter->count_groups-1]);
FONLINE_LOG("Высадили мобов++++++++++++++++++++++++++++++++++++++++++++");
  return 1;
}

void CServer::MOBs_EraseFromMap(uint16_t num_map)
{
  if(!num_map && num_map>=MAX_MAPS)
  {
    FONLINE_LOG("Ошибка - Попытка удалить мобов на несуществующей карте |%d|",num_map);
    return;
  }

  CCritter* cur_mob=NULL;
  for(cl_map::iterator it_m=map_cr[num_map].begin();it_m!=map_cr[num_map].end();)
  {
    cur_mob=(*it_m).second;
    it_m++;

    if(!GetBits(cur_mob->info.flags,FCRIT_MOB)) continue;

    map_cr[num_map].erase(cur_mob->info.id);
    cur_mob->info.map=0;
    ClearBits(tile[num_map][cur_mob->info.x][cur_mob->info.y].flags,FT_PLAYER);
  }

  MOBs_RefreshGroup(cur_mob->i_mob->num_group);

  mobs_group_busy.erase(cur_mob->i_mob->num_group);
  mobs_group_free.insert(cur_mob->i_mob->num_group);

}

void CServer::MOBs_RefreshGroup(uint16_t num_group)
{
  if(!num_group && num_group>=MOBS_MAX_GROUPS)
  {
    FONLINE_LOG("Ошибка - Попытка обновить несуществующую группу |%d|",num_group);
    return;
  }

  mobs_group* cur_group=&mobs_gr[num_group];

  CCritter* cur_mob=NULL;
  for(cl_map::iterator it_m=cur_group->mobs.begin();it_m!=cur_group->mobs.end();++it_m)
  {
    cur_mob=(*it_m).second;
    if(cur_mob->info.map) FONLINE_LOG("Ошибка - Не удаленный моб после очистки карты от мобов");

    prep_mob_map::iterator it_pm = prep_mob.find(cur_mob->i_mob->name);
    if(it_pm==prep_mob.end()) continue;

    MOB_Update(it_pm->second,cur_mob);
  }
}
