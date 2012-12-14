/********************************************************************
	author:	Oleg Mareskin
*********************************************************************/

#include "stdafx.h"

#include "CFileMngr.h"

#include <stdio.h>
#include <assert.h>

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//

char pathlst[][50]=
{
	"maps\\",
	"maps\\wm_mask\\",
};

namespace {

// Returns size of a file.
long FileSize(FILE* file) {
  assert(file != NULL);

  long pos = ftell(file);
  fseek(file, 0, SEEK_END);
  long result = ftell(file);
  fseek(file, pos, SEEK_SET);
  return result;
}

// Reads exactly given number of bytes from the file. 
// Returns -1 if there was some IO error.
int ReadExactly(FILE* file, void* buf, size_t size) {
  assert(file != NULL);
  assert(buf != NULL);
  
  size_t read = 0;
  while (read != size) {
    int err = ::fread(buf, 1, size - read, file);
    if (err <= 0) {
      return false;
    }
    read += err;
  }
  
  return true;
}

// Loads contents of a file. If the file does not exist or
// there is some other IO error, returns false. 
bool LoadFile(const char* path, void** buf, size_t* size) {
  assert(path != NULL);
  assert(buf != NULL);
  assert(size != NULL);

  FILE* file = ::fopen(path, "rb");
  if (file == NULL) {
    return false;
  }
  
  long fsize = FileSize(file);
  if (fsize < 0) {
    ::fclose(file);
    return false;
  }
  
  *size = fsize;
  
  // Allocate the memory. Note this one additional byte,
  // we will later set it to 0.
  *buf = ::malloc(*size + 1);
  if (*buf == NULL) {
    ::fclose(file);
    return false;
  }
  
  if (ReadExactly(file, *buf, *size) == false) {
    ::free(*buf);
    ::fclose(file);
    return false;
  }
  
  // Set the last allocated byte to zero.
  (*(char**)buf)[*size] = 0;
  
  ::fclose(file);
  
  return true;
}

}; // anonymous namespace


bool FileManager::Init() {
  assert(!initialized);
	LogExecStr("FileManager Initialization...\n");
	LogExecStr("FileManager Initialization complete\n");
	initialized = true;
	
	foDataPath = "./";
	return true;
}

void FileManager::Clear() {
	LogExecStr("FileManager Clear...\n");
	UnloadFile();
	LogExecStr("FileManager Clear complete\n");
}

void FileManager::UnloadFile() {
	SAFEDELA(buffer);
}

int FileManager::LoadFile(char* fileName, int pathType) {
  assert(initialized);

	if(!initialized) {
    LogExecStr("FileManager::LoadFile - FileMngr не был иницилазирован до загрузки файла %s",fileName);
		return 0;
	}
	UnloadFile();

  std::string path = "";
  path += pathlst[pathType];
  path += fileName;
  
  std::string fullPath = foDataPath + path;
  
	if (!::LoadFile(fullPath.c_str(), (void**) &buffer, &fileSize)) {
	  LogExecStr("FileManager::LoadFile - файл %s не найден\n", fullPath.c_str());
	  return 0;
	}

	return 1;
}

void FileManager::SetCurrentPosition(uint32_t pos)
{
	if(pos<fileSize) position=pos;
}

void FileManager::GoForward(uint32_t offs)
{
	if((position+offs)<fileSize) position+=offs;
}


int FileManager::GetStr(char* str,uint32_t len)
{
	if(position>=fileSize) return 0;

	int rpos=position;
	int wpos=0;
	for(;wpos<len && rpos<fileSize;rpos++)
	{
		if(buffer[rpos]==0xD)
		{
			rpos+=2;
			break;
		}

		str[wpos++]=buffer[rpos];
	}
	str[wpos]=0;
	position=rpos;

	return 1;
}

int FileManager::Read(void* ptr, size_t size)
{
	if(position+size>fileSize) return 0;

	memcpy(ptr,buffer+position,size);

	position+=size;

	return 1;
}

uint8_t FileManager::GetByte() //!Cvet
{
	if(position>=fileSize) return 0;
	uint8_t res=0;

	res=buffer[position++];

	return res;
}

uint16_t FileManager::GetWord()
{
	if(position>=fileSize) return 0;
	uint16_t res=0;

	uint8_t *cres=(uint8_t*)&res;
	cres[1]=buffer[position++];
	cres[0]=buffer[position++];

	return res;
}

uint16_t FileManager::GetRWord() //!Cvet
{
	if(position>=fileSize) return 0;
	uint16_t res=0;

	uint8_t *cres=(uint8_t*)&res;
	cres[0]=buffer[position++];
	cres[1]=buffer[position++];

	return res;
}

uint32_t FileManager::GetDWord()
{
	if(position>=fileSize) return 0;
	uint32_t res=0;
	uint8_t *cres=(uint8_t*)&res;
	for(int i=3;i>=0;i--)
	{
		cres[i]=buffer[position++];
	}

	return res;
}

uint32_t FileManager::GetRDWord() //!Cvet
{
	if(position>=fileSize) return 0;
	uint32_t res=0;
	uint8_t *cres=(uint8_t*)&res;
	for(int i=0;i<=3;i++)
	{
		cres[i]=buffer[position++];
	}

	return res;
}

int FileManager::GetFullPath(char* fname, int PathType, char* get_path) //!Cvet полный путь к файлу
{
	get_path[0]=0;
	strcpy(get_path,foDataPath.c_str());
	strcat(get_path,pathlst[PathType]);
	strcat(get_path,fname);

	WIN32_FIND_DATA fd;
	HANDLE f=FindFirstFile(get_path,&fd);

	if(f!=INVALID_HANDLE_VALUE) return 1;

//	LogExecStr("Файл:|%s|\n",get_path);

	return 0;
}