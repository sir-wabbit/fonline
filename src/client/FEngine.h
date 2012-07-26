#ifndef _FENGINE_H_
#define _FENGINE_H_

#include "CSpriteManager.h"
#include "SoundMngr.h"
#include "CHexField.h"
#include "Critter.h"
#include "CFont.h"
#include "netproto.h"

#include "BufMngr.h"
/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/
#define DI_BUF_SIZE 64

#define DI_ONDOWN(a,b) if((didod[ i ].dwOfs==a) && (didod[ i ].dwData & 0x80)) {b;}
#define DI_ONUP(a,b) if((didod[ i ].dwOfs==a) && !(didod[ i ].dwData & 0x80)) {b;}
#define DI_ONMOUSE(a,b) if(didod[ i ].dwOfs==a) {b;}

#define EDIT_LEN 94

#define MAX_MESS 500
#define MAX_MESS_IN_MESSBOX 15
#define MAX_MESSBOX EDIT_LEN*MAX_MESS_IN_MESSBOX

#define MAX_DIALOG_TEXT 4000

#define MAX_OBJECT_NAME 50
#define MAX_OBJECT_INFO 500

//!Cvet режимы экрана
const BYTE SCREEN_LOGIN			=0;
const BYTE SCREEN_REGISTRATION	=1;
const BYTE SCREEN_MAIN			=2;
const BYTE SCREEN_INVENTORY		=3;
const BYTE SCREEN_LOCAL_MAP		=4;
const BYTE SCREEN_GLOBAL_MAP	=5;
const BYTE SCREEN_DIALOG_NPC	=6;
const BYTE SCREEN_PIP_BOY		=7;
const BYTE SCREEN_SKILLBOX		=8;
const BYTE SCREEN_MENU_OPTION	=9;

//!Cvet режимы курсора
const BYTE CUR_DEFAULT			=0;
const BYTE CUR_MOVE				=1;
const BYTE CUR_USE_OBJECT		=2;
const BYTE CUR_USE_SKILL		=3;
const BYTE CUR_WAIT				=4;

//!Cvet LMenu
const DWORD LMENU_SHOW_TIME		=400;
//режимы
const BYTE LMENU_OFF			=0;
const BYTE LMENU_PLAYER			=1;
const BYTE LMENU_NPC			=2;
const BYTE LMENU_ITEM			=4;
const BYTE LMENU_SCENERY		=5;
//ячейки для lmenu
typedef vector<BYTE> LMenu_list;
const BYTE LMENU_NODE_LOOK		=0;
const BYTE LMENU_NODE_TALK		=1;
const BYTE LMENU_NODE_BREAK		=2;
const BYTE LMENU_NODE_PICK		=3;
const BYTE LMENU_NODE_GMFOLLOW	=4;
const BYTE LMENU_NODE_GMTAKE	=5;

//!Cvet действия чезена
typedef vector<BYTE> chosen_action_vec;
const BYTE ACTION_NONE					=0;
const BYTE ACTION_MOVE					=1;
const BYTE ACTION_SHOW_OBJECT			=2;
const BYTE ACTION_HIDE_OBJECT			=3;
const BYTE ACTION_ACT_UNACT_OBJECT		=4;
const BYTE ACTION_USE_OBJ_ON_CRITTER	=5;
const BYTE ACTION_USE_OBJ_ON_ITEM		=6;
const BYTE ACTION_USE_SKL_ON_CRITTER	=7;
const BYTE ACTION_USE_SKL_ON_ITEM		=8;
const BYTE ACTION_TALK_NPC				=9;
const BYTE ACTION_PICK_OBJ				=10;

// Анимация обычная набор спрайтов
struct AnyAnimData 
{
		AnyFrames* eng; // Анимация берем оружие в руки
		AnyAnimData():eng(NULL){};
		~AnyAnimData(){SAFEDEL(eng)};
};

typedef map<WORD, string, less<WORD> > string_map; //!Cvet

class CFEngine
{
public:
	int Init(HWND _hWnd);
	void Clear();
	void ClearCritters(); //!Cvet
	void RemoveCritter(CrID remid); //!Cvet
	void Restore();
	void RestoreDI();

	void DoLost(); //!Cvet

	void TryExit(); //!Cvet
    
	int Render();

	int Console();// консоль игрока

	void NetDiscon();
	WORD state;

	CFEngine();
private:
	bool crtd;
	bool islost;
	bool dilost;

	bool CtrlDwn;
	bool AltDwn; //!Cvet
	bool ShiftDwn;
	bool edit_mode;

