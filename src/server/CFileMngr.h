/********************************************************************
	author:	Oleg Mareskin
*********************************************************************/

#ifndef _CFILEMGR_H_
#define _CFILEMGR_H_

#include "main.h"

#define PT_MAPS		0
#define PT_MASKS	1

extern char pathlst[][50];

class FileManager
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


	FileManager(): crtd(0),hFile(NULL),fsize(0),cur_pos(0),buf(NULL){};
private:
	bool crtd;

	HANDLE hFile;
	DWORD fsize;
	DWORD cur_pos;
	BYTE* buf;


	char master_dat[1024];
	char crit_dat[1024];
	char fo_dat[1024];

	void LoadSimple(HANDLE hFile);
};

#endif