#ifndef _CSPRITEMGR_H_
#define _CSPRITEMGR_H_

#include <d3dx8.h>
#include <dinput.h>
#include <dxerr8.h>
#include <dsound.h>

#include <vector>
#include <map>

#include <assert.h>

#include "CFileMngr.h"
#include "netproto.h"

#include <base/math/rect.hpp>

//!Cvet ++++
#define COLOR_DEFAULT		D3DCOLOR_ARGB(255,((opt_gcolor_default >> 16) & 0xFF)+opt_light,\
							((opt_gcolor_default >> 8) & 0xFF)+opt_light,(opt_gcolor_default & 0xFF)+opt_light)

#define COLOR_CRITNAME		opt_tcolor_namecrit

#define COLOR_TEXT_DEFAULT	opt_tcolor_default
#define COLOR_TEXT_SHOUT	opt_tcolor_shout
#define COLOR_TEXT_WHISP	opt_tcolor_whisp
#define COLOR_TEXT_EMOTE	opt_tcolor_emote
#define COLOR_TEXT_SOCIAL	opt_tcolor_social

//#define COLOR_TEXT_AST	D3DCOLOR_ARGB(255,255,0,0)
//!Cvet ----

typedef std::map<CritterType, std::string> ctypes_map; //!Cvet перенес сюда

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
struct MYVERTEX
{
	FLOAT x,y,z,rhw;
	uint32_t Diffuse;
	FLOAT tu,tv;

