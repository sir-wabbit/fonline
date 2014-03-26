/********************************************************************
  author: Anton Tsvetinsky
*********************************************************************/

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <FOnlineCommon/Common.hpp>
#include <IniFile/IniFile.hpp>

#include "FOServ.h"

namespace {

int stricmp(const char* str1, const char* str2) {
  while (*str1 && *str2) {
    if (::tolower(*str1) > ::tolower(*str2)) {
      return 1;
    } else if (::tolower(*str1) > ::tolower(*str2)) {
      return -1;
    } else {
      return 0;
    }

    str1++, str2++;
  }

  if (*str1) {
    return 1;
  } else if (*str2) {
    return -1;
  } else {
    return 0;
  }
}

void ClearNpcInfo(npc_info* info) {
  for(dialogs_map::iterator dlgIt = info->dialogs.begin(); dlgIt!=info->dialogs.end(); ++dlgIt) {
    npc_dialog* dialog = dlgIt->second;

    for (answers_list::iterator answerIt = dialog->answers.begin(); answerIt != dialog->answers.end(); ++answerIt) {
      answer* answ = *answerIt;

      for (demand_list::iterator demandIt = answ->demands.begin(); demandIt != answ->demands.end(); ++demandIt) {
        SafeDelete(*demandIt);
      }
      answ->demands.clear();

      for (result_list::iterator resultIt = answ->results.begin(); resultIt != answ->results.end(); ++resultIt) {
        SafeDelete(*resultIt);
      }
      answ->results.clear();

      SafeDelete(answ);
    }

    dialog->answers.clear();

    SafeDelete(dlgIt->second);
  }

  info->dialogs.clear();
}

} // anonymous namespace

void CServer::NPC_ClearAll() {
  for(cl_map::iterator it = pc.begin(); it != pc.end(); ++it) {
    if (it->second->i_npc != NULL) {
      ClearNpcInfo(it->second->i_npc);
    }

    SafeDelete(it->second);
  }

  pc.clear();
}

