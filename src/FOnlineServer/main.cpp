#include "stdafx.h"

/********************************************************************
  created:  18:08:2004   23:02

  author:   Oleg Mareskin
  add/edit: Denis Balihin, Anton Tsvetinsky

  purpose:
*********************************************************************/


#include "main.h"
#include "FOServ.h"

#include <wchar.h>
#include <iostream>

int bQuit=0;
int FOQuit=0;

int logging=1;
uint32_t start_srv_time = GetMilliseconds();

//HANDLE hGameThread=NULL;
//DWORD dwGameThreadID=0;
int NumClients=0;
int NumCritters=0; //!Cvet всего криттеров в игре
//HANDLE hUpdateEvent;

CServer serv;

int main(int argc, char** argv) {
  if (!OpenLogFile("server.log")) {
    goto GAMELOOPEND;
  }

  //FONLINE_LOG("Version of Server: %d", FOSERVICE_VERSION);

  if (!serv.Init()) {
    goto GAMELOOPEND;
  }

  srand(GetMilliseconds());

  serv.RunGameLoop();

  FONLINE_LOG("***   Finishing Game loop   ***\n");

  FONLINE_LOG("Всего циклов:%d\n"
    "Средняя продолжительность цикла:%d\n"
    "Минимальная продолжительность цикла:%d\n"
    "Максимальная продолжительность цикла:%d\n"
    "Всего лагов (>100):%d\n",
    serv.loop_cycles,
    serv.loop_time/serv.loop_cycles,
    serv.loop_min,
    serv.loop_max,
    serv.lags_count);

  FONLINE_LOG("Средняя продолжительность по процессам:\n"
    "Сеть:%d\n"
    "Соединение:%d\n"
    "Прием:%d\n"
    "Обработка игроков:%d\n"
    "Обработка НПЦ:%d\n"
    "Посылка:%d\n"
    "Отключение:%d\n",
    serv.lt_FDsel/serv.loop_cycles,
    serv.lt_conn/serv.loop_cycles,
    serv.lt_input/serv.loop_cycles,
    serv.lt_proc_cl/serv.loop_cycles,
    serv.lt_proc_pc/serv.loop_cycles,
    serv.lt_output/serv.loop_cycles,
    serv.lt_discon/serv.loop_cycles);

  serv.Finish();

  CloseLogFile();

GAMELOOPEND:
  return 0;
}
