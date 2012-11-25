#include "stdafx.h" // инициализация большинства переменных

#include "common.h"

#include <Windows.h>
#include <dxerr8.h>
#include <tchar.h>
#include <assert.h>

VOID ToUTF16(LPCSTR lpText, LPWSTR lpWText, SIZE_T szWText) {
  size_t szText = lstrlenA(lpText);
  size_t wCaptionLen = MultiByteToWideChar(CP_UTF8, 0, lpText, szText, lpWText, szWText / sizeof(wchar_t) - 1);
  lpWText[wCaptionLen] = 0;
}

char* ToUTF8(LPCWSTR lpWText) {
  int iSize = WideCharToMultiByte(CP_UTF8, 0, lpWText, -1, NULL, 0, NULL, NULL);
  if (iSize == 0) {
    return NULL;
  }

  char* result = (char*) malloc(iSize + 1);

  iSize = WideCharToMultiByte(CP_UTF8, 0, lpWText, -1, result, iSize, NULL, NULL);
  if (iSize == 0) {
    return NULL;
  }

  return result;
}

int WINAPI MessageBoxU(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
  wchar_t wCaption[2048];
  wchar_t wText[2048];
  ToUTF16(lpCaption, wCaption, sizeof(wCaption));
  ToUTF16(lpText, wText, sizeof(wText));
  return MessageBoxW(hWnd, wText, wCaption, uType);
}

char* FormatStdCError(errno_t errorCode) {
  return strdup(strerror(errorCode));
}

char* FormatDirectXError(HRESULT errorCode) {
  LPCWSTR lpMsgBuf = DXGetErrorString8W(errorCode);
  
  char* result = ToUTF8(lpMsgBuf);

  if (result == NULL) {
    return strdup("Could not convert error message to UTF8.");
  }

  return result;
}

char* FormatWin32Error(DWORD errCode) {
  LPWSTR lpMsgBuf;

  BOOL bOk = FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    errCode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPWSTR) &lpMsgBuf,
    0,
    NULL
  );
  
  if (!bOk) {
    return strdup("Could not format error message.");
  }

  LPWSTR p = wcschr(lpMsgBuf, L'\r');
  if(p != NULL) {
    *p = L'\0';
  }

  char* result = ToUTF8(lpMsgBuf);
  
  if (result == NULL) {
    return strdup("Could not convert error message to UTF8.");
  }

  return result;
}

char* FormatLastWin32Error() {
  DWORD dwError = ::GetLastError();
  char* result = FormatWin32Error(dwError);
  ::SetLastError(dwError);
  return result;
}

bool cmn_Quit=0;
bool cmn_lost=0;

int cmn_scr_ox=0;
int cmn_scr_oy=0;

bool cmn_show_tiles=1;
bool cmn_show_roof=1; //!Cvet
bool cmn_show_items=1;
bool cmn_show_scen=1;
bool cmn_show_walls=1;
bool cmn_show_crit=1;
bool cmn_show_misc=1; //!Cvet

//holded keys
bool cmn_di_left=0;
bool cmn_di_right=0;
bool cmn_di_up=0;
bool cmn_di_down=0;

bool cmn_di_mleft=0;
bool cmn_di_mright=0;
bool cmn_di_mup=0;
bool cmn_di_mdown=0;

// console
bool cmn_console=0;
bool newplayer=0;
// interface bar
bool cmn_show_iface=1;
// для сообщений движка убиарет сообщения вверх
int move=1100;


int opt_fullscr=1;
int opt_vsync=1;
int opt_flushval=256;
int opt_basetex=256;
int opt_dbgclear=0;
int opt_light=0;

uint32_t opt_rport=0;
std::string opt_rhost="";
std::string opt_name="";
std::string opt_cases[5];
std::string opt_gender;

int opt_scroll_delay=0;
int opt_scroll_step=1;

int opt_mouse_speed=1;
std::string opt_masterpath;
std::string opt_critterpath;
std::string opt_fopath;
std::string opt_crfol;

int opt_sleep=0; //!Cvet
bool opt_msgbox_invert = false; //!Cvet
int opt_change_lang=CHANGE_LANG_CTRL_SHIFT; //!Cvet change language

//!Cvet colors +++++++++++++
uint32_t opt_gcolor_default=D3DCOLOR_ARGB(255,103,95,86);

uint32_t opt_tcolor_namecrit=0xFFFDFDB9;

uint32_t opt_tcolor_default=D3DCOLOR_ARGB(255,60,248,0);
uint32_t opt_tcolor_shout=0xFFFF0000;
uint32_t opt_tcolor_whisp=0xFF0000FF;
uint32_t opt_tcolor_emote=0xFFFF00FF;
uint32_t opt_tcolor_social=0xFFFFFFFF;

uint32_t opt_roof_alpha=0xC8;
//!Cvet -----------

int ac1,ac2,ac3,ac4,ac5,ac6,ac7,acres;

TICK opt_text_delay=3000;
uint32_t opt_screen_mode=0;

//!Cvet ++++++++++++++
int screen_width[]={800,1024,1280};
int screen_height[]={600,768,1024};
int view_width[]={27,35,45};
int view_height[]={55,68,90};
int view_cx[]={14,16,20};
int view_cy[]={26,34,46};
//!Cvet --------------

