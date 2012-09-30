#ifndef _CFILEMGR_H_
#define _CFILEMGR_H_

#include "datfile.h"
#include <stdint.h>

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


class FileManager {
public:
	int Init();
	void Clear();
	void UnloadFile();
	int LoadFile(char* fname, int PathType);

	int GetFullPath(char* fname, int PathType, char* get_path); //!Cvet полный путь к файлу

	void SetCurPos(uint32_t pos);
	void GoForward(uint32_t offs);

	int GetStr(char* str,uint32_t len);
	uint8_t GetByte(); //!Cvet
	uint16_t GetWord();
	uint16_t GetRWord(); //!Cvet
	uint32_t GetDWord();
	uint32_t GetRDWord(); //!Cvet
	int CopyMem(void* ptr, size_t size);

	uint8_t* GetBuf(){ return buffer; }; //!Cvet
	uint32_t GetFsize(){ return fileSize; }; //!Cvet


	FileManager(): initialized(0),file(NULL),fileSize(0),position(0),buffer(NULL),lpDAT(NULL),lpDATcr(NULL){};
private:
	bool initialized;

	HANDLE file;
	uint32_t fileSize;
	uint32_t position;
	uint8_t* buffer;

	TDatFile *lpDAT,*lpDATcr;

	char master_dat[1024];
	char crit_dat[1024];
	char fo_dat[1024];
};

#endif