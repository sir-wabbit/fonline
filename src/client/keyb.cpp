
#include "stdafx.h"
#include "keyb.h"
#include "common.h"

/********************************************************************
	created:	05:01:2005   22:04
	edit:		07:02:2007   15:15

	author:		Oleg Mareskin
	edit:		Denis Balikhin, Anton Tsvetinsky
	
	purpose:	
*********************************************************************/

struct keyb_data 
{
	char rus,s_rus,eng,s_eng;
	keyb_data(char arus,char as_rus,char aeng,char as_eng) {rus=arus;s_rus=as_rus;eng=aeng;s_eng=as_eng;};
	keyb_data() {rus='x';s_rus='x';eng='x';s_eng='x';};
};
typedef map<BYTE, keyb_data, less<BYTE> > keyb_data_map;

keyb_data_map data;

void InitKeyb()
{
	data[DIK_1]=		keyb_data('1','!','1','!');
	data[DIK_2]=		keyb_data('2','"','2','@');
	data[DIK_3]=		keyb_data('3','¹','3','#');
	data[DIK_4]=		keyb_data('4',';','4','$');
	data[DIK_5]=		keyb_data('5','%','5','%');
	data[DIK_6]=		keyb_data('6',':','6','^');
	data[DIK_7]=		keyb_data('7','?','7','&');
	data[DIK_8]=		keyb_data('8','*','8','*');
	data[DIK_9]=		keyb_data('9','(','9','(');
	data[DIK_0]=		keyb_data('0',')','0',')');
	data[DIK_MINUS]=	keyb_data('-','_','-','_');
	data[DIK_EQUALS]=	keyb_data('=','+','=','+');
	
	data[DIK_Q]=		keyb_data('é','É','q','Q');
	data[DIK_W]=		keyb_data('ö','Ö','w','W');
	data[DIK_E]=		keyb_data('ó','Ó','e','E');
	data[DIK_R]=		keyb_data('ê','Ê','r','R');
	data[DIK_T]=		keyb_data('å','Å','t','T');
	data[DIK_Y]=		keyb_data('í','Í','y','Y');
	data[DIK_U]=		keyb_data('ã','Ã','u','U');
	data[DIK_I]=		keyb_data('ø','Ø','i','I');
	data[DIK_O]=		keyb_data('ù','Ù','o','O');
	data[DIK_P]=		keyb_data('ç','Ç','p','P');
	data[DIK_LBRACKET]=	keyb_data('õ','Õ','[','{');
	data[DIK_RBRACKET]=	keyb_data('ú','Ú',']','}');
	
	data[DIK_A]=		keyb_data('ô','Ô','a','A');
	data[DIK_S]=		keyb_data('û','Û','s','S');
	data[DIK_D]=		keyb_data('â','Â','d','D');
	data[DIK_F]=		keyb_data('à','À','f','F');
	data[DIK_G]=		keyb_data('ï','Ï','g','G');
	data[DIK_H]=		keyb_data('ð','Ð','h','H');
	data[DIK_J]=		keyb_data('î','Î','j','J');
	data[DIK_K]=		keyb_data('ë','Ë','k','K');
	data[DIK_L]=		keyb_data('ä','Ä','l','L');
	data[DIK_SEMICOLON]=keyb_data('æ','Æ',';',':');
	data[DIK_APOSTROPHE]=keyb_data('ý','Ý', '\'' ,'\"');

	data[DIK_Z]=		keyb_data('ÿ','ß','z','Z');
	data[DIK_X]=		keyb_data('÷','×','x','X');
	data[DIK_C]=		keyb_data('ñ','Ñ','c','C');
	data[DIK_V]=		keyb_data('ì','Ì','v','V');
	data[DIK_B]=		keyb_data('è','È','b','B');
	data[DIK_N]=		keyb_data('ò','Ò','n','N');
	data[DIK_M]=		keyb_data('ü','Ü','m','M');
	
	data[DIK_COMMA]=	keyb_data('á','Á',',','<');
	data[DIK_PERIOD]=	keyb_data('þ','Þ','.','>');
	data[DIK_SLASH]=	keyb_data('.',',','/','?');

	data[DIK_SPACE]=	keyb_data(' ',' ',' ',' ');

	data[DIK_GRAVE]=	keyb_data('¸','¨','`','~'); //!Cvet
}

void ClearKeyb()
{
	data.clear();
}

int GetChar(BYTE DIK,char* str,int* position,WORD max,int lang,int shift) //!Cvet int* position
{
	if(max>=2000) return 0; //!Cvet

	int posit=0;
	if(!position)
	{
	  // XXX[27.7.2012 alex]: some fucking dumbass doesn't know what strlen is.
	  int i = 0;
		for(i=0;str[i];i++);
		posit=i;
	}
	else
	{
		posit=*position;
	}

//WriteLog("DIK:|%d|\n",DIK);
	//!Cvet +++
	if(DIK==DIK_RIGHT)
	{
		if(str[posit]) posit++;

		if(position) *position=posit;

		return 1;
	}
	if(DIK==DIK_LEFT)
	{
		if(posit) posit--;

		if(position) *position=posit;

		return 1;
	}
	//!Cvet ---
	if(DIK==DIK_BACK)
	{
		if(!str[0]) return 1;
		if(!posit) return 1; //!Cvet

		posit--; //!Cvet
    
    int i;
		for(i=posit;str[i];i++)
			if(str[i+1]) str[i]=str[i+1]; //!Cvet
		str[i-1]=0; //!Cvet

		if(position) *position=posit; //!Cvet
		return 1;
	}
	if(DIK==DIK_DELETE) //!Cvet
	{
		if(!str[0]) return 1;
		if(!str[posit]) return 1; //!Cvet

    int i;
		for(i=posit;str[i];i++)
			if(str[i+1]) str[i]=str[i+1]; //!Cvet
		str[i-1]=0; //!Cvet

		return 1;
	}
	
	keyb_data_map::iterator it=data.find(DIK);
	if(it==data.end()) return 0;

	for(int i=0;str[i];i++)
		if(i==(max-1)) return 1;

	char piece[2048];
	piece[0]=0;
	if(str[posit])
		strcpy(piece,&str[*position]);

	str[posit]=0;

	if(lang==LANG_RUS)
		str[posit]=shift?(*it).second.s_rus:(*it).second.rus; //!Cvet i -> posit
	else
		str[posit]=shift?(*it).second.s_eng:(*it).second.eng; //!Cvet i -> posit

	str[++posit]=0; //!Cvet
	strcat(&str[posit],piece); //!Cvet

	if(position) *position=posit; //!Cvet

	return 1;
}