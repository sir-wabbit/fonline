#include "stdafx.h"

/********************************************************************
	created:	18:08:2004   23:02

	author:		Oleg Mareskin
	add/edit:	Denis Balihin, Anton Tsvetinsky

	purpose:	
*********************************************************************/


#include "main.h"
#include "FOserv.h"

#include <wchar.h>
#include <iostream>

int bQuit=0;
int FOQuit=0;

int logging=1;
DWORD start_srv_time=GetTickCount();

HANDLE hGameThread=NULL;
DWORD dwGameThreadID=0;
int NumClients=0;
int NumCritters=0; //!Cvet всего криттеров в игре
HANDLE hUpdateEvent;


DWORD WINAPI GameLoopThread(void *);

#ifndef FOSERVICE_VERSION

	BOOL CALLBACK DlgProc(HWND hwndDlg,UINT msg,WPARAM wParam,LPARAM lParam);
	
	void UpdateInfo();
	
	HINSTANCE hInstance;//дескриптор
	HWND hWnd;
	HWND hDlg=NULL;
	
	CServer* serv;
	
	int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,LPSTR lpCmdLine,int nCmdShow)
	{
		MSG msg;//сообщения
	
		LoadLibrary("RICHED32.dll");
		
		hInstance = hCurrentInst;

		hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DLG), NULL, DlgProc);
		hUpdateEvent=CreateEvent(NULL,1,0,NULL);
		//hGameThread=CreateThread(NULL,0,GameLoopThread,NULL,0,&dwGameThreadID);

    LogExecStr("test");

		serv=new CServer;

	//организация цикла обработки сообщений
		while(!bQuit)
		{
			if(MsgWaitForMultipleObjects(1,&hUpdateEvent,0,INFINITE,QS_ALLINPUT)==(WAIT_OBJECT_0+1))
				while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else UpdateInfo();
		}

		SAFEDEL(serv);
		_CrtDumpMemoryLeaks();

		return 0;
	}
	
	void UpdateInfo()
	{
		wchar_t text[300];
		ResetEvent(hUpdateEvent);
		wsprintfW(text, L"Players online : %d", NumClients);
		SetDlgItemTextW(hDlg,IDC_COUNT, text);
	}

	BOOL CALLBACK DlgProc(HWND hwndDlg,UINT msg,WPARAM wParam,LPARAM lParam)
	{
		switch(msg)
		{
		case WM_INITDIALOG:
		  //hDlg = hwndDlg;
			PostMessage(hWnd,WM_SIZE,0,0);
			return 1;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
				FOQuit=1;
				bQuit=1;
				if(WaitForSingleObject(hGameThread,10000)==WAIT_TIMEOUT)
					MessageBox(NULL,"Wait timeout 10 sec passed","Error",MB_OK|MB_ICONERROR);
				CloseHandle(hGameThread);
				CloseHandle(hUpdateEvent);
				CloseWindow(hDlg);
				return 0;
			case IDC_STOP:
				if(!FOQuit)
				{
					FOQuit=1;
					SetDlgItemText(hDlg,IDC_STOP,"Запустить");
					EnableWindow(GetDlgItem(hDlg,IDC_RELOAD),0);
				}
				else if(!hGameThread)
					{
						FOQuit=0;
						SetDlgItemText(hDlg,IDC_STOP,"Остановить");
						EnableWindow(GetDlgItem(hDlg,IDC_RELOAD),1);
						hGameThread=CreateThread(NULL,0,GameLoopThread,NULL,0,&dwGameThreadID);
					}
				break;
			}
			return 0;
		}
		return 0;
	}

  void AppendToLogEditBox(wchar_t* text) {
    SendDlgItemMessageW(hDlg, IDC_EXECLOG, EM_SETSEL, -1, 0);
    SendDlgItemMessageW(hDlg, IDC_EXECLOG, EM_REPLACESEL, 0, (LPARAM) text); 
  }
  
	void LogExecStr(char* fmt, ...) {
		if(bQuit) return;
		
		char buf[2048];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);
    
    wchar_t wbuf[2048];
    mbstowcs(wbuf, buf, sizeof(wbuf) / sizeof(wchar_t) - 1);
    
		AppendToLogEditBox(wbuf);
	}

	DWORD WINAPI GameLoopThread(void *)
	{
		if(!serv->Init()) goto GAMELOOPEND;
		
		serv->RunGameLoop();
	
		serv->Finish();

	GAMELOOPEND:	
		hGameThread=NULL;
		ExitThread(0);
		return 0;
	}

