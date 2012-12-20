#ifndef _FENGINE_H_
#define _FENGINE_H_

#include <zlib.h>

#include "CSpriteManager.h"
#include "SoundMngr.h"
#include "CHexField.h"
#include "Critter.h"
#include "CFont.h"
#include "netproto.h"

#include <FOnlineCommon/buffer.hpp>
#include <FOnlineCommon/math/rect.hpp>

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
const uint8_t SCREEN_LOGIN			=0;
const uint8_t SCREEN_REGISTRATION	=1;
const uint8_t SCREEN_MAIN			=2;
const uint8_t SCREEN_INVENTORY		=3;
const uint8_t SCREEN_LOCAL_MAP		=4;
const uint8_t SCREEN_GLOBAL_MAP	=5;
const uint8_t SCREEN_DIALOG_NPC	=6;
const uint8_t SCREEN_PIP_BOY		=7;
const uint8_t SCREEN_SKILLBOX		=8;
const uint8_t SCREEN_MENU_OPTION	=9;

//!Cvet режимы курсора
const uint8_t CUR_DEFAULT			=0;
const uint8_t CUR_MOVE				=1;
const uint8_t CUR_USE_OBJECT		=2;
const uint8_t CUR_USE_SKILL		=3;
const uint8_t CUR_WAIT				=4;

//!Cvet LMenu
const uint32_t LMENU_SHOW_TIME		=400;
//режимы
const uint8_t LMENU_OFF			=0;
const uint8_t LMENU_PLAYER			=1;
const uint8_t LMENU_NPC			=2;
const uint8_t LMENU_ITEM			=4;
const uint8_t LMENU_SCENERY		=5;
//ячейки для lmenu
typedef std::vector<uint8_t> LMenu_list;
const uint8_t LMENU_NODE_LOOK		=0;
const uint8_t LMENU_NODE_TALK		=1;
const uint8_t LMENU_NODE_BREAK		=2;
const uint8_t LMENU_NODE_PICK		=3;
const uint8_t LMENU_NODE_GMFOLLOW	=4;
const uint8_t LMENU_NODE_GMTAKE	=5;

//!Cvet действия чезена
typedef std::vector<uint8_t> chosen_action_vec;
const uint8_t ACTION_NONE					=0;
const uint8_t ACTION_MOVE					=1;
const uint8_t ACTION_SHOW_OBJECT			=2;
const uint8_t ACTION_HIDE_OBJECT			=3;
const uint8_t ACTION_ACT_UNACT_OBJECT		=4;
const uint8_t ACTION_USE_OBJ_ON_CRITTER	=5;
const uint8_t ACTION_USE_OBJ_ON_ITEM		=6;
const uint8_t ACTION_USE_SKL_ON_CRITTER	=7;
const uint8_t ACTION_USE_SKL_ON_ITEM		=8;
const uint8_t ACTION_TALK_NPC				=9;
const uint8_t ACTION_PICK_OBJ				=10;

// Анимация обычная набор спрайтов
struct AnyAnimData 
{
		AnyFrames* eng; // Анимация берем оружие в руки
		AnyAnimData():eng(NULL){};
		~AnyAnimData() {
      if (eng != NULL) {
        delete eng;
        eng = NULL;
      }
		}
};

typedef std::map<uint16_t, std::string> string_map; //!Cvet

class FOnlineEngine
{
public:
	int Init(HWND _hWnd);
	void Clear();
	void ClearCritters(); //!Cvet
	void RemoveCritter(CritterID remid); //!Cvet
	void Restore();
	void RestoreDI();

	void DoLost(); //!Cvet

	void TryExit(); //!Cvet
    
	int Render();

	int Console();// консоль игрока

	void NetDiscon();
	uint16_t state;

	FOnlineEngine();
private:
	bool initialized;
	bool islost;
	bool dilost;

	bool CtrlDwn;
	bool AltDwn; //!Cvet
	bool ShiftDwn;
	bool edit_mode;

	uint16_t cur_hold;
//!Cvet +++++++++++++++++++++++++++++++
	void GetRandomSplash(char* splash_name);

