#ifndef _CHEXFIELD_H_
#define _CHEXFIELD_H_

#include "CSpriteManager.h"
#include "Critter.h"
#include "netproto.h" //!Cvet
/********************************************************************
	created:	2005   22:04
	edit:		2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

//!Cvet +++++++++++++++++++++
#define MAXTILEX 400
#define MAXTILEY 400

const uint8_t HEX_IMPASSABLE			=1;
const uint8_t HEX_IMPASSABLE_NOT_RAKED	=2;

//поиск пути
//максимальный путь
const uint8_t FINDPATH_MAX_PATH		=50;
//возвращаемые ошибки
const uint8_t FP_ERROR					=0;
const uint8_t FP_OK					=1;
const uint8_t FP_DEADLOCK				=2;
const uint8_t FP_TOOFAR				=3;
const uint8_t FP_ALREADY_HERE			=4;

//поиск цели
const uint8_t FINDTARGET_BARRIER		=10;
const uint8_t FINDTARGET_TOOFAR		=11;
const uint8_t FINDTARGET_ERROR			=12;
const uint8_t FINDTARGET_INVALID_TARG	=13;

//прозрачность
#define TILE_ALPHA	255
#define ROOF_ALPHA	opt_roof_alpha
//!Cvet ----------------------

#define ADD_X 2
#define ADD_Y 2

// Область видимости 
//#define VIEW_WIDTH  (opt_screen_mode?35:27)
//#define VIEW_HEIGHT (opt_screen_mode?65:51)
//#define VIEW_CX (opt_screen_mode?16:14)
//#define VIEW_CY (opt_screen_mode?34:26)
//!Cvet
#define VIEW_WIDTH  (view_width[opt_screen_mode])
#define VIEW_HEIGHT (view_height[opt_screen_mode])
#define VIEW_CX (view_cx[opt_screen_mode])
#define VIEW_CY (view_cy[opt_screen_mode])

#define MAX_WALL_CNT 3 // флаг стен

struct ViewField
{
	ViewField():mod_x(0),mod_y(0),cur_x(0),cur_y(0),scr_x(0),scr_y(0){};

	int mod_x, mod_y;
	int cur_x, cur_y;
	int scr_x, scr_y;
};

struct ScenObj
{
	uint16_t spr_id;

	ScenObj(): spr_id(0){};
	ScenObj(uint16_t _spr_id): spr_id(_spr_id){};
};

typedef std::vector<ScenObj*> scen_vect;

struct ItemObj
{
	uint16_t spr_id;

//!Cvet +++++++++++++++++++++++
	int hex_x;
	int hex_y;
	short scr_x;
	short scr_y;

	int* hex_scr_x;
	int* hex_scr_y;

	stat_obj* sobj;

	AnyFrames* anim;
	short cur_spr;
	short need_spr;
	TICK last_tick;

	ItemObj(): sobj(NULL),spr_id(0),hex_x(0),hex_y(0),scr_x(0),scr_y(0),hex_scr_x(NULL),hex_scr_y(NULL),anim(NULL),
	cur_spr(0),need_spr(0),last_tick(0){};
	ItemObj(stat_obj* _sobj, uint16_t _spr_id, int _hex_x, int _hex_y, short _scr_x, short _scr_y, int* _hex_scr_x, int* _hex_scr_y):
	sobj(_sobj),spr_id(_spr_id),hex_x(_hex_x),hex_y(_hex_y),scr_x(_scr_x),scr_y(_scr_y),hex_scr_x(_hex_scr_x),hex_scr_y(_hex_scr_y),anim(NULL),
	cur_spr(0),need_spr(0),last_tick(0){};
	~ItemObj(){SAFEDEL(anim);};

	void SetAnimFromEnd(){cur_spr=anim->cnt_frames-1;need_spr=0;SetAnimSpr(cur_spr);};
	void SetAnimFixFromEnd(){SetAnimFromEnd();need_spr=cur_spr;};
	void SetAnimFromStart(){cur_spr=0;need_spr=anim->cnt_frames-1;SetAnimSpr(cur_spr);};
	void SetAnimFixFromStart(){SetAnimFromStart();need_spr=cur_spr;};
	void SetAnimSpr(short _cur_spr){cur_spr=_cur_spr;spr_id=anim->ind[cur_spr];SetAnimOffs();last_tick=GetTickCount();};
	void SetAnimFixSpr(short _cur_spr){cur_spr=_cur_spr;need_spr=cur_spr;spr_id=anim->ind[cur_spr];SetAnimOffs();};
	void SetAnimOffs(){scr_x=0;scr_y=0;for(int i=0;i<=cur_spr;i++){scr_x+=anim->next_x[i];scr_y+=anim->next_y[i];}};
//!Cvet -----------------------
};

typedef std::vector<ItemObj*> item_vect;
/*
struct MiscObj //!Cvet
{
	uint16_t spr_id;

	MiscObj(): spr_id(0){};
	MiscObj(uint16_t _spr_id): spr_id(_spr_id){};
};

typedef vector<MiscObj*> misc_vect;
*/
// структура поля для рисования
struct Field
{
	CCritter* lpcrit; //криттер

