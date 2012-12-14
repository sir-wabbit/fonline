#ifndef __STDAFX_H__
#define __STDAFX_H__

/********************************************************************
	created:	17:08:2004   18:47 updated: begin 2007

	author:		Oleg Mareskin
	add/edit:	Denis Balikhin, Anton Cvetinsky (Cvet)
	
	purpose:	
*********************************************************************/
/*

#include <windows.h>

#define random(a) (rand()*a/(RAND_MAX+1))

#pragma warning (disable : 4786)

#define SAFEREL(x) {if(x) (x)->Release();(x)=NULL;}
#define SAFEDEL(x) {if(x) delete (x);(x)=NULL;}
#define SAFEDELA(x) {if(x) delete[] (x);(x)=NULL;}

struct IntRect
{
	int l;
	int t;
	int r;
	int b;
	IntRect():l(0),t(0),r(0),b(0){};
	IntRect(int al,int at,int ar,int ab):l(al),t(at),r(ar),b(ab){};

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
*/

#endif //__STDAFX_H__