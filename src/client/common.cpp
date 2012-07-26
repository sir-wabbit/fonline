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
char opt_rhost[1024]="";
char opt_name[MAX_NAME+1]="";
char opt_cases[5][MAX_NAME+1];
char opt_gender[10];

int opt_scroll_delay=0;
int opt_scroll_step=1;

int opt_mouse_speed=1;
char opt_masterpath[1024]="";
char opt_critterpath[1024]="";
char opt_fopath[1024]="";
char opt_crfol[1024]="";

int opt_sleep=0; //!Cvet
BOOL opt_msgbox_invert=FALSE; //!Cvet
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


void GetOptions()
{
	opt_fullscr=GetPrivateProfileInt("FEngine","fullscr",1,CFG_FILE);
	opt_vsync=GetPrivateProfileInt("FEngine","vsync",1,CFG_FILE);
	opt_dbgclear=GetPrivateProfileInt("FEngine","dbgclear",1,CFG_FILE);
	opt_light=GetPrivateProfileInt("FEngine","light",1,CFG_FILE);
	
	opt_scroll_delay=GetPrivateProfileInt("FEngine","scroll_delay",0,CFG_FILE);
	opt_scroll_step=GetPrivateProfileInt("FEngine","scroll_step",1,CFG_FILE);
	
	opt_mouse_speed=GetPrivateProfileInt("FEngine","mouse_speed",1,CFG_FILE);
	opt_text_delay=GetPrivateProfileInt("FEngine","text_delay",3000,CFG_FILE);
	opt_screen_mode=GetPrivateProfileInt("FEngine","screen_mode",0,CFG_FILE);
	if(opt_screen_mode>2 || opt_screen_mode<0) opt_screen_mode=0; //!Cvet
	
	opt_flushval=GetPrivateProfileInt("SprMan","flushval",256,CFG_FILE);
	opt_basetex=GetPrivateProfileInt("SprMan","basetex",256,CFG_FILE);

//!Cvet +++++++++++++++++++
//sleep
	opt_sleep=GetPrivateProfileInt("FEngine","sleep",0,CFG_FILE);
	if(opt_sleep<0 || opt_sleep>10) opt_sleep=0;

//msgbox_invert
	opt_msgbox_invert=GetPrivateProfileInt("Game","invert",FALSE,CFG_FILE);
	if(opt_msgbox_invert!=TRUE && opt_msgbox_invert!=FALSE) opt_msgbox_invert=FALSE;

//lang
	opt_change_lang=GetPrivateProfileInt("Game","lang_change",CHANGE_LANG_CTRL_SHIFT,CFG_FILE);
	if(opt_change_lang!=CHANGE_LANG_CTRL_SHIFT && opt_change_lang!=CHANGE_LANG_ALT_SHIFT && opt_change_lang!=CHANGE_LANG_RCTRL)
		opt_change_lang=CHANGE_LANG_CTRL_SHIFT;

//colors
	DWORD get_dword=0;

	get_dword=GetPrivateProfileInt("Game","color_game_default",0,CFG_FILE);
	if(get_dword) opt_gcolor_default=get_dword;

	get_dword=GetPrivateProfileInt("Game","color_crit_names",0,CFG_FILE);
	if(get_dword) opt_tcolor_namecrit=get_dword;

	get_dword=GetPrivateProfileInt("Game","color_text_default",0,CFG_FILE);
	if(get_dword) opt_tcolor_default=get_dword;
	get_dword=GetPrivateProfileInt("Game","color_text_shout",0,CFG_FILE);
	if(get_dword) opt_tcolor_shout=get_dword;
	get_dword=GetPrivateProfileInt("Game","color_text_whisp",0,CFG_FILE);
	if(get_dword) opt_tcolor_whisp=get_dword;
	get_dword=GetPrivateProfileInt("Game","color_text_emote",0,CFG_FILE);
	if(get_dword) opt_tcolor_emote=get_dword;
	get_dword=GetPrivateProfileInt("Game","color_text_social",0,CFG_FILE);
	if(get_dword) opt_tcolor_social=get_dword;

//alpha
	get_dword=GetPrivateProfileInt("Game","roof_alpha",0,CFG_FILE);
	if(get_dword) opt_roof_alpha=get_dword;
	
//!Cvet -------------------
	
	GetPrivateProfileString("system","master_dat","",opt_masterpath,1023,CFG_FILE);
	GetPrivateProfileString("system","critter_dat","",opt_critterpath,1023,CFG_FILE);
	GetPrivateProfileString("FileMan","fonline_dat","",opt_fopath,1023,CFG_FILE);

	GetPrivateProfileString("server","remote_host","localhost",opt_rhost,1023,CFG_FILE);
	opt_rport=GetPrivateProfileInt("server","remote_port",4000,CFG_FILE);
	GetPrivateProfileString("server","chosen_name","none",opt_name,MAX_NAME,CFG_FILE);
	char key[255];
	for(int i=0;i<5;i++)
	{
		sprintf(key,"case%d",i);
		GetPrivateProfileString("server",key,"none",opt_cases[i],MAX_NAME,CFG_FILE);
	}
	GetPrivateProfileString("server","chosen_gender","male",opt_gender,9,CFG_FILE);

	strcat(opt_crfol,opt_fopath);
	if(opt_crfol[strlen(opt_crfol)-1]!='\\') strcat(opt_crfol,"\\");
	strcat(opt_crfol,"art\\critters\\critters.fol");
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
    wvsprintf(str, frmt, list);
    va_end(list);

	DWORD br;
	if(!opt_fullscr) MessageBox(NULL,str,hdr,MB_OK|MB_ICONERROR);
		else 
		{
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