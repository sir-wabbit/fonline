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

const BYTE HEX_IMPASSABLE			=1;
const BYTE HEX_IMPASSABLE_NOT_RAKED	=2;

//поиск пути
//максимальный путь
const BYTE FINDPATH_MAX_PATH		=50;
//возвращаемые ошибки
const BYTE FP_ERROR					=0;
const BYTE FP_OK					=1;
const BYTE FP_DEADLOCK				=2;
const BYTE FP_TOOFAR				=3;
const BYTE FP_ALREADY_HERE			=4;

//поиск цели
const BYTE FINDTARGET_BARRIER		=10;
const BYTE FINDTARGET_TOOFAR		=11;
const BYTE FINDTARGET_ERROR			=12;
const BYTE FINDTARGET_INVALID_TARG	=13;

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
	WORD spr_id;

	ScenObj(): spr_id(0){};
	ScenObj(WORD _spr_id): spr_id(_spr_id){};
};

typedef vector<ScenObj*> scen_vect;

struct ItemObj
{
	WORD spr_id;

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
	ItemObj(stat_obj* _sobj, WORD _spr_id, int _hex_x, int _hex_y, short _scr_x, short _scr_y, int* _hex_scr_x, int* _hex_scr_y):
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

typedef vector<ItemObj*> item_vect;
/*
struct MiscObj //!Cvet
{
	WORD spr_id;

	MiscObj(): spr_id(0){};
	MiscObj(WORD _spr_id): spr_id(_spr_id){};
};

typedef vector<MiscObj*> misc_vect;
*/
// структура поля для рисования
struct Field
{
	CCritter* lpcrit; //криттер

	int to_draw;

	int scr_x, scr_y;

	WORD tile_id; //тайл земли
	WORD roof_id; //тайл крыши
	WORD wall_id[MAX_WALL_CNT]; //стены
	scen_vect sc_obj; //сценери
	item_vect itm_obj; //итемы
//	misc_vect msc_obj; //!Cvet другое

	DWORD pos;

	WORD flags;
	BOOL scroll_block; //!Cvet - скролл-локер

	int track; //!Cvet траектория

	Field(): scr_x(0),scr_y(0),tile_id(0),roof_id(0),lpcrit(NULL),to_draw(0),flags(0),
		scroll_block(FALSE),track(0) {for(int i=0;i<MAX_WALL_CNT;i++) wall_id[i]=0;}
};


typedef map<WORD, char*, less<WORD> > char_map;
typedef map<WORD, WORD, less<WORD> > word_map;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//!Cvet ++++++++
struct Drop
{
	WORD id;

	short x;
	short y;

	short ground_y;

	short adrop;
	//DWORD dtime;

	Drop():id(0),x(0),y(0),adrop(0),ground_y(0){};
	Drop(WORD _id, short _x, short _y, short _ground_y):id(_id),x(_x),y(_y),adrop(0),
		ground_y(_ground_y){};
};

typedef vector<Drop*> Rain;
//!Cvet --------

class CHexField
{
public:
	Field hex_field[MAXTILEX][MAXTILEY];

//!Cvet++++++++++++++++++++
	Rain rain;

	int FindStep(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, BYTE* steps=NULL);
	int CutPath(HexTYPE start_x, HexTYPE start_y, HexTYPE* end_x, HexTYPE* end_y, BYTE count_correct);
	int FindTarget(HexTYPE start_x, HexTYPE start_y, HexTYPE end_x, HexTYPE end_y, BYTE max_weapon_distance);

//траектория
	WORD pic_track1,pic_track2;
	bool ShowTrack;
	void SwitchShowTrack();
	BOOL IsShowTrack(){if(ShowTrack==true) return TRUE; return FALSE;}

//загрузка карт
	BOOL MapLoaded;
	BOOL IsMapLoaded() { if(MapLoaded==TRUE) return 1; return 0; };

//добавление/удаление объектов по ходу игры
	int AddObj(stat_obj* add_sobj, HexTYPE x, HexTYPE y, WORD tile_flags);
	void ChangeObj(stat_obj* chn_sobj, HexTYPE x, HexTYPE y, WORD tile_flags);
	void DelObj(stat_obj* del_sobj, HexTYPE x, HexTYPE y);

//обработака дин. объектов
	void ProcessObj();
	item_vect all_obj; //список всех объектов

//поиск и установка нового центра
	void FindSetCenter(int x, int y);

	WORD egg;
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
	WORD pic_drop; //!Cvet
	WORD pic_adrop[7]; //!Cvet
	bool ShowHex;
	WORD hex,hexb;

	int cnt_x,cnt_y;

//tiles
	LPDIRECT3DVERTEXBUFFER8 lpVBpr_tile; //подгототвленный буфер
	onesurf_vec prep_vec_tile; //указывается очередность смен текстур при прорисовке буфера
	WORD prep_cnt_tile;
//!Cvet roof
	LPDIRECT3DVERTEXBUFFER8 lpVBpr_roof;
	onesurf_vec prep_vec_roof;
	WORD prep_cnt_roof;

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

	CFileMngr fm;
	//когда fm загружает файл, он стирает кэш старого, а при загрузке карты он должен оставаться.
	CFileMngr fm_map; 

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

	int ParseItemObj(DWORD proto_id,DWORD id,DWORD x,DWORD y,WORD* hbmax,WORD* hemax,WORD* wrmax,WORD* wlmax); //!Cvet
	int ParseItemObjCont(DWORD proto_id,DWORD x,DWORD y); //!Cvet
	int ParseMiscObj(DWORD proto_id,DWORD id,DWORD x,DWORD y,WORD* hbmax,WORD* hemax,WORD* wrmax,WORD* wlmax); //!Cvet
	int ParseScenObj(DWORD proto_id,DWORD id,DWORD x,DWORD y,WORD* hbmax,WORD* hemax,WORD* wrmax,WORD* wlmax);
	int ParseWallObj(DWORD proto_id,DWORD id,DWORD x,DWORD y,WORD* hbmax,WORD* hemax,WORD* wrmax,WORD* wlmax);

	int IsVisible(int nx, int ny,WORD id);

};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
#endif