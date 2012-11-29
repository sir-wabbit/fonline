#ifndef __CRITTER_H__
#define __CRITTER_H__

#include "stdafx.h"

#include "CSpriteManager.h"
#include "CFont.h"
#include "netproto.h"
#include <base/buffer.hpp>

const uint8_t FIRST_FRAME	=0;
const uint8_t LAST_FRAME	=200;

const uint8_t ANIM1_STAY	=1;

const uint8_t ANIM2_USE	=12;
const uint8_t ANIM2_SIT	=11;

class CCritter
{
public:
	void SetDir(uint8_t dir);
	void RotCW();
	void RotCCW();

	void Animate(uint8_t action, uint8_t num_frame); //!Cvet анимация стойки с объектом weapon
	void SetAnimation(); //!Cvet
	void Action(Byte action, uint32_t action_tick); //!Cvet

	void Process(); //!Cvet CBufMngr дал указатель чтоб чезен мог работать с сетью

	uint32_t text_color; //!Cvet
	void SetText(char* str, uint32_t color); //!Cvet uint32_t text_color
	void SetName(char* str) {strncpy(name,str,31);name[30]=0;};
	void DrawText(CFOFont* lpfnt);
	void SetVisible(bool av) {visible=av;};
    
    // Смещения координаты фрейма в пикселях координатах
	uint16_t cur_id,miniplayer;
	short cur_ox;
	short cur_oy;
    // Координаты Дюда в хексах Den Baster
    HexTYPE hex_x;
	HexTYPE hex_y;
	uint8_t cur_dir; // направление

	CritterID id;
// Перенес из привата
	uint8_t weapon; // Тип оружия в руках для анимаций Den Baster !Cvet (предпоследняя буква)

	char name[MAX_NAME+1];
	char cases[5][MAX_NAME+1];
	
	bool human; //!Cvet является ли чезен игроком 0-нет 1-да
	uint16_t st[ALL_STATS]; //!Cvet статы 4-х значный XXXX
	uint16_t sk[ALL_SKILLS]; //!Cvet скилы 3-х значный XXX
	uint8_t pe[ALL_PERKS]; //!Cvet перки 1-а значный X

	uint8_t cond; //!Cvet
	uint8_t cond_ext; //!Cvet
	uint16_t flags; //!Cvet

	CritterType base_type; //!Cvet
	CritterType type;
	CritterType type_ext; //!Cvet

//!Cvet ++++++++++++++++++++++++++++++++++  Инвентарь
	dyn_map obj; //все объекты у игрока
	dyn_obj* a_obj; //активный объект в слот1
	dyn_obj* a_obj2; //активный объект в слот2
	dyn_obj* a_obj_arm; //активный объект в слоте армор
	dyn_obj* m_obj; //mouse object

	void AddObject(uint8_t aslot,uint32_t o_id,uint32_t broken_info,uint32_t time_wear,stat_obj* s_obj);
	int GetMaxDistance();

	void Init(); //инициализация остальных криттеров

	void RefreshWeap();
	void RefreshType();

	int Move(uint8_t dir); // Анимация движения

	uint8_t next_step[4];
	uint8_t cur_step;

	void SetCur_offs(short set_ox, short set_oy);
	void ChangeCur_offs(short change_ox, short change_oy);
	void AccamulateCur_offs();

	uint8_t movementType; //0- 1-бежать

	uint8_t rate_object; //тип использования объекта

	dyn_obj def_obj1;
	dyn_obj def_obj2;

	uint8_t alpha; //прозрачность криттера
//!Cvet ----------------------------------

	fonline::math::Rect<int> drect;
	
	int rit;
	//dtree_map::iterator rit; // индекс по которому можно найти любого Перса

	CCritter(CSpriteManager* alpSM) : lpSM(alpSM), cur_anim(NULL), cur_dir(0), cur_id(0), stay_wait(0), stay_tkr(0),
	                                  text_str(NULL), visible(0), weapon(0), rit(-1) {
	  strcpy(name,"none");
	}
	~CCritter(){SAFEDELA(text_str);};

//!Cvet ++++++++++++++++++++++++++++
	int Tick_count; //продолжительность действия
	TICK Tick_start; //время начала действия

	void Tick_Start(TICK tick_count) { Tick_count=tick_count; Tick_start=GetTickCount(); };
	void Tick_Null(){ Tick_count=0; };

	int IsFree() { if(GetTickCount()-Tick_start>=Tick_count) return 1; return 0; };

	uint8_t cnt_per_turn;
	uint16_t ticks_per_turn;
//!Cvet ----------------------------

	CritFrames* cur_anim; // текущий тип анимации //!Cvet вынес в паблик
	
private:
  void __invariant() {
    assert(this != NULL);
    assert(lpSM != NULL);
  }

	CSpriteManager* lpSM;
	
	
	TICK anim_tkr;//время начала анимации
	uint16_t currentFrame;
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
typedef std::map<CritterID, CCritter*> crit_map;

#endif//__CRITTER_H__