	uint8_t screen_mode;
	void SetScreen(uint8_t new_screen);
	void SetScreenCastling(uint8_t mode1, uint8_t mode2){ if		(screen_mode==mode1) screen_mode=mode2; 
													else  if(screen_mode==mode2) screen_mode=mode1; }
	BOOL IsScreen(uint8_t check_screen){ if(check_screen==screen_mode) return 1; return 0; };

	uint8_t cur_mode;
	void SetCur(uint8_t new_cur);
	void SetCurCastling(uint8_t cur1, uint8_t cur2){	if	   (cur_mode==cur1) cur_mode=cur2; 
												else if(cur_mode==cur2) cur_mode=cur1; }
	BOOL IsCur(uint8_t check_cur){ if(check_cur==cur_mode) return 1; return 0; };
//!Cvet -------------------------------
	uint8_t lang;
	char ed_str[EDIT_LEN+1];
	int cur_edit; //!Cvet

	HWND hWnd;

	LPDIRECT3D8 lpD3D;
	LPDIRECT3DDEVICE8 lpDevice;

	LPDIRECTINPUT8 lpDInput;
	LPDIRECTINPUTDEVICE8 lpKeyboard;
	LPDIRECTINPUTDEVICE8 lpMouse;

	CSpriteManager spriteManager;
	CHexField hexField;
	CFOFont fnt;
	SoundManager soundManager;

	uint16_t cur_def,cur_move,cur_move_block,cur_hand,cur_use_o,cur_use_s,cur_wait; //!Cvet указатели

	uint16_t splash,cur,cur_right,cur_left,cur_up,cur_down,cur_ru,cur_lu,cur_rd,cur_ld;
	int cur_x,cur_y;

	int InitDirectInput();

//	char* InpBuf;
//	uint32_t inlen;
//	uint32_t inpos;
//	char* OutBuf;
//	uint32_t outlen;
//	uint32_t outpos;
	char* ComBuf;
	uint32_t comlen;
	uint32_t compos;

	fonline::Buffer bin;
	fonline::Buffer bout;
	z_stream zstrm;
	bool zstrmok;

	uint32_t stat_com,stat_decom;

	SOCKADDR_IN remote;
	SOCKET sock;
	fd_set read_set,write_set,exc_set;
	int InitNet();
	int NetCon();
	void ParseSocket(uint16_t wait=0);
	int NetInput();
	int NetOutput();
	void NetProcess();
//!Cvet +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void Net_SendLogIn(char* login, char* pass);
	void Net_SendCreatePlayer(crit_info* newcr);

	void Net_SendUseSkill(uint8_t skill, uint32_t targ_id, uint8_t ori);
	void Net_SendUseObject(uint8_t type_target, uint32_t targ_id, uint8_t crit_ori, uint8_t crit_num_action, uint8_t crit_rate_object);
	void Net_SendPickObject(HexTYPE targ_x, HexTYPE targ_y, uint16_t id_sobj);
	void Net_SendChangeObject(uint32_t idobj, uint8_t num_slot);
	void Net_SendTalk(CritterID id_to_talk, uint8_t answer);
	void Net_SendGetTime();
	void Net_SendGiveGlobalInfo(uint8_t info_flags);
	void Net_SendRuleGlobal(uint8_t command, uint32_t param1=0, uint32_t param2=0);
	void Net_SendGiveMeMap(uint16_t map_num);
	void Net_SendLoadMapOK();
//!Cvet -------------------------------------------------------------
	void Net_SendText(char* str);
	void Net_SendDir(); //Cvet переделал
    void Net_SendMove(uint8_t* dir); //!Cvet переделал

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
	uint16_t Game_FPS;

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
	uint16_t inv_pic_b[367]; //большие
	uint16_t inv_pic_s[367]; //малые
	//картинки использования
	uint16_t pic_use[100];

  //Управление чузеном
	chosen_action_vec chosen_action;