	WORD cur_hold;
//!Cvet +++++++++++++++++++++++++++++++
	void GetRandomSplash(char* splash_name);

	BYTE screen_mode;
	void SetScreen(BYTE new_screen);
	void SetScreenCastling(BYTE mode1, BYTE mode2){ if		(screen_mode==mode1) screen_mode=mode2; 
													else  if(screen_mode==mode2) screen_mode=mode1; }
	BOOL IsScreen(BYTE check_screen){ if(check_screen==screen_mode) return 1; return 0; };

	BYTE cur_mode;
	void SetCur(BYTE new_cur);
	void SetCurCastling(BYTE cur1, BYTE cur2){	if	   (cur_mode==cur1) cur_mode=cur2; 
												else if(cur_mode==cur2) cur_mode=cur1; }
	BOOL IsCur(BYTE check_cur){ if(check_cur==cur_mode) return 1; return 0; };
//!Cvet -------------------------------
	BYTE lang;
	char ed_str[EDIT_LEN+1];
	int cur_edit; //!Cvet

	HWND hWnd;

	LPDIRECT3D8 lpD3D;
	LPDIRECT3DDEVICE8 lpDevice;

	LPDIRECTINPUT8 lpDInput;
	LPDIRECTINPUTDEVICE8 lpKeyboard;
	LPDIRECTINPUTDEVICE8 lpMouse;

	CSpriteManager sm;
	CHexField hf;
	CFOFont fnt;
	CSoundMngr sdm;

	WORD cur_def,cur_move,cur_move_block,cur_hand,cur_use_o,cur_use_s,cur_wait; //!Cvet указатели

	WORD splash,cur,cur_right,cur_left,cur_up,cur_down,cur_ru,cur_lu,cur_rd,cur_ld;
	int cur_x,cur_y;

	int InitDInput();

//	char* InpBuf;
//	DWORD inlen;
//	DWORD inpos;
//	char* OutBuf;
//	DWORD outlen;
//	DWORD outpos;
	char* ComBuf;
	DWORD comlen;
	DWORD compos;

	CBufMngr bin;
	CBufMngr bout;
	z_stream zstrm;
	bool zstrmok;

	DWORD stat_com,stat_decom;

	SOCKADDR_IN remote;
	SOCKET sock;
	fd_set read_set,write_set,exc_set;
	int InitNet();
	int NetCon();
	void ParseSocket(WORD wait=0);
	int NetInput();
	int NetOutput();
	void NetProcess();
//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void Net_SendLogIn(char* login, char* pass);
	void Net_SendCreatePlayer(crit_info* newcr);

	void Net_SendUseSkill(BYTE skill, DWORD targ_id, BYTE ori);
	void Net_SendUseObject(BYTE type_target, DWORD targ_id, BYTE crit_ori, BYTE crit_num_action, BYTE crit_rate_object);
	void Net_SendPickObject(HexTYPE targ_x, HexTYPE targ_y, WORD id_sobj);
	void Net_SendChangeObject(DWORD idobj, BYTE num_slot);
	void Net_SendTalk(CrID id_to_talk, BYTE answer);
	void Net_SendGetTime();
	void Net_SendGiveGlobalInfo(BYTE info_flags);
	void Net_SendRuleGlobal(BYTE command, DWORD param1=0, DWORD param2=0);
	void Net_SendGiveMeMap(WORD map_num);
	void Net_SendLoadMapOK();
//!Cvet -------------------------------------------------------------
	void Net_SendText(char* str);
	void Net_SendDir(); //Cvet переделал
    void Net_SendMove(BYTE* dir); //!Cvet переделал

	void Net_OnAddCritter();
	void Net_OnRemoveCritter();
	void Net_OnCritterText(); //!Cvet edit
//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void Net_OnAddObjOnMap();
	void Net_OnChangeObjOnMap();
	void Net_OnRemObjFromMap();

	void Net_OnCritterDir();
    void Net_OnCritterMove(); //движение
	void Net_OnCritterAction(); //действие

	void Net_OnChosenXY(); //помещение чузена в новую клетку
	void Net_OnChosenParams(); //передача параметров игроку
	void Net_OnChosenParam(); //передача параметра
	void Net_OnChosenLogin(); //сетевые сообщения
	void Net_OnChosenAddObject(); //добавление объекта
	void Net_OnChosenTalk();

	void Net_OnGameTime();

	void Net_OnLoadMap();
	void Net_OnMap();
	void Net_OnGlobalInfo();

	HexTYPE TargetX;
	HexTYPE TargetY;
	int GetMouseTile(int cursor_x, int cursor_y);