#include <IniFile/IniFile.hpp>

void LoadSettings()
{
  using namespace IniFile;

  RecordMap ini;
  bool result = LoadINI("data/FOnline.ini", ini);
  
  opt_fullscr = GetValue(ini, "FEngine.fullscr", 1);
  opt_vsync = GetValue(ini, "FEngine.vsync", 1);
	opt_dbgclear = GetValue(ini, "FEngine.dbgclear", 1);
	opt_light = GetValue(ini, "FEngine.light", 1);
	
	opt_scroll_delay = GetValue(ini, "FEngine.scroll_delay", 0);
	opt_scroll_step = GetValue(ini, "FEngine.scroll_step", 1);
	
	opt_mouse_speed = GetValue(ini, "FEngine.mouse_speed", 1);
	opt_text_delay = GetValue(ini, "FEngine.text_delay", 3000);
	opt_screen_mode = GetValue(ini, "FEngine.screen_mode", 0);
	if (opt_screen_mode > 2 || opt_screen_mode < 0) opt_screen_mode = 0; //!Cvet
	
	opt_flushval = GetValue(ini, "SprMan.flushval", 256);
	opt_basetex = GetValue(ini, "SprMan.basetex", 256);

	opt_sleep = GetValue(ini, "FEngine.sleep", 0);
	if (opt_sleep < 0 || opt_sleep > 10) opt_sleep = 0;

  //msgbox_invert
  opt_msgbox_invert = GetValue(ini, "Game.invert", false);

  //lang
	opt_change_lang = GetValue(ini, "Game.lang_change", CHANGE_LANG_CTRL_SHIFT);
	if (opt_change_lang != CHANGE_LANG_CTRL_SHIFT && opt_change_lang != CHANGE_LANG_ALT_SHIFT && opt_change_lang != CHANGE_LANG_RCTRL)
		opt_change_lang = CHANGE_LANG_CTRL_SHIFT;

//colors
	uint32_t get_dword=0;

	get_dword = GetValue(ini, "Game.color_game_default", 0);
	if (get_dword) opt_gcolor_default=get_dword;

	get_dword = GetValue(ini, "Game.color_crit_names", 0);
	if(get_dword) opt_tcolor_namecrit=get_dword;

  get_dword = GetValue(ini, "Game.color_text_default", 0);
  if(get_dword) opt_tcolor_default=get_dword;
  get_dword = GetValue(ini, "Game.color_text_shout", 0);
  if(get_dword) opt_tcolor_shout=get_dword;
  get_dword = GetValue(ini, "Game.color_text_whisp",0);
  if(get_dword) opt_tcolor_whisp=get_dword;
  get_dword = GetValue(ini, "Game.color_text_emote",0);
  if(get_dword) opt_tcolor_emote=get_dword;
  get_dword = GetValue(ini, "Game.color_text_social",0);
  if(get_dword) opt_tcolor_social=get_dword;

//alpha
	get_dword=GetValue(ini, "Game.roof_alpha",0);
	if(get_dword) opt_roof_alpha=get_dword;
	
//!Cvet -------------------
	
  opt_masterpath = GetValue<std::string>(ini, "system.master_dat","");
	opt_critterpath = GetValue<std::string>(ini, "system.critter_dat","");
	opt_fopath = GetValue<std::string>(ini, "FileMan.fonline_dat","");

	opt_rhost = GetValue<std::string>(ini, "server.remote_host","localhost");
	opt_rport = GetValue(ini, "server.remote_port", 4000);
	opt_name = GetValue<std::string>(ini, "server.chosen_name", "none");
	char key[255];
	for(int i=0;i<5;i++)
	{
		sprintf(key,"server.case%d",i);
		opt_cases[i] = GetValue<std::string>(ini, key, "none");
	}
	opt_gender = GetValue<std::string>(ini, "server.chosen_gender","male");

  opt_crfol = opt_fopath;
  if (opt_crfol != "" && opt_crfol[opt_crfol.size() - 1] != '\\') {
    opt_crfol += "\\";
  }
	opt_crfol += "art\\critters\\critters.fol";
}

FILE* logFile = NULL;

int OpenLogFile() {
  logFile = fopen("FOnline.log", "w+b");
  
  if (logFile == NULL) {
    return 0;
  }
  
	return 1;
}

void CloseLogFile() {
	if (logFile != NULL) {
	  fclose(logFile);
	  logFile = NULL;
	}
}

void WriteLog(char* fmt, ...) {
	if (logFile == NULL) {
	  return;
	}
	
	char buf[2048];

  va_list list;

  va_start(list, fmt);
  vsnprintf(buf, sizeof(buf) - 1, fmt, list);
  va_end(list);

  fprintf(logFile, "%s", buf);
  fflush(logFile);
}

void ReportErrorMessage(char* hdr, char* fmt, ...) {
	char buf[2048];

  va_list list;

  va_start(list, fmt);
  vsnprintf(buf, sizeof(buf) - 1, fmt, list);
  va_end(list);

	if(!opt_fullscr) {
	  MessageBoxU(NULL, buf, hdr, MB_OK|MB_ICONERROR);	
	} else {
		WriteLog("\n\nErrMsg> %s", buf);
	}
}