	int to_draw;

	int scr_x, scr_y;

	uint16_t tile_id; //тайл земли
	uint16_t roof_id; //тайл крыши
	uint16_t wall_id[MAX_WALL_CNT]; //стены
	scen_vect sc_obj; //сценери
	item_vect itm_obj; //итемы
//	misc_vect msc_obj; //!Cvet другое

	uint32_t pos;

	uint16_t flags;
	BOOL scroll_block; //!Cvet - скролл-локер

	int track; //!Cvet траектория

	Field(): scr_x(0),scr_y(0),tile_id(0),roof_id(0),lpcrit(NULL),to_draw(0),flags(0),
		scroll_block(FALSE),track(0) {for(int i=0;i<MAX_WALL_CNT;i++) wall_id[i]=0;}
};


typedef std::map<uint16_t, char*> char_map;
typedef std::map<uint16_t, uint16_t> word_map;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//!Cvet ++++++++
struct Drop
{
	uint16_t id;

	short x;
	short y;

	short ground_y;

	short adrop;
	//uint32_t dtime;

	Drop():id(0),x(0),y(0),adrop(0),ground_y(0){};
	Drop(uint16_t _id, short _x, short _y, short _ground_y):id(_id),x(_x),y(_y),adrop(0),
		ground_y(_ground_y){};
};

typedef std::vector<Drop*> Rain;
//!Cvet --------

class CHexField
{
public:
	Field hex_field[MAXTILEX][MAXTILEY];

//!Cvet++++++++++++++++++++
	Rain rain;

	int FindStep(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, uint8_t* steps=NULL);
	int CutPath(HexTYPE start_x, HexTYPE start_y, HexTYPE* end_x, HexTYPE* end_y, uint8_t count_correct);
	int FindTarget(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, uint8_t max_weapon_distance);

//траектория
	uint16_t pic_track1,pic_track2;
	bool ShowTrack;
	void SwitchShowTrack();
	BOOL IsShowTrack(){if(ShowTrack==true) return TRUE; return FALSE;}

//загрузка карт
	BOOL MapLoaded;
	BOOL IsMapLoaded() { if(MapLoaded==TRUE) return 1; return 0; };

//добавление/удаление объектов по ходу игры
	int AddObj(stat_obj* add_sobj, HexTYPE x, HexTYPE y, uint16_t tile_flags);
	void ChangeObj(stat_obj* chn_sobj, HexTYPE x, HexTYPE y, uint16_t tile_flags);
	void DelObj(stat_obj* del_sobj, HexTYPE x, HexTYPE y);

//обработака дин. объектов
	void ProcessObj();
	item_vect all_obj; //список всех объектов

//поиск и установка нового центра
	void FindSetCenter(int x, int y);