	CCritter* GetMouseCritter(int cursor_x, int cursor_y);
	int GetMouseScenery(int cursor_x, int cursor_y);
	ItemObj* GetMouseItem(int cursor_x, int cursor_y);

	TICK Ping;
	WORD Game_FPS;

	char opt_login[MAX_LOGIN+1];
	char opt_pass[MAX_LOGIN+1];

//Параметры
	void CreateParamsMaps();
	params_map stats_map;
	params_map skills_map;
	params_map perks_map;
	params_map object_map;
	void CreateStringsParamsMaps();
	params_str_map stats_str_map;
	params_str_map skills_str_map;
	params_str_map perks_str_map;
	//params_str_map object_map;

//Объекты в игре
	stat_map all_s_obj; //вся статика в игре
	//имена объектов
	string_map name_obj;
	//инфо объектов
	string_map info_obj;
	//картинки в инвентаре
	WORD inv_pic_b[367]; //большие
	WORD inv_pic_s[367]; //малые
	//картинки использования
	WORD pic_use[100];

//Управление чузеном
	chosen_action_vec chosen_action;

	HexTYPE PathMoveX;
	HexTYPE PathMoveY;
	CCritter* curTargetCrit;
	CCritter* tosendTargetCrit;
	ItemObj* curTargetObj;
	ItemObj* tosendTargetObj;

	void SetChosenAction(BYTE set_action){ chosen_action.clear(); chosen_action.push_back(set_action); };
	void AddChosenAction(BYTE add_action){ chosen_action.push_back(add_action); };
	void EraseFrontChosenAction(){chosen_action.erase(chosen_action.begin());if(!chosen_action.size()) chosen_action.push_back(ACTION_NONE);};
	void EraseBackChosenAction(){chosen_action.pop_back();if(!chosen_action.size()) chosen_action.push_back(ACTION_NONE);};
	int IsChosenAction(BYTE check_action){ if(*chosen_action.begin()==check_action) return 1; return 0; };

	void ChosenProcess();

//ИНТЕРФЕЙС=======================================================================
	int Init_Iface();

	int numIface; //номер анимации интерфейса

//Инвентарь=======================================================================
	WORD invbox,invokon,invokoff,invscrupin,invscrupout,invscrupoff,invscrdwin,invscrdwout,invscrdwoff; //анимация

	BYTE InvHold; //номер зажатой кнопки - нмера внизу //0 - кнопка не зажата
	int scroll_items;

	int InvX,InvY; //х,у основного меню окна, все остальное будет отталкиваться именно от этих координат

	INTRECT InvMain,InvObl,InvChosen;
	int HeightItem;// = 30; //высота картинки в инвенторе, всего там 10
	//слот1,2, армор
	INTRECT InvSlot1,InvSlot2,InvArmor;
	//кнопки
	INTRECT InvBtnUp,InvBtnDown,InvBtnOk;
	//точки вывода инфы
	INTRECT txtObject;
	//это нужно для перетаскивания окна инвентари
	int invvectx,invvecty;

	void InvDraw(); 
	void InvMouseMove();
	int InvMouseDown();
	int InvMouseUp();

//Основной интерфейс==============================================================
	WORD ifacen,panel,intscrupon,intscrdownon,intchangesloton,
	intinvon,intmenuon,intskillon,intmapon,intinfoon,intpipon;
	WORD diodeG,diodeY,diodeR;

	BYTE IntHold; //номер зажатой кнопки - нмера внизу //0 - кнопка не зажата
	int IntX,IntY; //х,у основного меню окна, все остальное будет отталкиваться именно от этих координат

	INTRECT IntMain; 
	//слоты
	INTRECT IntObject; //слот объекта
	//кнопки
	INTRECT IntBScrUp,IntBScrDown,IntBChangeSlot,IntBInv,IntBMenu,IntBSkill,IntBMap,IntBInfo,IntBPip;
	//точки вывода инфы
	INTRECT IntTXT;
	//action points //15 зеленых(200мс) 3 желтых(1000мс) 2 красных(10000мс)
	INTRECT IntAP,IntHP,IntAC;

	int IntAPstepX,IntAPstepY;

	void IntDraw();
	int IntMouseDown();
	int IntMouseUp();
	void IntMouseMove();

	//мессаги о сотоянии криттера
	WORD chosen_mess_pic;
	int IntMessX,IntMessY;
	int IntMessStepX,IntMessStepY;
	INTRECT IntMess;

//Меню левой кнопки (LMenu)=======================================================
	WORD lm_talk_off,lm_talk_on,lm_look_off,lm_look_on,lm_break_off,lm_break_on,
		lm_use_off,lm_use_on,lm_gmfollow_off,lm_gmfollow_on,lm_gmtake_off,lm_gmtake_on;

