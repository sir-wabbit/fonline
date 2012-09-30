#ifndef __CFONT_H__
#define __CFONT_H__

/********************************************************************
	created:	01:12:2004   07:20

	author:		Oleg Mareskin
	edit:		Anton Tsvetinsky aka Cvet
	
	purpose:	
*********************************************************************/

#include "CSpriteManager.h"

struct Letter
{
  uint16_t dx,dy;
  uint8_t w,h;
  short y_offs;
  Letter():dx(0),dy(0),w(0),h(0){};
};

//!Cvet +++++++++++++
struct Font
{
	LPDIRECT3DTEXTURE8 font_surf;

	Letter let[256];
	int eth;
	int etw;
	int *maxx;
	USHORT max_cnt;
	int cur_pos;
	FLOAT xyarr[256][4];
};

#define MAX_FONT		3

#define FONT_DEF		0
#define FONT_NUM		1
#define FONT_BIG_NUM	2
#define FONT_NOT_NUM	3
//!Cvet -------------

#define FT_NOBREAK		1
#define FT_CENTERX		2
#define FT_CENTERY		4
#define FT_BOTTOM		8
//!Cvet +++
#define FT_UPPER		16 //текст выводиться начиная с нижней границы
#define FT_COLORIZE		32 //раскрасска. без флага цвет береться из передоваемого аргумента col
//!Cvet ---

class CFOFont
{
public:

	int Init(LPDIRECT3DDEVICE8 lpD3Device,LPDIRECT3DVERTEXBUFFER8 aVB,LPDIRECT3DINDEXBUFFER8 aIB);
	void Clear();

	void PreRestore();
	void PostRestore(LPDIRECT3DVERTEXBUFFER8 aVB,LPDIRECT3DINDEXBUFFER8 aIB);

	void MyDrawText(RECT r,char* astr,uint32_t flags, uint32_t col=NULL, int num_font=FONT_DEF);
  
	
     CFOFont();
	~CFOFont();
private:

	int Flush(int* cur_pos); //!Cvet int* cur_pos

	bool crtd;

	int spr_cnt;//на сколько спрайтов рассчитан. при достижении рисуется сам.

	LPDIRECT3DDEVICE8 lpDevice;

	LPDIRECT3DVERTEXBUFFER8 lpVB;//буфер с вершинами для спрайтов
	LPDIRECT3DINDEXBUFFER8 lpIB;//буфер с индексами

	//Буфер вершин в системной памяти для постоянно меняющихся спрайтов
	MYVERTEX* lpWaitBuf;

	Font fonts[MAX_FONT];
};


#endif //__CFONT_H__