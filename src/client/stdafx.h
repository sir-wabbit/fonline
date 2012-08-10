#ifndef __STDAFX_H__
#define __STDAFX_H__

/********************************************************************
	created:	17:08:2004   18:47 updated: begin 2007

	author:		Oleg Mareskin
	add/edit:	Denis Balikhin, Anton Cvetinsky (Cvet)
	
	purpose:	
*********************************************************************/


#include <windows.h>
#include "resource.h"

#define random(a) (rand()*a/(RAND_MAX+1))

#include <d3dx8.h>
#include <dinput.h>
#include <dxerr8.h>
#include <dsound.h> //!Cvet

#pragma warning (disable : 4786)

#include <crtdbg.h>

#define SAFEREL(x) {if(x) (x)->Release();(x)=NULL;}
#define SAFEDEL(x) {if(x) delete (x);(x)=NULL;}
#define SAFEDELA(x) {if(x) delete[] (x);(x)=NULL;}

struct INTRECT
{
	int l;
	int t;
	int r;
	int b;
	INTRECT():l(0),t(0),r(0),b(0){};
	INTRECT(int al,int at,int ar,int ab):l(al),t(at),r(ar),b(ab){};

	int& operator[](int index) //!Cvet +++
	{ 
		switch(index)
		{
		case 0: return l;
		case 1: return t;
		case 2: return r;
		case 3: return b;
		}
		return l;
	} //!Cvet ---
};

//#define MODE_WIDTH (opt_screen_mode?1024:800)
//#define MODE_HEIGHT (opt_screen_mode?768:600)
#define MODE_WIDTH (screen_width[opt_screen_mode]) //!Cvet
#define MODE_HEIGHT (screen_height[opt_screen_mode]) //!Cvet

//!Cvet ++++
//для работы с двоичными константами
#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
#define BIN8(v) (BIN__B(BIN__N(0x##v)))

#define BIN16(bin16,bin8)	((BIN8(bin16)<<8)|(BIN8(bin8)))

//для работы с битами (по игре - флагами)
#define BITS(x,y) ((x)&(y))
#define FLAG BITS

#define SET_BITS(x,y) (x)=(x)|(y)
#define SETFLAG SET_BITS

//#define UNSET_BITS(x,y) {if((x)&(y)) (x)=(x)^(y);}
#define UNSET_BITS(x,y) (x)=((x)|(y))^(y)
#define UNSETFLAG UNSET_BITS

#define BREAK_BEGIN do{
#define BREAK_END }while(0)
//!Cvet ----

#endif //__STDAFX_H__