	bool LMenu_try_activated;
	DWORD LMenu_start_time;
	int LMenu_cur_x,LMenu_cur_y;
	int LMenu_x,LMenu_y;
	int LMenu_node_height;

	LMenu_list* LMenu_nodes;
	int LMenu_cur_node;

	WORD Node_stay_mouse;

	LMenu_list LMenu_crit_nodes,LMenu_scenery_nodes,LMenu_item_nodes;

	BYTE LMenu_mode;

	void SetLMenu(BYTE set_lmenu);
	BOOL IsLMenu() { if(LMenu_mode!=LMENU_OFF) return 1; return 0; };
	void LMenuTryCreate();
	void LMenuDraw();
	void LMenuMouseMove();
	void LMenuMouseUp();

//Логин/пасс====================================================================
	WORD loginpic;

	BYTE LogFocus; //позиция курсора 0-нигде 1-login 2-pass
	BYTE LogMsg; //номер сообщения

	int LogX,LogY;

	INTRECT LogMain,LogWLogin,LogWPass,LogBOk,LogBReg,LogBExit;

	void ShowLogIn();
	void LogInput();
	
//Регистрация===================================================================
	crit_info New_cr;

	WORD registpic;

	BYTE RegFocus;

	int RegX,RegY;

	INTRECT RegMain,RegWS,RegWP,RegWE,RegWC,RegWI,RegWA,RegWL,RegWLogin,RegWPass,RegWName,
		RegWCases0,RegWCases1,RegWCases2,RegWCases3,RegWCases4,RegWBType,RegWGender,RegWAge,RegBReg,RegBBack;
	
	int CheckRegData(crit_info* newcr);

	void ShowRegistration();
	void RegInput();

//Диалог========================================================================
	WORD dialog_begin,dialog_answ,dialog_end;

	BYTE DlgHold;

	int DlgNextAnswX,DlgNextAnswY;
	int DlgCurAnsw;
	BYTE all_answers;

	int dlgvectx,dlgvecty;

	char text_dialog[MAX_DIALOG_TEXT+1];
	char text_answer[MAX_ANSWERS][MAX_DIALOG_TEXT+1];

	int DlgX,DlgY;

	INTRECT DlgMain,DlgBegin,DlgEnd,DlgText,DlgAnsw;

	void DlgDraw();

	void DlgMouseMove();
	void DlgMouseDown();
	void DlgMouseUp();

	int LoadDialogFromFile(CrID id_npc, DWORD id_dialog, char* dialog);

//Мини-карта=====================================================================
	WORD lmap_main,lmap_bok_on,lmap_bscan_on,lmap_blohi_on,lmap_pix;

	Pix_vec lmap_prep_pix;

	INTRECT LmapMain,LmapWMap,LmapBOk,LmapBScan,LmapBLoHi;

	short LmapX,LmapY;
	int lmapvectx,lmapvecty;
	int LmapHold;

	short lmap_zoom;
	bool switch_HI;

	void LmapPrepareMap();
	void LmapDraw();
	void LmapMouseDown();
	void LmapMouseMove();
	void LmapMouseUp();

//Глобалмапа======================================================================
	WORD gm_pic[5][6];
	WORD gm_lock_contr_pic,gm_bstop_on,gm_bspeed0_on,gm_bspeed1_on,gm_bspeed2_on,
		gm_btolocal_on,gm_iface_pic,gm_iface_pic2,gm_gr_loc_pic,gm_gr_targ_pic;
	WORD gm_loc48_pic,gm_loc24_pic,gm_loc12_pic;

	int GmapX,GmapY,GmapMapScrX,GmapMapScrY,GmapWNameStepX,GmapWNameStepY,
		GmapPLockContrX,GmapPLockContrY,GmapHold;

	INTRECT GmapMain,GmapWMap,GmapBToLocal,GmapBStop,GmapBSpeed0,GmapBSpeed1,GmapBSpeed2,
		GmapWName,GmapWChat,GmapWPanel,GmapWGroupInfo;

	int gmapvectx,gmapvecty;
	bool GmapMapScrollEnable;

	void GmapNullParams();
	void GmapProcess();

	bool gm_process;
	bool lock_group_contr;

	//cities
	struct GM_city
	{
		WORD num;
		WORD x;
		WORD y;
		BYTE radius;

