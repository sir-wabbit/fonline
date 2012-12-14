/********************************************************************
	author:	Oleg Mareskin
*********************************************************************/

#ifndef _CFILEMGR_H_
#define _CFILEMGR_H_

#include "main.h"
#include <stdint.h>
#include <string>

#define PT_SERVER_MAPS		0
#define PT_SERVER_MASKS	1

extern char pathlst[][50];

class FileManager
{
public:
	bool Init();
	void Clear();
	void UnloadFile();
	int LoadFile(char* fname, int PathType);

	int GetFullPath(char* fname, int PathType, char* get_path); //!Cvet полный путь к файлу

	void SetCurrentPosition(uint32_t pos);
	void GoForward(uint32_t offs);

	int GetStr(char* str,uint32_t len);
	uint8_t GetByte(); //!Cvet
	uint16_t GetWord();
	uint16_t GetRWord(); //!Cvet
	uint32_t GetDWord();
	uint32_t GetRDWord(); //!Cvet
	int Read(void* ptr, size_t size);

	char* GetBufferPtr(){ return buffer; }; //!Cvet
	size_t GetFileSize(){ return fileSize; }; //!Cvet


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