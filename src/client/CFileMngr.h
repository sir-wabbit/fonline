#ifndef _CFILEMGR_H_
#define _CFILEMGR_H_

#include "datfile.h"

#define PT_ART_CRITTERS 0
#define PT_ART_INTRFACE 1
#define PT_ART_INVEN    2
#define PT_ART_ITEMS    3
#define PT_ART_MISC     4
#define PT_ART_SCENERY  5
#define PT_ART_SKILLDEX 6
#define PT_ART_SPLASH   7
#define PT_ART_TILES    8
#define PT_ART_WALLS    9

#define PT_MAPS         10

#define PT_PRO_ITEMS    11
#define PT_PRO_MISC     12
#define PT_PRO_SCENERY  13
#define PT_PRO_TILES    14
#define PT_PRO_WALLS    15

#define PT_SND_MUSIC    16
#define PT_SND_SFX      17
	
#define PT_TXT_GAME     18

extern char pathlst[][50];


class CFileMngr
{
public:
	int Init();
	void Clear();
	void UnloadFile();
	int LoadFile(char* fname, int PathType);

	int GetFullPath(char* fname, int PathType, char* get_path); //!Cvet полный путь к файлу

	void SetCurPos(DWORD pos);
	void GoForward(DWORD offs);

	int GetStr(char* str,DWORD len);
	BYTE GetByte(); //!Cvet
	WORD GetWord();
	WORD GetRWord(); //!Cvet
	DWORD GetDWord();
	DWORD GetRDWord(); //!Cvet
	int CopyMem(void* ptr, size_t size);

	BYTE* GetBuf(){ return buf; }; //!Cvet
	DWORD GetFsize(){ return fsize; }; //!Cvet


	CFileMngr(): crtd(0),hFile(NULL),fsize(0),cur_pos(0),buf(NULL),lpDAT(NULL),lpDATcr(NULL){};
private:
	bool crtd;

	HANDLE hFile;
	DWORD fsize;
	DWORD cur_pos;
	BYTE* buf;

	TDatFile *lpDAT,*lpDATcr;

	char master_dat[1024];
	char crit_dat[1024];
	char fo_dat[1024];

	void LoadSimple(HANDLE hFile);
};

#endif