#else

//////////////////////////////////////////////////////////////////////////
//					SERVICE VERSION
//////////////////////////////////////////////////////////////////////////

	CServer serv;
	HANDLE hLogFile=NULL;

	SERVICE_STATUS          FOServiceStatus;  
	SERVICE_STATUS_HANDLE   FOServiceStatusHandle; 
	 
	VOID  WINAPI FOServiceStart (DWORD argc, LPTSTR *argv); 
	VOID  WINAPI FOServiceCtrlHandler (DWORD opcode); 
	 
	VOID _CRTAPI1 main()
	{
		SC_HANDLE SCManager = OpenSCManager( 
			NULL,                    // local machine 
			NULL,                    // ServicesActive database 
		    SC_MANAGER_ALL_ACCESS);  // 

		if(!SCManager) return;

		SC_HANDLE SCServ=OpenService(SCManager,"FOService",SERVICE_QUERY_CONFIG);
		if(!SCServ)
		{
			//регистрируем сервис
			HMODULE hm=GetModuleHandle(NULL);
			char pathstr[1024];
			GetModuleFileName(hm,pathstr,1024);
			SCServ=CreateService(SCManager,"FOService","FOService",SERVICE_ALL_ACCESS,
				SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,pathstr,NULL,NULL,NULL,NULL,NULL); //!Cvet заменил с SERVICE_AUTO_START на SERVICE_DEMAND_START
			CloseServiceHandle(SCServ);
			CloseServiceHandle(SCManager);
			MessageBox(NULL,"Registering FOserver service: OK.\nStart service from the control panel","Registering",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		LPQUERY_SERVICE_CONFIG lpqscBuf; 
	    DWORD dwBytesNeeded; 

	    lpqscBuf = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 4096); 
	 
	    // Get and print the information configuration. 
	 
	    if (!QueryServiceConfig(SCServ,lpqscBuf,4096,&dwBytesNeeded)) 
	    {
			CloseServiceHandle(SCServ);
			CloseServiceHandle(SCManager);
			LocalFree(lpqscBuf);     
			return;
	    }
		int last=0;
		for(int i=0;lpqscBuf->lpBinaryPathName[i];i++)
			if(lpqscBuf->lpBinaryPathName[i]=='\\') last=i;
		lpqscBuf->lpBinaryPathName[last+1]=0;
		SetCurrentDirectory(lpqscBuf->lpBinaryPathName);
		CloseServiceHandle(SCManager);
		LocalFree(lpqscBuf);     

		SERVICE_TABLE_ENTRY   DispatchTable[] = 
	    { 
	        { TEXT("FOService"), FOServiceStart}, 
	        { NULL, NULL }, 
	    };

		StartServiceCtrlDispatcher( DispatchTable);
	}

	void WINAPI FOServiceStart (DWORD argc, LPTSTR *argv)  
	{
	    FOServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
	    FOServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
	    FOServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
	    FOServiceStatus.dwWin32ExitCode      = 0; 
	    FOServiceStatus.dwServiceSpecificExitCode = 0; 
	    FOServiceStatus.dwCheckPoint         = 0; 

	    FOServiceStatus.dwWaitHint           = 0; 

	    FOServiceStatusHandle = RegisterServiceCtrlHandler( 
	        TEXT("FOService"), 
	        FOServiceCtrlHandler); 
	 
	    if (!FOServiceStatusHandle) 
	    { 
	        return;
	    } 
	 
		hGameThread=CreateThread(NULL,0,GameLoopThread,NULL,0,&dwGameThreadID);
	 
	    // Initialization complete - report running status. 
	    FOServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	    FOServiceStatus.dwCheckPoint         = 0; 
	    FOServiceStatus.dwWaitHint           = 0; 
	 
	    SetServiceStatus (FOServiceStatusHandle, &FOServiceStatus);

	    return; 
	} 
	
	VOID WINAPI FOServiceCtrlHandler (DWORD Opcode)  
	{ 
	    switch(Opcode) 
	    {
		case SERVICE_CONTROL_STOP: 
	
	        // Do whatever it takes to stop here. 
	            FOServiceStatus.dwWin32ExitCode = 0; 
	            FOServiceStatus.dwCurrentState  = SERVICE_STOP_PENDING; 
	            FOServiceStatus.dwCheckPoint    = 0; 
	            FOServiceStatus.dwWaitHint      = 0; 
	 
	            SetServiceStatus (FOServiceStatusHandle, 
	                &FOServiceStatus);
	
				FOQuit=1;
				bQuit=1;
				//CloseHandle(hLogFile);
				//hLogFile=NULL;
				if(hGameThread)
					WaitForSingleObject(hGameThread,5000);
				CloseHandle(hGameThread);

	            FOServiceStatus.dwWin32ExitCode = 0; 
	            FOServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
	            FOServiceStatus.dwCheckPoint    = 0; 
	            FOServiceStatus.dwWaitHint      = 0; 

	            SetServiceStatus (FOServiceStatusHandle, 
	                &FOServiceStatus);

	            return; 
	         case SERVICE_CONTROL_INTERROGATE: 
	        // Fall through to send current status. 
	            break;
	        default:
				break;
	    }
	 
	    // Send current status. 
	    SetServiceStatus (FOServiceStatusHandle,  &FOServiceStatus);

	    return;
	}

	int LogStart() //!Cvet
	{
		if(hLogFile) return 1;

		logging=1;

		hLogFile=CreateFile(".\\FOserv.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,NULL);

		if(hLogFile==INVALID_HANDLE_VALUE) return 0;

		return 1;
	}

	void LogFinish() //!Cvet
	{
		if(!hLogFile) return;

		CloseHandle(hLogFile);
		hLogFile=NULL;
	}

	void LogExecStr(char* frmt, ...)
	{
		if(!logging) return;

		char str[2048];
		char str2[2048];
	
	    va_list list;
	
	    va_start(list, frmt);
	    wvsprintf(str2, frmt, list);
	    va_end(list);

		sprintf(str,"(%d)%s",GetTickCount()-start_srv_time,str2);

		switch(logging)
		{
		case 1:
			DWORD br;
			WriteFile(hLogFile,str,strlen(str),&br,NULL);
			break;
		case 2:
			serv.sql.WriteLog(str);
			break;
		}
	}

	DWORD WINAPI GameLoopThread(void *)
	{
		if(!LogStart()) goto GAMELOOPEND; //!Cvet
	
		LogExecStr("Version of Server: %d\n",FOSERVICE_VERSION);

		if(!serv.Init()) goto GAMELOOPEND;

		logging=GetPrivateProfileInt("LOGGING","logging",1,".\\foserv.cfg"); //!Cvet

		srand(GetTickCount()); //!Cvet

		serv.RunGameLoop();

	//!Cvet +++
		logging=1;

		LogExecStr("***   Finishing Game loop   ***\n\n");

		LogExecStr("Всего циклов:%d\n"
		"Средняя продолжительность цикла:%d\n"
		"Минимальная продолжительность цикла:%d\n"
		"Максимальная продолжительность цикла:%d\n"
		"Всего лагов (>100):%d\n",
		serv.loop_cycles,
		serv.loop_time/serv.loop_cycles,
		serv.loop_min,
		serv.loop_max,
		serv.lags_count);

		LogExecStr("Средняя продолжительность по процессам:\n"
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
	//!Cvet ---

		serv.Finish();

		LogFinish(); //!Cvet
		
	GAMELOOPEND:	
		hGameThread=NULL;
		ExitThread(0);
		return 0;
	}
#endif