	HexTYPE PathMoveX;
	HexTYPE PathMoveY;
	CCritter* curTargetCrit;
	CCritter* tosendTargetCrit;
	ItemObj* curTargetObj;
	ItemObj* tosendTargetObj;

	void SetChosenAction(uint8_t set_action){ chosen_action.clear(); chosen_action.push_back(set_action); };
	void AddChosenAction(uint8_t add_action){ chosen_action.push_back(add_action); };
	void EraseFrontChosenAction(){chosen_action.erase(chosen_action.begin());if(!chosen_action.size()) chosen_action.push_back(ACTION_NONE);};
	void EraseBackChosenAction(){chosen_action.pop_back();if(!chosen_action.size()) chosen_action.push_back(ACTION_NONE);};
	int IsChosenAction(uint8_t check_action){ if(*chosen_action.begin()==check_action) return 1; return 0; };

	void ChosenProcess();

//ИНТЕРФЕЙС=======================================================================
	int Init_Iface();

	int numIface; //номер анимации интерфейса

//Инвентарь=======================================================================
	uint16_t invbox,invokon,invokoff,invscrupin,invscrupout,invscrupoff,invscrdwin,invscrdwout,invscrdwoff; //анимация

	uint8_t InvHold; //номер зажатой кнопки - нмера внизу //0 - кнопка не зажата
	int scroll_items;

	int InvX,InvY; //х,у основного меню окна, все остальное будет отталкиваться именно от этих координат

  fonline::math::Rect<int> InvMain,InvObl,InvChosen;
	int HeightItem;// = 30; //высота картинки в инвенторе, всего там 10
	//слот1,2, армор
	fonline::math::Rect<int> InvSlot1,InvSlot2,InvArmor;
	//кнопки
	fonline::math::Rect<int> InvBtnUp,InvBtnDown,InvBtnOk;
	//точки вывода инфы
	fonline::math::Rect<int> txtObject;
	//это нужно для перетаскивания окна инвентари
	int invvectx,invvecty;

	void InvDraw(); 
	void InvMouseMove();
	int InvMouseDown();
	int InvMouseUp();

//Основной интерфейс==============================================================
	uint16_t ifacen,panel,intscrupon,intscrdownon,intchangesloton,
	intinvon,intmenuon,intskillon,intmapon,intinfoon,intpipon;
	uint16_t diodeG,diodeY,diodeR;

	uint8_t IntHold; //номер зажатой кнопки - нмера внизу //0 - кнопка не зажата
	int IntX,IntY; //х,у основного меню окна, все остальное будет отталкиваться именно от этих координат

	fonline::math::Rect<int> IntMain; 
	//слоты
	fonline::math::Rect<int> IntObject; //слот объекта
	//кнопки
	fonline::math::Rect<int> IntBScrUp,IntBScrDown,IntBChangeSlot,IntBInv,IntBMenu,IntBSkill,IntBMap,IntBInfo,IntBPip;
	//точки вывода инфы
	fonline::math::Rect<int> IntTXT;
	//action points //15 зеленых(200мс) 3 желтых(1000мс) 2 красных(10000мс)
	fonline::math::Rect<int> IntAP,IntHP,IntAC;

	int IntAPstepX,IntAPstepY;

	void IntDraw();
	int IntMouseDown();
	int IntMouseUp();
	void IntMouseMove();

	//мессаги о сотоянии криттера
	uint16_t chosen_mess_pic;
	int IntMessX,IntMessY;
	int IntMessStepX,IntMessStepY;
	fonline::math::Rect<int> IntMess;

  //Меню левой кнопки (LMenu)=======================================================
	uint16_t lm_talk_off,lm_talk_on,lm_look_off,lm_look_on,lm_break_off,lm_break_on,
		lm_use_off,lm_use_on,lm_gmfollow_off,lm_gmfollow_on,lm_gmtake_off,lm_gmtake_on;

	bool LMenu_try_activated;
	uint32_t LMenu_start_time;
	int LMenu_cur_x,LMenu_cur_y;
	int LMenu_x,LMenu_y;
	int LMenu_node_height;

	LMenu_list* LMenu_nodes;
	int LMenu_cur_node;

