#ifndef __STDAFX_H__
#define __STDAFX_H__

/********************************************************************
	created:	18:08:2004   22:42

	author:		Oleg Mareskin
	
	purpose:	
*********************************************************************/

#define FD_SETSIZE 1024

#include <windows.h>
#include <mysql.h>

#include "math.h" //!Cvet

#define random(a) (rand()*a/(RAND_MAX+1))

#ifndef FOSERVICE_VERSION
	#include <richedit.h>
	#include "resource.h"
	#include <crtdbg.h>
#endif

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmySQL.lib")

#pragma warning (disable : 4786)

#include <map>
#include <string> //!Cvet
#include <set> //!Cvet
#include <list> //!Cvet
#include <vector>
using namespace std;

//!Cvet скриптинг ++++
//#define ANGELSCRIPT_EXPORT 1
#include "AngelScript/angelscript.h"
#include "AngelScript/scriptstring.h"
//#pragma comment(lib,"angelscript.lib")
//!Cvet ----

#define SAFEREL(x) {if(x) (x)->Release();(x)=NULL;}
#define SAFEDEL(x) {if(x) delete (x);(x)=NULL;}
#define SAFEDELA(x) {if(x) delete[] (x);(x)=NULL;}


#define chSTR2(x) #x
#define chSTR(x) chSTR2(x)
#define chMSG(desc) message(__FILE__ "(" chSTR(__LINE__) "):" #desc)

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

//убирание минуса
#define ABS(x) ((x)>=0?(x):-(x))

//
#define BREAK_BEGIN do{
#define BREAK_END }while(0)
//!Cvet ----

#endif //__STDAFX_H__