	uint16_t egg;
//!Cvet--------------------

	int Init(CSpriteManager* lpsm);
	void Clear();

	void PreRestore();
	void PostRestore();
 
	int LoadMap(char* fname); //загрузка карты
	int UnLoadMap(); //!Cvet

	void SetCenter2(int x, int y);

	void OnChangeCol(){RebuildTiles();};

	void DrawMap2();

	int Scroll();

	void SwitchShowHex();
	void SwitchShowRain(); //!Cvet

	void SetCrit(int x,int y,CCritter* pcrit);

//!Cvet +++++
	void TransitCritter(CCritter* pcrit, int dir, int x, int y, bool null_offs);

	void MoveCritter(CCritter* pcrit, int move_dir, int new_x, int new_y);
	void MoveCritter(CCritter* pcrit, int new_x, int new_y);
	void MoveCritter(CCritter* pcrit, int move_dir);

	int GetDir(int old_x, int old_y, int new_x, int new_y);
	int GetFarDir(int old_x, int old_y, int new_x, int new_y);
	void SetMoveOffs(CCritter* pcrit,int dir);
//!Cvet -----

	void RemoveCrit(CCritter* pcrit);

	void RebuildTiles(); //!Cvet вынес из привата

	int GetTilePixel(int pix_x, int pix_y, HexTYPE* tile_x, HexTYPE* tile_y);
	ItemObj* GetItemPixel(int pix_x, int pix_y);
	
	CHexField();
private:
	bool crtd;

	bool ShowRain; //!Cvet
	uint16_t pic_drop; //!Cvet
	uint16_t pic_adrop[7]; //!Cvet
	bool ShowHex;
	uint16_t hex,hexb;

	int cnt_x,cnt_y;

//tiles
	LPDIRECT3DVERTEXBUFFER8 lpVBpr_tile; //подгототвленный буфер
	onesurf_vec prep_vec_tile; //указывается очередность смен текстур при прорисовке буфера
	uint16_t prep_cnt_tile;
//!Cvet roof
	LPDIRECT3DVERTEXBUFFER8 lpVBpr_roof;
	onesurf_vec prep_vec_roof;
	uint16_t prep_cnt_roof;

	ViewField* view2;

	char_map item_proto; //!Cvet
	char_map scen_proto;

	word_map loaded_item; //!Cvet
	word_map loaded_scen;
	word_map loaded_wall;
	word_map loaded_tile;
//	word_map loaded_misc; //!Cvet

	char_map item_fnames; //!Cvet
	char_map scen_fnames;
	char_map wall_fnames;
	char_map tile_fnames;
//	char_map misc_fnames; //!Cvet
	
	dtree_map dtree; //дерево порядка отрисовки объектов.
	dtree_map dtree_roof_rain; //дождь на крыше

	FileManager fm;
	//когда fm загружает файл, он стирает кэш старого, а при загрузке карты он должен оставаться.
	FileManager fm_map; 

	CSpriteManager* lpSM;

	int LoadList(char* lname,int PathType,char_map* pmap);
	int DropScript();
	int LoadObj();

	int hbegin;
	int hend;
	int wright;
	int wleft;

	int v2h,v2w,v2c_x,v2c_y;

	void InitView2(int cx,int cy); // Инициализация области видимости

	int ParseItemObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax); //!Cvet
	int ParseItemObjCont(uint32_t proto_id,uint32_t x,uint32_t y); //!Cvet
	int ParseMiscObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax); //!Cvet
	int ParseScenObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax);
	int ParseWallObj(uint32_t proto_id,uint32_t id,uint32_t x,uint32_t y,uint16_t* hbmax,uint16_t* hemax,uint16_t* wrmax,uint16_t* wlmax);

	int IsVisible(int nx, int ny,uint16_t id);

};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
#endif