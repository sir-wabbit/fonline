#include "stdafx.h" // инициализация большинства переменных

#include "common.h"

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

DWORD opt_rport=0;
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
DWORD opt_gcolor_default=D3DCOLOR_ARGB(255,103,95,86);

DWORD opt_tcolor_namecrit=0xFFFDFDB9;

DWORD opt_tcolor_default=D3DCOLOR_ARGB(255,60,248,0);
DWORD opt_tcolor_shout=0xFFFF0000;
DWORD opt_tcolor_whisp=0xFF0000FF;
DWORD opt_tcolor_emote=0xFFFF00FF;
DWORD opt_tcolor_social=0xFFFFFFFF;

DWORD opt_roof_alpha=0xC8;
//!Cvet -----------

int ac1,ac2,ac3,ac4,ac5,ac6,ac7,acres;

TICK opt_text_delay=3000;
DWORD opt_screen_mode=0;

//!Cvet ++++++++++++++
int screen_width[]={800,1024,1280};
int screen_height[]={600,768,1024};
int view_width[]={27,35,45};
int view_height[]={55,68,90};
int view_cx[]={14,16,20};
int view_cy[]={26,34,46};
//!Cvet --------------

#include <base/ini_file.hpp>

void GetOptions()
{
  using namespace fonline::ini;

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
	DWORD get_dword=0;

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



HANDLE hLogFile=NULL;

int StartLogFile()
{
	hLogFile=CreateFile("FOnline.log",GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,NULL);
	if(hLogFile==INVALID_HANDLE_VALUE) return 0;

	return 1;
}

void CloseLogFile()
{
	if(hLogFile) CloseHandle(hLogFile);
}

void WriteLog(char* frmt, ...)
{
	if(!hLogFile) return;
	
	char str[2048];

    va_list list;

    va_start(list, frmt);
    wvsprintf(str, frmt, list);
    va_end(list);

	DWORD br;
	WriteFile(hLogFile,str,strlen(str),&br,NULL);
}

void ErrMsg(char* hdr, char* frmt, ...)
{
	char str[2048];

  va_list list;

  va_start(list, frmt);
  size_t len = vsprintf(str, frmt, list);
  va_end(list);

	DWORD br;
	if(!opt_fullscr) {
    wchar_t wbuf[2048];
    size_t wlen = MultiByteToWideChar(CP_UTF8, 0, str, len, wbuf, sizeof(wbuf) / sizeof(wchar_t) - 1);
    wbuf[wlen] = 0;
    
    wchar_t whdr[2048];
    size_t whdrlen = MultiByteToWideChar(CP_UTF8, 0, hdr, strlen(hdr), whdr, sizeof(whdr) / sizeof(wchar_t) - 1);
    whdr[whdrlen] = 0;
    
	  MessageBoxW(NULL, wbuf, whdr, MB_OK|MB_ICONERROR);
	
	} else {
		char str2[1024]="\n\nErrMsg> ";
		strcat(str2,str);
		strcat(str2,"\n");
		WriteFile(hLogFile,str2,strlen(str2),&br,NULL);
	}
}

void InfoMsg(char* frmt, ...)
{
	char str[2048];

    va_list list;

    va_start(list, frmt);
    wvsprintf(str, frmt, list);
    va_end(list);

	MessageBox(NULL,str,"FOnline Info",MB_OK|MB_ICONINFORMATION);
}