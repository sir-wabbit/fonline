//FEngine_Iface.cpp by Cvet
/********************************************************************
	created:	19:01:2007   00:00;

	author:		Anton Tsvetinsky
	
	purpose from baster:	дополнительная фенечка
*********************************************************************/
#include "stdafx.h"

#include "FEngine.h"
#include "common.h"
#include "keyb.h"

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

const BYTE DLG_HOLD_NONE	=0;
const BYTE DLG_HOLD_MAIN	=1;
const BYTE DLG_HOLD_ANSW	=2;

char* LoginMess[50]=
{
	{""}, //0
	{"ОШИБКА!!! Логин не может быть короче четырех символов."}, //1
	{"ОШИБКА!!! Пароль не может быть короче четырех символов."}, //2
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Аккаунт с таким именем уже есть - измените логин."}, //3
	{"ОШИБКА ПРИ АУНТЕФИКАЦИИ!!! Игрок под этим логином уже в игре - обязательно обратитесь к администратору сервера."}, //4
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Неверные данные SPECIAL."}, //5
	{"РЕГИСТРАЦИЯ ПРОЙДЕНА УСПЕШНО. ЖДИТЕ ПОДТВЕЖДЕНИЯ НА ВАШ E-MAIL."}, //6
	{"ИДЕТ СОЕДИНЕНИЕ С СЕРВЕРОМ.... ЖДИТЕ...."}, //7
	{"СБОЙ СОЕДИНЕНИЯ!!! При повторениях сбоя - обратитесь к админестратору сервера."}, //8
	{"ОШИБКА ПРИ АУНТЕФИКАЦИИ!!! Неверный логин или пароль."}, //9
	{"СОЕДИНЕНИЕ УСТАНОВЛЕНО.... АУНТЕФИКАЦИЯ.... ЖДИТЕ...."}, //10
	{"ОШИБКА!!! Клетка занята... попробуйте немного позже"}, //11
	{"ОТКЛЮЧЕНИЕ ПО КОМАНДЕ ИГРОКА"}, //12
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Длинное или короткое имя."}, //13
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Длинное или короткое одно(несколько) из склонений."}, //14
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Невеный пол."}, //15
	{"ОШИБКА ПРИ РЕГИСТРАЦИИ!!! Неверный возраст."}, //16
	{"ОШИБКА!!! Нудалось подключиться к серверу игры."}, //17
};

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

