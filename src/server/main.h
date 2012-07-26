#ifndef __MAIN_H__
#define __MAIN_H__

/********************************************************************
	created:	18:08:2004   23:02

	author:		Oleg Mareskin
	
	purpose:	
*********************************************************************/


extern int bQuit;
extern int FOQuit;
extern int NumClients;
extern HANDLE hUpdateEvent;

#define MAX_BUF_LEN 4096

extern DWORD start_srv_time;
extern int logging;
int LogStart(); //!Cvet
void LogFinish(); //!Cvet
void LogExecStr(char* frmt, ...);

#endif //__MAIN_H__