int CServer::NPC_LoadAll()
{
  char file_name[256];

  CCritter* npc=NULL;

  sprintf(file_name,"%slist_npc.npc", PATH_NPC);

  FILE* cf2;

  if((cf2=fopen(file_name, "rt"))==NULL)
  {
    FONLINE_LOG("Файл не найден |%s|", file_name);
    return 0;
  }

  char ch;
  char npc_name[256];
  int npc_id=0;

  while(!feof(cf2))
  {
    fscanf(cf2,"%c",&ch);

    if(ch!='#') continue;

    FONLINE_LOG("Инициализация НПЦ:...");

    if(fscanf(cf2,"%d%s",&npc_id,&npc_name[0])!=2)
    {
      FONLINE_LOG("ошибка в чтении данных");
      return 0;
    }

    FONLINE_LOG("%s...",npc_name);

    npc=new CCritter;
    npc->i_npc=new npc_info;

    npc->info.id=npc_id;

    if(npc_id<NPC_MIN_ID || npc_id>NPC_MAX_ID)
    {
      FONLINE_LOG("неверный ID");
      return 0;
    }

    sprintf(file_name,"%s%s.npc", PATH_NPC, npc_name);

    if(sql.CountRows("npc","id",npc->info.id))
    {
      FONLINE_LOG("найден в базе...");

      if(!sql.LoadDataNPC(&npc->info))
      {
        FONLINE_LOG("Ошибка при загрузке основных параметров из БД");
        return 0;
      }
    }
    else
    {
      FONLINE_LOG("в файле |%s|...",file_name);

      bool Err_load=false;

      Err_load=false;

      using namespace IniFile;

      RecordMap mapInfo;
      LoadINI(file_name, mapInfo);

    //основные параметры
    //  if((npc->info.id    =GetPrivateProfileInt("info","id"   ,-1,file_name))==-1) Err_load=true;
      if ((npc->info.base_type  = GetValue<int>(mapInfo, "info.base_type", -1)) == -1) Err_load=true;
      if ((npc->info.map = GetValue<int>(mapInfo, "info.map", -1)) == -1) Err_load=true;
      if ((npc->info.x = GetValue<int>(mapInfo, "info.x", -1)) == -1) Err_load=true;
      if ((npc->info.y = GetValue<int>(mapInfo, "info.y", -1)) == -1) Err_load=true;
      if ((npc->info.ori = GetValue<int>(mapInfo, "info.ori", -1)) == -1) Err_load=true;

      std::string tmp;

      tmp = GetValue<std::string>(mapInfo, "info.name", "e");
      tmp.copy(npc->info.name, MAX_NAME, 0);
      if(npc->info.name[0]    =='e') Err_load=true;

      tmp = GetValue<std::string>(mapInfo, "info.cases0", "e");
      tmp.copy(npc->info.cases[0], MAX_NAME, 0);
      if(npc->info.cases[0][0]  =='e') Err_load=true;

      tmp = GetValue<std::string>(mapInfo, "info.cases1", "e");
      tmp.copy(npc->info.cases[1], MAX_NAME, 0);
      if(npc->info.cases[1][0]  =='e') Err_load=true;

      tmp = GetValue<std::string>(mapInfo, "info.cases2", "e");
      tmp.copy(npc->info.cases[2], MAX_NAME, 0);
      if(npc->info.cases[2][0]  =='e') Err_load=true;

      tmp = GetValue<std::string>(mapInfo, "info.cases3", "e");
      tmp.copy(npc->info.cases[3], MAX_NAME, 0);
      if(npc->info.cases[3][0]  =='e') Err_load=true;

      tmp = GetValue<std::string>(mapInfo, "info.cases4", "e");
      tmp.copy(npc->info.cases[4], MAX_NAME, 0);
      if(npc->info.cases[4][0]  =='e') Err_load=true;

    //объекты
      npc->info.a_obj=&npc->info.def_obj1;
      npc->info.a_obj_arm=&npc->info.def_obj2;
      npc->info.a_obj->object=all_s_obj[npc->info.base_type];
      npc->info.a_obj_arm->object=all_s_obj[npc->info.base_type+200];

    //статы, скиллы, перки
      int go=0;
      for(go=0; go<ALL_STATS ; go++) npc->info.st[go]=5;
      for(go=0; go<ALL_SKILLS; go++) npc->info.sk[go]=5;
      for(go=0; go<ALL_PERKS ; go++) npc->info.pe[go]=0;
      npc->info.st[ST_GENDER]=0;
      npc->info.st[ST_AGE]=50;

      npc->info.st[ST_CURRENT_HP]=50;
      npc->info.st[ST_MAX_LIFE]=50;

      npc->InitNPC();

    //состояния
      npc->info.cond=COND_LIFE;
      npc->info.cond_ext=COND_LIFE_NONE;
      npc->info.flags=FCRIT_NPC;
      npc->state=STATE_GAME;

      if(Err_load==true)
      {
        FONLINE_LOG("Ошибка при загрузке основных параметров из файла");
        return 0;
      }
    }

  //переменные
    FILE* cf;
    char p_tmp1[200];
    int p_tmpi=0;
    if((cf=fopen(file_name, "rt"))==NULL)
    {
      FONLINE_LOG("Файл не найден |%s|", file_name);
      return 0;
    }

    while(!feof(cf))
    {
      fscanf(cf, "%s", p_tmp1);
      if(!stricmp(p_tmp1,"[vars]"))
      {
        char var_name[30];
        int var_count=0;
        int var_min=0;
        int var_max=0;

        while(!feof(cf))
        {
          fscanf(cf, "%s", p_tmp1);
          if(!stricmp(p_tmp1,"[end_vars]")) break;
        //имя
      //    if(sql.GetInt("npc_vars_templates","COUNT(*)","name",p_tmp1))
      //      { FONLINE_LOG("Ошибка в переменных - реиндитификация"); return 0; }
          strcpy(var_name,p_tmp1);

        //count
          fscanf(cf, "%d", &p_tmpi);
          var_count=p_tmpi;

        //min
          fscanf(cf, "%s%d", p_tmp1, &p_tmpi);
          var_min=p_tmpi;

        //max
          fscanf(cf, "%s%d", p_tmp1, &p_tmpi);
          var_max=p_tmpi;

          if(var_count<var_min || var_count>var_max) {
            FONLINE_LOG("Ошибка в переменных - неверные данные");
            return 0;
          }

          sql.Query("INSERT INTO npc_vars_templates (npc_id,name,count,min,max) VALUES('%d','%s','%d','%d','%d')",
            npc->info.id,var_name,var_count,var_min,var_max);
        }
        break;
      }
    }
    fclose(cf);

  //диалоги
    int read_int=0;
    int read_int2=0;
    int read_int3=0;
    char read_str[100];
    char read_str2[100];
    char read_str3[100];
    char ch[20];
    bool read_proc = false;

    if((cf=fopen(file_name, "rt"))!=NULL)
    {
      while(!feof(cf))
      {
        fscanf(cf, "%c", ch);
        if(ch[0]=='&')
        {
          read_proc=true;
          break;
        }
      }

      npc_dialog* dlg;
      answer* answ;
//ДИАЛОГИ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      while((!feof(cf))&&(read_proc))
      {
        dlg=new npc_dialog;
      //ID диалога
        fscanf(cf, "%d", &read_int);
        dlg->id=read_int;
      //ID текста
        fscanf(cf, "%d", &read_int);
        dlg->id_text=read_int;
      //действия при неответе
        fscanf(cf, "%s", read_str);
        dlg->not_answer=0;
      //время на прочтение
        fscanf(cf, "%d", &read_int);
        if(read_int) dlg->time_break=read_int;
//ДИАЛОГИ----------------------------------------------------------------------------------------
        fscanf(cf, "%c", ch);
        if(ch[0]=='@') continue;
        else if(ch[0]=='&') break;
        else if(ch[0]!='#')
        {
          read_proc=false;
          break;
        }
//ОТВЕТЫ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        while(!feof(cf))
        {
          answ=new answer;
        //ID линка
          fscanf(cf, "%d", &read_int);
          answ->link=read_int;
        //ID текста
          fscanf(cf, "%d", &read_int);
          answ->id_text=read_int;
//ОТВЕТЫ----------------------------------------------------------------------------------------
          fscanf(cf, "%c", ch);
//УСЛОВИЯ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(ch[0]=='u')
          {
            demand* new_demand;
            params_map::iterator it_d;
            while(!feof(cf))
            {
              fscanf(cf, "%c", ch);
              if(ch[0]!='*') break;
              new_demand=new demand;
            //название требования
              fscanf(cf, "%s", read_str);

              if(!stricmp(read_str,"stat"))
              {
                fscanf(cf, "%s", read_str2); //название стата
                fscanf(cf, "%1s", read_str3); //оператор сравнения
                fscanf(cf, "%d", &read_int); //значение

                new_demand->type=DEMAND_STAT;
                it_d=stats_map.find(read_str2);
                if(it_d==stats_map.end()) {SafeDelete(new_demand); FONLINE_LOG("Неизвестный стат %s", read_str2); return 0;}
                new_demand->param=(*it_d).second;
                new_demand->oper=read_str3[0];
                new_demand->count=read_int;
              }
              else if(!stricmp(read_str,"skill"))
              {
                fscanf(cf, "%s", read_str2); //название скилла
                fscanf(cf, "%1s", read_str3); //оператор сравнения
                fscanf(cf, "%d", &read_int); //значение

                new_demand->type=DEMAND_SKILL;
                it_d=skills_map.find(read_str2);
                if(it_d==skills_map.end()) {SafeDelete(new_demand); FONLINE_LOG("Неизвестный скилл %s", read_str2); return 0;}
                new_demand->param=(*it_d).second;
                new_demand->oper=read_str3[0];
                new_demand->count=read_int;
              }
              else if(!stricmp(read_str,"perk"))
              {
                fscanf(cf, "%s", read_str2); //название перка
                fscanf(cf, "%1s", read_str3); //оператор сравнения
                fscanf(cf, "%d", &read_int); //значение

                new_demand->type=DEMAND_PERK;
                it_d=perks_map.find(read_str2);
                if(it_d==perks_map.end()) {SafeDelete(new_demand); FONLINE_LOG("Неизвестный перк %s", read_str2); return 0;}
                new_demand->param=(*it_d).second;
                new_demand->oper=read_str3[0];
                new_demand->count=read_int;
              }
              else if(!stricmp(read_str,"var"))
              {
                fscanf(cf, "%s", read_str2); //название переменной
                fscanf(cf, "%1s", read_str3); //оператор сравнения
                fscanf(cf, "%d", &read_int); //значение

                new_demand->type=DEMAND_VAR;

            //    if(!sql.GetInt("npc_vars_templates","COUNT(*)","name",read_str2)) {SafeDelete(new_result); FONLINE_LOG("Неизвестная переменная %s", read_str2); return 0;}
                new_demand->var_name=read_str2;
                new_demand->oper=read_str3[0];
                new_demand->count=read_int;
              }
              else if(!stricmp(read_str,"pvar"))
              {
                fscanf(cf, "%d", &read_int); //номер переменной
                fscanf(cf, "%1s", read_str3); //оператор сравнения
                fscanf(cf, "%d", &read_int2); //значение

                new_demand->type=DEMAND_PVAR;

                new_demand->var_num=read_int;
                new_demand->oper=read_str3[0];
                new_demand->count=read_int2;
              }
              else if(!stricmp(read_str,"quest"))
              {
                fscanf(cf, "%d", &read_int); //номер квеста
                fscanf(cf, "%d", &read_int2); //номер выборки
                fscanf(cf, "%d", &read_int3); //значение квеста

                new_demand->type=DEMAND_QUEST;

                new_demand->var_num=read_int;
                new_demand->oper=read_int2;
                new_demand->count=read_int3;
              }
              else if(!stricmp(read_str,"item"))
              {
                fscanf(cf, "%d", &read_int); //номер итема
                fscanf(cf, "%1s", read_str2); //оператор сравнения

                SafeDelete(new_demand);
                continue;
              }
              else
              {
                SafeDelete(new_demand);
                FONLINE_LOG("Неизвестное условие %s", read_str);
                continue;
              }
              answ->demands.push_back(new_demand);
            }
          }
//УСЛОВИЯ----------------------------------------------------------------------------------------
//РЕЗУЛЬТАТ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(ch[0]=='r')
          {
            result* new_result;
            params_map::iterator it_r;
            while(!feof(cf))
            {
              fscanf(cf, "%c", ch);
              if(ch[0]!='*') break;
              new_result=new result;
            //название результата
              fscanf(cf, "%s", read_str);
              if(!stricmp(read_str,"stat"))
                {
                fscanf(cf, "%s", read_str2); //название стата
                fscanf(cf, "%1s", read_str3); //оператор присваивания
                fscanf(cf, "%d", &read_int); //значение

                new_result->type=RESULT_STAT;
                it_r=stats_map.find(read_str2);
                if(it_r==stats_map.end()) {SafeDelete(new_result); FONLINE_LOG("Неизвестный стат %s", read_str2); return 0;}
                new_result->param=(*it_r).second;
                new_result->oper=read_str3[0];
                new_result->count=read_int;
              }
              else if(!stricmp(read_str,"skill"))
              {
                fscanf(cf, "%s", read_str2); //название скилла
                fscanf(cf, "%1s", read_str3); //оператор присваивания
                fscanf(cf, "%d", &read_int); //значение

                new_result->type=RESULT_SKILL;
                it_r=skills_map.find(read_str2);
                if(it_r==skills_map.end()) {SafeDelete(new_result); FONLINE_LOG("Неизвестный скилл %s", read_str2); return 0;}
                new_result->param=(*it_r).second;
                new_result->oper=read_str3[0];
                new_result->count=read_int;
              }
              else if(!stricmp(read_str,"perk"))
              {
                fscanf(cf, "%s", read_str2); //название перка
                fscanf(cf, "%1s", read_str3); //оператор присваивания
                fscanf(cf, "%d", &read_int); //значение

                new_result->type=RESULT_PERK;
                it_r=perks_map.find(read_str2);
                if(it_r==perks_map.end()) {SafeDelete(new_result); FONLINE_LOG("Неизвестный перк %s", read_str2); return 0;}
                new_result->param=(*it_r).second;
                new_result->oper=read_str3[0];
                new_result->count=read_int;
              }
              else if(!stricmp(read_str,"var"))
              {
                fscanf(cf, "%s", read_str2); //название переменной
                fscanf(cf, "%1s", read_str3); //оператор присваивания
                fscanf(cf, "%d", &read_int); //значение

                new_result->type=RESULT_VAR;

            //    if(!sql.GetInt("npc_vars_templates","COUNT(*)","name",read_str2)) {SafeDelete(new_result); FONLINE_LOG("Неизвестная переменная %s", read_str2); return 0;}
                new_result->var_name=read_str2;
                new_result->oper=read_str3[0];
                new_result->count=read_int;
              }
              else if(!stricmp(read_str,"pvar"))
              {
                fscanf(cf, "%d", &read_int); //название переменной
                fscanf(cf, "%1s", read_str3); //оператор присваивания
                fscanf(cf, "%d", &read_int2); //значение

                new_result->type=RESULT_PVAR;

                new_result->var_num=read_int;
                new_result->oper=read_str3[0];
                new_result->count=read_int2;
              }
              else if(!stricmp(read_str,"quest"))
              {
                fscanf(cf, "%d", &read_int); //номер квеста
                fscanf(cf, "%d", &read_int2); //номер выборки
                fscanf(cf, "%d", &read_int3); //значение квеста

                new_result->type=RESULT_QUEST;

                new_result->var_num=read_int;
                new_result->oper=read_int2;
                new_result->count=read_int3;
              }
              else if(!stricmp(read_str,"item"))
              {
                fscanf(cf, "%d", &read_int); //номер итема
                fscanf(cf, "%2s", read_str2); //оператор присваивания

                SafeDelete(new_result);
                continue;
              }
              else if(!stricmp(read_str,"lock"))
              {
                fscanf(cf, "%d", &read_int); //время блокировки

                SafeDelete(new_result);
                continue;
              }
              else
              {
                SafeDelete(new_result);
                FONLINE_LOG("Неизвестный результат %s", read_str);
                continue;
              }
              answ->results.push_back(new_result);
            }
          }
//РЕЗУЛЬТАТ----------------------------------------------------------------------------------------
        //проверки
          if(feof(cf))
          {
            read_proc=false;
            break;
          }
          if(ch[0]=='#')
          {
            dlg->answers.push_back(answ);
            continue;
          }
          if((ch[0]=='@')||(ch[0]=='&'))
          {
            dlg->answers.push_back(answ);
            break;
          }
          read_proc=false;
          break;
        }
        npc->i_npc->dialogs[dlg->id]=dlg;
        if(ch[0]=='&') break;
      }

      fclose(cf);
    }
    else
    {
      FONLINE_LOG("Файл не найден");
      return 0;
    }

    if(!read_proc)
    {
      FONLINE_LOG("Ошибка при инициализации");
      return 0;
    }

    if(!sql.CountRows("npc","id",npc->info.id))
      if(!sql.NewNPC(&npc->info))
      {
        FONLINE_LOG("Ошибка регистрации НПЦ");
        return 0;
      }

    if(AddCrToMap(npc,npc->info.map,npc->info.x,npc->info.y)!=TR_OK)
    {
      FONLINE_LOG("Не удалось высадить НПЦ");
      return 0;
    }

    pc[npc->info.id]=npc;
    cr[npc->info.id]=npc;

    FONLINE_LOG("OK");
  }

  fclose(cf2);

  return 1;
/*
//Отладка
  FONLINE_LOG("");
  FONLINE_LOG("Отладочная инфа:");

  FONLINE_LOG("Имя:%s, к0:%s, к2:%s, к3:%s, к4:%s", npc->info.name, npc->info.cases[0],
    npc->info.cases[1], npc->info.cases[2], npc->info.cases[3], npc->info.cases[4]);
  FONLINE_LOG("id:%d, карта:%d, x:%d, y:%d, ориентация:%d, тип нпц:%d", npc->info.id,
    npc->info.map, npc->info.x, npc->info.y, npc->info.ori, npc->info.base_type);

  npc_dialog* dlg=NULL;
  answer* answ=NULL;

  FONLINE_LOG("Всего диалогов: %d", npc->i_npc->dialogs.size());

  for(dialogs_map::iterator it=npc->i_npc->dialogs.begin(); it!=npc->i_npc->dialogs.end(); it++)
  {
    FONLINE_LOG("");

    dlg=(*it).second;

    FONLINE_LOG("Диалог:%d, текст №%d, время на прочтение:%d, нет ответа:%d", dlg->id, dlg->id_text,
      dlg->time_break, dlg->not_answer);

    for(answers_list::iterator it_a=dlg->answers.begin(); it_a!=dlg->answers.end(); it_a++)
    {
      answ=(*it_a);
      FONLINE_LOG("Основной текст:%d, линк:%d, блокировка:%d, ", answ->id_text,
        answ->link_dialog, answ->lock_answered);

      if(answ->demand.size())
      {
        FONLINE_LOG("требования |");

        for(demand_map::iterator it_dm=answ->demand.begin(); it_dm!=answ->demand.end(); it_dm++)
          FONLINE_LOG(" %d > %d |", (*it_dm).first, (*it_dm).second);
      }
      else
        FONLINE_LOG("требований нет");

      FONLINE_LOG(", ");

      if(answ->overpatching.size())
      {
        FONLINE_LOG("результат |");

        for(overpatching_map::iterator it_op=answ->overpatching.begin(); it_op!=answ->overpatching.end(); it_op++)
          FONLINE_LOG(" %d > %d |", (*it_op).first, (*it_op).second);
      }
      else
        FONLINE_LOG("результатов нет");

      FONLINE_LOG("");
    }
  }

  FONLINE_LOG("");

  return 1;
*/
}

