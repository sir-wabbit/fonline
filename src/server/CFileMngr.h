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
	bool Init();
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

	char* GetBuf(){ return buffer; }; //!Cvet
	size_t GetFsize(){ return fileSize; }; //!Cvet


	FileManager(): initialized(0), fileSize(0),position(0),buffer(NULL){};
private:
	bool initialized;

	size_t fileSize;
	size_t position;
	char* buffer;

  std::string masterDatPath;
  std::string critterDatPath;
  std::string foDataPath;
};

#endif