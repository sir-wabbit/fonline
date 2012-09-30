#ifndef __KEYB_H__
#define __KEYB_H__

/********************************************************************
	created:	05:01:2005   22:04
	edit:		07:02:2007   15:15

	author:		Oleg Mareskin
	edit:		Anton Tsvetinsky aka Cvet
	
	purpose:	
*********************************************************************/

#include <stdint.h>

#define LANG_RUS 0
#define LANG_ENG 1

void InitKeyb();
void ClearKeyb();
int GetChar(uint8_t DIK,char* str,int* position,uint16_t max,int lang,int shift); //!Cvet int* position


#endif //__KEYB_H__