void CServer::NPC_Remove(CCritter* npc)
{

}

void CServer::NPC_Process(CCritter* npc)
{
  return;

  if(npc->info.cond!=COND_LIFE) return;
  if(npc->vis_cl.empty()) return;
  if (npc->info.break_time + npc->info.start_bt > GetMilliseconds()) {
    return;
  }

//  npc->info.break_time=0;

  uint16_t move_params=BIN16(00000000,00111100);

  HexTYPE stepy;
  if(npc->info.y%2)
    stepy=1;
  else
    stepy=-1;

  uint8_t move_res=MoveToTile(npc,npc->info.x,npc->info.y+stepy);

  switch(move_res)
  {
  case MR_STEP:
    SetVisCr(npc);
    //SetVisibleObj(acl);
    SendA_Move(npc,move_params);
    break;
/*  case MR_TRANSIT:
    map_cr[old_map].erase(acl->info.id);
    DelClFromAllListId(acl->info.id,old_map);
    Send_LoadMap(acl);
    acl->state=STATE_LOGINOK;
    break;*/
  default:
  case MR_FALSE:
    SetCheat(npc, "Process_Move - попытка походить в занятую клетку");
    break;
  }

  npc->info.start_bt=GetMilliseconds();
  npc->info.break_time=2000;
}