	uint16_t Node_stay_mouse;

	LMenu_list LMenu_crit_nodes,LMenu_scenery_nodes,LMenu_item_nodes;

	uint8_t LMenu_mode;

	void SetLMenu(uint8_t set_lmenu);
	BOOL IsLMenu() { if(LMenu_mode!=LMENU_OFF) return 1; return 0; };
	void LMenuTryCreate();
	void LMenuDraw();
	void LMenuMouseMove();
	void LMenuMouseUp();

//Логин/пасс====================================================================
	uint16_t loginpic;

	uint8_t LogFocus; //позиция курсора 0-нигде 1-login 2-pass
	uint8_t LogMsg; //номер сообщения

	int LogX,LogY;

	fonline::math::Rect<int> LogMain,LogWLogin,LogWPass,LogBOk,LogBReg,LogBExit;

	void ShowLogIn();
	void LogInput();
	
//Регистрация===================================================================
	crit_info New_cr;

	uint16_t registpic;

	uint8_t RegFocus;

	int RegX,RegY;

	fonline::math::Rect<int> RegMain,RegWS,RegWP,RegWE,RegWC,RegWI,RegWA,RegWL,RegWLogin,RegWPass,RegWName,
		RegWCases0,RegWCases1,RegWCases2,RegWCases3,RegWCases4,RegWBType,RegWGender,RegWAge,RegBReg,RegBBack;
	
	int CheckRegData(crit_info* newcr);

	void ShowRegistration();
	void RegInput();

//Диалог========================================================================
	uint16_t dialog_begin,dialog_answ,dialog_end;

	uint8_t DlgHold;

	int DlgNextAnswX,DlgNextAnswY;
	int DlgCurAnsw;
	uint8_t all_answers;

	int dlgvectx,dlgvecty;

	char text_dialog[MAX_DIALOG_TEXT+1];
	char text_answer[MAX_ANSWERS][MAX_DIALOG_TEXT+1];

	int DlgX,DlgY;

	fonline::math::Rect<int> DlgMain,DlgBegin,DlgEnd,DlgText,DlgAnsw;

	void DlgDraw();

	void DlgMouseMove();
	void DlgMouseDown();
	void DlgMouseUp();

	int LoadDialogFromFile(CritterID id_npc, uint32_t id_dialog, char* dialog);

//Мини-карта=====================================================================
	uint16_t lmap_main,lmap_bok_on,lmap_bscan_on,lmap_blohi_on,lmap_pix;

	Pix_vec lmap_prep_pix;

	fonline::math::Rect<int> LmapMain,LmapWMap,LmapBOk,LmapBScan,LmapBLoHi;

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
	uint16_t gm_pic[5][6];
	uint16_t gm_lock_contr_pic,gm_bstop_on,gm_bspeed0_on,gm_bspeed1_on,gm_bspeed2_on,
		gm_btolocal_on,gm_iface_pic,gm_iface_pic2,gm_gr_loc_pic,gm_gr_targ_pic;
	uint16_t gm_loc48_pic,gm_loc24_pic,gm_loc12_pic;

	int GmapX,GmapY,GmapMapScrX,GmapMapScrY,GmapWNameStepX,GmapWNameStepY,
		GmapPLockContrX,GmapPLockContrY,GmapHold;

	fonline::math::Rect<int> GmapMain,GmapWMap,GmapBToLocal,GmapBStop,GmapBSpeed0,GmapBSpeed1,GmapBSpeed2,
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
		uint16_t num;
		uint16_t x;
		uint16_t y;
		uint8_t radius;