		GM_city():num(0),x(0),y(0),radius(0){};
		GM_city(WORD _num,WORD _x,WORD _y,BYTE _radius):num(_num),x(_x),y(_y),radius(_radius){};
	};

	vector<GM_city*> gm_cities;

	//params
	float GmapGroupXf,GmapGroupYf;
	DWORD gm_last_tick;
	int GmapGroupX,GmapGroupY,GmapMoveX,GmapMoveY;
	float GmapSpeedX,GmapSpeedY;
	BYTE GmapSpeed;

	//crits
	WORD gm_cr_online,gm_cr_offline,gm_cr_npc;

	struct GM_crit
	{
		CrID crid;
		char name[MAX_NAME+1];

		bool chosen;

		bool player;
		bool npc;
		bool mob;

		bool rule;
		bool disconnect;

		bool ignore;

		GM_crit():crid(0),chosen(false),player(false),npc(false),mob(false),rule(false),disconnect(false),ignore(false)
		{name[0]=NULL;};
	};
	vector<GM_crit*> gm_crits;

	//tabs
	WORD gm_wtab_pic,gm_wblanktab_pic,gm_btabloc_picdn,gm_tabs_scrup_picdn,gm_tabs_scrdn_picdn;
	WORD gm_locpic[10];

	INTRECT GmapWTabs,GmapWTabLoc,GmapBTabLoc,GmapBTabsScrUp,GmapBTabsScrDn;
	int GmapTabW,GmapTabH,GmapTabNextX,GmapTabNextY,GmapCurHoldBLoc;

	int gm_tab_scr_x,gm_tab_scr_y;
	DWORD last_scr;


	void GmapDraw();
	void GmapInput();
	void GmapMouseDown();
	void GmapMouseUp();
	void GmapMouseMove();

//Skillbox========================================================================
	WORD sbox_main,sbox_bcancel_on,sbox_bsneak_on,sbox_blockpick_on,sbox_bsteal_on,
		sbox_btrap_on,sbox_bfirstaid_on,sbox_bdoctor_on,sbox_bscience_on,sbox_brepair_on;

	INTRECT SboxMain,SboxBCancel,SboxBSneak,SboxBLockpick,SboxBSteal,
		SboxBTrap,SboxBFirstAid,SboxBDoctor,SboxBScience,SboxBRepair;

	INTRECT SboxTSneak,SboxTLockpick,SboxTSteal,SboxTTrap,SboxTFirstAid,
		SboxTDoctor,SboxTScience,SboxTRepair;

	int SboxX,SboxY;
	int sboxvectx,sboxvecty;
	int SboxHold;

	void SboxDraw();
	void SboxMouseDown();
	void SboxMouseMove();
	void SboxMouseUp();

//Menu Options
	WORD mopt_main,mopt_bresume_on,mopt_bexit_on;

	int MoptX,MoptY;
	int MoptHold;

	INTRECT MoptMain,MoptBResume,MoptBExit;

	void MoptDraw();
	void MoptMouseDown();
	void MoptMouseUp();

//PlayerInfo======================================================================

//PipBoy==========================================================================

//Cursor==========================================================================
	void CurDraw();

//================================================================================

//Игровое время
	WORD Game_Hours,Game_Mins,Game_Time,Game_Year;
	BYTE Game_Day,Game_Month;

	DWORD Game_TimeTick;
	DWORD Game_CurTime;

	void ProccessDayTime();
	void SetDayTime(DWORD time_ms);

	BYTE dayR,dayG,dayB;
//!Cvet ---------------------------------------------------------------------------------------

	void ParseInput();

	void SetColor(BYTE r,BYTE g,BYTE b);
	void SetColor(DWORD color); //!Cvet

	crit_map critters;
	CCritter* AddCritter(crit_info* pinfo);

	CCritter* lpChosen;

	// для отладки
	CrID LstMoveId;
    CrID LstAddCritId;
	CrID LstDelCritId;
	CrID LstSayCritId;
	char newbie[31];

//!Cvet ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	char mess_str[EDIT_LEN+1]; // строка для окна сообщений расширяемая
	char all_mess[300][MAX_TEXT+128];
	BYTE max_mess,scr_mess;

	void AddMess(DWORD text_color,char* message_text, ...);
	void MessBoxDrawText();
	void MessBoxInput();

//	char mess_str[EDIT_LEN+1]; // строка для окна сообщений расширяемая

//Работа со строками
	void StrChangeValue(char* str, int value){for(int i=0;str[i];++i)str[i]+=value;};
//!Cvet ------------------------------------------------------
};


#endif