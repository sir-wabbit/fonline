#include <dinput.h>

#include "stdafx.h"
#include "keyb.h"
#include "common.h"
//#include <SimpleLeakDetector/SimpleLeakDetector.hpp>

/********************************************************************
  created:  05:01:2005   22:04
  edit:   07:02:2007   15:15

  author:   Oleg Mareskin
  edit:   Denis Balikhin, Anton Tsvetinsky

  purpose:
*********************************************************************/

struct keyb_data
{
  const char* rus;
  const char* s_rus;
  const char* eng;
  const char* s_eng;

  keyb_data(const char* arus, const char* as_rus, const char* aeng, const char* as_eng) {
    rus=arus;
    s_rus=as_rus;
    eng=aeng;
    s_eng=as_eng;
  };
  keyb_data() {
    rus="x";
    s_rus="x";
    eng="x";
    s_eng="x";
  };
};
typedef std::map<uint16_t, keyb_data> keyb_data_map;

keyb_data_map data;

void InitKeyboard()
{
  data[DIK_1]=    keyb_data("1", "!", "1", "!");
  data[DIK_2]=    keyb_data("2", "\"", "2", "@");
  data[DIK_3]=    keyb_data("3", "№", "3", "#");
  data[DIK_4]=    keyb_data("4", ";", "4", "$");
  data[DIK_5]=    keyb_data("5", "%", "5", "%");
  data[DIK_6]=    keyb_data("6", ":", "6", "^");
  data[DIK_7]=    keyb_data("7", "?", "7", "&");
  data[DIK_8]=    keyb_data("8", "*", "8", "*");
  data[DIK_9]=    keyb_data("9", "(", "9", "(");
  data[DIK_0]=    keyb_data("0", ")", "0", ")");
  data[DIK_MINUS]=  keyb_data("-", "_", "-", "_");
  data[DIK_EQUALS]= keyb_data("=", "+", "=", "+");

  data[DIK_Q]=    keyb_data("й", "Й", "q", "Q");
  data[DIK_W]=    keyb_data("ц", "Ц", "w", "W");
  data[DIK_E]=    keyb_data("у", "У", "e", "E");
  data[DIK_R]=    keyb_data("к", "К", "r", "R");
  data[DIK_T]=    keyb_data("е", "Е", "t", "T");
  data[DIK_Y]=    keyb_data("н", "Н", "y", "Y");
  data[DIK_U]=    keyb_data("г", "Г", "u", "U");
  data[DIK_I]=    keyb_data("ш", "Ш", "i", "I");
  data[DIK_O]=    keyb_data("щ", "Щ", "o", "O");
  data[DIK_P]=    keyb_data("з", "З", "p", "P");
  data[DIK_LBRACKET]= keyb_data("х", "Х", "[", "{");
  data[DIK_RBRACKET]= keyb_data("ъ", "Ъ", "]", "}");

  data[DIK_A]=    keyb_data("ф", "Ф", "a", "A");
  data[DIK_S]=    keyb_data("ы", "Ы", "s", "S");
  data[DIK_D]=    keyb_data("в", "В", "d", "D");
  data[DIK_F]=    keyb_data("а", "А", "f", "F");
  data[DIK_G]=    keyb_data("п", "П", "g", "G");
  data[DIK_H]=    keyb_data("р", "Р", "h", "H");
  data[DIK_J]=    keyb_data("о", "О", "j", "J");
  data[DIK_K]=    keyb_data("л", "Л", "k", "K");
  data[DIK_L]=    keyb_data("д", "Д", "l", "L");
  data[DIK_SEMICOLON]=keyb_data("ж", "Ж", ";", ":");
  data[DIK_APOSTROPHE]=keyb_data("э", "Э", "\"" , "\"");

  data[DIK_Z]=    keyb_data("я", "Я", "z", "Z");
  data[DIK_X]=    keyb_data("ч", "Ч", "x", "X");
  data[DIK_C]=    keyb_data("с", "С", "c", "C");
  data[DIK_V]=    keyb_data("м", "М", "v", "V");
  data[DIK_B]=    keyb_data("и", "И", "b", "B");
  data[DIK_N]=    keyb_data("т", "Т", "n", "N");
  data[DIK_M]=    keyb_data("ь", "Ь", "m", "M");

  data[DIK_COMMA]=  keyb_data("б", "Б", ",", "<");
  data[DIK_PERIOD]= keyb_data("ю", "Ю", ".", ">");
  data[DIK_SLASH]=  keyb_data(".", ",", "/", "?");

  data[DIK_SPACE]=  keyb_data(" ", " ", " ", " ");

  data[DIK_GRAVE]=  keyb_data("ё", "Ё", "`", "~"); //!Cvet
}

void ClearKeyb()
{
  data.clear();
}

int GetChar(uint16_t DIK, char* str, int* position, uint16_t max, int lang, int shift) //!Cvet int* position
{
  if(max>=2000) return 0; //!Cvet

  int current = 0;

  if(!position) {
    current = strlen(str);
  } else {
    current = *position;
  }

  // FIXME[21.12.2012 alex]: unicode

  if(DIK==DIK_RIGHT) {
    if (str[current]) {
      current++;
    }

    if (position) {
      *position = current;
    }

    return 1;
  } else if(DIK==DIK_LEFT) {
    if(current) current--;

    if(position) *position=current;

    return 1;
  }

  if(DIK==DIK_BACK) {
    if(!str[0]) return 1;
    if(!current) return 1; //!Cvet

    current--; //!Cvet

  int i;
    for(i=current;str[i];i++)
      if(str[i+1]) str[i]=str[i+1]; //!Cvet
    str[i-1]=0; //!Cvet

    if(position) *position=current; //!Cvet
    return 1;
  } else if (DIK==DIK_DELETE) //!Cvet
  {
    if(!str[0]) return 1;
    if(!str[current]) return 1; //!Cvet

  int i;
    for(i=current;str[i];i++)
      if(str[i+1]) str[i]=str[i+1]; //!Cvet
    str[i-1]=0; //!Cvet

    return 1;
  }

  keyb_data_map::iterator it=data.find(DIK);
  if (it==data.end()) {
    return 0;
  }

  //for(int i=0;str[i];i++)
  //  if(i==(max-1)) return 1;

  const char* text;
  size_t textLen;
  if (lang == LANG_RUS) {
    text = shift?(*it).second.s_rus:(*it).second.rus;
  } else {
    text = shift?(*it).second.s_eng:(*it).second.eng;
  }
  textLen = strlen(text);


  if (strlen(str) + textLen > max) {
    return 1;
  }

  char rest[2048];
  strncpy_s(rest, sizeof(rest), str + current, sizeof(rest) - 1);
  strcpy(str + current, text);
  current += textLen;
  strcat(str + current, rest);

  if (position) {
    *position = current;
  }

  return 1;
}
