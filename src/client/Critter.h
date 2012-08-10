#ifndef __CRITTER_H__
#define __CRITTER_H__
/********************************************************************
	created:	21:08:2004   20:06

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/
#include "CSpriteManager.h"
#include "CFont.h"
#include "netproto.h"
#include <base/buffer.hpp>

const BYTE FIRST_FRAME	=0;
const BYTE LAST_FRAME	=200;

const BYTE ANIM1_STAY	=1;

const BYTE ANIM2_USE	=12;
const BYTE ANIM2_SIT	=11;

class CCritter
{
public:
	void SetDir(BYTE dir);
	void RotCW();
	void RotCCW();

	void Animate(BYTE action, BYTE num_frame); //!Cvet анимация стойки с объектом weapon
	void SetAnimation(); //!Cvet
	void Action(Byte action, DWORD action_tick); //!Cvet

	void Process(); //!Cvet CBufMngr дал указатель чтоб чезен мог работать с сетью

	DWORD text_color; //!Cvet
	void SetText(char* str, DWORD color); //!Cvet DWORD text_color
	void SetName(char* str) {strncpy(name,str,31);name[30]=0;};
	void DrawText(CFOFont* lpfnt);
	void SetVisible(bool av) {visible=av;};
    
    // Смещения координаты фрейма в пикселях координатах
	WORD cur_id,miniplayer;
	short cur_ox;
	short cur_oy;
    // Координаты Дюда в хексах Den Baster
    HexTYPE hex_x;
	HexTYPE hex_y;
	BYTE cur_dir; // направление

	CrID id;
// Перенес из привата
	BYTE weapon; // Тип оружия в руках для анимаций Den Baster !Cvet (предпоследняя буква)

	char name[MAX_NAME+1];
	char cases[5][MAX_NAME+1];
	
	bool human; //!Cvet является ли чезен игроком 0-нет 1-да
	WORD st[ALL_STATS]; //!Cvet статы 4-х значный XXXX
	WORD sk[ALL_SKILLS]; //!Cvet скилы 3-х значный XXX
	BYTE pe[ALL_PERKS]; //!Cvet перки 1-а значный X

	BYTE cond; //!Cvet
	BYTE cond_ext; //!Cvet
	WORD flags; //!Cvet

	CrTYPE base_type; //!Cvet
	CrTYPE type;
	CrTYPE type_ext; //!Cvet

//!Cvet ++++++++++++++++++++++++++++++++++  Инвентарь
	dyn_map obj; //все объекты у игрока
	dyn_obj* a_obj; //активный объект в слот1
	dyn_obj* a_obj2; //активный объект в слот2
	dyn_obj* a_obj_arm; //активный объект в слоте армор
	dyn_obj* m_obj; //mouse object

	void AddObject(BYTE aslot,DWORD o_id,DWORD broken_info,DWORD time_wear,stat_obj* s_obj);
	int GetMaxDistance();

	void Initialization(); //инициализация остальных криттеров

	void RefreshWeap();
	void RefreshType();

	int Move(BYTE dir); // Анимация движения

	BYTE next_step[4];
	BYTE cur_step;

	void SetCur_offs(short set_ox, short set_oy);
	void ChangeCur_offs(short change_ox, short change_oy);
	void AccamulateCur_offs();

	BYTE move_type; //0- 1-бежать

	BYTE rate_object; //тип использования объекта

	dyn_obj def_obj1;
	dyn_obj def_obj2;

	BYTE alpha; //прозрачность криттера
//!Cvet ----------------------------------

	INTRECT drect;
	
	dtree_map::iterator rit; // индекс по которому можно найти любого Перса

	CCritter(CSpriteManager* alpSM):lpSM(alpSM),cur_anim(NULL),cur_dir(0),cur_id(0),stay_wait(0),stay_tkr(0),text_str(NULL),visible(0),weapon(0){strcpy(name,"none");};
	~CCritter(){SAFEDELA(text_str);};

//!Cvet ++++++++++++++++++++++++++++
	int Tick_count; //продолжительность действия
	TICK Tick_start; //время начала действия

	void Tick_Start(TICK tick_count) { Tick_count=tick_count; Tick_start=GetTickCount(); };
	void Tick_Null(){ Tick_count=0; };

	int IsFree() { if(GetTickCount()-Tick_start>=Tick_count) return 1; return 0; };

	BYTE cnt_per_turn;
	WORD ticks_per_turn;
//!Cvet ----------------------------

	CritFrames* cur_anim; // текущий тип анимации //!Cvet вынес в паблик
	
private:

	CSpriteManager* lpSM;
	
	
	TICK anim_tkr;//время начала анимации
	WORD cur_afrm;
	TICK change_tkr;//время со смены кадра

	int stay_wait; // для стоячей анимации

	TICK stay_tkr;

	char base_fname[256];

	char* text_str;
	TICK SetTime;
	int text_delay; //!Cvet

	bool visible;
};

//список critters, которые присутствуют
typedef std::map<CrID, CCritter*> crit_map;

#endif//__CRITTER_H__