int CFEngine::Init_Iface()
{
	WriteLog("Инициализация интерфейса...\n");

	CreateStringsParamsMaps();

	GetPrivateProfileString("LOGIN","login","",opt_login,MAX_LOGIN,CFG_FILE);
	GetPrivateProfileString("LOGIN","pass","",opt_pass,MAX_LOGIN,CFG_FILE);

//============================================================================================
	char key1[255];
	char key2[255];
	int IB;

	numIface=GetPrivateProfileInt("Game","Iface",0,CFG_FILE);
	sprintf(key1,"Iface%d",numIface);

//берем настройки для инвентаря
	InvX=GetPrivateProfileInt(key1,"InvX",100,CFG_INT_FILE);
	InvY=GetPrivateProfileInt(key1,"InvY",100,CFG_INT_FILE);
	scroll_items=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvMain%d",IB); InvMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvObl%d",IB); InvObl[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	HeightItem=GetPrivateProfileInt(key1,"HeightItem",100,CFG_INT_FILE);
	for(IB=0;IB<=1;IB++) { sprintf(key2,"InvChosen%d",IB); InvChosen[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvSlot1%d",IB); InvSlot1[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvSlot2%d",IB); InvSlot2[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvArmor%d",IB); InvArmor[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvBtnUp%d",IB); InvBtnUp[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvBtnDown%d",IB); InvBtnDown[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"InvBtnOk%d",IB); InvBtnOk[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"txtObject%d",IB); txtObject[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

//берем настройки для интерфейса
	IntX=GetPrivateProfileInt(key1,"IntX",-1,CFG_INT_FILE);
	switch(IntX)
	{
	case -1:
		IntX=(MODE_WIDTH-GetPrivateProfileInt(key1,"IntMain2",MODE_WIDTH,CFG_INT_FILE))/2;
		break;
	case -2:
		IntX=MODE_WIDTH-GetPrivateProfileInt(key1,"IntMain2",MODE_WIDTH,CFG_INT_FILE);
		break;
	case -3:
		IntX=0;
		break;
	default:
		if(IntX<0) IntX=0;
		if(IntX+GetPrivateProfileInt(key1,"IntMain2",MODE_WIDTH,CFG_INT_FILE)>MODE_WIDTH) IntX=0;
		break;
	}

	IntY=MODE_HEIGHT-GetPrivateProfileInt(key1,"IntMain3",300,CFG_INT_FILE);

	IntAP[0]=GetPrivateProfileInt(key1,"IntAP0",1,CFG_INT_FILE); IntAP.l+=IntX;
	IntAP[1]=GetPrivateProfileInt(key1,"IntAP1",1,CFG_INT_FILE); IntAP.t+=IntY;
	IntAPstepX=GetPrivateProfileInt(key1,"IntAPstepX",1,CFG_INT_FILE);
	IntAPstepY=GetPrivateProfileInt(key1,"IntAPstepY",1,CFG_INT_FILE);

	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntMain%d",IB); IntMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntMain[IB]+=IntX; else IntMain[IB]+=IntY;}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntObject%d",IB); IntObject[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntObject[IB]+=IntX; else IntObject[IB]+=IntY;}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBScrUp%d",IB); IntBScrUp[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBScrUp[IB]+=IntX; else IntBScrUp[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBScrDown%d",IB); IntBScrDown[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBScrDown[IB]+=IntX; else IntBScrDown[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBChangeSlot%d",IB); IntBChangeSlot[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBChangeSlot[IB]+=IntX; else IntBChangeSlot[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBInv%d",IB); IntBInv[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBInv[IB]+=IntX; else IntBInv[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBMenu%d",IB); IntBMenu[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBMenu[IB]+=IntX; else IntBMenu[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBSkill%d",IB); IntBSkill[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBSkill[IB]+=IntX; else IntBSkill[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBMap%d",IB); IntBMap[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBMap[IB]+=IntX; else IntBMap[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBInfo%d",IB); IntBInfo[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBInfo[IB]+=IntX; else IntBInfo[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntBPip%d",IB); IntBPip[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntBPip[IB]+=IntX; else IntBPip[IB]+=IntY;}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntTXT%d",IB); IntTXT[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntTXT[IB]+=IntX; else IntTXT[IB]+=IntY;}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntHP%d",IB); IntHP[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntHP[IB]+=IntX; else IntHP[IB]+=IntY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"IntAC%d",IB); IntAC[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) IntAC[IB]+=IntX; else IntAC[IB]+=IntY;}

	//инициализация интерфейсбокса
	max_mess=0; scr_mess=0;
//	AddMess("Загрузка ПипБоя...");
//	AddMess("Загрузка голодисков...");
//	AddMess("Загрузка предметов...");
//	AddMess("Загрузка статистики...");
//	AddMess("Загрузка $$$$$$...");
//	AddMess("Загрузка завершена...");
//	AddMess("Загрузка ПипБоя...");
//	AddMess("Загрузка голодисков...");
//	AddMess("Загрузка предметов...");
//	AddMess("Загрузка статистики...");
//	AddMess("Загрузка $$$$$$...");
//	AddMess("Загрузка завершена...");

	//мессаги чезена
	IntMess[2]=GetPrivateProfileInt(key1,"IntMessW",50,CFG_INT_FILE);
	IntMess[3]=GetPrivateProfileInt(key1,"IntMessH",40,CFG_INT_FILE);
	IntMessX=GetPrivateProfileInt(key1,"IntMessX",0,CFG_INT_FILE);
	IntMessY=GetPrivateProfileInt(key1,"IntMessY",0,CFG_INT_FILE);
	IntMessStepX=GetPrivateProfileInt(key1,"IntMessStepX",0,CFG_INT_FILE);
	IntMessStepY=GetPrivateProfileInt(key1,"IntMessStepY",40,CFG_INT_FILE);

//берем настройки для меню логин/пасс
	LogX=(MODE_WIDTH-GetPrivateProfileInt(key1,"LogMain2",0,CFG_INT_FILE))/2;
	LogY=(MODE_HEIGHT-GetPrivateProfileInt(key1,"LogMain3",0,CFG_INT_FILE))/2;
	LogFocus=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogMain%d",IB); LogMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogMain[IB]+=LogX; else LogMain[IB]+=LogY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogWLogin%d",IB); LogWLogin[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogWLogin[IB]+=LogX; else LogWLogin[IB]+=LogY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogWPass%d",IB); LogWPass[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogWPass[IB]+=LogX; else LogWPass[IB]+=LogY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogBOk%d",IB); LogBOk[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogBOk[IB]+=LogX; else LogBOk[IB]+=LogY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogBReg%d",IB); LogBReg[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogBReg[IB]+=LogX; else LogBReg[IB]+=LogY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LogBExit%d",IB); LogBExit[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) LogBExit[IB]+=LogX; else LogBExit[IB]+=LogY;}

//Регистрация
	RegX=(MODE_WIDTH-GetPrivateProfileInt(key1,"RegMain2",300,CFG_INT_FILE))/2;
	RegY=(MODE_HEIGHT-GetPrivateProfileInt(key1,"RegMain3",300,CFG_INT_FILE))/2;
	RegFocus=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWS%d",IB); RegWS[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWS[IB]+=RegX; else RegWS[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWP%d",IB); RegWP[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWP[IB]+=RegX; else RegWP[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWE%d",IB); RegWE[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWE[IB]+=RegX; else RegWE[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWC%d",IB); RegWC[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWC[IB]+=RegX; else RegWC[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWI%d",IB); RegWI[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWI[IB]+=RegX; else RegWI[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWA%d",IB); RegWA[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWA[IB]+=RegX; else RegWA[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWL%d",IB); RegWL[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWL[IB]+=RegX; else RegWL[IB]+=RegY;}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWLogin%d",IB); RegWLogin[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWLogin[IB]+=RegX; else RegWLogin[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWPass%d",IB); RegWPass[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWPass[IB]+=RegX; else RegWPass[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWName%d",IB); RegWName[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWName[IB]+=RegX; else RegWName[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWCases0%d",IB); RegWCases0[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWCases0[IB]+=RegX; else RegWCases0[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWCases1%d",IB); RegWCases1[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWCases1[IB]+=RegX; else RegWCases1[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWCases2%d",IB); RegWCases2[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWCases2[IB]+=RegX; else RegWCases2[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWCases3%d",IB); RegWCases3[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWCases3[IB]+=RegX; else RegWCases3[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWCases4%d",IB); RegWCases4[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWCases4[IB]+=RegX; else RegWCases4[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWBType%d",IB); RegWBType[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWBType[IB]+=RegX; else RegWBType[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWGender%d",IB); RegWGender[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWGender[IB]+=RegX; else RegWGender[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegWAge%d",IB); RegWAge[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegWAge[IB]+=RegX; else RegWAge[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegBReg%d",IB); RegBReg[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegBReg[IB]+=RegX; else RegBReg[IB]+=RegY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"RegBBack%d",IB); RegBBack[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) RegBBack[IB]+=RegX; else RegBBack[IB]+=RegY;}

	New_cr.st[ST_STRENGHT	]=5;
	New_cr.st[ST_PERCEPTION	]=5;
	New_cr.st[ST_ENDURANCE	]=5;
	New_cr.st[ST_CHARISMA	]=5;
	New_cr.st[ST_INTELLECT	]=5;
	New_cr.st[ST_AGILITY	]=5;
	New_cr.st[ST_LUCK		]=5;
	strcpy(New_cr.name,"noname");
	strcpy(New_cr.cases[0],"кого");
	strcpy(New_cr.cases[1],"кому");
	strcpy(New_cr.cases[2],"кого");
	strcpy(New_cr.cases[3],"с кем");
	strcpy(New_cr.cases[4],"о ком");
	strcpy(New_cr.login,"login");
	strcpy(New_cr.pass,"password");
	New_cr.base_type=0;
	New_cr.st[ST_AGE]=22;
	New_cr.st[ST_GENDER]=0;

//Диалог
	DlgX=GetPrivateProfileInt(key1,"DlgX",100,CFG_INT_FILE);
	DlgY=GetPrivateProfileInt(key1,"DlgY",100,CFG_INT_FILE);

	DlgHold=DLG_HOLD_NONE;
	DlgCurAnsw=-1;
	all_answers=0;
	dlgvectx=0;
	dlgvecty=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"DlgBegin%d",IB); DlgBegin[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"DlgEnd%d",IB); DlgEnd[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"DlgText%d",IB); DlgText[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"DlgAnsw%d",IB); DlgAnsw[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	DlgNextAnswX=GetPrivateProfileInt(key1,"DlgNextAnswX",1,CFG_INT_FILE);
	DlgNextAnswY=GetPrivateProfileInt(key1,"DlgNextAnswY",1,CFG_INT_FILE);

//LMenu
	Node_stay_mouse=NULL;
	LMenu_cur_node=0;
	LMenu_try_activated=false;
	LMenu_start_time=0;
	SetLMenu(LMENU_OFF);

	LMenu_node_height=GetPrivateProfileInt(key1,"LMenu_node_height",40,CFG_INT_FILE);

	//ноды крита
	LMenu_crit_nodes.push_back(LMENU_NODE_LOOK);
	LMenu_crit_nodes.push_back(LMENU_NODE_BREAK);
	//ноды сценери
	LMenu_scenery_nodes.push_back(LMENU_NODE_BREAK);
	//ноды объектов
	LMenu_item_nodes.push_back(LMENU_NODE_BREAK);

//Мини-карта
	LmapX=GetPrivateProfileInt(key1,"LmapX",100,CFG_INT_FILE);
	LmapY=GetPrivateProfileInt(key1,"LmapY",100,CFG_INT_FILE);

	lmapvectx=0;
	lmapvecty=0;

	lmap_zoom=2;

	switch_HI=false;

	LmapHold=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"LmapMain%d",IB); LmapMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LmapWMap%d",IB); LmapWMap[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LmapBOk%d",IB); LmapBOk[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LmapBScan%d",IB); LmapBScan[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"LmapBLoHi%d",IB); LmapBLoHi[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

//Skillbox
	SboxX=GetPrivateProfileInt(key1,"SboxX",300,CFG_INT_FILE);
	SboxY=GetPrivateProfileInt(key1,"SboxY",20,CFG_INT_FILE);

	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxMain%d",IB);		SboxMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBCancel%d",IB);	SboxBCancel[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBSneak%d",IB);		SboxBSneak[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBLockpick%d",IB);	SboxBLockpick[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBSteal%d",IB);		SboxBSteal[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBTrap%d",IB);		SboxBTrap[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBFirstAid%d",IB);	SboxBFirstAid[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBDoctor%d",IB);	SboxBDoctor[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBScience%d",IB);	SboxBScience[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxBRepair%d",IB);	SboxBRepair[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTSneak%d",IB);		SboxTSneak[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTLockpick%d",IB);	SboxTLockpick[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTSteal%d",IB);		SboxTSteal[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTTrap%d",IB);		SboxTTrap[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTFirstAid%d",IB);	SboxTFirstAid[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTDoctor%d",IB);	SboxTDoctor[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTScience%d",IB);	SboxTScience[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"SboxTRepair%d",IB);	SboxTRepair[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	sboxvectx=0;
	sboxvecty=0;
	SboxHold=0;

//Menu option
	MoptX=(MODE_WIDTH-GetPrivateProfileInt(key1,"MoptMain2",300,CFG_INT_FILE))/2;
	MoptY=(MODE_HEIGHT-GetPrivateProfileInt(key1,"MoptMain3",300,CFG_INT_FILE))/2;
	MoptHold=0;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"MoptMain%d",IB); MoptMain[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) MoptMain[IB]+=MoptX; else MoptMain[IB]+=MoptY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"MoptBResume%d",IB); MoptBResume[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) MoptBResume[IB]+=MoptX; else MoptBResume[IB]+=MoptY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"MoptBExit%d",IB); MoptBExit[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) MoptBExit[IB]+=MoptX; else MoptBExit[IB]+=MoptY;}

//Global map
	GmapX=(MODE_WIDTH-GetPrivateProfileInt(key1,"GmapMainW",0,CFG_INT_FILE))/2;
	GmapY=(MODE_HEIGHT-GetPrivateProfileInt(key1,"GmapMainH",0,CFG_INT_FILE))/2;

	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWMap%d",IB); GmapWMap[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWMap[IB]+=GmapX; else GmapWMap[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBToLocal%d",IB); GmapBToLocal[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBToLocal[IB]+=GmapX; else GmapBToLocal[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBStop%d",IB); GmapBStop[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBStop[IB]+=GmapX; else GmapBStop[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBSpeed0%d",IB); GmapBSpeed0[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBSpeed0[IB]+=GmapX; else GmapBSpeed0[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBSpeed1%d",IB); GmapBSpeed1[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBSpeed1[IB]+=GmapX; else GmapBSpeed1[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBSpeed2%d",IB); GmapBSpeed2[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBSpeed2[IB]+=GmapX; else GmapBSpeed2[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWName%d",IB); GmapWName[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWName[IB]+=GmapX; else GmapWName[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWChat%d",IB); GmapWChat[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWChat[IB]+=GmapX; else GmapWChat[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWPanel%d",IB); GmapWPanel[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWPanel[IB]+=GmapX; else GmapWPanel[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWGroupInfo%d",IB); GmapWGroupInfo[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWGroupInfo[IB]+=GmapX; else GmapWGroupInfo[IB]+=GmapY;}
	GmapMapScrX=(GmapWMap[2]-GmapWMap[0])/2+GmapWMap[0];
	GmapMapScrY=(GmapWMap[3]-GmapWMap[1])/2+GmapWMap[1];
	GmapPLockContrX=GetPrivateProfileInt(key1,"GmapPLockContrX",0,CFG_INT_FILE)+GmapX;
	GmapPLockContrY=GetPrivateProfileInt(key1,"GmapPLockContrY",0,CFG_INT_FILE)+GmapY;
	GmapWNameStepX=GetPrivateProfileInt(key1,"GmapWNameStepX",0,CFG_INT_FILE);
	GmapWNameStepY=GetPrivateProfileInt(key1,"GmapWNameStepY",40,CFG_INT_FILE);

	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWTabs%d",IB); GmapWTabs[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapWTabs[IB]+=GmapX; else GmapWTabs[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBTabsScrUp%d",IB); GmapBTabsScrUp[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBTabsScrUp[IB]+=GmapX; else GmapBTabsScrUp[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBTabsScrDn%d",IB); GmapBTabsScrDn[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);
	if(IB%2==0) GmapBTabsScrDn[IB]+=GmapX; else GmapBTabsScrDn[IB]+=GmapY;}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapWTabLoc%d",IB); GmapWTabLoc[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}
	for(IB=0;IB<=3;IB++) { sprintf(key2,"GmapBTabLoc%d",IB); GmapBTabLoc[IB]=GetPrivateProfileInt(key1,key2,1,CFG_INT_FILE);}

	GmapTabW=GetPrivateProfileInt(key1,"GmapTabW",0,CFG_INT_FILE);
	GmapTabH=GetPrivateProfileInt(key1,"GmapTabH",0,CFG_INT_FILE);
	GmapTabNextX=GetPrivateProfileInt(key1,"GmapTabNextX",0,CFG_INT_FILE);
	GmapTabNextY=GetPrivateProfileInt(key1,"GmapTabNextY",0,CFG_INT_FILE);

	GmapNullParams();
	GmapHold=0;
	gm_tab_scr_x=0;
	gm_tab_scr_y=0;
	gmapvectx=0;
	gmapvecty=0;
	GmapMapScrollEnable=false;

//загрузка графики=============================================================================
	char f_name[64];
//Интерфейс
	GetPrivateProfileString(key1,"IntMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(ifacen=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"IntPanelPic","error",f_name,64,CFG_INT_FILE);
	if(!(panel=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"IntAPgreenPic","error",f_name,64,CFG_INT_FILE);
	if(!(diodeG=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntAPyellowPic","error",f_name,64,CFG_INT_FILE);
	if(!(diodeY=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntAPredPic","error",f_name,64,CFG_INT_FILE);
	if(!(diodeR=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"IntBScrUpPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intscrupon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBScrDownPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intscrdownon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBChangePicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intchangesloton=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBInvPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intinvon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBMenuPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intmenuon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBSkillPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intskillon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBMapPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intmapon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBInfoPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intinfoon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"IntBPipPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(intpipon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	int t1;
	FILE *cf;

	//картинка использования
	WriteLog("Загрузка названий типов использования объектов...");
	if((cf = fopen("data\\objects\\pic_use.txt", "rt")) != NULL)
	{
		t1=0;
		while(!feof(cf))
		{
			fscanf(cf, "%s", &f_name);
			pic_use[t1]=sm.LoadSprite(f_name,PT_ART_INTRFACE);

			if(!pic_use[t1])
			{
				WriteLog("Ошибка загрузки %s\n",f_name);
				return 0;
			}

			t1++;
		}
		fclose(cf);
		WriteLog("Выполнено\n");
	} 
	else
	{
		WriteLog("Файл не найден\n");
		return 0;
	}

	//картинка мессаги чезена
	GetPrivateProfileString(key1,"IntMessPic","error",f_name,64,CFG_INT_FILE);
	if(!(chosen_mess_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Инвентарь
	GetPrivateProfileString(key1,"InvMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(invbox=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"InvBOkPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(invokoff=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"InvBOkPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(invokon=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"InvBUpPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(invscrupout=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"InvBUpPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(invscrupin=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"InvBUpPicNa","error",f_name,64,CFG_INT_FILE);
	if(!(invscrupoff=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"InvBDnPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(invscrdwout=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"InvBDnPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(invscrdwin=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"InvBDnPicNa","error",f_name,64,CFG_INT_FILE);
	if(!(invscrdwoff=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	//большая и малая картинка в инвентаре
	WriteLog("Загрузка картинок в инвентарь статических объектов...");
	if((cf = fopen("data\\objects\\pic_inv.txt", "rt")) != NULL)
	{
		t1=0;
		while(!feof(cf))
		{
			fscanf(cf, "%s", &f_name);
			inv_pic_b[t1]=sm.LoadSprite(f_name,PT_ART_INVEN);
			inv_pic_s[t1]=sm.LoadMiniSprite(f_name,2,PT_ART_INVEN);

			if((!inv_pic_b[t1])||(!inv_pic_s[t1]))
			{
				WriteLog("Ошибка %d=%s\n",t1,f_name);
				return 0;
			}

			t1++;
		}
		fclose(cf);
		WriteLog("Выполнено\n");
	} 
	else
	{
		WriteLog("Файл не найден\n");
		return 0;
	}

//Логин/пасс
	GetPrivateProfileString(key1,"LogMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(loginpic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Регистрация
	GetPrivateProfileString(key1,"RegMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(registpic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Диалог
	GetPrivateProfileString(key1,"DlgBeginPic","error",f_name,64,CFG_INT_FILE);
	if(!(dialog_begin=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"DlgAnswPic","error",f_name,64,CFG_INT_FILE);
	if(!(dialog_answ=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"DlgEndPic","error",f_name,64,CFG_INT_FILE);
	if(!(dialog_end=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

// курсоры
	if(!(cur_move=sm.LoadSprite("msef001.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_move_block=sm.LoadSprite("msef002.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_use_o=sm.LoadSprite("acttohit.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_use_s=sm.LoadSprite("crossuse.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_wait=sm.LoadSprite("wait2.frm",PT_ART_INTRFACE))) return 0;

	if(!(cur_hand=sm.LoadSprite("hand.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_def=sm.LoadSprite("actarrow.frm",PT_ART_INTRFACE))) return 0;

	if(!(cur_right=sm.LoadSprite("screast.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_left=sm.LoadSprite("scrwest.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_up=sm.LoadSprite("scrnorth.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_down=sm.LoadSprite("scrsouth.frm",PT_ART_INTRFACE))) return 0;

	if(!(cur_ru=sm.LoadSprite("scrneast.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_lu=sm.LoadSprite("scrnwest.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_rd=sm.LoadSprite("scrseast.frm",PT_ART_INTRFACE))) return 0;
	if(!(cur_ld=sm.LoadSprite("scrswest.frm",PT_ART_INTRFACE))) return 0;

//Графика LMenu
	GetPrivateProfileString(key1,"LMenuTalkPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_talk_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuTalkPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_talk_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"LMenuLookPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_look_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuLookPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_look_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"LMenuCancelPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_break_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuCancelPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_break_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"LMenuUsePicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_use_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuUsePicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_use_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"LMenuGMFollowPicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_gmfollow_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuGMFollowPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_gmfollow_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"LMenuGMTakePicUp","error",f_name,64,CFG_INT_FILE);
	if(!(lm_gmtake_off=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LMenuGMTakePicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lm_gmtake_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Мини-карта
	GetPrivateProfileString(key1,"LmapMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(lmap_main=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LmapBOkPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lmap_bok_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LmapBScanPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lmap_bscan_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"LmapBLoHiPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(lmap_blohi_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	if(!(lmap_pix=sm.LoadSprite("green_pix.png",PT_ART_INTRFACE))) return 0;

//Skillbox
	GetPrivateProfileString(key1,"SboxMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_main=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBCancelPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bcancel_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBSneakPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bsneak_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBLockpickPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_blockpick_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBStealPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bsteal_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBTrapPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_btrap_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBFirstAidPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bfirstaid_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBDoctorPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bdoctor_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBSciencePicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_bscience_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"SboxBRepairPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(sbox_brepair_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Menu option
	GetPrivateProfileString(key1,"MoptMainPic","error",f_name,64,CFG_INT_FILE);
	if(!(mopt_main=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"MoptBResumePicDn","error",f_name,64,CFG_INT_FILE);
	if(!(mopt_bresume_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"MoptBExitPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(mopt_bexit_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

//Global map
	sprintf(key2,"GmapMain%dx%dPic",MODE_WIDTH,MODE_HEIGHT);
	GetPrivateProfileString(key1,key2,"error",f_name,64,CFG_INT_FILE);
	if(!(gm_iface_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	if(opt_screen_mode==2)
	{
		sprintf(key2,"GmapMain%dx%dPic2",MODE_WIDTH,MODE_HEIGHT);
		GetPrivateProfileString(key1,key2,"error",f_name,64,CFG_INT_FILE);
		if(!(gm_iface_pic2=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	}

	for(int px=0;px<5;++px)
		for(int py=0;py<6;++py)
		{
			sprintf(key2,"GmapPMap_%d_%d",px,py);
			GetPrivateProfileString(key1,key2,"error",f_name,64,CFG_INT_FILE);
			if(!(gm_pic[px][py]=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
		}

	GetPrivateProfileString(key1,"GmapPLockContrPic","error",f_name,64,CFG_INT_FILE);
	if(!(gm_lock_contr_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBStopPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_bstop_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBSpeed0PicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_bspeed0_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBSpeed1PicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_bspeed1_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBSpeed2PicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_bspeed2_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBToLocalPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_btolocal_on=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapPGroupLocPic","error",f_name,64,CFG_INT_FILE);
	if(!(gm_gr_loc_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapPGroupTargPic","error",f_name,64,CFG_INT_FILE);
	if(!(gm_gr_targ_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"GmapLoc48","error",f_name,64,CFG_INT_FILE);
	if(!(gm_loc48_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapLoc24","error",f_name,64,CFG_INT_FILE);
	if(!(gm_loc24_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapLoc12","error",f_name,64,CFG_INT_FILE);
	if(!(gm_loc12_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"GmapPCrOnline","error",f_name,64,CFG_INT_FILE);
	if(!(gm_cr_online=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapPCrOffline","error",f_name,64,CFG_INT_FILE);
	if(!(gm_cr_offline=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapPCrNPC","error",f_name,64,CFG_INT_FILE);
	if(!(gm_cr_npc=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	GetPrivateProfileString(key1,"GmapTabPic","error",f_name,64,CFG_INT_FILE);
	if(!(gm_wtab_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBlankTabPic","error",f_name,64,CFG_INT_FILE);
	if(!(gm_wblanktab_pic=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBTabLocPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_btabloc_picdn=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBTabsScrUpPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_tabs_scrup_picdn=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	GetPrivateProfileString(key1,"GmapBTabsScrDnPicDn","error",f_name,64,CFG_INT_FILE);
	if(!(gm_tabs_scrdn_picdn=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;

	gm_locpic[0]=NULL;
	for(int i=1;i<10;++i)
	{
		sprintf(key2,"GmapLocPic%d",i);
		GetPrivateProfileString(key1,key2,"error",f_name,64,CFG_INT_FILE);
		if(!(gm_locpic[i]=sm.LoadSprite(f_name,PT_ART_INTRFACE))) return 0;
	}
//===========================================================================================

//Звуки
TICK gtime=GetTickCount();
//	WORD snd1;
//	if(!(snd1=sdm.LoadSound("01.ogg",PT_SND_MUSIC)))
//	{
//		WriteLog("OGG FALSE\n");
//		return 0;
//	}
	sdm.LPESound("intro.ogg",PT_SND_MUSIC);
WriteLog("time load sound:%d\n",GetTickCount()-gtime);
//	sdm.PlaySound(snd1);

	SetCur(CUR_DEFAULT);

	WriteLog("Инициализация интерфейса прошла успешно\n");

	return 1;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::InvDraw() 
{
	sm.DrawSprite(invbox,InvX,InvY,COLOR_DEFAULT); //инвентарь

	if(scroll_items<=0) 
		sm.DrawSprite(invscrupoff,InvBtnUp[0]+InvX,InvBtnUp[1]+InvY,COLOR_DEFAULT); //кнопка вверх выключена
	else 
		if(InvHold==1) 
			sm.DrawSprite(invscrupin,InvBtnUp[0]+InvX,InvBtnUp[1]+InvY,COLOR_DEFAULT); //кнопка вверх нажата
		else 
			sm.DrawSprite(invscrupout,InvBtnUp[0]+InvX,InvBtnUp[1]+InvY,COLOR_DEFAULT); //кнопка вверх отпущена

	int count_items=lpChosen->obj.size();
	if(scroll_items>=count_items-(InvObl[3]-InvObl[1])/HeightItem)
		sm.DrawSprite(invscrdwoff,InvBtnDown[0]+InvX,InvBtnDown[1]+InvY,COLOR_DEFAULT); //кнопка вниз выключена
	else 
		if(InvHold==2) sm.DrawSprite(invscrdwin,InvBtnDown[0]+InvX,InvBtnDown[1]+InvY,COLOR_DEFAULT); //кнопка вниз нажата
		else 
			sm.DrawSprite(invscrdwout,InvBtnDown[0]+InvX,InvBtnDown[1]+InvY,COLOR_DEFAULT); //кнопка вниз отпущена

	if(InvHold==3) 
		sm.DrawSprite(invokon,InvBtnOk[0]+InvX,InvBtnOk[1]+InvY,COLOR_DEFAULT); //конопка ОК
	else 
		sm.DrawSprite(invokoff,InvBtnOk[0]+InvX,InvBtnOk[1]+InvY,COLOR_DEFAULT); //конопка ОК

	sm.DrawSprite(lpChosen->cur_id,InvChosen[0]+InvX,InvChosen[1]+InvY,COLOR_DEFAULT); //отрисовка чезена

	//отрисовка объектов в инвентаре
	int IB=0;
	int IB2=0;
	SpriteInfo* si;
	for(dyn_map::iterator it=lpChosen->obj.begin();it!=lpChosen->obj.end();it++)
	{
		if(IB>=scroll_items && IB<scroll_items+(InvObl[3]-InvObl[1])/HeightItem)
		{
			si=sm.GetSpriteInfo(inv_pic_s[(*it).second->object->p[OBJ_PIC_INV]]);
			//sm.DrawSpriteSize(
			sm.DrawSprite(inv_pic_s[(*it).second->object->p[OBJ_PIC_INV]],
				InvObl[0]+InvX+(InvObl[2]-InvObl[0]-si->w)/2,
				InvObl[1]+InvY+(IB2*HeightItem),COLOR_DEFAULT);
			IB2++;
		}
		IB++;
	}

	//отрисовка объектов в слот1
	if(lpChosen->a_obj->object->p[OBJ_PIC_INV])
	{
		si=sm.GetSpriteInfo(inv_pic_b[lpChosen->a_obj->object->p[OBJ_PIC_INV]]);
		sm.DrawSprite(inv_pic_b[lpChosen->a_obj->object->p[OBJ_PIC_INV]],
			InvSlot1[0]+InvX+(InvSlot1[2]-InvSlot1[0]-si->w)/2,
			InvSlot1[1]+InvY+(InvSlot1[3]-InvSlot1[1]-si->h)/2,COLOR_DEFAULT);
	}
	//отрисовка объектов в слот2
	if(lpChosen->a_obj2)
		sm.DrawSprite(inv_pic_b[lpChosen->a_obj2->object->p[OBJ_PIC_INV]],InvSlot2[0]+InvX,InvSlot2[1]+InvY,COLOR_DEFAULT);
	//отрисовка объектов в armor
	if(lpChosen->a_obj_arm->object->p[OBJ_PIC_INV])
	{
		si=sm.GetSpriteInfo(inv_pic_b[lpChosen->a_obj_arm->object->p[OBJ_PIC_INV]]);
		sm.DrawSprite(inv_pic_b[lpChosen->a_obj_arm->object->p[OBJ_PIC_INV]],
			InvArmor[0]+InvX+(InvArmor[2]-InvArmor[0]-si->w)/2,
			InvArmor[1]+InvY+(InvArmor[3]-InvArmor[1]-si->h)/2,COLOR_DEFAULT);
	}

	sm.Flush();

		char playstr[1024];
	RECT r1={txtObject[0]+InvX,txtObject[1]+InvY,txtObject[2]+InvX,txtObject[3]+InvY};

	wsprintf(playstr,	"%s\n"
						"------------------------------------\n"
						"ST %d  Жизнь: %d\n"
						"PE %d  Броня: %d\n"
						"EN %d Нормально 0/0%\n"
						"CH %d  Лазер     0/0%\n"
						"IN %d  Огонь     0/0%\n"
						"AG %d  Плазма    0/0%\n"
						"LK %d  Взрвчатка 0/0%\n"
						"------------------------------------\n"
						"%s\n"
						"хз\n"
						"\n"
						"------------------------------------\n"
						"Нет предмета\n"
						"Только для рации\n"
						"\n"
						"\n"
						"  Общий вес: 4/250",
	lpChosen->name,
	lpChosen->st[ST_STRENGHT	],lpChosen->st[ST_CURRENT_HP	],
	lpChosen->st[ST_PERCEPTION	],lpChosen->st[ST_ARMOR_CLASS	],
	lpChosen->st[ST_ENDURANCE	],
	lpChosen->st[ST_CHARISMA	],
	lpChosen->st[ST_INTELLECT	],
	lpChosen->st[ST_AGILITY		],
	lpChosen->st[ST_LUCK		],
	name_obj[lpChosen->a_obj->object->id].c_str());

	fnt.MyDrawText(r1,playstr,0);
}

int CFEngine::InvMouseDown()
{
	//hold 0-нигде 1-scrup 2-scrdown 3-ОК 4-main
	//основной инвентарь 
	if((cur_x>=InvObl[0]+InvX)&&(cur_y>=InvObl[1]+InvY)&&(cur_x<=InvObl[2]+InvX)&&(cur_y<=InvObl[3]+InvY))
	{
		int IB=0;
		int pos_cur=(cur_y-(InvObl[1]+InvY))/HeightItem;
		for(dyn_map::iterator it=lpChosen->obj.begin();it!=lpChosen->obj.end();it++)
		{
			if(IB-scroll_items==pos_cur)
			{
				lpChosen->m_obj=(*it).second;
				lpChosen->obj.erase(it);
				break;
			}
			IB++;
		}
		if(lpChosen->m_obj)
			cur_hold=inv_pic_b[lpChosen->m_obj->object->p[OBJ_PIC_INV]];
		return 1;
	}
	//слот 1
	if(cur_x>=InvSlot1[0]+InvX && cur_y>=InvSlot1[1]+InvY && cur_x<=InvSlot1[2]+InvX && cur_y<=InvSlot1[3]+InvY && lpChosen->IsFree())
	{
		if(!lpChosen->a_obj->object->p[OBJ_HIDDEN])
		{
			if(lpChosen->a_obj->object->type==OBJ_TYPE_WEAPON)
				lpChosen->Action(4,lpChosen->a_obj->object->p[OBJ_TIME_HIDE]);
			else
				lpChosen->Action(ANIM2_USE,lpChosen->a_obj->object->p[OBJ_TIME_HIDE]);

			Net_SendChangeObject(0,1);
			
			lpChosen->m_obj=lpChosen->a_obj;
			lpChosen->a_obj=&lpChosen->def_obj1;
			lpChosen->rate_object=1;

			lpChosen->RefreshWeap();

			if(lpChosen->m_obj)
				cur_hold=inv_pic_b[lpChosen->m_obj->object->p[OBJ_PIC_INV]];

			return 1;
		}
	}
	//слот 2
/*	if((cur_x>=InvSlot2[0]+InvX)&&(cur_y>=InvSlot2[1]+InvY)&&(cur_x<=InvSlot2[2]+InvX)&&(cur_y<=InvSlot2[3]+InvY))
	{
		if(lpChosen->a_obj2)
		{
			lpChosen->m_obj=lpChosen->a_obj2;
			lpChosen->a_obj2=NULL;
		}
		if(lpChosen->m_obj)
			cur_hold=inv_pic_b[lpChosen->m_obj->object->p[9]];
		return 1;
	}*///!@!
	//слот армор	
	if(cur_x>=InvArmor[0]+InvX && cur_y>=InvArmor[1]+InvY && cur_x<=InvArmor[2]+InvX && cur_y<=InvArmor[3]+InvY && lpChosen->IsFree())
	{
		if(!lpChosen->a_obj_arm->object->p[OBJ_HIDDEN])
		{
			lpChosen->Action(ANIM2_USE,lpChosen->a_obj_arm->object->p[OBJ_TIME_HIDE]);

			lpChosen->m_obj=lpChosen->a_obj_arm;
			lpChosen->a_obj_arm=&lpChosen->def_obj2;

			Net_SendChangeObject(0,2);

			if(lpChosen->m_obj)
				cur_hold=inv_pic_b[lpChosen->m_obj->object->p[OBJ_PIC_INV]];

			return 1;
		}
	}

	if((cur_x>=InvBtnUp[0]+InvX)	&&(cur_y>=InvBtnUp[1]+InvY)		&&(cur_x<=InvBtnUp[2]+InvX)		&&(cur_y<=InvBtnUp[3]+InvY))	{ InvHold=1; return 1; }
	if((cur_x>=InvBtnDown[0]+InvX)	&&(cur_y>=InvBtnDown[1]+InvY)	&&(cur_x<=InvBtnDown[2]+InvX)	&&(cur_y<=InvBtnDown[3]+InvY))	{ InvHold=2; return 1; }
	if((cur_x>=InvBtnOk[0]+InvX)	&&(cur_y>=InvBtnOk[1]+InvY)		&&(cur_x<=InvBtnOk[2]+InvX)		&&(cur_y<=InvBtnOk[3]+InvY))	{ InvHold=3; return 1; }

	if((cur_x>=InvMain[0]+InvX)&&(cur_y>=InvMain[1]+InvY)&&(cur_x<=InvMain[2]+InvX)&&(cur_y<=InvMain[3]+InvY))
	{ InvHold=4; invvectx=cur_x-InvX; invvecty=cur_y-InvY; return 1; }

return 0;
}

int CFEngine::InvMouseUp()
{
	if(lpChosen->m_obj)
	{
	//слот1
		if(cur_x>=InvSlot1[0]+InvX && cur_y>=InvSlot1[1]+InvY && cur_x<=InvSlot1[2]+InvX && cur_y<=InvSlot1[3]+InvY && lpChosen->m_obj->object->type!=OBJ_TYPE_ARMOR && lpChosen->a_obj->object->id<500 && lpChosen->IsFree()) 
		{
			lpChosen->a_obj=lpChosen->m_obj;
			lpChosen->RefreshWeap();
			lpChosen->m_obj=NULL;
			cur_hold=0;
			lpChosen->rate_object=1;

			if(lpChosen->a_obj->object->type==OBJ_TYPE_WEAPON)
				lpChosen->Action(3,lpChosen->a_obj->object->p[OBJ_TIME_SHOW]);
			else
				lpChosen->Action(ANIM2_USE,lpChosen->a_obj->object->p[OBJ_TIME_SHOW]);

			Net_SendChangeObject(lpChosen->a_obj->id,1);
			return 1;
		}
/*		//слот2
		if((cur_x>=InvSlot2[0]+InvX)&&(cur_y>=InvSlot2[1]+InvY)&&(cur_x<=InvSlot2[2]+InvX)&&(cur_y<=InvSlot2[3]+InvY)&&(!lpChosen->m_obj->object->p[2])&&(!lpChosen->a_obj2)) 
		{
			lpChosen->a_obj2=lpChosen->m_obj;
			lpChosen->m_obj=NULL;
			cur_hold=0;
			return 1;
		}*///!@!
	//слот армор
		if(cur_x>=InvArmor[0]+InvX && cur_y>=InvArmor[1]+InvY && cur_x<=InvArmor[2]+InvX && cur_y<=InvArmor[3]+InvY && lpChosen->m_obj->object->type==OBJ_TYPE_ARMOR && lpChosen->a_obj_arm->object->id<500 && lpChosen->IsFree()) 
		{
			lpChosen->a_obj_arm=lpChosen->m_obj;

			lpChosen->Action(ANIM2_USE,lpChosen->a_obj_arm->object->p[OBJ_TIME_SHOW]);

			lpChosen->m_obj=NULL;
			cur_hold=0;

			Net_SendChangeObject(lpChosen->a_obj_arm->id,2);
			return 1;
		}
		if((cur_x<InvMain[0]+InvX || cur_y<InvMain[1]+InvY || cur_x>InvMain[2]+InvX || cur_y>InvMain[3]+InvY) && lpChosen->IsFree()) 
		{
			Net_SendChangeObject(lpChosen->m_obj->id,3);

			hf.AddObj(lpChosen->m_obj->object,lpChosen->hex_x,lpChosen->hex_y,0);

			lpChosen->Action(ANIM2_SIT,ACTTIME_DROP_OBJ);

			lpChosen->m_obj=NULL;
			cur_hold=0;
			return 1;
		}
	//инвертарь
		if(lpChosen->m_obj) //все что нипопало никуда - поподает в инвентарь
		{
			lpChosen->obj[lpChosen->m_obj->id]=lpChosen->m_obj;
			lpChosen->m_obj=NULL;
			cur_hold=0;
			return 1;
		}
	}

	if((cur_x>=InvBtnUp[0]+InvX)&&(cur_y>=InvBtnUp[1]+InvY)&&(cur_x<=InvBtnUp[2]+InvX)&&(cur_y<=InvBtnUp[3]+InvY)&&(InvHold==1))
		if(scroll_items>0) scroll_items--;

	int count_items=lpChosen->obj.size();
	if((cur_x>=InvBtnDown[0]+InvX)&&(cur_y>=InvBtnDown[1]+InvY)&&(cur_x<=InvBtnDown[2]+InvX)&&(cur_y<=InvBtnDown[3]+InvY)&&(InvHold==2))
		if(scroll_items<count_items-(InvObl[3]-InvObl[1])/HeightItem) scroll_items++;

	if((cur_x>=InvBtnOk[0]+InvX)&&(cur_y>=InvBtnOk[1]+InvY)&&(cur_x<=InvBtnOk[2]+InvX)&&(cur_y<=InvBtnOk[3]+InvY)&&(InvHold==3))
		SetScreen(SCREEN_MAIN);

//	if(InvHold==4)
//	{
//		InvX=cur_x-invvectx; InvY=cur_y-invvecty;
//	}

	cur_hold=0;
	InvHold=0;

	return 1;
}

void CFEngine::InvMouseMove()
{
	if(InvHold!=4) return;

	InvX=cur_x-invvectx;
	InvY=cur_y-invvecty;

	if(InvX<0) InvX=0;
	if(InvX+InvMain[2]>MODE_WIDTH) InvX=MODE_WIDTH-InvMain[2];
	if(InvY<0) InvY=0;
	if(InvY+InvMain[3]>IntY) InvY=IntY-InvMain[3];
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::IntDraw()
{
	if(edit_mode) sm.DrawSprite(panel,IntX+5,IntY-40,COLOR_DEFAULT);

	sm.DrawSprite(ifacen,IntX,IntY,COLOR_DEFAULT);

	if(IntHold==1) sm.DrawSprite(intscrupon,IntBScrUp[0],IntBScrUp[1],COLOR_DEFAULT);
	if(IntHold==2) sm.DrawSprite(intscrdownon,IntBScrDown[0],IntBScrDown[1],COLOR_DEFAULT);
	if(IntHold==3) sm.DrawSprite(intchangesloton,IntBChangeSlot[0],IntBChangeSlot[1],COLOR_DEFAULT);
	if(IntHold==4) sm.DrawSprite(intinvon,IntBInv[0],IntBInv[1],COLOR_DEFAULT);
	if(IntHold==5) sm.DrawSprite(intmenuon,IntBMenu[0],IntBMenu[1],COLOR_DEFAULT);
	if(IntHold==6) sm.DrawSprite(intskillon,IntBSkill[0],IntBSkill[1],COLOR_DEFAULT);
	if(IntHold==7) sm.DrawSprite(intmapon,IntBMap[0],IntBMap[1],COLOR_DEFAULT);
	if(IntHold==8) sm.DrawSprite(intinfoon,IntBInfo[0],IntBInfo[1],COLOR_DEFAULT);
	if(IntHold==9) sm.DrawSprite(intpipon,IntBPip[0],IntBPip[1],COLOR_DEFAULT);

	if(lpChosen->a_obj->object->p[OBJ_PIC_INV])
	{
		SpriteInfo* si=sm.GetSpriteInfo(inv_pic_b[lpChosen->a_obj->object->p[OBJ_PIC_INV]]);
		sm.DrawSprite(inv_pic_b[lpChosen->a_obj->object->p[OBJ_PIC_INV]],
			IntObject[0]+(IntObject[2]-IntObject[0]-si->w)/2,
			IntObject[1]+(IntObject[3]-IntObject[1]-si->h)/2,COLOR_DEFAULT);
	}

	if(lpChosen->a_obj->object->type==OBJ_TYPE_WEAPON)
	{
		switch(lpChosen->rate_object)
		{
		case 1: sm.DrawSprite(pic_use[lpChosen->a_obj->object->p[OBJ_WEAP_PA_PIC]],IntObject[0]+10,IntObject[1]+10,COLOR_DEFAULT); break;
		case 2: sm.DrawSprite(pic_use[lpChosen->a_obj->object->p[OBJ_WEAP_SA_PIC]],IntObject[0]+10,IntObject[1]+10,COLOR_DEFAULT); break;
		case 3: sm.DrawSprite(pic_use[lpChosen->a_obj->object->p[OBJ_WEAP_TA_PIC]],IntObject[0]+10,IntObject[1]+10,COLOR_DEFAULT); break;
		default: sm.DrawSprite(pic_use[10],IntObject[0]+5,IntObject[1],COLOR_DEFAULT); break;
		}
	}
	else
		sm.DrawSprite(pic_use[10],IntObject[0]+5,IntObject[1],COLOR_DEFAULT);

	//диодики
	if(!lpChosen->IsFree())
	{
		sm.DrawSprite(diodeG,IntAP[0],IntAP[1],COLOR_DEFAULT);
		int timed=lpChosen->Tick_count-(GetTickCount()-lpChosen->Tick_start);
		for(int num_d=1; num_d<=20; num_d++)
		{
			if(num_d<=timed/500 && num_d<=14)
				sm.DrawSprite(diodeG,IntAP[0]+num_d*IntAPstepX,IntAP[1]+num_d*IntAPstepY,COLOR_DEFAULT);
			else
			if(num_d>14 && num_d<=17 && num_d-15<=(timed-7500)/3000 && timed>7500)
				sm.DrawSprite(diodeY,IntAP[0]+num_d*IntAPstepX,IntAP[1]+num_d*IntAPstepY,COLOR_DEFAULT);
			else
			if(num_d>17 && num_d<20 && num_d-18<=(timed-16500)/10000 && timed>16500)
				sm.DrawSprite(diodeR,IntAP[0]+num_d*IntAPstepX,IntAP[1]+num_d*IntAPstepY,COLOR_DEFAULT);
		}
	}

	//дефолтное действие
	if(Node_stay_mouse) sm.DrawSprite(Node_stay_mouse,cur_x+22,cur_y+21,COLOR_DEFAULT);

	//мессаги чезена
	int max_chosen_mess=0;
	if(lpChosen->pe[PE_HIDE_MODE]) max_chosen_mess++;

	for(int i=0;i<max_chosen_mess;++i)
		sm.DrawSprite(chosen_mess_pic,IntMessX+IntMessStepX*i,IntMessY+IntMessStepY*i,COLOR_DEFAULT);

	sm.Flush();

	//текст мессаг чезена
	int cur_chosen_mess=0;
	RECT rcm={IntMess[0],IntMess[1],IntMess[2],IntMess[3]};

	if(lpChosen->pe[PE_HIDE_MODE])
	{
		rcm.left=IntMessX+IntMessStepX*cur_chosen_mess;
		rcm.right=IntMessX+IntMessStepX*cur_chosen_mess+IntMess[2];
		rcm.top=IntMessY+IntMessStepY*cur_chosen_mess;
		rcm.bottom=IntMessY+IntMessStepY*cur_chosen_mess+IntMess[3];
		cur_chosen_mess++;
		fnt.MyDrawText(rcm,"Скрытен",FT_CENTERX|FT_CENTERY);
	}

	//HP, AC
	RECT rhp={IntHP[0],IntHP[1],IntHP[2],IntHP[3]};
	RECT rac={IntAC[0],IntAC[1],IntAC[2],IntAC[3]};
	char bin_str[5];

	sprintf(bin_str,"%03d",lpChosen->st[ST_CURRENT_HP]);
	if((lpChosen->st[ST_CURRENT_HP]*100)/lpChosen->st[ST_MAX_LIFE]<=20)
		StrChangeValue(&bin_str[0],0x20);
	else if((lpChosen->st[ST_CURRENT_HP]*100)/lpChosen->st[ST_MAX_LIFE]<=40)
		StrChangeValue(&bin_str[0],0x10);
	fnt.MyDrawText(rhp,bin_str,0,COLOR_DEFAULT,FONT_NUM);

	sprintf(bin_str,"%03d",lpChosen->st[ST_ARMOR_CLASS]);
	fnt.MyDrawText(rac,bin_str,0,COLOR_DEFAULT,FONT_NUM);

	//мессбох
	MessBoxDrawText();

	if(edit_mode)
	{
		RECT r3={IntX+15,IntY-40,IntX+800,IntY-16};
		char str_to_edit[2048];
		static bool show_cur=true;
		static TICK show_cur_last_time=GetTickCount();

		if(GetTickCount()>show_cur_last_time+400)
		{
			show_cur=!show_cur;
			show_cur_last_time=GetTickCount();
		}

		str_to_edit[0]=0;
		strcpy(str_to_edit,ed_str);
		if(show_cur)
			str_to_edit[cur_edit]='|';
		else
			str_to_edit[cur_edit]='.';

		str_to_edit[cur_edit+1]=0;
		strcat(str_to_edit,&ed_str[cur_edit]);
		fnt.MyDrawText(r3,str_to_edit,FT_NOBREAK|FT_BOTTOM);
	}
}

int CFEngine::IntMouseDown()
{
	IntHold=0;

	if(!lpChosen->IsFree()) return 0;

	if((cur_x>=IntObject[0])		&&(cur_y>=IntObject[1])			&&(cur_x<=IntObject[2])			&&(cur_y<=IntObject[3]))		{ IntHold=10; return 1;}

	if(!IsCur(CUR_DEFAULT) && !IsCur(CUR_MOVE)) return 0;

	if((cur_x>=IntBScrUp[0])		&&(cur_y>=IntBScrUp[1])			&&(cur_x<=IntBScrUp[2])			&&(cur_y<=IntBScrUp[3]))		{ IntHold=1; return 1;}
	if((cur_x>=IntBScrDown[0])		&&(cur_y>=IntBScrDown[1])		&&(cur_x<=IntBScrDown[2])		&&(cur_y<=IntBScrDown[3]))		{ IntHold=2; return 1;}
	if((cur_x>=IntBChangeSlot[0])	&&(cur_y>=IntBChangeSlot[1])	&&(cur_x<=IntBChangeSlot[2])	&&(cur_y<=IntBChangeSlot[3]))	{ IntHold=3; return 1;}
	if((cur_x>=IntBInv[0])			&&(cur_y>=IntBInv[1])			&&(cur_x<=IntBInv[2])			&&(cur_y<=IntBInv[3]))			{ IntHold=4; return 1;}
	if((cur_x>=IntBMenu[0])			&&(cur_y>=IntBMenu[1])			&&(cur_x<=IntBMenu[2])			&&(cur_y<=IntBMenu[3]))			{ IntHold=5; return 1;}
	if((cur_x>=IntBSkill[0])		&&(cur_y>=IntBSkill[1])			&&(cur_x<=IntBSkill[2])			&&(cur_y<=IntBSkill[3]))		{ IntHold=6; return 1;}
	if((cur_x>=IntBMap[0])			&&(cur_y>=IntBMap[1])			&&(cur_x<=IntBMap[2])			&&(cur_y<=IntBMap[3]))			{ IntHold=7; return 1;}
	if((cur_x>=IntBInfo[0])			&&(cur_y>=IntBInfo[1])			&&(cur_x<=IntBInfo[2])			&&(cur_y<=IntBInfo[3]))			{ IntHold=8; return 1;}
	if((cur_x>=IntBPip[0])			&&(cur_y>=IntBPip[1])			&&(cur_x<=IntBPip[2])			&&(cur_y<=IntBPip[3]))			{ IntHold=9; return 1;}	
	if((cur_x>=IntMain[0])			&&(cur_y>=IntMain[1])			&&(cur_x<=IntMain[2])			&&(cur_y<=IntMain[3]))			{ return 1;}

	if(!IsLMenu() && IsCur(CUR_DEFAULT))
	{
		if((curTargetCrit=GetMouseCritter(cur_x,cur_y)))
		{
			IntHold=11;
			return 1;
		}
		else if((curTargetObj=GetMouseItem(cur_x,cur_y)))
		{
			IntHold=12;
			return 1;
		}
	}

	return 0;
}

int CFEngine::IntMouseUp()
{
	if(cur_x>=IntBScrUp[0] && cur_y>=IntBScrUp[1] && cur_x<=IntBScrUp[2] && cur_y<=IntBScrUp[3] && IntHold==1 )
	{
		if(opt_msgbox_invert && scr_mess>0) scr_mess--;
		if(!opt_msgbox_invert && scr_mess<max_mess-1) scr_mess++;
	}
	if(cur_x>=IntBScrDown[0] && cur_y>=IntBScrDown[1] && cur_x<=IntBScrDown[2] && cur_y<=IntBScrDown[3] && IntHold==2 )
	{
		if(opt_msgbox_invert && scr_mess<max_mess-1) scr_mess++;
		if(!opt_msgbox_invert && scr_mess>0) scr_mess--;
	}
	if(cur_x>=IntBChangeSlot[0] && cur_y>=IntBChangeSlot[1] && cur_x<=IntBChangeSlot[2] && cur_y<=IntBChangeSlot[3] && IntHold==3 && lpChosen->IsFree())
	{
/*		dyn_obj* temp_obj;
		temp_obj=lpChosen->a_obj2;
		lpChosen->a_obj2=lpChosen->a_obj;
		lpChosen->a_obj=temp_obj;

		if(temp_obj)
		{
			if(temp_obj->object->weap>1)
				lpChosen->ShowObject(temp_obj->object->weap);
			else
				if(lpChosen->a_obj2)
					if(lpChosen->a_obj2->object->weap>1)
						lpChosen->HideObject(lpChosen->a_obj2->object->weap);
		}
		else
		{
			if(lpChosen->a_obj2)
				if(lpChosen->a_obj2->object->weap>1)
					lpChosen->HideObject(lpChosen->a_obj2->object->weap);
		}*/
		}
	if((cur_x>=IntBInv[0])&&(cur_y>=IntBInv[1])&&(cur_x<=IntBInv[2])&&(cur_y<=IntBInv[3])&&(IntHold==4))
	{ SetScreen(SCREEN_INVENTORY); }
	if((cur_x>=IntBMenu[0])&&(cur_y>=IntBMenu[1])&&(cur_x<=IntBMenu[2])&&(cur_y<=IntBMenu[3])&&(IntHold==5))
	{ SetScreen(SCREEN_MENU_OPTION); }
	if((cur_x>=IntBSkill[0])&&(cur_y>=IntBSkill[1])&&(cur_x<=IntBSkill[2])&&(cur_y<=IntBSkill[3])&&(IntHold==6))
	{ SetScreen(SCREEN_SKILLBOX); }
	if((cur_x>=IntBMap[0])&&(cur_y>=IntBMap[1])&&(cur_x<=IntBMap[2])&&(cur_y<=IntBMap[3])&&(IntHold==7))
	{ SetScreen(SCREEN_LOCAL_MAP); }
	if((cur_x>=IntBInfo[0])&&(cur_y>=IntBInfo[1])&&(cur_x<=IntBInfo[2])&&(cur_y<=IntBInfo[3])&&(IntHold==8))
	{ SetScreen(SCREEN_GLOBAL_MAP); }
	if((cur_x>=IntBPip[0])&&(cur_y>=IntBPip[1])&&(cur_x<=IntBPip[2])&&(cur_y<=IntBPip[3])&&(IntHold==9))
	{}
	if(cur_x>=IntObject[0] && cur_y>=IntObject[1] && cur_x<=IntObject[2] && cur_y<=IntObject[3] && IntHold==10 && lpChosen->IsFree())
	{ SetChosenAction(ACTION_ACT_UNACT_OBJECT); }

	if(!IsLMenu() && IntHold==11 && curTargetCrit)
	{
		if(curTargetCrit->id>=USERS_START_ID)
		{
			if(curTargetCrit==lpChosen)
				AddMess(COLOR_TEXT_DEFAULT,"Вы разглядываете свою пиписку...");
			else
				AddMess(COLOR_TEXT_DEFAULT,"Это %s.",curTargetCrit->name);
		}
		else if(curTargetCrit->id>=NPC_MIN_ID && curTargetCrit->id<=NPC_MAX_ID)
		{
			tosendTargetCrit=curTargetCrit;
			SetChosenAction(ACTION_TALK_NPC);
		}
	}

	if(!IsLMenu() && IntHold==12 && curTargetObj)
	{
		tosendTargetObj=curTargetObj;
		SetChosenAction(ACTION_PICK_OBJ);
	}

	IntHold=0;

	return 1;
}

void CFEngine::IntMouseMove()
{
	static DWORD mouse_stay=GetTickCount();
	static int x_stay=cur_x;
	static int y_stay=cur_y;

	if(!IsLMenu() && IsCur(CUR_DEFAULT) && !IntHold && (cur_x<IntMain[0] || cur_y<IntMain[1] || cur_x>IntMain[2] || cur_y>IntMain[3]))
	{
		if(cur_x!=x_stay || cur_y!=y_stay)
		{
			x_stay=cur_x;
			y_stay=cur_y;
			mouse_stay=GetTickCount();
			Node_stay_mouse=NULL;
		}
		else if((GetTickCount()-mouse_stay)>LMENU_SHOW_TIME)
		{
			if((curTargetCrit=GetMouseCritter(cur_x,cur_y)))
			{
				if(curTargetCrit->id>=NPC_MIN_ID && curTargetCrit->id<=NPC_MAX_ID)
					Node_stay_mouse=lm_talk_off;
				else
					Node_stay_mouse=lm_look_off;
			}
			else if((curTargetObj=GetMouseItem(cur_x,cur_y)))
			{
				Node_stay_mouse=lm_use_off;
			}
			else
				Node_stay_mouse=NULL;
		}
	}
	else
		Node_stay_mouse=NULL;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::AddMess(DWORD text_color,char* message_text, ...)
{
	if(++max_mess>=MAX_MESS)
	{
		max_mess=1;
		scr_mess=0;
	}
	char temp_str[MAX_TEXT+128];

	va_list list;
	va_start(list, message_text);
	wvsprintf(temp_str,message_text,list);
	va_end(list);


	sprintf(all_mess[max_mess],"|%d - %s\n",text_color,temp_str);
}

void CFEngine::MessBoxDrawText()
{
	if(!max_mess) return;

	char str[MAX_MESSBOX];

	RECT r0={0,0,0,0};

	if(!IsScreen(SCREEN_GLOBAL_MAP))
	{
		r0.left=IntTXT[0];
		r0.top=IntTXT[1];
		r0.right=IntTXT[2];
		r0.bottom=IntTXT[3];
	}
	else
	{
		r0.left=GmapWChat[0];
		r0.top=GmapWChat[1];
		r0.right=GmapWChat[2];
		r0.bottom=GmapWChat[3];
	}

	str[0]=0;

	if(opt_msgbox_invert==TRUE)
	{
		for(int m=0; m<MAX_MESS_IN_MESSBOX; m++)
		{
			if(max_mess-1-scr_mess-m<0) break;
			strcat(str,all_mess[max_mess-scr_mess-m]);
		}
		str[strlen(str)-1]=0;

		fnt.MyDrawText(r0,str,FT_COLORIZE);
	}
	else
	{
		for(int m=MAX_MESS_IN_MESSBOX; m>=0; m--)
		{
			if(m>max_mess-1-scr_mess) continue;

		//	if(max_mess-1-scr_mess-m<0) continue;
			strcat(str,all_mess[max_mess-scr_mess-m]);
		}
		str[strlen(str)-1]=0;

		fnt.MyDrawText(r0,str,FT_UPPER|FT_BOTTOM|FT_COLORIZE);
	}
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::ShowLogIn()
{
	lpDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	lpDevice->BeginScene();
	//графика
	sm.DrawSprite(loginpic,LogX,LogY,COLOR_DEFAULT);

	sm.Flush();
	//текст
	RECT rlogin	={LogWLogin[0],LogWLogin[1],LogWLogin[2],LogWLogin[3]};
	RECT rpass	={LogWPass[0] ,LogWPass[1] ,LogWPass[2] ,LogWPass[3] };

	if(LogFocus==1)
		fnt.MyDrawText(rlogin,opt_login	,FT_CENTERX|FT_NOBREAK,D3DCOLOR_XRGB(0,255,0));
	else
		fnt.MyDrawText(rlogin,opt_login	,FT_CENTERX|FT_NOBREAK,D3DCOLOR_XRGB(0,100,0));
	if(LogFocus==2)
		fnt.MyDrawText(rpass,opt_pass	,FT_CENTERX|FT_NOBREAK,D3DCOLOR_XRGB(0,255,0));
	else
		fnt.MyDrawText(rpass,opt_pass	,FT_CENTERX|FT_NOBREAK,D3DCOLOR_XRGB(0,100,0));

	RECT rlogmess={0,0,MODE_WIDTH,15};
	// TODO[30.9.2012 alex]: fix it
	//fnt.MyDrawText(rlogmess,LoginMess[LogMsg],0,D3DCOLOR_XRGB(255,0,0));

	RECT rlogconn={0,15,MODE_WIDTH,30};
	switch (state)
	{
	case STATE_CONN:		fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: СОЕДИНЕНИЕ"				,0,D3DCOLOR_XRGB(0,0,255)); break;
	case STATE_DROPLINK:	fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: ОБРЫВ СВЯЗИ"			,0,D3DCOLOR_XRGB(0,0,255)); break;
	case STATE_DISCONNECT:	fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: ОТСОЕДИНЕН"				,0,D3DCOLOR_XRGB(0,0,255)); break;
	case STATE_GAME:		fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: В ИГРЕ"					,0,D3DCOLOR_XRGB(0,0,255)); break;
	case STATE_LOGINOK:		fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: АУНТЕФИКАЦИЯ ПРОЙДЕНА"	,0,D3DCOLOR_XRGB(0,0,255)); break;
	case STATE_INIT_NET:	fnt.MyDrawText(rlogconn,"СОСТОЯНИЕ: ИНИЦИАЛИЗАЦИЯ СЕТИ"		,0,D3DCOLOR_XRGB(0,0,255)); break;
	}
	//курсор
	SpriteInfo* si=sm.GetSpriteInfo(cur);
	int curx=cur_x-(si->w >> 1)+si->offs_x;
	int cury=cur_y-si->h+si->offs_y;
	sm.DrawSprite(cur,curx,cury,COLOR_DEFAULT);

	sm.Flush();
	lpDevice->EndScene();
	lpDevice->Present(NULL,NULL,NULL,NULL);
}

void CFEngine::LogInput()
{
	DIDEVICEOBJECTDATA didod[DI_BUF_SIZE];
	DWORD dwElements;

    dwElements = DI_BUF_SIZE;
    HRESULT hr = lpKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("LogParseInput> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}

	for(int i=0;i<dwElements;i++) 
	{
		if(LogFocus==1)
		{
			int fnd=0;
			for(WORD tst=0;tst<DI_BUF_SIZE;tst++)
			{
				lang=LANG_ENG;
				DI_ONDOWN(tst, if(GetChar(tst,opt_login,NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
			}
			if(fnd) break;
		}
		if(LogFocus==2)
		{
			int fnd=0;
			for(WORD tst=0;tst<DI_BUF_SIZE;tst++)
			{
				lang=LANG_ENG;
				DI_ONDOWN(tst, if(GetChar(tst,opt_pass,NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
			}
			if(fnd) break;
		}

	}

	dwElements = DI_BUF_SIZE;
	hr = lpMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("LogParseInput mouse> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}
		
	for(DWORD i=0;i<dwElements;i++) 
	{
		DI_ONMOUSE(DIMOFS_X, cur_x+=didod[i].dwData*opt_mouse_speed );
		DI_ONMOUSE(DIMOFS_Y, cur_y+=didod[i].dwData*opt_mouse_speed );

		DI_ONDOWN(DIMOFS_BUTTON0,
			LogFocus=0;
			if(cur_x>=LogWLogin[0] && cur_y>=LogWLogin[1] && cur_x<=LogWLogin[2] && cur_y<=LogWLogin[3]) LogFocus=1;
			if(cur_x>=LogWPass[0] && cur_y>=LogWPass[1] && cur_x<=LogWPass[2] && cur_y<=LogWPass[3]) LogFocus=2;
			if(cur_x>=LogBOk[0] && cur_y>=LogBOk[1] && cur_x<=LogBOk[2] && cur_y<=LogBOk[3])
			{
				//прверка на длинну если < 4 символов то ничего не произойдет
				if(opt_login[3]==NULL) { LogMsg=1; return; }
				if(opt_pass[3]==NULL) { LogMsg=2; return; }
				//сохраняем логин пасс
				WritePrivateProfileString("LOGIN","login",opt_login,CFG_FILE);
				WritePrivateProfileString("LOGIN","pass",opt_pass,CFG_FILE);
				//соединяемся с сервером
				LogMsg=7;
				state=STATE_INIT_NET;

				SetCur(CUR_WAIT);
			}
			if(cur_x>=LogBReg[0] && cur_y>=LogBReg[1] && cur_x<=LogBReg[2] && cur_y<=LogBReg[3])
			{
				SetScreen(SCREEN_REGISTRATION);
			}
			if(cur_x>=LogBExit[0] && cur_y>=LogBExit[1] && cur_x<=LogBExit[2] && cur_y<=LogBExit[3])
			{
				TryExit();
			}
		);
	}
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::ShowRegistration()
{
	lpDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	lpDevice->BeginScene();
	//графика
	sm.DrawSprite(registpic,RegX,RegY,COLOR_DEFAULT);

	sm.Flush();
	//текст
//	RECT rwall={RegWAllS[0],RegWAllS[1],RegWAllS[2],RegWAllS[3]};
	RECT rws={RegWS[0],RegWS[1],RegWS[2],RegWS[3]};
	RECT rwp={RegWP[0],RegWP[1],RegWP[2],RegWP[3]};
	RECT rwe={RegWE[0],RegWE[1],RegWE[2],RegWE[3]};
	RECT rwc={RegWC[0],RegWC[1],RegWC[2],RegWC[3]};
	RECT rwi={RegWI[0],RegWI[1],RegWI[2],RegWI[3]};
	RECT rwa={RegWA[0],RegWA[1],RegWA[2],RegWA[3]};
	RECT rwl={RegWL[0],RegWL[1],RegWL[2],RegWL[3]};
	RECT rwlogin={RegWLogin[0],RegWLogin[1],RegWLogin[2],RegWLogin[3]};
	RECT rwpass={RegWPass[0],RegWPass[1],RegWPass[2],RegWPass[3]};
	RECT rwname={RegWName[0],RegWName[1],RegWName[2],RegWName[3]};
	RECT rwcases0={RegWCases0[0],RegWCases0[1],RegWCases0[2],RegWCases0[3]};
	RECT rwcases1={RegWCases1[0],RegWCases1[1],RegWCases1[2],RegWCases1[3]};
	RECT rwcases2={RegWCases2[0],RegWCases2[1],RegWCases2[2],RegWCases2[3]};
	RECT rwcases3={RegWCases3[0],RegWCases3[1],RegWCases3[2],RegWCases3[3]};
	RECT rwcases4={RegWCases4[0],RegWCases4[1],RegWCases4[2],RegWCases4[3]};
	RECT rwbtype={RegWBType[0],RegWBType[1],RegWBType[2],RegWBType[3]};
	RECT rwgender={RegWGender[0],RegWGender[1],RegWGender[2],RegWGender[3]};
	RECT rwage={RegWAge[0],RegWAge[1],RegWAge[2],RegWAge[3]};

	if(RegFocus==1) fnt.MyDrawText(rwlogin,New_cr.login,0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwlogin,New_cr.login,0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==2) fnt.MyDrawText(rwpass,New_cr.pass,0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwpass,New_cr.pass,0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==3) fnt.MyDrawText(rwname,New_cr.name,0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwname,New_cr.name,0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==4) fnt.MyDrawText(rwcases0,New_cr.cases[0],0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwcases0,New_cr.cases[0],0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==5) fnt.MyDrawText(rwcases1,New_cr.cases[1],0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwcases1,New_cr.cases[1],0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==6) fnt.MyDrawText(rwcases2,New_cr.cases[2],0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwcases2,New_cr.cases[2],0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==7) fnt.MyDrawText(rwcases3,New_cr.cases[3],0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwcases3,New_cr.cases[3],0,D3DCOLOR_XRGB(0,255,0));
	if(RegFocus==8) fnt.MyDrawText(rwcases4,New_cr.cases[4],0,D3DCOLOR_XRGB(0,0,255));
		else fnt.MyDrawText(rwcases4,New_cr.cases[4],0,D3DCOLOR_XRGB(0,255,0));

	char stradd[20];
	sprintf(stradd,"%d",New_cr.st[ST_STRENGHT	]);
	fnt.MyDrawText(rws,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_PERCEPTION	]);
	fnt.MyDrawText(rwp,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_ENDURANCE	]);
	fnt.MyDrawText(rwe,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_CHARISMA	]);
	fnt.MyDrawText(rwc,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_INTELLECT	]);
	fnt.MyDrawText(rwi,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_AGILITY	]);
	fnt.MyDrawText(rwa,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_LUCK		]);
	fnt.MyDrawText(rwl,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.base_type);
	fnt.MyDrawText(rwbtype,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_GENDER		]);
	fnt.MyDrawText(rwgender,stradd,0,D3DCOLOR_XRGB(0,255,0));
	sprintf(stradd,"%d",New_cr.st[ST_AGE		]);
	fnt.MyDrawText(rwage,stradd,0,D3DCOLOR_XRGB(0,255,0));

	RECT rlogmess={0,0,MODE_WIDTH,15};
	fnt.MyDrawText(rlogmess,LoginMess[LogMsg],0,D3DCOLOR_XRGB(255,0,0));
	//курсор
	CurDraw();

	sm.Flush();
	lpDevice->EndScene();
	lpDevice->Present(NULL,NULL,NULL,NULL);
}

void CFEngine::RegInput()
{
	DIDEVICEOBJECTDATA didod[DI_BUF_SIZE];
	DWORD dwElements;

    dwElements = DI_BUF_SIZE;
    HRESULT hr = lpKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("RegParseInput> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}

	for(int i=0;i<dwElements;i++) 
	{
		int fnd=0;
		for(WORD tst=0;tst<DI_BUF_SIZE;tst++)
		{
			switch (RegFocus)
			{
			case 1: //login
				lang=LANG_ENG; DI_ONDOWN(tst, if(GetChar(tst,New_cr.login,NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 2: //pass
				lang=LANG_ENG; DI_ONDOWN(tst, if(GetChar(tst,New_cr.pass,NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 3: //name
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.name,NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 4: //cases0
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.cases[0],NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 5: //cases1
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.cases[1],NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 6: //cases2
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.cases[2],NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 7: //cases3
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.cases[3],NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			case 8: //cases4
				lang=LANG_RUS; DI_ONDOWN(tst, if(GetChar(tst,New_cr.cases[4],NULL,MAX_LOGIN,lang,ShiftDwn)) {fnd=1;break;});
				break;
			}
		}
		if(fnd) break;
	}

	dwElements = DI_BUF_SIZE;
	hr = lpMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("RegParseInput mouse> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}
		
	for(DWORD i=0;i<dwElements;i++) 
	{
		DI_ONMOUSE(DIMOFS_X, cur_x+=didod[i].dwData*opt_mouse_speed );
		DI_ONMOUSE(DIMOFS_Y, cur_y+=didod[i].dwData*opt_mouse_speed );

	//левая кнопка мыши
		DI_ONDOWN(DIMOFS_BUTTON0,
			RegFocus=0;
			//login
			if(cur_x>=RegWLogin[0] && cur_y>=RegWLogin[1] && cur_x<=RegWLogin[2] && cur_y<=RegWLogin[3]) RegFocus=1;
			//pass
			if(cur_x>=RegWPass[0] && cur_y>=RegWPass[1] && cur_x<=RegWPass[2] && cur_y<=RegWPass[3]) RegFocus=2;
			//name
			if(cur_x>=RegWName[0] && cur_y>=RegWName[1] && cur_x<=RegWName[2] && cur_y<=RegWName[3]) RegFocus=3;
			//cases0
			if(cur_x>=RegWCases0[0] && cur_y>=RegWCases0[1] && cur_x<=RegWCases0[2] && cur_y<=RegWCases0[3]) RegFocus=4;
			//cases1
			if(cur_x>=RegWCases1[0] && cur_y>=RegWCases1[1] && cur_x<=RegWCases1[2] && cur_y<=RegWCases1[3]) RegFocus=5;
			//cases2
			if(cur_x>=RegWCases2[0] && cur_y>=RegWCases2[1] && cur_x<=RegWCases2[2] && cur_y<=RegWCases2[3]) RegFocus=6;
			//cases3
			if(cur_x>=RegWCases3[0] && cur_y>=RegWCases3[1] && cur_x<=RegWCases3[2] && cur_y<=RegWCases3[3]) RegFocus=7;
			//cases4
			if(cur_x>=RegWCases4[0] && cur_y>=RegWCases4[1] && cur_x<=RegWCases4[2] && cur_y<=RegWCases4[3]) RegFocus=8;
			//S
			if(cur_x>=RegWS[0] && cur_y>=RegWS[1] && cur_x<=RegWS[2] && cur_y<=RegWS[3]) if((++New_cr.st[ST_STRENGHT	])>10) New_cr.st[ST_STRENGHT	]=10;
			//P
			if(cur_x>=RegWP[0] && cur_y>=RegWP[1] && cur_x<=RegWP[2] && cur_y<=RegWP[3]) if((++New_cr.st[ST_PERCEPTION	])>10) New_cr.st[ST_PERCEPTION	]=10;
			//E
			if(cur_x>=RegWE[0] && cur_y>=RegWE[1] && cur_x<=RegWE[2] && cur_y<=RegWE[3]) if((++New_cr.st[ST_ENDURANCE	])>10) New_cr.st[ST_ENDURANCE	]=10;
			//C
			if(cur_x>=RegWC[0] && cur_y>=RegWC[1] && cur_x<=RegWC[2] && cur_y<=RegWC[3]) if((++New_cr.st[ST_CHARISMA	])>10) New_cr.st[ST_CHARISMA	]=10;
			//I
			if(cur_x>=RegWI[0] && cur_y>=RegWI[1] && cur_x<=RegWI[2] && cur_y<=RegWI[3]) if((++New_cr.st[ST_INTELLECT	])>10) New_cr.st[ST_INTELLECT	]=10;
			//A
			if(cur_x>=RegWA[0] && cur_y>=RegWA[1] && cur_x<=RegWA[2] && cur_y<=RegWA[3]) if((++New_cr.st[ST_AGILITY		])>10) New_cr.st[ST_AGILITY		]=10;
			//L
			if(cur_x>=RegWL[0] && cur_y>=RegWL[1] && cur_x<=RegWL[2] && cur_y<=RegWL[3]) if((++New_cr.st[ST_LUCK		])>10) New_cr.st[ST_LUCK		]=10;
			//gender
			if(cur_x>=RegWGender[0] && cur_y>=RegWGender[1] && cur_x<=RegWGender[2] && cur_y<=RegWGender[3]) if((++New_cr.st[ST_GENDER])>1) New_cr.st[ST_GENDER]=1;
			//age
			if(cur_x>=RegWAge[0] && cur_y>=RegWAge[1] && cur_x<=RegWAge[2] && cur_y<=RegWAge[3]) if((++New_cr.st[ST_AGE])>80) New_cr.st[ST_AGE]=14;
			//base_type
			if(cur_x>=RegWBType[0] && cur_y>=RegWBType[1] && cur_x<=RegWBType[2] && cur_y<=RegWBType[3]) if((++New_cr.base_type)>2) New_cr.base_type=2;
			//registration
			if(cur_x>=RegBReg[0] && cur_y>=RegBReg[1] && cur_x<=RegBReg[2] && cur_y<=RegBReg[3]) 
				if(CheckRegData(&New_cr))
				{
					state=STATE_INIT_NET;
					SetCur(CUR_WAIT);
				}
			//back
			if(cur_x>=RegBBack[0] && cur_y>=RegBBack[1] && cur_x<=RegBBack[2] && cur_y<=RegBBack[3]) SetScreen(SCREEN_LOGIN);
		);

	//правая кнопка мыши
		DI_ONDOWN(DIMOFS_BUTTON1,
			RegFocus=0;
			//S
			if(cur_x>=RegWS[0] && cur_y>=RegWS[1] && cur_x<=RegWS[2] && cur_y<=RegWS[3]) if((--New_cr.st[ST_STRENGHT	])<1) New_cr.st[ST_STRENGHT		]=1;
			//P
			if(cur_x>=RegWP[0] && cur_y>=RegWP[1] && cur_x<=RegWP[2] && cur_y<=RegWP[3]) if((--New_cr.st[ST_PERCEPTION	])<1) New_cr.st[ST_PERCEPTION	]=1;
			//E
			if(cur_x>=RegWE[0] && cur_y>=RegWE[1] && cur_x<=RegWE[2] && cur_y<=RegWE[3]) if((--New_cr.st[ST_ENDURANCE	])<1) New_cr.st[ST_ENDURANCE	]=1;
			//C
			if(cur_x>=RegWC[0] && cur_y>=RegWC[1] && cur_x<=RegWC[2] && cur_y<=RegWC[3]) if((--New_cr.st[ST_CHARISMA	])<1) New_cr.st[ST_CHARISMA		]=1;
			//I
			if(cur_x>=RegWI[0] && cur_y>=RegWI[1] && cur_x<=RegWI[2] && cur_y<=RegWI[3]) if((--New_cr.st[ST_INTELLECT	])<1) New_cr.st[ST_INTELLECT	]=1;
			//A
			if(cur_x>=RegWA[0] && cur_y>=RegWA[1] && cur_x<=RegWA[2] && cur_y<=RegWA[3]) if((--New_cr.st[ST_AGILITY		])<1) New_cr.st[ST_AGILITY		]=1;
			//L
			if(cur_x>=RegWL[0] && cur_y>=RegWL[1] && cur_x<=RegWL[2] && cur_y<=RegWL[3]) if((--New_cr.st[ST_LUCK		])<1) New_cr.st[ST_LUCK			]=1;
			//gender
			if(cur_x>=RegWGender[0] && cur_y>=RegWGender[1] && cur_x<=RegWGender[2] && cur_y<RegWGender[3]) if(New_cr.st[ST_GENDER]) New_cr.st[ST_GENDER]--;
			//age
			if(cur_x>=RegWAge[0] && cur_y>=RegWAge[1] && cur_x<=RegWAge[2] && cur_y<=RegWAge[3]) if((--New_cr.st[ST_AGE])<14) New_cr.st[ST_AGE]=80;
			//base_type
			if(cur_x>=RegWBType[0] && cur_y>=RegWBType[1] && cur_x<=RegWBType[2] && cur_y<=RegWBType[3]) if(New_cr.base_type) New_cr.base_type--;
		);
	}
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::DlgDraw()
{
	SpriteInfo* spr_inf=sm.GetSpriteInfo(dialog_begin);
	if(!spr_inf) return;

	sm.DrawSprite(dialog_begin,DlgX,DlgY,COLOR_DEFAULT);
	for(int ddg=0; ddg<all_answers; ddg++)
	{
		sm.DrawSprite(dialog_answ,DlgX+DlgNextAnswX*ddg,DlgY+spr_inf->h+DlgNextAnswY*ddg,COLOR_DEFAULT);
	}
	sm.DrawSprite(dialog_end,DlgX+DlgNextAnswX*all_answers,DlgY+spr_inf->h+DlgNextAnswY*all_answers,COLOR_DEFAULT);

	sm.Flush();

	RECT rmt={DlgText[0]+DlgX,DlgText[1]+DlgY,DlgText[2]+DlgX,DlgText[3]+DlgY};
	fnt.MyDrawText(rmt,text_dialog,0,D3DCOLOR_XRGB(0,200,200)); //текст нпц

	RECT rat={0,0,0,0};
	for(int ddt=0; ddt<all_answers; ddt++)
	{
		rat.left  =DlgAnsw[0]+DlgNextAnswX*ddt+DlgX;
		rat.top   =DlgAnsw[1]+DlgNextAnswY*ddt+DlgY;
		rat.right =DlgAnsw[2]+DlgNextAnswX*ddt+DlgX;
		rat.bottom=DlgAnsw[3]+DlgNextAnswY*ddt+DlgY;

		if(DlgHold==DLG_HOLD_ANSW)
		{
			if(ddt==DlgCurAnsw)
			{
				fnt.MyDrawText(rat,text_answer[ddt],0,D3DCOLOR_XRGB(0,0,220)); //выбор
			}
			else
				fnt.MyDrawText(rat,text_answer[ddt],0,D3DCOLOR_XRGB(0,150,0)); //обычный
		}
		else
		{
			if(ddt==DlgCurAnsw)
			{
				fnt.MyDrawText(rat,text_answer[ddt],0,D3DCOLOR_XRGB(0,240,0)); //подсветка
			}
			else
				fnt.MyDrawText(rat,text_answer[ddt],0,D3DCOLOR_XRGB(0,150,0)); //обычный
		}
	}
}

void CFEngine::DlgMouseMove()
{
	int dmm=0;

	switch (DlgHold)
	{
	case DLG_HOLD_NONE:
		for(dmm=0; dmm<all_answers; dmm++)
			if((cur_x>=DlgAnsw[0]+DlgNextAnswX*dmm+DlgX)&&(cur_y>=DlgAnsw[1]+DlgNextAnswY*dmm+DlgY)&&
				(cur_x<=DlgAnsw[2]+DlgNextAnswX*dmm+DlgX)&&(cur_y<=DlgAnsw[3]+DlgNextAnswY*dmm+DlgY))
			{
				DlgCurAnsw=dmm;
				return;
			} 
		DlgCurAnsw=-1;
		break;
	case DLG_HOLD_MAIN:
		DlgX=cur_x-dlgvectx;
		DlgY=cur_y-dlgvecty;

		if(DlgX<0) DlgX=0;
		if(DlgX+DlgMain[2]>MODE_WIDTH) DlgX=MODE_WIDTH-DlgMain[2];
		if(DlgY<0) DlgY=0;
		if(DlgY+DlgMain[3]>IntY) DlgY=IntY-DlgMain[3];
		break;
	case DLG_HOLD_ANSW:
		break;
	default:
		DlgHold=DLG_HOLD_NONE;
		break;
	}
}

void CFEngine::DlgMouseDown()
{
	DlgHold=DLG_HOLD_NONE;

	if((cur_x>=DlgAnsw[0]+DlgX)&&(cur_y>=DlgAnsw[1]+DlgY)&&
		(cur_x<=DlgAnsw[2]+DlgNextAnswX*all_answers+DlgX)&&(cur_y<=DlgAnsw[3]+DlgNextAnswY*all_answers+DlgY))
	{
		DlgHold=DLG_HOLD_ANSW;
		return;
	}

	if((cur_x>=DlgMain[0]+DlgX)&&(cur_y>=DlgMain[1]+DlgY)&&(cur_x<=DlgMain[2]+DlgX)&&(cur_y<=DlgMain[3]+DlgY))
	{
		dlgvectx=cur_x-DlgX;
		dlgvecty=cur_y-DlgY;
		DlgHold=DLG_HOLD_MAIN;
		return;
	}
	
	return;
}

void CFEngine::DlgMouseUp()
{
	if(DlgHold==DLG_HOLD_ANSW)
		if((DlgCurAnsw>=0)&&(DlgCurAnsw<all_answers))
			if((cur_x>=DlgAnsw[0]+DlgNextAnswX*DlgCurAnsw+DlgX)&&(cur_y>=DlgAnsw[1]+DlgNextAnswY*DlgCurAnsw+DlgY)&&
				(cur_x<=DlgAnsw[2]+DlgNextAnswX*DlgCurAnsw+DlgX)&&(cur_y<=DlgAnsw[3]+DlgNextAnswY*DlgCurAnsw+DlgY))
					Net_SendTalk(tosendTargetCrit->id,DlgCurAnsw);

	DlgCurAnsw=-1;
	DlgHold=DLG_HOLD_NONE;
}

int CFEngine::LoadDialogFromFile(CrID id_npc, DWORD id_dialog, char* dialog)
{
//определяем путь
	char path_text[64];
	sprintf(path_text,"%s%d.dlg",PATH_TEXT_FILES,id_npc);
//читаем тексты
	char key1[24];
	sprintf(key1,"%d",id_dialog);
	GetPrivateProfileString("dialogs",key1,"Err",dialog,MAX_DIALOG_TEXT,path_text);
	if(!stricmp(dialog,"Err")) return 0;
//считаем кол-во вариантов
	int all_texts=0;
	int find_char=0;
	for(find_char=0; find_char<strlen(dialog); find_char++)
		if(dialog[find_char]=='%' && dialog[find_char+1]=='%')
		{
			all_texts++;
			find_char++;
		}
//	if(!all_texts) return 1;
//выбираем рандомный текст
	int find_text=random((all_texts+1));
//создаем временный диалог
	char temp_dialog[MAX_DIALOG_TEXT+1];
	strcpy(temp_dialog,dialog);
//форматируем текст
	/*char* text_f;
	bool none=true;
	while(void)
	{
		if((text_f=strstr(temp_dialog,"@pname"))) {  }
	}*/
//выдергиваем текст
	int cur_text=0;
	int cur_char=0;
	for(find_char=0; find_char<strlen(temp_dialog); find_char++)
	{
		if(cur_text==find_text)
		{
			if(temp_dialog[find_char]=='%' && temp_dialog[find_char+1]=='%') break;

			if(temp_dialog[find_char]=='#') { dialog[cur_char++]='\n'; continue; }

			if(temp_dialog[find_char]=='@')
			{
				char cr_textf[MAX_NAME+1];
				sscanf(&temp_dialog[++find_char],"%s",&cr_textf);

				int cr_textf_cur=strlen(cr_textf);
				for(;;) { if(!cr_textf_cur) break; if(cr_textf[--cr_textf_cur]=='@') break; }
				if(!cr_textf_cur) continue;
				cr_textf[cr_textf_cur]=0;

				if(!stricmp(cr_textf,"pname")) { strcpy(&dialog[cur_char],lpChosen->name); find_char+=6; cur_char+=strlen(lpChosen->name); }
				else if(!stricmp(cr_textf,"pcases0")) { strcpy(&dialog[cur_char],lpChosen->cases[0]); find_char+=8; cur_char+=strlen(lpChosen->cases[0]); }
				else if(!stricmp(cr_textf,"pcases1")) { strcpy(&dialog[cur_char],lpChosen->cases[1]); find_char+=8; cur_char+=strlen(lpChosen->cases[0]); }
				else if(!stricmp(cr_textf,"pcases2")) { strcpy(&dialog[cur_char],lpChosen->cases[2]); find_char+=8; cur_char+=strlen(lpChosen->cases[0]); }
				else if(!stricmp(cr_textf,"pcases3")) { strcpy(&dialog[cur_char],lpChosen->cases[3]); find_char+=8; cur_char+=strlen(lpChosen->cases[0]); }
				else if(!stricmp(cr_textf,"pcases4")) { strcpy(&dialog[cur_char],lpChosen->cases[4]); find_char+=8; cur_char+=strlen(lpChosen->cases[0]); }
			}

			dialog[cur_char++]=temp_dialog[find_char];

			continue;
		}

		if(temp_dialog[find_char]=='%' && temp_dialog[find_char+1]=='%')
		{
			find_char++;
			cur_text++;

			if(temp_dialog[find_char+1]=='#') find_char++;
		}
	}

	dialog[cur_char]=0;

	return 1;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::LMenuTryCreate()
{
	if(GetTickCount()-LMenu_start_time < LMENU_SHOW_TIME) return;
	if(IsLMenu()) return;

	SetLMenu(LMENU_OFF);
	LMenu_nodes=NULL;
	LMenu_cur_x=cur_x;
	LMenu_cur_y=cur_y;

	switch (screen_mode)
	{
	case SCREEN_MAIN:
		if((curTargetCrit=GetMouseCritter(cur_x,cur_y)))
		{
			if(curTargetCrit->id>=USERS_START_ID)
				SetLMenu(LMENU_PLAYER);
			else if(curTargetCrit->id>=NPC_MIN_ID && curTargetCrit->id<=NPC_MAX_ID)
				SetLMenu(LMENU_NPC);
		}
		else if((curTargetObj=GetMouseItem(cur_x,cur_y)))
		{
			SetLMenu(LMENU_ITEM);
		}
//		else if((TargetID=GetMouseScenery(cur_x,cur_y)))
//		{
//			SetLMenu(LMENU_SCENERY);
//			LMenu_nodes=&LMenu_scenery_nodes;
//		}
		break;
	case SCREEN_INVENTORY:

		break;
	}

	if(!LMenu_nodes) 
	{
		LMenu_start_time=GetTickCount();
		SetLMenu(LMENU_OFF);
		return;
	}

	LMenu_cur_node=0;
	LMenu_try_activated=false;	
}

void CFEngine::LMenuDraw()
{
	if(!IsLMenu()) return;
	if(!LMenu_nodes) { WriteLog("!!!WARNING!!! Отрисовка ЛМеню - пустой указатель\n"); return; }

	int count_node=0;
	for(LMenu_list::iterator it_l=LMenu_nodes->begin(); it_l!=LMenu_nodes->end(); it_l++)
	{
		switch (*it_l)
		{
		case LMENU_NODE_LOOK:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_look_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_look_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		case LMENU_NODE_TALK:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_talk_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_talk_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		case LMENU_NODE_BREAK:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_break_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_break_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		case LMENU_NODE_PICK:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_use_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_use_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		case LMENU_NODE_GMFOLLOW:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_gmfollow_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_gmfollow_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		case LMENU_NODE_GMTAKE:
			if(count_node==LMenu_cur_node)
				sm.DrawSprite(lm_gmtake_on,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			else
				sm.DrawSprite(lm_gmtake_off,LMenu_cur_x,LMenu_cur_y+LMenu_node_height*count_node,COLOR_DEFAULT);
			break;
		default:
			WriteLog("!!!WORNING!!! Отрисовка ЛМеню - неизвестное состояние меню\n");
			break;
		}

		count_node++;
	}
}

void CFEngine::LMenuMouseMove()
{
	if(!LMenu_nodes) { WriteLog("!!!WARNING!!! Движение ЛМеню - невалидный указатель\n"); return; }

	LMenu_cur_node=(cur_y-LMenu_cur_y)/LMenu_node_height;
	if(LMenu_cur_node<0) LMenu_cur_node=0;
	if(LMenu_cur_node>LMenu_nodes->size()-1) LMenu_cur_node=LMenu_nodes->size()-1;
}

void CFEngine::LMenuMouseUp()
{
	LMenu_list::iterator it_l=LMenu_nodes->begin();
	it_l+=LMenu_cur_node;

	crit_map::iterator it_crit;

	switch (screen_mode)
	{
	case SCREEN_MAIN:
		switch(LMenu_mode)
		{
		case LMENU_PLAYER:
			if(!curTargetCrit) break;

			switch (*it_l)
			{
			case LMENU_NODE_LOOK:
				if(curTargetCrit==lpChosen)
					AddMess(COLOR_TEXT_DEFAULT,"Вы разглядываете свою пиписку...");
				else
					AddMess(COLOR_TEXT_DEFAULT,"Это %s.",curTargetCrit->name);
				break;
			case LMENU_NODE_GMFOLLOW:
				Net_SendRuleGlobal(GM_RULE_COMMAND_ADDPREPCRIT,curTargetCrit->id);
				break;
			case LMENU_NODE_GMTAKE:
				break;
			case LMENU_NODE_BREAK:
				break;
			}

			break;
		case LMENU_NPC:
			if(!curTargetCrit) break;

			switch (*it_l)
			{
			case LMENU_NODE_LOOK:
				AddMess(COLOR_TEXT_DEFAULT,"Вы разглядываете %s.",curTargetCrit->cases[2]);
				break;
			case LMENU_NODE_TALK:
				tosendTargetCrit=curTargetCrit;
				SetChosenAction(ACTION_TALK_NPC);
				break;
			case LMENU_NODE_GMFOLLOW:
				break;
			case LMENU_NODE_GMTAKE:
				break;
			case LMENU_NODE_BREAK:
				break;
			}

			break;
		case LMENU_ITEM:
			if(!curTargetObj) break;
			if(!curTargetObj->sobj) break;

			switch (*it_l)
			{
			case LMENU_NODE_LOOK:
				AddMess(COLOR_TEXT_DEFAULT,"%s - %s",name_obj[curTargetObj->sobj->id].c_str(),info_obj[curTargetObj->sobj->id].c_str());
				break;
			case LMENU_NODE_PICK:
				tosendTargetObj=curTargetObj;
				SetChosenAction(ACTION_PICK_OBJ);
				break;
			case LMENU_NODE_BREAK:
				break;
			}

			break;
		}
		break;
	case SCREEN_INVENTORY:
		
		break;
	}

	cur_x=LMenu_cur_x;
	cur_y=LMenu_cur_y;

	SetLMenu(LMENU_OFF);
	LMenu_try_activated=false;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::SetScreen(BYTE new_screen)
{
	screen_mode=new_screen;

	SetCur(CUR_DEFAULT);

	switch(screen_mode)
	{
	case SCREEN_LOGIN:
		break;
	case SCREEN_REGISTRATION:
		break;
	case SCREEN_MAIN:
		break;
	case SCREEN_INVENTORY:
		break;
	case SCREEN_LOCAL_MAP:
		LmapPrepareMap();
		break;
	case SCREEN_GLOBAL_MAP:
		break;
	case SCREEN_DIALOG_NPC:
		DlgMain[0]=0;
		DlgMain[1]=0;
		DlgMain[2]=DlgBegin[2];
		DlgMain[3]=DlgBegin[3]+DlgEnd[3]+DlgNextAnswY*all_answers;

		if(DlgY+DlgMain[3]>IntY) DlgY=IntY-DlgMain[3];
	case SCREEN_PIP_BOY:
		break;
	}
}

void CFEngine::SetCur(BYTE new_cur)
{
	cur_mode=new_cur;

	if(cur_mode==CUR_DEFAULT) cur=cur_def;
	if(cur_mode==CUR_WAIT) cur=cur_wait;

	switch(screen_mode)
	{
	case SCREEN_LOGIN:
		break;
	case SCREEN_REGISTRATION:
		break;
	case SCREEN_MAIN:
		break;
	case SCREEN_INVENTORY:
		break;
	case SCREEN_LOCAL_MAP:
		break;
	case SCREEN_GLOBAL_MAP:
		break;
	case SCREEN_DIALOG_NPC:
		break;
	case SCREEN_PIP_BOY:
		break;
	}
}

void CFEngine::SetLMenu(BYTE set_lmenu)
{
	LMenu_mode=set_lmenu;

	switch(LMenu_mode)
	{
	case LMENU_OFF:
		LMenu_nodes=NULL;
		break;
	case LMENU_PLAYER:
	case LMENU_NPC:
		if(!curTargetCrit)
		{
			WriteLog("Ошибка - SetLMenu - Player or NPC - передача пустого указателя!\n");
			SetLMenu(LMENU_OFF);
			return;
		}

		LMenu_crit_nodes.clear();
		LMenu_crit_nodes.push_back(LMENU_NODE_LOOK);
		if(LMenu_mode==LMENU_NPC)
		{
			LMenu_crit_nodes.push_back(LMENU_NODE_TALK);
		}
		else
		{
			if(curTargetCrit!=lpChosen)
			{
				LMenu_crit_nodes.push_back(LMENU_NODE_GMFOLLOW);
			//	LMenu_crit_nodes.push_back(LMENU_NODE_GMTAKE);
			}
		}
		LMenu_crit_nodes.push_back(LMENU_NODE_BREAK);

		LMenu_nodes=&LMenu_crit_nodes;
		break;
	case LMENU_ITEM:
		if(!curTargetObj)
		{
			WriteLog("Ошибка - SetLMenu - Item - передача пустого указателя!\n");
			SetLMenu(LMENU_OFF);
			return;
		}

		//set_item->sobj

		LMenu_item_nodes.clear();

		LMenu_item_nodes.push_back(LMENU_NODE_LOOK);
		LMenu_item_nodes.push_back(LMENU_NODE_PICK);
		LMenu_item_nodes.push_back(LMENU_NODE_BREAK);

		LMenu_mode=LMENU_ITEM;
		LMenu_nodes=&LMenu_item_nodes;
		break;
	case LMENU_SCENERY:
		LMenu_scenery_nodes.clear();
		LMenu_scenery_nodes.push_back(LMENU_NODE_BREAK);
		break;
	}
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::CreateStringsParamsMaps()
{
	stats_str_map.insert(params_str_map::value_type(ST_STRENGHT,			"Сила"));
	stats_str_map.insert(params_str_map::value_type(ST_PERCEPTION,			"Восприятие"));
	stats_str_map.insert(params_str_map::value_type(ST_ENDURANCE,			"Выносливость"));
	stats_str_map.insert(params_str_map::value_type(ST_CHARISMA,			"Обояние"));
	stats_str_map.insert(params_str_map::value_type(ST_INTELLECT,			"Ум"));
	stats_str_map.insert(params_str_map::value_type(ST_AGILITY,				"Координация"));
	stats_str_map.insert(params_str_map::value_type(ST_LUCK,				"Удача"));
	stats_str_map.insert(params_str_map::value_type(ST_MAX_LIFE,			"Максимальные баллы жизней"));
	stats_str_map.insert(params_str_map::value_type(ST_MAX_COND,			"Максимальная кондиция"));
	stats_str_map.insert(params_str_map::value_type(ST_ARMOR_CLASS,			"Класс брони"));
	stats_str_map.insert(params_str_map::value_type(ST_MELEE_DAMAGE,		"Вред невооруженный"));
	stats_str_map.insert(params_str_map::value_type(ST_WEAPON_DAMAGE,		"Вред оружием"));
	stats_str_map.insert(params_str_map::value_type(ST_CARRY_WEIGHT,		"Максимальный груз"));
	stats_str_map.insert(params_str_map::value_type(ST_SEQUENCE,			"Реакция"));
	stats_str_map.insert(params_str_map::value_type(ST_HEALING_RATE,		"Лечение"));
	stats_str_map.insert(params_str_map::value_type(ST_CRITICAL_CHANCE,		"Критический шанс"));
	stats_str_map.insert(params_str_map::value_type(ST_MAX_CRITICAL,		"Максимальный критический шанс"));
	stats_str_map.insert(params_str_map::value_type(ST_INGURE_ABSORB,		"Порог ранения"));
	stats_str_map.insert(params_str_map::value_type(ST_LASER_ABSORB,		"Порог повреждения лазером"));
	stats_str_map.insert(params_str_map::value_type(ST_FIRE_ABSORB,			"Порог повреждения огнем"));
	stats_str_map.insert(params_str_map::value_type(ST_PLASMA_ABSORB,		"Порог повреждения плазмой"));
	stats_str_map.insert(params_str_map::value_type(ST_ELECTRO_ABSORB,		"Порог повреждения электричеством"));
	stats_str_map.insert(params_str_map::value_type(ST_EMP_ABSORB,			"Порог повреждения ЕМП"));
	stats_str_map.insert(params_str_map::value_type(ST_BLAST_ABSORB,		"Порог повреждения при взрыве"));
	stats_str_map.insert(params_str_map::value_type(ST_INGURE_RESIST,		"Сопротевляемость ранению"));
	stats_str_map.insert(params_str_map::value_type(ST_LASER_RESIST,		"Сопротивляемость ранению лазером"));
	stats_str_map.insert(params_str_map::value_type(ST_FIRE_RESIST,			"Сопротивляемость ранению огнем"));
	stats_str_map.insert(params_str_map::value_type(ST_PLASMA_RESIST,		"Сопротивляемость ранению плазмой"));
	stats_str_map.insert(params_str_map::value_type(ST_ELECTRO_RESIST,		"Сопротивляемость ранению электричеством"));
	stats_str_map.insert(params_str_map::value_type(ST_EMP_RESIST,			"Сопротивляемость ранению ЕМП"));
	stats_str_map.insert(params_str_map::value_type(ST_BLAST_RESIST,		"Сопротивляемость ранению при взрыве"));
	stats_str_map.insert(params_str_map::value_type(ST_RADIATION_RESISTANCE,"Сопротивляемость радиации"));
	stats_str_map.insert(params_str_map::value_type(ST_POISON_RESISTANCE,	"Сопротивляемость ядам"));
	stats_str_map.insert(params_str_map::value_type(ST_AGE,					"Возраст"));
	stats_str_map.insert(params_str_map::value_type(ST_GENDER,				"Пол"));
	stats_str_map.insert(params_str_map::value_type(ST_CURRENT_HP,			"Текущие баллы жизни"));
	stats_str_map.insert(params_str_map::value_type(ST_POISONING_LEVEL,		"Текужий уровень ядов"));
	stats_str_map.insert(params_str_map::value_type(ST_RADIATION_LEVEL,		"Текущий уровень радиации"));
	stats_str_map.insert(params_str_map::value_type(ST_CURRENT_STANDART,	"Текущая кондиция"));

	skills_str_map.insert(params_str_map::value_type(SK_SMALL_GUNS,			"Малое оружие"));
	skills_str_map.insert(params_str_map::value_type(SK_BIG_GUNS,			"Большое оружие"));
	skills_str_map.insert(params_str_map::value_type(SK_ENERGY_WEAPONS,		"Энергитическое оружие"));
	skills_str_map.insert(params_str_map::value_type(SK_UNARMED,			"Безоружный"));
	skills_str_map.insert(params_str_map::value_type(SK_MELEE_WEAPONS,		"Холодное оружие"));
	skills_str_map.insert(params_str_map::value_type(SK_THROWING,			"Метательное"));
	skills_str_map.insert(params_str_map::value_type(SK_FIRST_AID,			"Первая помощь"));
	skills_str_map.insert(params_str_map::value_type(SK_DOCTOR,				"Доктор"));
	skills_str_map.insert(params_str_map::value_type(SK_SNEAK,				"Скрытность"));
	skills_str_map.insert(params_str_map::value_type(SK_LOCKPICK,			"Взломщик"));
	skills_str_map.insert(params_str_map::value_type(SK_STEAL,				"Воровство"));
	skills_str_map.insert(params_str_map::value_type(SK_TRAPS,				"Ловушки"));
	skills_str_map.insert(params_str_map::value_type(SK_SCIENCE,			"Наука"));
	skills_str_map.insert(params_str_map::value_type(SK_REPAIR,				"Починка"));
	skills_str_map.insert(params_str_map::value_type(SK_SPEECH,				"Речь"));
	skills_str_map.insert(params_str_map::value_type(SK_BARTER,				"Бартер"));
	skills_str_map.insert(params_str_map::value_type(SK_GAMBLING,			"Игра"));
	skills_str_map.insert(params_str_map::value_type(SK_OUTDOORSMAN,		"Скаут"));

	perks_str_map.insert(params_str_map::value_type(PE_FAST_METABOLISM,		"1"));
	perks_str_map.insert(params_str_map::value_type(PE_BRUISER,				"1"));
	perks_str_map.insert(params_str_map::value_type(PE_SMALL_FRAME,			"1"));
	perks_str_map.insert(params_str_map::value_type(PE_ONE_HANDER,			"1"));
	perks_str_map.insert(params_str_map::value_type(PE_FINESSE,				"1"));
	perks_str_map.insert(params_str_map::value_type(PE_KAMIKAZE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_HEAVY_HANDED,		""));
	perks_str_map.insert(params_str_map::value_type(PE_FAST_SHOT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_BLOODY_MESS,			""));
	perks_str_map.insert(params_str_map::value_type(PE_JINXED,				""));
	perks_str_map.insert(params_str_map::value_type(PE_GOOD_NATURED,		""));
	perks_str_map.insert(params_str_map::value_type(PE_CHEM_RELIANT,		""));
	perks_str_map.insert(params_str_map::value_type(PE_CHEM_RESISTANT,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NIGHT_PERSON,		""));
	perks_str_map.insert(params_str_map::value_type(PE_SKILLED,				""));
	perks_str_map.insert(params_str_map::value_type(PE_GIFTED,				""));
	perks_str_map.insert(params_str_map::value_type(PE_AWARENESS,			""));
	perks_str_map.insert(params_str_map::value_type(PE_A_MELEE_ATT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_A_MELEE_DAM,			""));
	perks_str_map.insert(params_str_map::value_type(PE_A_MOVE,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_DAM,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_SPEED,				""));
	perks_str_map.insert(params_str_map::value_type(PE_PASS_FRONT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_RAPID_HEAL,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MORE_CRIT_DAM,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NIGHT_SIGHT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_PRESENCE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_RES_NUKLEAR,			""));
	perks_str_map.insert(params_str_map::value_type(PE_ENDURENCE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_STR_BACK,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MARKSMAN,			""));
	perks_str_map.insert(params_str_map::value_type(PE_STEALHING,			""));
	perks_str_map.insert(params_str_map::value_type(PE_LIFEFULL,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MERCHANT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_FORMED,				""));
	perks_str_map.insert(params_str_map::value_type(PE_HEALER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_TR_DIGGER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_BEST_HITS,			""));
	perks_str_map.insert(params_str_map::value_type(PE_COMPASION,			""));
	perks_str_map.insert(params_str_map::value_type(PE_KILLER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_SNIPER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_SILENT_DEATH,		""));
	perks_str_map.insert(params_str_map::value_type(PE_C_FIGHTER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MIND_BLOCK,			""));
	perks_str_map.insert(params_str_map::value_type(PE_PROLONGATION_LIFE,	""));
	perks_str_map.insert(params_str_map::value_type(PE_RECOURCEFULNESS,		""));
	perks_str_map.insert(params_str_map::value_type(PE_SNAKE_EATER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_REPEARER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MEDIC,				""));
	perks_str_map.insert(params_str_map::value_type(PE_SKILLED_THIEF,		""));
	perks_str_map.insert(params_str_map::value_type(PE_SPEAKER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_GUTCHER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_UNKNOWN_1,			""));
	perks_str_map.insert(params_str_map::value_type(PE_PICK_POCKER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_GHOST,				""));
	perks_str_map.insert(params_str_map::value_type(PE_CHAR_CULT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_THIFER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_DISCOVER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_OVERROAD,			""));
	perks_str_map.insert(params_str_map::value_type(PE_ANIMAL_FRIENDSHIP,	""));
	perks_str_map.insert(params_str_map::value_type(PE_SCOUT,				""));
	perks_str_map.insert(params_str_map::value_type(PE_MIST_CHAR,			""));
	perks_str_map.insert(params_str_map::value_type(PE_RANGER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_PICK_POCKET_2,		""));
	perks_str_map.insert(params_str_map::value_type(PE_INTERLOCUTER,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NOVICE,				""));
	perks_str_map.insert(params_str_map::value_type(PE_PRIME_SKILL,			""));
	perks_str_map.insert(params_str_map::value_type(PE_MUTATION,			""));
	perks_str_map.insert(params_str_map::value_type(PE_NARC_NUKACOLA,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NARC_BUFFOUT,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NARC_MENTAT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_NARC_PSYHO,			""));
	perks_str_map.insert(params_str_map::value_type(PE_NARC_RADAWAY,		""));
	perks_str_map.insert(params_str_map::value_type(PE_DISTANT_WEAP,		""));
	perks_str_map.insert(params_str_map::value_type(PE_ACCURARY_WEAP,		""));
	perks_str_map.insert(params_str_map::value_type(PE_PENETRATION_WEAP,	""));
	perks_str_map.insert(params_str_map::value_type(PE_KILLER_WEAP,			""));
	perks_str_map.insert(params_str_map::value_type(PE_ENERGY_ARMOR,		""));
	perks_str_map.insert(params_str_map::value_type(PE_BATTLE_ARMOR,		""));
	perks_str_map.insert(params_str_map::value_type(PE_WEAP_RANGE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_RAPID_RELOAD,		""));
	perks_str_map.insert(params_str_map::value_type(PE_NIGHT_SPYGLASS,		""));
	perks_str_map.insert(params_str_map::value_type(PE_FLAMER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_APA_I,				""));
	perks_str_map.insert(params_str_map::value_type(PE_APA_II,				""));
	perks_str_map.insert(params_str_map::value_type(PE_FORCEAGE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_DEADLY_NARC,			""));
	perks_str_map.insert(params_str_map::value_type(PE_CHARMOLEANCE,		""));
	perks_str_map.insert(params_str_map::value_type(PE_GEKK_SKINER,			""));
	perks_str_map.insert(params_str_map::value_type(PE_SKIN_ARMOR,			""));
	perks_str_map.insert(params_str_map::value_type(PE_A_SKIN_ARMOR,		""));
	perks_str_map.insert(params_str_map::value_type(PE_SUPER_ARMOR,			""));
	perks_str_map.insert(params_str_map::value_type(PE_A_SUPER_ARMOR,		""));
	perks_str_map.insert(params_str_map::value_type(PE_VAULT_INOCUL,		""));
	perks_str_map.insert(params_str_map::value_type(PE_ADRENALIN_RUSH,		""));
	perks_str_map.insert(params_str_map::value_type(PE_CAREFULL,			""));
	perks_str_map.insert(params_str_map::value_type(PE_INTELEGENCE,			""));
	perks_str_map.insert(params_str_map::value_type(PE_PYROKASY,			""));
	perks_str_map.insert(params_str_map::value_type(PE_DUDE,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_STR,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_PER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_END,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_CHA,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_INT,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_AGL,				""));
	perks_str_map.insert(params_str_map::value_type(PE_A_LUC,				""));
	perks_str_map.insert(params_str_map::value_type(PE_PURERER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_IMAG,				""));
	perks_str_map.insert(params_str_map::value_type(PE_EVASION,				""));
	perks_str_map.insert(params_str_map::value_type(PE_DROSHKADRAT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_KARMA_GLOW,			""));
	perks_str_map.insert(params_str_map::value_type(PE_SILENT_STEPS,		""));
	perks_str_map.insert(params_str_map::value_type(PE_ANATOMY,				""));
	perks_str_map.insert(params_str_map::value_type(PE_CHAMER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_ORATOR,				""));
	perks_str_map.insert(params_str_map::value_type(PE_PACKER,				""));
	perks_str_map.insert(params_str_map::value_type(PE_EDD_GAYAN_MANIAC,	""));
	perks_str_map.insert(params_str_map::value_type(PE_FAST_REGENERATION,	""));
	perks_str_map.insert(params_str_map::value_type(PE_VENDOR,				""));
	perks_str_map.insert(params_str_map::value_type(PE_STONE_WALL,			""));
	perks_str_map.insert(params_str_map::value_type(PE_THIEF_AGAIN,			""));
	perks_str_map.insert(params_str_map::value_type(PE_WEAPON_SKILL,		""));
	perks_str_map.insert(params_str_map::value_type(PE_MAKE_VAULT,			""));
	perks_str_map.insert(params_str_map::value_type(PE_ALC_BUFF_1,			""));
	perks_str_map.insert(params_str_map::value_type(PE_ALC_BUFF_2,			""));
/*	perks_str_map.insert(params_str_map::value_type(!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	perks_str_map.insert(params_str_map::value_type("!",!));
	*/
	perks_str_map.insert(params_str_map::value_type(PE_HIDE_MODE,			""));

/*	object_map.insert(params_str_map::value_type(OBJ_TYPE_ARMOR",			OBJ_TYPE_ARMOR));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_CONTAINER",		OBJ_TYPE_CONTAINER));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_DRUG",			OBJ_TYPE_DRUG));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_WEAPON",			OBJ_TYPE_WEAPON));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_AMMO",			OBJ_TYPE_AMMO));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_MISC",			OBJ_TYPE_MISC));
	object_map.insert(params_str_map::value_type(OBJ_TYPE_KEY",			OBJ_TYPE_KEY));
	object_map.insert(params_str_map::value_type(OBJ_NAME",				OBJ_NAME));
	object_map.insert(params_str_map::value_type(OBJ_INFO",				OBJ_INFO));
	object_map.insert(params_str_map::value_type(OBJ_TIME_SHOW",			OBJ_TIME_SHOW));
	object_map.insert(params_str_map::value_type(OBJ_TIME_HIDE",			OBJ_TIME_HIDE));
	object_map.insert(params_str_map::value_type(OBJ_DISTANCE_LIGHT",		OBJ_DISTANCE_LIGHT));
	object_map.insert(params_str_map::value_type(OBJ_INTENSITY_LIGHT",		OBJ_INTENSITY_LIGHT));
	object_map.insert(params_str_map::value_type(OBJ_PASSED",				OBJ_PASSED));
	object_map.insert(params_str_map::value_type(OBJ_RAKED",				OBJ_RAKED));
	object_map.insert(params_str_map::value_type(OBJ_TRANSPARENT",			OBJ_TRANSPARENT));
	object_map.insert(params_str_map::value_type(OBJ_CAN_USE",				OBJ_CAN_USE));
	object_map.insert(params_str_map::value_type(OBJ_CAN_PICK_UP",			OBJ_CAN_PICK_UP));
	object_map.insert(params_str_map::value_type(OBJ_CAN_USE_ON_SMTH",		OBJ_CAN_USE_ON_SMTH));
	object_map.insert(params_str_map::value_type(OBJ_HIDDEN",				OBJ_HIDDEN));
	object_map.insert(params_str_map::value_type(OBJ_WEIGHT",				OBJ_WEIGHT));
	object_map.insert(params_str_map::value_type(OBJ_SIZE",				OBJ_SIZE));
	object_map.insert(params_str_map::value_type(OBJ_TWOHANDS",			OBJ_TWOHANDS));
	object_map.insert(params_str_map::value_type(OBJ_PIC_MAP",			OBJ_PIC_MAP));
	object_map.insert(params_str_map::value_type(OBJ_PIC_INV",				OBJ_PIC_INV));
	object_map.insert(params_str_map::value_type(OBJ_SOUND",				OBJ_SOUND));
	object_map.insert(params_str_map::value_type(OBJ_LIVETIME",			OBJ_LIVETIME));
	object_map.insert(params_str_map::value_type(OBJ_COST",				OBJ_COST));
	object_map.insert(params_str_map::value_type(OBJ_MATERIAL",			OBJ_MATERIAL));
	object_map.insert(params_str_map::value_type(OBJ_ARM_ANIM0_MALE",		OBJ_ARM_ANIM0_MALE));
	object_map.insert(params_str_map::value_type(OBJ_ARM_ANIM0_MALE2",		OBJ_ARM_ANIM0_MALE2));
	object_map.insert(params_str_map::value_type(OBJ_ARM_ANIM0_FEMALE",	OBJ_ARM_ANIM0_FEMALE));
	object_map.insert(params_str_map::value_type(OBJ_ARM_ANIM0_FEMALE2",	OBJ_ARM_ANIM0_FEMALE2));
	object_map.insert(params_str_map::value_type(OBJ_ARM_AC",				OBJ_ARM_AC));
	object_map.insert(params_str_map::value_type(OBJ_ARM_PERK",			OBJ_ARM_PERK));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_NORMAL",		OBJ_ARM_DR_NORMAL));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_LASER",		OBJ_ARM_DR_LASER));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_FIRE",			OBJ_ARM_DR_FIRE));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_PLASMA",		OBJ_ARM_DR_PLASMA));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_ELECTR",		OBJ_ARM_DR_ELECTR));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_EMP",			OBJ_ARM_DR_EMP));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DR_EXPLODE",		OBJ_ARM_DR_EXPLODE));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_NORMAL",		OBJ_ARM_DT_NORMAL));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_LASER",		OBJ_ARM_DT_LASER));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_FIRE",			OBJ_ARM_DT_FIRE));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_PLASMA",		OBJ_ARM_DT_PLASMA));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_ELECTR",		OBJ_ARM_DT_ELECTR));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_EMP",			OBJ_ARM_DT_EMP));
	object_map.insert(params_str_map::value_type(OBJ_ARM_DT_EXPLODE",		OBJ_ARM_DT_EXPLODE));
	object_map.insert(params_str_map::value_type(OBJ_CONT_SIZE",			OBJ_CONT_SIZE));
	object_map.insert(params_str_map::value_type(OBJ_CONT_FLAG",			OBJ_CONT_FLAG));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_STAT0",			OBJ_DRUG_STAT0));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_STAT1",			OBJ_DRUG_STAT1));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_STAT2",			OBJ_DRUG_STAT2));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT0_S0",		OBJ_DRUG_AMOUNT0_S0));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT0_S1",		OBJ_DRUG_AMOUNT0_S1));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT0_S2",		OBJ_DRUG_AMOUNT0_S2));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_DURATION1",		OBJ_DRUG_DURATION1));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT1_S0",		OBJ_DRUG_AMOUNT1_S0));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT1_S1",		OBJ_DRUG_AMOUNT1_S1));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT1_S2",		OBJ_DRUG_AMOUNT1_S2));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_DURATION2",		OBJ_DRUG_DURATION2));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT2_S0",		OBJ_DRUG_AMOUNT2_S0));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT2_S1",		OBJ_DRUG_AMOUNT2_S1));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_AMOUNT2_S2",		OBJ_DRUG_AMOUNT2_S2));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_ADDICTION",		OBJ_DRUG_ADDICTION));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_W_EFFECT",		OBJ_DRUG_W_EFFECT));
	object_map.insert(params_str_map::value_type(OBJ_DRUG_W_ONSET",		OBJ_DRUG_W_ONSET));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_ANIM1",			OBJ_WEAP_ANIM1));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TIME_ACTIV",		OBJ_WEAP_TIME_ACTIV));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TIME_UNACTIV",	OBJ_WEAP_TIME_UNACTIV));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_VOL_HOLDER",		OBJ_WEAP_VOL_HOLDER));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_CALIBER",		OBJ_WEAP_CALIBER));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_VOL_HOLDER_E",	OBJ_WEAP_VOL_HOLDER_E));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_CALIBER_E",		OBJ_WEAP_CALIBER_E));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_CR_FAILTURE",	OBJ_WEAP_CR_FAILTURE));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TYPE_ATTACK",	OBJ_WEAP_TYPE_ATTACK));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_COUNT_ATTACK",	OBJ_WEAP_COUNT_ATTACK));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_SKILL",		OBJ_WEAP_PA_SKILL));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_HOLDER",		OBJ_WEAP_PA_HOLDER));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_PIC",			OBJ_WEAP_PA_PIC));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_DMG_MIN",		OBJ_WEAP_PA_DMG_MIN));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_DMG_MAX",		OBJ_WEAP_PA_DMG_MAX));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_MAX_DIST",	OBJ_WEAP_PA_MAX_DIST));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_EFF_DIST",	OBJ_WEAP_PA_EFF_DIST));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_ANIM2",		OBJ_WEAP_PA_ANIM2));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_TIME",		OBJ_WEAP_PA_TIME));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_AIM",			OBJ_WEAP_PA_AIM));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_ROUND",		OBJ_WEAP_PA_ROUND));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_PA_REMOVE",		OBJ_WEAP_PA_REMOVE));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_SKILL",		OBJ_WEAP_SA_SKILL));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_HOLDER",		OBJ_WEAP_SA_HOLDER));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_PIC",			OBJ_WEAP_SA_PIC));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_DMG_MIN",		OBJ_WEAP_SA_DMG_MIN));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_DMG_MAX",		OBJ_WEAP_SA_DMG_MAX));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_MAX_DIST",	OBJ_WEAP_SA_MAX_DIST));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_EFF_DIST",	OBJ_WEAP_SA_EFF_DIST));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_ANIM2",		OBJ_WEAP_SA_ANIM2));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_TIME",		OBJ_WEAP_SA_TIME));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_AIM",			OBJ_WEAP_SA_AIM));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_ROUND",		OBJ_WEAP_SA_ROUND));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_SA_REMOVE",		OBJ_WEAP_SA_REMOVE));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_SKILL",		OBJ_WEAP_TA_SKILL));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_HOLDER",		OBJ_WEAP_TA_HOLDER));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_PIC",			OBJ_WEAP_TA_PIC));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_DMG_MIN",		OBJ_WEAP_TA_DMG_MIN));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_DMG_MAX",		OBJ_WEAP_TA_DMG_MAX));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_MAX_DIST",	OBJ_WEAP_TA_MAX_DIST));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_EFF_DIST",	OBJ_WEAP_TA_EFF_DIST));	
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_ANIM2",		OBJ_WEAP_TA_ANIM2));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_TIME",		OBJ_WEAP_TA_TIME));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_AIM",			OBJ_WEAP_TA_AIM));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_ROUND",		OBJ_WEAP_TA_ROUND));
	object_map.insert(params_str_map::value_type(OBJ_WEAP_TA_REMOVE",		OBJ_WEAP_TA_REMOVE));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_CALIBER",		OBJ_AMMO_CALIBER));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_TYPE_DAMAGE",	OBJ_AMMO_TYPE_DAMAGE));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_QUANTITY",		OBJ_AMMO_QUANTITY));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_AC",				OBJ_AMMO_AC));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_DR",				OBJ_AMMO_DR));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_DM",				OBJ_AMMO_DM));
	object_map.insert(params_str_map::value_type(OBJ_AMMO_DD",				OBJ_AMMO_DD));
*/
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::LmapPrepareMap()
{
	int bx,by,ex,ey;
	bx=lpChosen->hex_x;
	by=lpChosen->hex_y;
	ex=lpChosen->hex_x;
	ey=lpChosen->hex_y;

	int maxpixx,maxpixy;
	maxpixx=(LmapWMap[2]-LmapWMap[0])/2/lmap_zoom;
	maxpixy=(LmapWMap[3]-LmapWMap[1])/2/lmap_zoom;

	bx-=maxpixx;
	by-=maxpixy;
	ex+=maxpixx;
	ey+=maxpixy;

	for(Pix_vec::iterator it_p=lmap_prep_pix.begin();it_p!=lmap_prep_pix.end();it_p++)
		SAFEDEL((*it_p));
	lmap_prep_pix.clear();

	DWORD cur_color=0;
	int pix_x=LmapWMap[2]-LmapWMap[0], pix_y=0;
	for(int i1=bx;i1<ex;i1++)
	{
		for(int i2=by;i2<ey;i2++)
		{
			pix_y+=lmap_zoom;

			if(i1<0 || i2<0 || i1>=MAXTILEX || i2>=MAXTILEY) continue;

      // XXX[27.7.2012 alex]: this code needs Vector class with a length() method
			if(sqrt(pow(lpChosen->hex_x-i1,2.0)+pow(lpChosen->hex_y-i2,2.0))==20+lpChosen->st[ST_PERCEPTION]*2)
			{
				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x,
					LmapWMap[1]+pix_y,0xAA00FFFF,&LmapX,&LmapY));
				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x+(lmap_zoom-1),
					LmapWMap[1]+pix_y+((lmap_zoom-1)/2),0xAA00FFFF,&LmapX,&LmapY));
			}

			if(FLAG(hf.hex_field[i2][i1].flags,FT_BLOCK))
			{
				if(switch_HI==false)
				{
					if(!hf.hex_field[i2][i1].wall_id[0]
					&& !hf.hex_field[i2][i1].wall_id[1]
					&& !hf.hex_field[i2][i1].wall_id[2]) continue;
				}
				else
					if(hf.hex_field[i2][i1].scroll_block==TRUE) continue;

				if(!hf.hex_field[i2][i1].wall_id[0]
				&& !hf.hex_field[i2][i1].wall_id[1]
				&& !hf.hex_field[i2][i1].wall_id[2])
					cur_color=0x7F00FF00;
				else
					cur_color=0xFF00FF00;
			}
			else if(hf.hex_field[i2][i1].lpcrit)
			{
				if(hf.hex_field[i2][i1].lpcrit==lpChosen)
					cur_color=0xFF0000FF;
				else
					cur_color=0xFFFF0000;

				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x+(lmap_zoom-1),
					LmapWMap[1]+pix_y,cur_color,&LmapX,&LmapY));

				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x,
					LmapWMap[1]+pix_y+((lmap_zoom-1)/2),cur_color,&LmapX,&LmapY));

/*
				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+maxpixx+(lmap_zoom-1),
					LmapWMap[1]+maxpixy,cur_color,&LmapX,&LmapY,
					&hf.hex_field[i2][i1].lpcrit->hex_x,&hf.hex_field[i2][i1].lpcrit->hex_y));

				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+maxpixx,
					LmapWMap[1]+maxpixy+((lmap_zoom-1)/2),cur_color,&LmapX,&LmapY,
					&hf.hex_field[i2][i1].lpcrit->hex_x,&hf.hex_field[i2][i1].lpcrit->hex_y));

				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+maxpixx,
					LmapWMap[1]+maxpixy,cur_color,&LmapX,&LmapY,
					&hf.hex_field[i2][i1].lpcrit->hex_x,&hf.hex_field[i2][i1].lpcrit->hex_y));

				lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+maxpixx+(lmap_zoom-1),
					LmapWMap[1]+maxpixy+((lmap_zoom-1)/2),cur_color,&LmapX,&LmapY,
					&hf.hex_field[i2][i1].lpcrit->hex_x,&hf.hex_field[i2][i1].lpcrit->hex_y));

				continue;
*/
			}
			else if(FLAG(hf.hex_field[i2][i1].flags,FT_TRANSIT))
				cur_color=0xFFFFFFFF;
			else
				continue;

			lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x,
				LmapWMap[1]+pix_y,cur_color,&LmapX,&LmapY));
			lmap_prep_pix.push_back(new PrepPix(LmapWMap[0]+pix_x+(lmap_zoom-1),
				LmapWMap[1]+pix_y+((lmap_zoom-1)/2),cur_color,&LmapX,&LmapY));
		}
		pix_x-=lmap_zoom;
		pix_y=0;
	}
}

void CFEngine::LmapDraw()
{
	sm.DrawSprite(lmap_main,LmapMain[0]+LmapX,LmapMain[1]+LmapY,COLOR_DEFAULT);

	sm.DrawPrepLines(&lmap_prep_pix);

	RECT r0={LmapWMap[0]+LmapX,LmapWMap[1]+LmapY,LmapWMap[0]+LmapX+100,LmapWMap[1]+LmapY+15};
	char str1[128];
	sprintf(str1,"ZOOM: %d",lmap_zoom-1);
	fnt.MyDrawText(r0,str1,0);

	if(LmapHold==1) sm.DrawSprite(lmap_bok_on,LmapBOk[0]+LmapX,LmapBOk[1]+LmapY,COLOR_DEFAULT);
	if(LmapHold==2) sm.DrawSprite(lmap_bscan_on,LmapBScan[0]+LmapX,LmapBScan[1]+LmapY,COLOR_DEFAULT);

	if(switch_HI==true) sm.DrawSprite(lmap_blohi_on,LmapBLoHi[0]+LmapX,LmapBLoHi[1]+LmapY,COLOR_DEFAULT);
}

void CFEngine::LmapMouseDown()
{
	LmapHold=0;
	if(cur_x>=LmapBOk[0]+LmapX   && cur_y>=LmapBOk[1]+LmapY   && cur_x<=LmapBOk[2]+LmapX   && cur_y<=LmapBOk[3]+LmapY)
	{ LmapHold=1; return; }
	if(cur_x>=LmapBScan[0]+LmapX && cur_y>=LmapBScan[1]+LmapY && cur_x<=LmapBScan[2]+LmapX && cur_y<=LmapBScan[3]+LmapY)
	{ LmapHold=2; return; }
	if(cur_x>=LmapBLoHi[0]+LmapX && cur_y>=LmapBLoHi[1]+LmapY && cur_x<=LmapBLoHi[2]+LmapX && cur_y<=LmapBLoHi[3]+LmapY)
	{ LmapHold=3; return; }
	if(cur_x>=LmapMain[0]+LmapX  && cur_y>=LmapMain[1]+LmapY  && cur_x<=LmapMain[2]+LmapX  && cur_y<=LmapMain[3]+LmapY)
	{ LmapHold=4; lmapvectx=cur_x-LmapX; lmapvecty=cur_y-LmapY; return; }
}

void CFEngine::LmapMouseMove()
{
	if(LmapHold==4)
	{
		LmapX=cur_x-lmapvectx;
		LmapY=cur_y-lmapvecty;

		if(LmapX<0) LmapX=0;
		if(LmapX+LmapMain[2]>MODE_WIDTH) LmapX=MODE_WIDTH-LmapMain[2];
		if(LmapY<0) LmapY=0;
		if(LmapY+LmapMain[3]>IntY) LmapY=IntY-LmapMain[3];
	}
}

void CFEngine::LmapMouseUp()
{
	if(cur_x>=LmapBOk[0]+LmapX   && cur_y>=LmapBOk[1]+LmapY   && cur_x<=LmapBOk[2]+LmapX   && cur_y<=LmapBOk[3]+LmapY   && LmapHold==1)
	{
		SetScreen(SCREEN_MAIN);
	}
	if(cur_x>=LmapBScan[0]+LmapX && cur_y>=LmapBScan[1]+LmapY && cur_x<=LmapBScan[2]+LmapX && cur_y<=LmapBScan[3]+LmapY && LmapHold==2)
	{
		lmap_zoom*=1.5f;
		if(lmap_zoom>13) lmap_zoom=2;
		LmapPrepareMap();
	}
	if(cur_x>=LmapBLoHi[0]+LmapX && cur_y>=LmapBLoHi[1]+LmapY && cur_x<=LmapBLoHi[2]+LmapX && cur_y<=LmapBLoHi[3]+LmapY && LmapHold==3)
	{
		switch_HI=!switch_HI;
		LmapPrepareMap();
	}

	LmapHold=0;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::GmapNullParams()
{
	GmapGroupXf=0;
	GmapGroupYf=0;
	GmapGroupX=0;
	GmapGroupY=0;
	GmapMoveX=0;
	GmapMoveY=0;
	GmapSpeedX=0;
	GmapSpeedY=0;
	GmapSpeed=10;
	last_scr=0;
	GmapCurHoldBLoc=0;
	lock_group_contr=true;

	for(std::vector<GM_city*>::iterator it_c=gm_cities.begin();it_c!=gm_cities.end();++it_c)
		delete (*it_c);
	gm_cities.clear();

	for(std::vector<GM_crit*>::iterator it_cr=gm_crits.begin();it_cr!=gm_crits.end();++it_cr)
		delete (*it_cr);
	gm_crits.clear();

	gm_process=false;
}

void CFEngine::GmapProcess()
{
	if(gm_process==false)
	{
		SetCur(CUR_WAIT);
		return;
	}
	else
		SetCur(CUR_DEFAULT);

	if(GmapMapScrX>GmapWMap[0]) GmapMapScrX=GmapWMap[0];
	if(GmapMapScrY>GmapWMap[1]) GmapMapScrY=GmapWMap[1];
	if(GmapMapScrX<GmapWMap[2]-GM_MAXX) GmapMapScrX=GmapWMap[2]-GM_MAXX;
	if(GmapMapScrY<GmapWMap[3]-GM_MAXY) GmapMapScrY=GmapWMap[3]-GM_MAXY;

	DWORD gm_time=GetTickCount()-gm_last_tick;
	if(gm_time>=GM_ZONE_TIME_PROC)
	{
		GmapGroupXf+=GmapSpeedX*((float)(gm_time)/GM_ZONE_TIME_PROC);
		GmapGroupYf+=GmapSpeedY*((float)(gm_time)/GM_ZONE_TIME_PROC);

		if(GmapGroupXf>GM_MAXX-1 || GmapGroupYf>GM_MAXY-1 || GmapGroupXf<0 || GmapGroupYf<0)
		{
			GmapGroupXf=GmapGroupX;
			GmapGroupYf=GmapGroupY;

			GmapSpeedX=0;
			GmapSpeedY=0;
		}

		GmapGroupX=GmapGroupXf;
		GmapGroupY=GmapGroupYf;

		if(GmapGroupX==GmapMoveX && GmapGroupY==GmapMoveY)
		{
			GmapSpeedX=0;
			GmapSpeedY=0;
		}

		gm_last_tick=GetTickCount();
	}

	if(GmapHold==8) //scrUP
	{
		if(cur_x>=GmapBTabsScrUp[0] && cur_y>=GmapBTabsScrUp[1] && cur_x<=GmapBTabsScrUp[2] && cur_y<=GmapBTabsScrUp[3])
		{
			if(GetTickCount()-last_scr>=10)
			{
				if(GmapTabNextX) gm_tab_scr_x-=(GetTickCount()-last_scr)/5;
				if(GmapTabNextY) gm_tab_scr_y-=(GetTickCount()-last_scr)/5;

				if(gm_tab_scr_x<0) gm_tab_scr_x=0;
				if(gm_tab_scr_y<0) gm_tab_scr_y=0;

				last_scr=GetTickCount();
			}
		}
		else
			GmapHold=0;
	}

	if(GmapHold==9) //scrDOWN
	{
		if(cur_x>=GmapBTabsScrDn[0] && cur_y>=GmapBTabsScrDn[1] && cur_x<=GmapBTabsScrDn[2] && cur_y<=GmapBTabsScrDn[3])
		{
			if(GetTickCount()-last_scr>=10)
			{
				if(GmapTabNextX) gm_tab_scr_x+=(GetTickCount()-last_scr)/5;
				if(GmapTabNextY) gm_tab_scr_y+=(GetTickCount()-last_scr)/5;

			//	if(gm_tab_scr_x<0) gm_tab_scr_x=0;
			//	if(gm_tab_scr_y<0) gm_tab_scr_y=0;

				last_scr=GetTickCount();
			}
		}
		else
			GmapHold=0;
	}
}

void CFEngine::GmapDraw()
{
//	lpDevice->Clear(0,0,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	lpDevice->BeginScene();

	int gm_count_group=gm_crits.size();

	if(gm_process==true)
	{
		for(int px=0;px<5;++px)
			for(int py=0;py<6;++py)
			{
				int mx1=px*512+GmapMapScrX;
				int my1=py*512+GmapMapScrY;
				int mx2=px*512+GmapMapScrX+512;
				int my2=py*512+GmapMapScrY+512;

				int sx1=GmapWMap[0];
				int sy1=GmapWMap[1];
				int sx2=GmapWMap[2];
				int sy2=GmapWMap[3];

				if( (sx1>=mx1 && sx1<=mx2 && sy1>=my1 && sy1<=my2)||
					(sx2>=mx1 && sx2<=mx2 && sy1>=my1 && sy1<=my2)||
					(sx1>=mx1 && sx1<=mx2 && sy2>=my1 && sy2<=my2)||
					(sx2>=mx1 && sx2<=mx2 && sy2>=my1 && sy2<=my2)||

					(mx1>=sx1 && mx1<=sx2 && my1>=sy1 && my1<=sy2)||
					(mx2>=sx1 && mx2<=sx2 && my1>=sy1 && my1<=sy2)||
					(mx1>=sx1 && mx1<=sx2 && my2>=sy1 && my2<=sy2)||
					(mx2>=sx1 && mx2<=sx2 && my2>=sy1 && my2<=sy2))
				sm.DrawSprite(gm_pic[px][py],
					px*512+GmapMapScrX,
					py*512+GmapMapScrY,
					COLOR_DEFAULT);
			}

		SpriteInfo* sprinf=NULL;

		for(std::vector<GM_city*>::iterator it_c=gm_cities.begin();it_c!=gm_cities.end();++it_c)
		{
			GM_city* city=(*it_c);

			WORD cur_loc_pic=NULL;
			switch(city->radius)
			{
			case 24:
				cur_loc_pic=gm_loc48_pic;
				break;
			case 12:
				cur_loc_pic=gm_loc24_pic;
				break;
			case 6:
				cur_loc_pic=gm_loc12_pic;
				break;
			default:
				continue;
			}

			int loc_pic_x1=city->x+GmapMapScrX-city->radius;
			int loc_pic_y1=city->y+GmapMapScrY-city->radius;
			int loc_pic_x2=city->x+GmapMapScrX+city->radius;
			int loc_pic_y2=city->y+GmapMapScrY+city->radius;

			if(loc_pic_x1<=GmapWMap[2] && loc_pic_y1<=GmapWMap[3] && loc_pic_x2>=GmapWMap[0] && loc_pic_y2>=GmapWMap[1])
				sm.DrawSprite(cur_loc_pic,loc_pic_x1,loc_pic_y1,COLOR_DEFAULT,50);
		}

		if(!(sprinf=sm.GetSpriteInfo(gm_gr_loc_pic))) return; //!!!write warning
		sm.DrawSprite(gm_gr_loc_pic,GmapGroupX+GmapMapScrX-sprinf->w/2,GmapGroupY+GmapMapScrY-sprinf->h/2,COLOR_DEFAULT);
		if(!(sprinf=sm.GetSpriteInfo(gm_gr_targ_pic))) return; //!!!write warning
		sm.DrawSprite(gm_gr_targ_pic,GmapMoveX+GmapMapScrX-sprinf->w/2,GmapMoveY+GmapMapScrY-sprinf->h/2,COLOR_DEFAULT);

		int cur_tabx=GmapWTabs[0]-gm_tab_scr_x;
		int cur_taby=GmapWTabs[1]-gm_tab_scr_y;
		int cur_city=0;

		for(std::vector<GM_city*>::iterator it_c=gm_cities.begin();it_c!=gm_cities.end();++it_c)
		{
			GM_city* city=(*it_c);

			sm.DrawSprite(gm_wtab_pic,cur_tabx,cur_taby,COLOR_DEFAULT);
			sm.DrawSprite(gm_locpic[city->num],GmapWTabLoc[0]+cur_tabx,GmapWTabLoc[1]+cur_taby,COLOR_DEFAULT);

			if(GmapHold==7 && GmapCurHoldBLoc==cur_city) //button location
				sm.DrawSprite(gm_btabloc_picdn,GmapBTabLoc[0]+cur_tabx,GmapBTabLoc[1]+cur_taby,COLOR_DEFAULT);

			cur_tabx+=GmapTabNextX;
			cur_taby+=GmapTabNextY;
			cur_city++;
		}

		while(true)
		{
			if(cur_tabx>GmapWTabs[2] || cur_taby>GmapWTabs[3]) break;

			sm.DrawSprite(gm_wblanktab_pic,cur_tabx,cur_taby,COLOR_DEFAULT);

			cur_tabx+=GmapTabNextX;
			cur_taby+=GmapTabNextY;
		}
	}

	sm.DrawSprite(gm_iface_pic,0,0,COLOR_DEFAULT);
	if(opt_screen_mode==2) sm.DrawSprite(gm_iface_pic2,640,0,COLOR_DEFAULT);

	if(gm_process==true)
	{
		switch(GmapSpeed)
		{
		case GM_SPEED_SLOW:
			sm.DrawSprite(gm_bspeed0_on,GmapBSpeed0[0],GmapBSpeed0[1],COLOR_DEFAULT);
			break;
		case GM_SPEED_NORM:
			sm.DrawSprite(gm_bspeed1_on,GmapBSpeed1[0],GmapBSpeed1[1],COLOR_DEFAULT);
			break;
		case GM_SPEED_FAST:
			sm.DrawSprite(gm_bspeed2_on,GmapBSpeed2[0],GmapBSpeed2[1],COLOR_DEFAULT);
			break;
		//default:
		//	Net_SendGiveGlobalInfo(GM_INFO_PARAM);
		//	break;
		}

		switch(GmapHold)
		{
		case 2: sm.DrawSprite(gm_btolocal_on,GmapBToLocal[0],GmapBToLocal[1],COLOR_DEFAULT); break; //to_local
		case 3: sm.DrawSprite(gm_bstop_on,GmapBStop[0],GmapBStop[1],COLOR_DEFAULT); break; //stop
		case 4: sm.DrawSprite(gm_bspeed0_on,GmapBSpeed0[0],GmapBSpeed0[1],COLOR_DEFAULT); break; //speed0
		case 5: sm.DrawSprite(gm_bspeed1_on,GmapBSpeed1[0],GmapBSpeed1[1],COLOR_DEFAULT); break; //speed1
		case 6: sm.DrawSprite(gm_bspeed2_on,GmapBSpeed2[0],GmapBSpeed2[1],COLOR_DEFAULT); break; //speed2
		case 8: sm.DrawSprite(gm_tabs_scrup_picdn,GmapBTabsScrUp[0],GmapBTabsScrUp[1],COLOR_DEFAULT); break; //scrUp
		case 9: sm.DrawSprite(gm_tabs_scrdn_picdn,GmapBTabsScrDn[0],GmapBTabsScrDn[1],COLOR_DEFAULT); break; //scrDown
		}

		if(lock_group_contr==true) sm.DrawSprite(gm_lock_contr_pic,GmapPLockContrX,GmapPLockContrY,COLOR_DEFAULT);
	}

//TEXT START======================================================================

	sm.Flush();

	if(gm_process==true)
	{
		//params
		RECT r0={GmapWGroupInfo[0],GmapWGroupInfo[1],GmapWGroupInfo[2],GmapWGroupInfo[3]};
		char gmstr[2048];
		char* str_speed[3]=
		{
			{"Осторожный"},
			{"Нормальный"},
			{"Быстрый"}
		};

		sprintf(gmstr,"Текущие координаты:\n"
					"%d,%d\n"
					"Направляетесь в:\n"
					"%d,%d\n"
					"Тип передвижения:\n"
					"%s\n"
					"Группа:%d чел.\n\n"
					"fps:%d(sleep:%d)",
					GmapGroupX,GmapGroupY,GmapMoveX,GmapMoveY,
					str_speed[GmapSpeed],
					gm_count_group,
					Game_FPS,opt_sleep);

		fnt.MyDrawText(r0,gmstr,0);

		//crits
		RECT r1={0,0,0,0};
		int cur_crit=0;
		for(std::vector<GM_crit*>::iterator it_cr=gm_crits.begin();it_cr!=gm_crits.end();++it_cr)
		{
			GM_crit* crit=(*it_cr);

			r1.left=cur_crit*GmapWNameStepX+GmapWName[0];
			r1.top=cur_crit*GmapWNameStepY+GmapWName[1];
			r1.right=cur_crit*GmapWNameStepX+GmapWName[2];
			r1.bottom=cur_crit*GmapWNameStepY+GmapWName[3];

			fnt.MyDrawText(r1,crit->name,FT_NOBREAK);

			cur_crit++;
		}

		//map coord
		if(cur_x>=GmapWMap[0] && cur_y>=GmapWMap[1] && cur_x<=GmapWMap[2] && cur_y<=GmapWMap[3])
		{
			SpriteInfo* cur_info=sm.GetSpriteInfo(cur);
			RECT rmc={cur_x+cur_info->w,cur_y+cur_info->h,cur_x+cur_info->w+150,cur_y+cur_info->h+100};
			sprintf(gmstr,
				"Coord: %d : %d\n"
				"Zone: %d : %d",
				cur_x-GmapMapScrX,cur_y-GmapMapScrY,
				GM_ZONE(cur_x-GmapMapScrX),GM_ZONE(cur_y-GmapMapScrY));
			fnt.MyDrawText(rmc,gmstr,0);
		}

		//panel
		RECT r3={GmapWPanel[0],GmapWPanel[1],GmapWPanel[2],GmapWPanel[3]};
		char str_to_edit[2048];
		static bool show_cur=true;
		static TICK show_cur_last_time=GetTickCount();

		if(GetTickCount()>show_cur_last_time+400)
		{
			show_cur=!show_cur;
			show_cur_last_time=GetTickCount();
		}

		str_to_edit[0]=0;
		strcpy(str_to_edit,ed_str);
		if(show_cur)
			str_to_edit[cur_edit]='|';
		else
			str_to_edit[cur_edit]='.';

		str_to_edit[cur_edit+1]=0;
		strcat(str_to_edit,&ed_str[cur_edit]);

		fnt.MyDrawText(r3,str_to_edit,FT_NOBREAK|FT_BOTTOM);

		MessBoxDrawText();

//TEXT END========================================================================

		cur_crit=0;
		int posx,posy;
		WORD cur_pic;
		for(std::vector<GM_crit*>::iterator it_cr=gm_crits.begin();it_cr!=gm_crits.end();++it_cr)
		{
			GM_crit* crit=(*it_cr);

			if(crit->player==true)
				if(crit->disconnect==true)
					cur_pic=gm_cr_offline;
				else
					cur_pic=gm_cr_online;

			if(crit->npc==true) cur_pic=gm_cr_npc;

			SpriteInfo* sprinf=NULL;
			if(!(sprinf=sm.GetSpriteInfo(cur_pic))) continue;

			posx=cur_crit*GmapWNameStepX+GmapWName[2]-sprinf->w;
			posy=cur_crit*GmapWNameStepY+GmapWName[1]-3;

			sm.DrawSprite(cur_pic,posx,posy,COLOR_DEFAULT,100);

			cur_crit++;
		}
	}

	CurDraw();

	sm.Flush();

	lpDevice->EndScene();
	lpDevice->Present(NULL,NULL,NULL,NULL);
}

void CFEngine::GmapInput()
{
	DIDEVICEOBJECTDATA didod[DI_BUF_SIZE];
	DWORD dwElements;

    dwElements = DI_BUF_SIZE;
    HRESULT hr = lpKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("RegParseInput> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}

	for(int i=0;i<dwElements;i++) 
	{
		DI_ONDOWN( DIK_RETURN , 
			if(ed_str[0]) Net_SendText(ed_str);
				
			ed_str[0]=0;
			cur_edit=0;
		);

		DI_ONDOWN( DIK_RCONTROL ,CtrlDwn=1;
			if(opt_change_lang==CHANGE_LANG_RCTRL) lang=(lang==LANG_RUS)?LANG_ENG:LANG_RUS;
		);
		DI_ONUP( DIK_RCONTROL ,CtrlDwn=0 );
		DI_ONDOWN( DIK_LCONTROL ,CtrlDwn=1 );
		DI_ONUP( DIK_LCONTROL ,CtrlDwn=0 );

		DI_ONDOWN( DIK_LMENU ,AltDwn=1 );
		DI_ONUP( DIK_LMENU ,AltDwn=0 );
		DI_ONDOWN( DIK_RMENU ,AltDwn=1 );
		DI_ONUP( DIK_RMENU ,AltDwn=0 );

		DI_ONDOWN( DIK_LSHIFT ,ShiftDwn=1;
			if(CtrlDwn && opt_change_lang==CHANGE_LANG_CTRL_SHIFT) lang=(lang==LANG_RUS)?LANG_ENG:LANG_RUS;
			if(AltDwn && opt_change_lang==CHANGE_LANG_ALT_SHIFT) lang=(lang==LANG_RUS)?LANG_ENG:LANG_RUS;
		);
		DI_ONUP( DIK_LSHIFT ,ShiftDwn=0 );
		DI_ONDOWN( DIK_RSHIFT ,ShiftDwn=1;
			if(CtrlDwn && opt_change_lang==CHANGE_LANG_CTRL_SHIFT) lang=(lang==LANG_RUS)?LANG_ENG:LANG_RUS;
			if(AltDwn && opt_change_lang==CHANGE_LANG_ALT_SHIFT) lang=(lang==LANG_RUS)?LANG_ENG:LANG_RUS;
		);
		DI_ONUP( DIK_RSHIFT ,ShiftDwn=0 );

		int fnd=0;
		for(WORD tst=0;tst<256;tst++)
		{
			DI_ONDOWN(tst, //last_key=NULL; 
				if(GetChar(tst,ed_str,&cur_edit,EDIT_LEN,lang,ShiftDwn)) {fnd=1;break;});
			//DI_ONUP(tst, last_key=NULL;);
		}
		//if(fnd) { last_key=tst; tick_press=GetTickCount(); time_press=600; break; }
	}

	dwElements = DI_BUF_SIZE;
	hr = lpMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
	if(hr!=DI_OK)
	{
		dilost=1;
		WriteLog("RegParseInput mouse> %s\n",(char*)DXGetErrorString8(hr));
		return;
	}
		
	for(DWORD i=0;i<dwElements;i++) 
	{
		DI_ONMOUSE(DIMOFS_X, cur_x+=didod[i].dwData*opt_mouse_speed );
		DI_ONMOUSE(DIMOFS_Y, cur_y+=didod[i].dwData*opt_mouse_speed );

		GmapMouseMove();

		DI_ONDOWN(DIMOFS_BUTTON0,
			GmapMouseDown();
		);

		DI_ONUP(DIMOFS_BUTTON0,
			GmapMouseUp();
		);

		DI_ONDOWN(DIMOFS_BUTTON1,
			if(cur_x>=GmapWMap[0] && cur_y>=GmapWMap[1] && cur_x<=GmapWMap[2] && cur_y<=GmapWMap[3])
			{
				gmapvectx=cur_x-GmapMapScrX;
				gmapvecty=cur_y-GmapMapScrY;
				GmapMapScrollEnable=true;
			}
		);

		DI_ONUP(DIMOFS_BUTTON1,
			GmapMapScrollEnable=false;
		);
	}
}

void CFEngine::GmapMouseDown()
{
	GmapHold=0;
	if(cur_x>=GmapWMap[0] && cur_y>=GmapWMap[1] && cur_x<=GmapWMap[2] && cur_y<=GmapWMap[3])
	{ GmapHold=1; return; }

	if(lock_group_contr==true) return;

	if(cur_x>=GmapBToLocal[0] && cur_y>=GmapBToLocal[1] && cur_x<=GmapBToLocal[2] && cur_y<=GmapBToLocal[3])
	{ GmapHold=2; return; }
	if(cur_x>=GmapBStop[0] && cur_y>=GmapBStop[1] && cur_x<=GmapBStop[2] && cur_y<=GmapBStop[3])
	{ GmapHold=3; return; }
	if(cur_x>=GmapBSpeed0[0] && cur_y>=GmapBSpeed0[1] && cur_x<=GmapBSpeed0[2] && cur_y<=GmapBSpeed0[3])
	{ GmapHold=4; return; }
	if(cur_x>=GmapBSpeed1[0] && cur_y>=GmapBSpeed1[1] && cur_x<=GmapBSpeed1[2] && cur_y<=GmapBSpeed1[3])
	{ GmapHold=5; return; }
	if(cur_x>=GmapBSpeed2[0] && cur_y>=GmapBSpeed2[1] && cur_x<=GmapBSpeed2[2] && cur_y<=GmapBSpeed2[3])
	{ GmapHold=6; return; }
	if(cur_x>=GmapWTabs[0] && cur_y>=GmapWTabs[1] && cur_x<=GmapWTabs[2] && cur_y<=GmapWTabs[3])
	{
		int tab_x=(cur_x-(GmapWTabs[0]-gm_tab_scr_x))%GmapTabW;
		int tab_y=(cur_y-(GmapWTabs[1]-gm_tab_scr_y))%GmapTabH;

		if(tab_x>=GmapBTabLoc[0] && tab_y>=GmapBTabLoc[1] && tab_x<=GmapBTabLoc[2] && tab_y<=GmapBTabLoc[3])
		{
			if(GmapTabNextX) GmapCurHoldBLoc=(cur_x-(GmapWTabs[0]-gm_tab_scr_x))/GmapTabW;
			if(GmapTabNextY) GmapCurHoldBLoc=(cur_y-(GmapWTabs[1]-gm_tab_scr_y))/GmapTabH;

			GmapHold=7;
		}
		return;
	}
	if(cur_x>=GmapBTabsScrUp[0] && cur_y>=GmapBTabsScrUp[1] && cur_x<=GmapBTabsScrUp[2] && cur_y<=GmapBTabsScrUp[3])
	{ GmapHold=8; last_scr=GetTickCount(); return; }
	if(cur_x>=GmapBTabsScrDn[0] && cur_y>=GmapBTabsScrDn[1] && cur_x<=GmapBTabsScrDn[2] && cur_y<=GmapBTabsScrDn[3])
	{ GmapHold=9; last_scr=GetTickCount(); return; }
}

void CFEngine::GmapMouseUp()
{
	BREAK_BEGIN

		if(GmapHold==1 && cur_x>=GmapWMap[0] && cur_y>=GmapWMap[1] && cur_x<=GmapWMap[2] && cur_y<=GmapWMap[3])
		{
			Net_SendRuleGlobal(GM_RULE_COMMAND_SETMOVE,cur_x-GmapMapScrX,cur_y-GmapMapScrY);
			break;
		}
		if(GmapHold==2 && cur_x>=GmapBToLocal[0] && cur_y>=GmapBToLocal[1] && cur_x<=GmapBToLocal[2] && cur_y<=GmapBToLocal[3])
		{
			WORD city_num=0;
			for(std::vector<GM_city*>::iterator it_c=gm_cities.begin();it_c!=gm_cities.end();++it_c)
			  // XXX[27.7.2012 alex]: this code needs Vector class with a length() method
				if(sqrt(pow((*it_c)->x-GmapGroupX,2.0)+pow((*it_c)->y-GmapGroupY,2.0))<=(*it_c)->radius)
				{
					city_num=(*it_c)->num;
					break;
				}

			Net_SendRuleGlobal(GM_RULE_COMMAND_TOLOCAL,city_num,0);
			break;
		}
		if(GmapHold==3 && cur_x>=GmapBStop[0] && cur_y>=GmapBStop[1] && cur_x<=GmapBStop[2] && cur_y<=GmapBStop[3])
		{
			Net_SendRuleGlobal(GM_RULE_COMMAND_STOP);
			break;
		}
		if(GmapHold==4 && cur_x>=GmapBSpeed0[0] && cur_y>=GmapBSpeed0[1] && cur_x<=GmapBSpeed0[2] && cur_y<=GmapBSpeed0[3])
		{
			if(GmapSpeed!=GM_SPEED_SLOW)
				Net_SendRuleGlobal(GM_RULE_COMMAND_SETSPEED,GM_SPEED_SLOW);
			break;
		}
		if(GmapHold==5 && cur_x>=GmapBSpeed1[0] && cur_y>=GmapBSpeed1[1] && cur_x<=GmapBSpeed1[2] && cur_y<=GmapBSpeed1[3])
		{
			if(GmapSpeed!=GM_SPEED_NORM)
				Net_SendRuleGlobal(GM_RULE_COMMAND_SETSPEED,GM_SPEED_NORM);
			break;
		}
		if(GmapHold==6 && cur_x>=GmapBSpeed2[0] && cur_y>=GmapBSpeed2[1] && cur_x<=GmapBSpeed2[2] && cur_y<=GmapBSpeed2[3])
		{
			if(GmapSpeed!=GM_SPEED_FAST)
				Net_SendRuleGlobal(GM_RULE_COMMAND_SETSPEED,GM_SPEED_FAST);
			break;
		}
		if(GmapHold==7 && cur_x>=GmapWTabs[0] && cur_y>=GmapWTabs[1] && cur_x<=GmapWTabs[2] && cur_y<=GmapWTabs[3])
		{
			if(GmapCurHoldBLoc<gm_cities.size())
			{
				GM_city* city=gm_cities[GmapCurHoldBLoc];
				Net_SendRuleGlobal(GM_RULE_COMMAND_SETMOVE,city->x,city->y);
			}

			break;
		}
		if(GmapHold==8 && cur_x>=GmapBTabsScrUp[0] && cur_y>=GmapBTabsScrUp[1] && cur_x<=GmapBTabsScrUp[2] && cur_y<=GmapBTabsScrUp[3])
		{
			break;
		}
		if(GmapHold==9 && cur_x>=GmapBTabsScrDn[0] && cur_y>=GmapBTabsScrDn[1] && cur_x<=GmapBTabsScrDn[2] && cur_y<=GmapBTabsScrDn[3])
		{
			break;
		}

	BREAK_END;

	GmapHold=0;
}

void CFEngine::GmapMouseMove()
{
	if(GmapMapScrollEnable==true)
	{
		if(cur_x<GmapWMap[0]) cur_x=GmapWMap[0];
		if(cur_y<GmapWMap[1]) cur_y=GmapWMap[1];
		if(cur_x>GmapWMap[2]) cur_x=GmapWMap[2];
		if(cur_y>GmapWMap[3]) cur_y=GmapWMap[3];

		GmapMapScrX=cur_x-gmapvectx;
		GmapMapScrY=cur_y-gmapvecty;
	}
}
//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::SboxDraw()
{
	sm.DrawSprite(sbox_main,SboxMain[0]+SboxX,SboxMain[1]+SboxY,COLOR_DEFAULT);

	if(SboxHold==1) sm.DrawSprite(sbox_bcancel_on,SboxBCancel[0]+SboxX,SboxBCancel[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==2) sm.DrawSprite(sbox_bsneak_on,SboxBSneak[0]+SboxX,SboxBSneak[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==3) sm.DrawSprite(sbox_blockpick_on,SboxBLockpick[0]+SboxX,SboxBLockpick[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==4) sm.DrawSprite(sbox_bsteal_on,SboxBSteal[0]+SboxX,SboxBSteal[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==5) sm.DrawSprite(sbox_btrap_on,SboxBTrap[0]+SboxX,SboxBTrap[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==6) sm.DrawSprite(sbox_bfirstaid_on,SboxBFirstAid[0]+SboxX,SboxBFirstAid[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==7) sm.DrawSprite(sbox_bdoctor_on,SboxBDoctor[0]+SboxX,SboxBDoctor[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==8) sm.DrawSprite(sbox_bscience_on,SboxBScience[0]+SboxX,SboxBScience[1]+SboxY,COLOR_DEFAULT);
	if(SboxHold==9) sm.DrawSprite(sbox_brepair_on,SboxBRepair[0]+SboxX,SboxBRepair[1]+SboxY,COLOR_DEFAULT);

	sm.Flush();

	RECT rsneak={SboxTSneak[0]+SboxX,SboxTSneak[1]+SboxY,SboxTSneak[2]+SboxX,SboxTSneak[3]+SboxY};
	RECT rlockpick={SboxTLockpick[0]+SboxX,SboxTLockpick[1]+SboxY,SboxTLockpick[2]+SboxX,SboxTLockpick[3]+SboxY};
	RECT rsteal={SboxTSteal[0]+SboxX,SboxTSteal[1]+SboxY,SboxTSteal[2]+SboxX,SboxTSteal[3]+SboxY};
	RECT rtraps={SboxTTrap[0]+SboxX,SboxTTrap[1]+SboxY,SboxTTrap[2]+SboxX,SboxTTrap[3]+SboxY};
	RECT rfirstaid={SboxTFirstAid[0]+SboxX,SboxTFirstAid[1]+SboxY,SboxTFirstAid[2]+SboxX,SboxTFirstAid[3]+SboxY};
	RECT rdoctor={SboxTDoctor[0]+SboxX,SboxTDoctor[1]+SboxY,SboxTDoctor[2]+SboxX,SboxTDoctor[3]+SboxY};
	RECT rscience={SboxTScience[0]+SboxX,SboxTScience[1]+SboxY,SboxTScience[2]+SboxX,SboxTScience[3]+SboxY};
	RECT rrepair={SboxTRepair[0]+SboxX,SboxTRepair[1]+SboxY,SboxTRepair[2]+SboxX,SboxTRepair[3]+SboxY};
	char bin_str[5];

	sprintf(bin_str,"%03d",lpChosen->sk[SK_SNEAK]);
	fnt.MyDrawText(rsneak,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_LOCKPICK]);
	fnt.MyDrawText(rlockpick,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_STEAL]);
	fnt.MyDrawText(rsteal,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_TRAPS]);
	fnt.MyDrawText(rtraps,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_FIRST_AID]);
	fnt.MyDrawText(rfirstaid,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_DOCTOR]);
	fnt.MyDrawText(rdoctor,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_SCIENCE]);
	fnt.MyDrawText(rscience,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
	sprintf(bin_str,"%03d",lpChosen->sk[SK_REPAIR]);
	fnt.MyDrawText(rrepair,bin_str,0,COLOR_DEFAULT,FONT_BIG_NUM);
}

void CFEngine::SboxMouseDown()
{
	SboxHold=0;
	if(cur_x>=SboxBCancel[0]+SboxX && cur_y>=SboxBCancel[1]+SboxY && cur_x<=SboxBCancel[2]+SboxX && cur_y<=SboxBCancel[3]+SboxY)
	{ SboxHold=1; return; }
	if(cur_x>=SboxBSneak[0]+SboxX && cur_y>=SboxBSneak[1]+SboxY && cur_x<=SboxBSneak[2]+SboxX && cur_y<=SboxBSneak[3]+SboxY)
	{ SboxHold=2; return; }
	if(cur_x>=SboxBLockpick[0]+SboxX && cur_y>=SboxBLockpick[1]+SboxY && cur_x<=SboxBLockpick[2]+SboxX && cur_y<=SboxBLockpick[3]+SboxY)
	{ SboxHold=3; return; }
	if(cur_x>=SboxBSteal[0]+SboxX && cur_y>=SboxBSteal[1]+SboxY && cur_x<=SboxBSteal[2]+SboxX && cur_y<=SboxBSteal[3]+SboxY)
	{ SboxHold=4; return; }
	if(cur_x>=SboxBTrap[0]+SboxX && cur_y>=SboxBTrap[1]+SboxY && cur_x<=SboxBTrap[2]+SboxX && cur_y<=SboxBTrap[3]+SboxY)
	{ SboxHold=5; return; }
	if(cur_x>=SboxBFirstAid[0]+SboxX && cur_y>=SboxBFirstAid[1]+SboxY && cur_x<=SboxBFirstAid[2]+SboxX && cur_y<=SboxBFirstAid[3]+SboxY)
	{ SboxHold=6; return; }
	if(cur_x>=SboxBDoctor[0]+SboxX && cur_y>=SboxBDoctor[1]+SboxY && cur_x<=SboxBDoctor[2]+SboxX && cur_y<=SboxBDoctor[3]+SboxY)
	{ SboxHold=7; return; }
	if(cur_x>=SboxBScience[0]+SboxX && cur_y>=SboxBScience[1]+SboxY && cur_x<=SboxBScience[2]+SboxX && cur_y<=SboxBScience[3]+SboxY)
	{ SboxHold=8; return; }
	if(cur_x>=SboxBRepair[0]+SboxX && cur_y>=SboxBRepair[1]+SboxY && cur_x<=SboxBRepair[2]+SboxX && cur_y<=SboxBRepair[3]+SboxY)
	{ SboxHold=9; return; }
	if(cur_x>=SboxMain[0]+SboxX && cur_y>=SboxMain[1]+SboxY && cur_x<=SboxMain[2]+SboxX && cur_y<=SboxMain[3]+SboxY)
	{ SboxHold=10; sboxvectx=cur_x-SboxX; sboxvecty=cur_y-SboxY; return; }
}

void CFEngine::SboxMouseMove()
{
	if(SboxHold==10)
	{
		SboxX=cur_x-sboxvectx;
		SboxY=cur_y-sboxvecty;

		if(SboxX<0) SboxX=0;
		if(SboxX+SboxMain[2]>MODE_WIDTH) SboxX=MODE_WIDTH-SboxMain[2];
		if(SboxY<0) SboxY=0;
		if(SboxY+SboxMain[3]>IntY) SboxY=IntY-SboxMain[3];
	}
}

void CFEngine::SboxMouseUp()
{
	BREAK_BEGIN
		if(!lpChosen->IsFree()) break;

		if(cur_x>=SboxBCancel[0]+SboxX && cur_y>=SboxBCancel[1]+SboxY && cur_x<=SboxBCancel[2]+SboxX && cur_y<=SboxBCancel[3]+SboxY && SboxHold==1)
		{ SetScreen(SCREEN_MAIN); break; }
		if(cur_x>=SboxBSneak[0]+SboxX && cur_y>=SboxBSneak[1]+SboxY && cur_x<=SboxBSneak[2]+SboxX && cur_y<=SboxBSneak[3]+SboxY && SboxHold==2)
		{
			if(lpChosen->cond==COND_LIFE && lpChosen->cond_ext==COND_LIFE_NONE)
			{
				Net_SendUseSkill(SK_SNEAK,NULL,lpChosen->cur_dir);
				SetScreen(SCREEN_MAIN);
			}
			break;
		}
		if(cur_x>=SboxBLockpick[0]+SboxX && cur_y>=SboxBLockpick[1]+SboxY && cur_x<=SboxBLockpick[2]+SboxX && cur_y<=SboxBLockpick[3]+SboxY && SboxHold==3)
		{ break; }
		if(cur_x>=SboxBSteal[0]+SboxX && cur_y>=SboxBSteal[1]+SboxY && cur_x<=SboxBSteal[2]+SboxX && cur_y<=SboxBSteal[3]+SboxY && SboxHold==4)
		{ break; }
		if(cur_x>=SboxBTrap[0]+SboxX && cur_y>=SboxBTrap[1]+SboxY && cur_x<=SboxBTrap[2]+SboxX && cur_y<=SboxBTrap[3]+SboxY && SboxHold==5)
		{ break; }
		if(cur_x>=SboxBFirstAid[0]+SboxX && cur_y>=SboxBFirstAid[1]+SboxY && cur_x<=SboxBFirstAid[2]+SboxX && cur_y<=SboxBFirstAid[3]+SboxY && SboxHold==6)
		{ break; }
		if(cur_x>=SboxBDoctor[0]+SboxX && cur_y>=SboxBDoctor[1]+SboxY && cur_x<=SboxBDoctor[2]+SboxX && cur_y<=SboxBDoctor[3]+SboxY && SboxHold==7)
		{ break; }
		if(cur_x>=SboxBScience[0]+SboxX && cur_y>=SboxBScience[1]+SboxY && cur_x<=SboxBScience[2]+SboxX && cur_y<=SboxBScience[3]+SboxY && SboxHold==8)
		{ break; }
		if(cur_x>=SboxBRepair[0]+SboxX && cur_y>=SboxBRepair[1]+SboxY && cur_x<=SboxBRepair[2]+SboxX && cur_y<=SboxBRepair[3]+SboxY && SboxHold==9)
		{ break; }
		if(cur_x>=SboxMain[0]+SboxX && cur_y>=SboxMain[1]+SboxY && cur_x<=SboxMain[2]+SboxX && cur_y<=SboxMain[3]+SboxY && SboxHold==10)
		{ break; }
	BREAK_END;

	SboxHold=0;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::MoptDraw()
{
	sm.DrawSprite(mopt_main,MoptMain[0],MoptMain[1],COLOR_DEFAULT);

	if(MoptHold==1) sm.DrawSprite(mopt_bresume_on,MoptBResume[0],MoptBResume[1],COLOR_DEFAULT);
	if(MoptHold==2) sm.DrawSprite(mopt_bexit_on,MoptBExit[0],MoptBExit[1],COLOR_DEFAULT);
}

void CFEngine::MoptMouseDown()
{
	MoptHold=0;
	if(cur_x>=MoptBResume[0] && cur_y>=MoptBResume[1] && cur_x<=MoptBResume[2] && cur_y<=MoptBResume[3])
	{ MoptHold=1; return; }
	if(cur_x>=MoptBExit[0] && cur_y>=MoptBExit[1] && cur_x<=MoptBExit[2] && cur_y<=MoptBExit[3])
	{ MoptHold=2; return; }
}

void CFEngine::MoptMouseUp()
{
	if(cur_x>=MoptBResume[0] && cur_y>=MoptBResume[1] && cur_x<=MoptBResume[2] && cur_y<=MoptBResume[3] && MoptHold==1)
	{ SetScreen(SCREEN_MAIN); }
	if(cur_x>=MoptBExit[0] && cur_y>=MoptBExit[1] && cur_x<=MoptBExit[2] && cur_y<=MoptBExit[3] && MoptHold==2)
	{ TryExit(); }

	MoptHold=0;
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

void CFEngine::CurDraw()
{
	if((IsCur(CUR_DEFAULT) || IsCur(CUR_USE_OBJECT) || IsCur(CUR_USE_SKILL) || IsCur(CUR_WAIT)) && !IsLMenu())
	{
		SpriteInfo* si=sm.GetSpriteInfo(cur);
		int x=cur_x-(si->w >> 1)+si->offs_x;
		int y=cur_y-si->h+si->offs_y;
		sm.DrawSprite(cur,x,y,COLOR_DEFAULT);
	}
	else if(IsCur(CUR_MOVE))
	{
		if(!GetMouseTile(cur_x,cur_y))
			sm.DrawSprite(cur_def,cur_x,cur_y,COLOR_DEFAULT);
		else
		{
			if(!FLAG(hf.hex_field[TargetY][TargetX].flags,FT_NOWAY))
				sm.DrawSprite(cur_move,hf.hex_field[TargetY][TargetX].scr_x+1,
					hf.hex_field[TargetY][TargetX].scr_y-1,COLOR_DEFAULT);
			else
				sm.DrawSprite(cur_move_block,hf.hex_field[TargetY][TargetX].scr_x+1,
					hf.hex_field[TargetY][TargetX].scr_y-1,COLOR_DEFAULT);
		}
	}
}

//==============================================================================================================================
//******************************************************************************************************************************
//==============================================================================================================================

//FEngine_Iface.cpp by Cvet