		GM_city():num(0),x(0),y(0),radius(0){};
		GM_city(uint16_t _num,uint16_t _x,uint16_t _y,uint8_t _radius):num(_num),x(_x),y(_y),radius(_radius){};
	};

	std::vector<GM_city*> gm_cities;

	//params
	float GmapGroupXf,GmapGroupYf;
	uint32_t gm_last_tick;
	int GmapGroupX,GmapGroupY,GmapMoveX,GmapMoveY;
	float GmapSpeedX,GmapSpeedY;
	uint8_t GmapSpeed;

	//crits
	uint16_t gm_cr_online,gm_cr_offline,gm_cr_npc;

	struct GM_crit
	{
		CritterID crid;
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
	std::vector<GM_crit*> gm_crits;

	//tabs
	uint16_t gm_wtab_pic,gm_wblanktab_pic,gm_btabloc_picdn,gm_tabs_scrup_picdn,gm_tabs_scrdn_picdn;
	uint16_t gm_locpic[10];

	fonline::math::Rect<int> GmapWTabs,GmapWTabLoc,GmapBTabLoc,GmapBTabsScrUp,GmapBTabsScrDn;
	int GmapTabW,GmapTabH,GmapTabNextX,GmapTabNextY,GmapCurHoldBLoc;

	int gm_tab_scr_x,gm_tab_scr_y;
	uint32_t last_scr;


	void GmapDraw();
	void GmapInput();
	void GmapMouseDown();
	void GmapMouseUp();
	void GmapMouseMove();

//Skillbox========================================================================
	uint16_t sbox_main,sbox_bcancel_on,sbox_bsneak_on,sbox_blockpick_on,sbox_bsteal_on,
		sbox_btrap_on,sbox_bfirstaid_on,sbox_bdoctor_on,sbox_bscience_on,sbox_brepair_on;

	fonline::math::Rect<int> SboxMain,SboxBCancel,SboxBSneak,SboxBLockpick,SboxBSteal,
		SboxBTrap,SboxBFirstAid,SboxBDoctor,SboxBScience,SboxBRepair;

	fonline::math::Rect<int> SboxTSneak,SboxTLockpick,SboxTSteal,SboxTTrap,SboxTFirstAid,
		SboxTDoctor,SboxTScience,SboxTRepair;

	int SboxX,SboxY;
	int sboxvectx,sboxvecty;
	int SboxHold;

	void SboxDraw();
	void SboxMouseDown();
	void SboxMouseMove();
	void SboxMouseUp();

//Menu Options
	uint16_t mopt_main,mopt_bresume_on,mopt_bexit_on;

	int MoptX,MoptY;
	int MoptHold;

	fonline::math::Rect<int> MoptMain,MoptBResume,MoptBExit;

	void MoptDraw();
	void MoptMouseDown();
	void MoptMouseUp();

//PlayerInfo======================================================================

//PipBoy==========================================================================

//Cursor==========================================================================
	void CurDraw();

//================================================================================

//Игровое время
	uint16_t Game_Hours,Game_Mins,Game_Time,Game_Year;
	uint8_t Game_Day,Game_Month;

	uint32_t Game_TimeTick;
	uint32_t Game_CurTime;

	void ProccessDayTime();
	void SetDayTime(uint32_t time_ms);

	uint8_t dayR,dayG,dayB;
//!Cvet ---------------------------------------------------------------------------------------

	void ParseInput();

	void SetColor(uint8_t r,uint8_t g,uint8_t b);
	void SetColor(uint32_t color); //!Cvet

	crit_map critters;
	CCritter* AddCritter(crit_info* pinfo);

	CCritter* lpChosen;

	// для отладки
	CritterID LstMoveId;
    CritterID LstAddCritId;
	CritterID LstDelCritId;
	CritterID LstSayCritId;
	char newbie[31];

//!Cvet ++++++++++++++++++++++++++++++++++++++++++++++++++++++
	char mess_str[EDIT_LEN+1]; // строка для окна сообщений расширяемая
	char all_mess[300][MAX_TEXT+128];
	uint8_t max_mess,scr_mess;

	void AddMess(uint32_t text_color,char* message_text, ...);
	void MessBoxDrawText();
	void MessBoxInput();

//	char mess_str[EDIT_LEN+1]; // строка для окна сообщений расширяемая

//Работа со строками
	void StrChangeValue(char* str, int value){for(int i=0;str[i];++i)str[i]+=value;};
//!Cvet ------------------------------------------------------
};


#endif