void CServer::NPC_Dialog_Close(CCritter* npc, CCritter* acl, uint8_t onhead_say)
{
  npc->i_npc->talking=0;

//  npc->info.break_time<=0;

  npc->i_npc->compiled_dialog.id=0;
  if(acl) Send_Talk(acl,NULL);
}

int CServer::NPC_Dialog_Compile(CCritter* npc, CCritter* acl, npc_dialog* new_dlg)
{
  npc->i_npc->compiled_dialog=(*new_dlg); //конструктор копирования должен??? уничтожить все старые объекты !!!!!!!!!!
  npc_dialog* cmp_dlg=&npc->i_npc->compiled_dialog;

  if(cmp_dlg->id==1) return 0;

//составляем ветку ответов
  for(answers_list::iterator it_a=cmp_dlg->answers.begin();it_a!=cmp_dlg->answers.end();it_a++)
  {
  //смотрим требование
    if(!NPC_Check_demand(npc,acl,(*it_a))) cmp_dlg->answers.erase(it_a);

    if(it_a==cmp_dlg->answers.end()) break;
  }

  if(!cmp_dlg->answers.size()) return 0;

  return 1;
}

int CServer::NPC_Check_demand(CCritter* npc, CCritter* acl, answer* check_answ)
{
  if(!check_answ->demands.size()) return 1;

  for(demand_list::iterator it_d=check_answ->demands.begin();it_d!=check_answ->demands.end();it_d++)
  {
    switch ((*it_d)->type)
    {
    case DEMAND_NONE:
      //SafeDelete((*it_d));
      //check_answ->demands.erase(it_d);
      continue;
    case DEMAND_STAT:
      if((*it_d)->oper=='>')     { if(acl->info.st[(*it_d)->param]> (*it_d)->count) continue; }
      else if((*it_d)->oper=='<') { if(acl->info.st[(*it_d)->param]< (*it_d)->count) continue; }
      else if((*it_d)->oper=='=') { if(acl->info.st[(*it_d)->param]==(*it_d)->count) continue; }
      return 0;
    case DEMAND_SKILL:
      if((*it_d)->oper=='>')     { if(acl->info.sk[(*it_d)->param]> (*it_d)->count) continue; }
      else if((*it_d)->oper=='<') { if(acl->info.sk[(*it_d)->param]< (*it_d)->count) continue; }
      else if((*it_d)->oper=='=') { if(acl->info.sk[(*it_d)->param]==(*it_d)->count) continue; }
      return 0;
    case DEMAND_PERK:
      if((*it_d)->oper=='>')     { if(acl->info.pe[(*it_d)->param]> (*it_d)->count) continue; }
      else if((*it_d)->oper=='<') { if(acl->info.pe[(*it_d)->param]< (*it_d)->count) continue; }
      else if((*it_d)->oper=='=') { if(acl->info.pe[(*it_d)->param]==(*it_d)->count) continue; }
      return 0;
    case DEMAND_VAR:
      if(sql.CheckVarNPC(npc->info.id,(*it_d)->var_name,acl->info.id,(*it_d)->oper,(*it_d)->count)) continue;
      return 0;
    case DEMAND_PVAR:
      if(sql.CheckVar(acl->info.id,(*it_d)->var_num,(*it_d)->oper,(*it_d)->count)) continue;
      return 0;
    case DEMAND_QUEST:
      if(sql.CheckQuest(acl->info.id,(*it_d)->var_num,(*it_d)->oper,(*it_d)->count)) continue;
      return 0;
    default:
      continue;
    }
  }
  return 1;
}

