#include <Windows.h>

#include "common.h"
#include "FEngine.h"

#include "resource.h"

//#include <SimpleLeakDetector/SimpleLeakDetector.hpp>

/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

LRESULT APIENTRY WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
FOnlineEngine* engine;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInst, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wndClass;
	
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "FOnline";
	
	if (!RegisterClass(&wndClass)) {
	  char* msg = FormatLastWin32Error();
	  ReportErrorMessage("Win32 error.", "Could not register window class: %s", msg);
	  free(msg);
	  return -1;
	}
	
	if (!OpenLogFile("FOnlineClient.log")) {
	  ReportErrorMessage("IO error.", "Could not open log file.");
		return -1;
	}

	LoadSettings();

  size_t modeWidth = screen_width[opt_screen_mode];
  size_t modeHeight = screen_height[opt_screen_mode];

	HWND hWnd = CreateWindow(
		"FOnline",
		"Fallout Online",
		WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_SIZEBOX) & (~WS_SYSMENU),
		0,0,modeWidth + 5, modeHeight + 25,
		NULL,
		NULL,
		hInstance,
		NULL
  );
	
	if (hWnd == NULL) {
    char* msg = FormatLastWin32Error();
    ReportErrorMessage("Win32 error.", "Could not create window: %s", msg);
    free(msg);
    return -1;
  }

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	WriteLog("Starting FOnline...\n");

	srand(GetTickCount());

	engine = new FOnlineEngine;

	if (!engine->Init(hWnd)) {
		WriteLog("Could not initialize the engine.\n");
		DestroyWindow(hWnd);
		return 0;
	}
	
	MSG msg;
	while(!cmn_Quit) {
		if(!cmn_lost) {
			if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				engine->Render();

				if (opt_sleep) Sleep(opt_sleep);
			}
		} else {
			GetMessage(&msg, NULL, NULL, NULL);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	WriteLog("\nFOnline Closed\n");
	CloseLogFile();

	delete engine;
	
  //SimpleLeakDetector::PrintAllLeaks();

	_CrtDumpMemoryLeaks();
	return 0;
}

LRESULT APIENTRY WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
	case WM_DESTROY://Вызывается при разрушении окна
		if(engine) engine->Clear();
		cmn_Quit=1;
		return 0;
	case WM_KEYDOWN:
		switch(wParam) {
		case VK_ESCAPE: //!Cvet переделал
			engine->TryExit();
			break;
		case VK_F12: //!Cvet сворачивание
			ShowWindow(hWnd, SW_MINIMIZE);
			engine->DoLost();
			break;
		}
		return 0;
	case WM_ACTIVATE:
		if(LOWORD(wParam) == WA_INACTIVE)// && HIWORD(wParam))
		{
			engine->DoLost();
		}
		if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
			if(engine) {
			  engine->Restore();
			  engine->RestoreDI();
			}
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}