	MYVERTEX(): x(0),y(0),z(0),rhw(1),tu(0),tv(0),Diffuse(0){};
};
#define D3DFVF_MYVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct FLTRECT
{
	FLOAT x1,y1,x2,y2;
	FLTRECT():x1(0),y1(0),x2(0),y2(0){};
	FLTRECT(FLOAT _x1,FLOAT _y1,FLOAT _x2,FLOAT _y2):x1(_x1),y1(_y1),x2(_x2),y2(_y2){};

	void operator() (FLOAT _x1,FLOAT _y1,FLOAT _x2,FLOAT _y2) {x1=_x1;y1=_y1;x2=_x2;y2=_y2;};
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

typedef std::vector<LPDIRECT3DTEXTURE8> surf_vect;

// структура спрайта для DX8
struct SpriteInfo
{
	LPDIRECT3DTEXTURE8 lpSurf;
	FLTRECT spr_rect;
	uint16_t w,h;
	short offs_x,offs_y;
	SpriteInfo(): lpSurf(NULL),w(0),h(0),offs_x(0),offs_y(0){};
};

typedef std::map<uint16_t, SpriteInfo*> sprinfo_map;

struct OneSurface
{
	LPDIRECT3DTEXTURE8 lpSurf;
	uint16_t cnt;
	OneSurface(LPDIRECT3DTEXTURE8 lps):lpSurf(lps),cnt(1){};
};
typedef std::vector<OneSurface*> onesurf_vec;

struct PrepSprite
{
	int scr_x, scr_y;
	uint16_t spr_id;
	uint16_t* lp_sprid;
	short* lp_ox;
	short* lp_oy;
	uint8_t* alpha; //!Cvet

	PrepSprite() {};
	PrepSprite(int x, int y, uint16_t id,uint16_t* lpid=NULL,short* alp_ox=NULL,short* alp_oy=NULL, uint8_t* _alpha=NULL): 
	scr_x(x), scr_y(y), spr_id(id),lp_sprid(lpid),lp_ox(alp_ox),lp_oy(alp_oy),alpha(_alpha) {};
};
typedef std::multimap<uint32_t, PrepSprite*> dtree_map;

typedef std::vector<PrepSprite*> spr_vec; //!Cvet
typedef std::vector<spr_vec> spr_vec_vec; //!Cvet

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

struct CritFrames 
{
  void __invariant() {
    assert(this != NULL);
    assert(ind != NULL);
    assert(next_x != NULL);
    assert(next_y != NULL);
  }

	uint16_t* ind; //индексы кадров анимации
	uint16_t dir_offs[6]; //смещения в индексах для всех 6-ти направлений.
	short *next_x;//смещение кадров относительно предыдущих
	short *next_y;//смещение кадров относительно предыдущих
	uint8_t cnt_frames; //кол-во кадров в анимации на каждое направление
	uint16_t ticks;

	CritFrames(): ind(NULL), next_x(NULL), next_y(NULL) { };
	~CritFrames() {
	  if (ind != NULL) {
	    delete [] ind;
	    ind = NULL;
	  }
	  if (next_x != NULL) {
	    delete [] next_x;
	    next_x = NULL;
	  }
    if (next_y != NULL) {
      delete [] next_y;
      next_y = NULL;
    }
	};
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
// Фреймы любые
struct AnyFrames 
{
	short offs_x; //!Cvet
	short offs_y; //!Cvet

	uint16_t* ind; //индексы кадров анимации
	short *next_x;//смещение кадров относительно предыдущих
	short *next_y;//смещение кадров относительно предыдущих
	uint8_t cnt_frames; //кол-во кадров в анимации
	uint16_t ticks;// периоды анимаций

	AnyFrames(): ind(NULL),next_x(NULL),next_y(NULL),offs_x(0),offs_y(0){};
	~AnyFrames() {
    if (ind != NULL) {
      delete [] ind;
      ind = NULL;
    }
    if (next_x != NULL) {
      delete [] next_x;
      next_x = NULL;
    }
    if (next_y != NULL) {
      delete [] next_y;
      next_y = NULL;
    }
	};
};
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

//!Cvet +++++++
struct PrepPix
{
	short x;
	short y;
	short* lp_ox;
	short* lp_oy;
	uint16_t* lp_ox2;
	uint16_t* lp_oy2;

	uint32_t color;

	PrepPix():x(0),y(0),color(0),lp_ox(NULL),lp_oy(NULL),lp_ox2(NULL),lp_oy2(NULL){};
	PrepPix(short _x, short _y, uint32_t _color,
		short* _lp_ox=NULL, short* _lp_oy=NULL, uint16_t* _lp_ox2=NULL, uint16_t* _lp_oy2=NULL):
	x(_x),y(_y),color(_color),lp_ox(_lp_ox),lp_oy(_lp_oy),lp_ox2(_lp_ox2),lp_oy2(_lp_oy2){};
};

typedef std::vector<PrepPix*> Pix_vec;
//!Cvet -------

class CSpriteManager
{
public:

	int Init(LPDIRECT3DDEVICE8 lpD3Device);
	void Clear();

	void PreRestore();
	void PostRestore();

	int LoadRix(char *fname, int PathType);
	int LoadMiniSprite(char *fname,double size,int PathType,SpriteInfo** ppInfo=NULL); // Для инвентаря уменьшенные спрайты
	int LoadSprite(char *fname,int PathType,SpriteInfo** ppInfo=NULL);
	int LoadAnimation(char *fname,int PathType,CritFrames* pframes);
	int LoadAnimationD(char *fname,int PathType,CritFrames* pframes); //!Cvet edit
	int LoadAnyAnimation(char *fname,int PathType, AnyFrames* aanim,SpriteInfo** ppInfo = NULL);

	void NextSurface();
	SpriteInfo* GetSpriteInfo(uint16_t id) {return spriteData[id];};

	int PrepareBuffer(dtree_map* lpdtree,LPDIRECT3DVERTEXBUFFER8* lplpBuf,onesurf_vec* lpsvec, uint32_t color=NULL, uint8_t alpha=NULL); //!Cvet uint32_t color

	int Flush();

	int DrawSprite(uint16_t id, int x, int y, uint32_t color, uint32_t alpha=NULL); //!Cvet uint32_t color uint32_t alpha
  int DrawSpriteSize(uint16_t id, int x, int y,double size, uint32_t color); // Динамически масштабирует //!Cvet uint32_t color
	void DrawTreeCntr(dtree_map* lpdtree);
	void DrawPrepared(LPDIRECT3DVERTEXBUFFER8 lpBuf,onesurf_vec* lpsvec, uint16_t cnt);

	uint32_t GetLoadedCnt() {return spriteData.size();};

	void SetColor(uint32_t c){col=c;};

	LPDIRECT3DVERTEXBUFFER8 GetVB() {return lpVB;};
	LPDIRECT3DINDEXBUFFER8 GetIB() {return lpIB;};

  void GetDrawCntrRect(PrepSprite* prep, fonline::math::Rect<int>* prect);

	CSpriteManager();
	~CSpriteManager(){
	  for(int d1 = 0; d1 < 100; d1++) {
		  for(int d2 = 0; d2 < 27; d2++) {
			  for(int d3 = 0; d3 < 27; d3++) {
				  if (CrAnim[d1][d2][d3]) {
				    delete [] CrAnim[d1][d2][d3];
				    CrAnim[d1][d2][d3] = NULL;
				  }
				}
		  }
		}
	}; //!Cvet

//!Cvet ++++++++++++++++++++++++++++++++++++++
	ctypes_map crit_types;
	CritFrames* CrAnim[150][27][27];
	int LoadCritTypes();
	int LoadAnimCr(CritterType anim_type, uint8_t anim_ind1, uint8_t anim_ind2);
	int EraseAnimCr(CritterType anim_type, uint8_t anim_ind1, uint8_t anim_ind2);

	int CheckPixTransp();

	void DrawPrepPix(Pix_vec* prep_pix);
	void DrawPrepLines(Pix_vec* prep_pix);
//!Cvet --------------------------------------
private:
	bool initialized;

	int LoadSpriteAlt(char *fname,int PathType,SpriteInfo** ppInfo=NULL); //!Cvet загрузка альтернативной графики

	surf_vect surfaceList;
	sprinfo_map spriteData;
	
	onesurf_vec call_vec;
	OneSurface* last_call;

	uint32_t next_id;

	LPDIRECT3DTEXTURE8 lastSurface;
	LPDIRECT3DTEXTURE8 currentSurface;
	uint16_t last_w,last_h;//размеры загружаемой тестуры
	uint16_t busy_w,busy_h;//размеры занятой области
	uint16_t free_x,free_y;//размеры занятой области
  
	int maxSpriteCount;//на сколько спрайтов рассчитан. при достижении рисуется сам.
	int currentPosition;//текущая позиция записи.

	LPDIRECT3DDEVICE8 lpDevice;

	LPDIRECT3DVERTEXBUFFER8 lpVB;//буфер с вершинами для спрайтов
	LPDIRECT3DINDEXBUFFER8 lpIB;//буфер с индексами

	//Буфер вершин в системной памяти для постоянно меняющихся спрайтов
	MYVERTEX* lpWaitBuf;

	LPDIRECT3DTEXTURE8 CreateNewSurf(uint16_t w, uint16_t h);

	FileManager fm;

	uint32_t col;
};

#endif