void CServer::NPC_Use_result(CCritter* npc, CCritter* acl, answer* use_answ)
{
  if(!use_answ->results.size()) return;

  for(result_list::iterator it_r=use_answ->results.begin(); it_r!=use_answ->results.end(); it_r++)
  {
    switch ((*it_r)->type)
    {
    case RESULT_NONE:
      //SafeDelete((*it_r));
      //check_answ->demands.erase(it_r);
      continue;
    case RESULT_STAT:
      if((*it_r)->oper=='+')     acl->info.st[(*it_r)->param]+=(*it_r)->count;
      else if((*it_r)->oper=='-') acl->info.st[(*it_r)->param]-=(*it_r)->count;
      else if((*it_r)->oper=='*') acl->info.st[(*it_r)->param]*=(*it_r)->count;
      else if((*it_r)->oper=='/') acl->info.st[(*it_r)->param]/=(*it_r)->count;
      else if((*it_r)->oper=='=') acl->info.st[(*it_r)->param] =(*it_r)->count;
    //посылаем уведомление
      Send_Param(acl,TYPE_STAT,(*it_r)->param);
      continue;
    case RESULT_SKILL:
      if((*it_r)->oper=='+')     acl->info.sk[(*it_r)->param]+=(*it_r)->count;
      else if((*it_r)->oper=='-') acl->info.sk[(*it_r)->param]-=(*it_r)->count;
      else if((*it_r)->oper=='*') acl->info.sk[(*it_r)->param]*=(*it_r)->count;
      else if((*it_r)->oper=='/') acl->info.sk[(*it_r)->param]/=(*it_r)->count;
      else if((*it_r)->oper=='=') acl->info.sk[(*it_r)->param] =(*it_r)->count;
    //посылаем уведомление
      Send_Param(acl,TYPE_STAT,(*it_r)->param);
      continue;
    case RESULT_PERK:
      if((*it_r)->oper=='+')     acl->info.pe[(*it_r)->param]+=(*it_r)->count;
      else if((*it_r)->oper=='-') acl->info.pe[(*it_r)->param]-=(*it_r)->count;
      else if((*it_r)->oper=='*') acl->info.pe[(*it_r)->param]*=(*it_r)->count;
      else if((*it_r)->oper=='/') acl->info.pe[(*it_r)->param]/=(*it_r)->count;
      else if((*it_r)->oper=='=') acl->info.pe[(*it_r)->param] =(*it_r)->count;
    //посылаем уведомление
      Send_Param(acl,TYPE_STAT,(*it_r)->param);
      continue;
    case RESULT_VAR:
      sql.ChangeVarNPC(npc->info.id,(*it_r)->var_name,acl->info.id,(*it_r)->oper,(*it_r)->count);
      continue;
    case RESULT_PVAR:
      sql.ChangeVar(acl->info.id,(*it_r)->var_num,(*it_r)->oper,(*it_r)->count);
      continue;
    case RESULT_QUEST:
      sql.ChangeQuest(acl->info.id,(*it_r)->var_num,(*it_r)->oper,(*it_r)->count);
      continue;
    default:
      continue;
    }
  }
}

void CServer::Process_Talk_NPC(CCritter* acl)
{
  CritterID id_npc_talk;
  uint8_t num_answer;

  acl->bin >> id_npc_talk;
  acl->bin >> num_answer;

  if(!acl->info.map) return;
//находим непися
  CCritter* npc;
  cl_map::iterator it=pc.find(id_npc_talk);
  if(it==pc.end())
  {
    SetCheat(acl,"Process_Talk_NPC - не найден НПЦ");
    return;
  }
  npc=(*it).second;

  if(npc->info.cond!=COND_LIFE)
  {
    SetCheat(acl,"Process_Talk_NPC - попытка заговорить с не живым НПЦ");
    NPC_Dialog_Close(npc,acl,NPC_SAY_NONE);
    return;
  }

  int dist=(int) sqrt(pow(acl->info.x-npc->info.x,2.0)+pow(acl->info.y-npc->info.y,2.0));

  if(dist>TALK_NPC_DISTANCE)
  {
    SetCheat(acl,"Process_Talk_NPC - дистанция разговора превышает максимальную");
    NPC_Dialog_Close(npc,acl,NPC_SAY_NONE);
    return;
  }

  Skill_Sneak_UnSet(acl);

  if(!npc->i_npc->dialogs.size())
  {
    NPC_Dialog_Close(npc,acl,NPC_SAY_HELLO);
    return;
  }

  dialogs_map::iterator it_d;
  answers_list::iterator it_a;
  npc_dialog* send_dialog=&npc->i_npc->compiled_dialog;

//продолжаем разговор
  if(npc->i_npc->talking==acl->info.id)
  {
    if(!send_dialog)
    {
      FONLINE_LOG("Диалог - Ошибка. Пустой указатель на предыдущий диалог");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
    if(send_dialog->id==0 || send_dialog->id==1)
    {
      FONLINE_LOG("Диалог - Ошибка. ID диалога равна %d", send_dialog->id);
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
    if(num_answer+1 > send_dialog->answers.size())
    {
      FONLINE_LOG("Диалог - Ошибка. Ответ первышает максимальное значение ответов");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  //находим ид ответа
    it_a=send_dialog->answers.begin()+num_answer;
    if(!(*it_a))
    {
      FONLINE_LOG("Диалог - Ошибка. Пустой указатель ответа");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  //выполняем результат ответа
    NPC_Use_result(npc,acl,(*it_a));
  //проверяем на нулевой линк, т.е. выход
    if(!(*it_a)->link)
    {
      NPC_Dialog_Close(npc,acl,NPC_SAY_NONE);
      return;
    }
  //проверяем на еденичный линк, т.е. возврат к предыдущему
    if((*it_a)->link==1)
    {
      //!!!!!!!!!!!
    }
  //ищем диалог
    it_d=npc->i_npc->dialogs.find((*it_a)->link);
    if(it_d==npc->i_npc->dialogs.end())
    {
      FONLINE_LOG("Диалог - Ошибка. Не найден диалог по ответу");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }

  //компануем диалог
    if(!NPC_Dialog_Compile(npc,acl,(*it_d).second))
    {
      FONLINE_LOG("Диалог - Ошибка. Неудалось скомпоновать диалог");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  }
//начинаем разговор
  else if(!npc->i_npc->talking)// && !npc->info.break_time)
  {
  //впервые
    it_d=npc->i_npc->dialogs.begin();

    int go_dialog=0;
    for(it_a=(*it_d).second->answers.begin(); it_a!=(*it_d).second->answers.end(); it_a++)
    {
      if(NPC_Check_demand(npc,acl,(*it_a)))
      {
        go_dialog=(*it_a)->link;
        break;
      }
    }
    if(it_a==(*it_d).second->answers.end())
    {
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  //выполняем результат
    NPC_Use_result(npc,acl,(*it_a));

    if(!go_dialog)
    {
      NPC_Dialog_Close(npc,acl,NPC_SAY_FUCKOFF);
      return;
    }

    it_d=npc->i_npc->dialogs.find(go_dialog);

    if(it_d==npc->i_npc->dialogs.end())
    {
      FONLINE_LOG("Диалог - Ошибка. Не найден диалог по предустановкам");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  //компануем диалог
    if(!NPC_Dialog_Compile(npc,acl,(*it_d).second))
    {
      FONLINE_LOG("Диалог - Ошибка. Неудалось скомпоновать диалог");
      NPC_Dialog_Close(npc,acl,NPC_SAY_ERROR);
      return;
    }
  }
//нпц занят
  else
  {
    NPC_Dialog_Close(npc,acl,NPC_SAY_IMBYSY);
    return;
  }

//посылаем
  Send_Talk(acl,&npc->i_npc->compiled_dialog);

//  npc->info.start_bt=GetTickCount();
//  npc->info.break_time=TALK_MAX_TIME; //!!!!!!

  npc->i_npc->talking=acl->info.id;
}
