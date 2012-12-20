#include "FileManager.hpp"

#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>

#include <FOnlineCommon/Common.hpp>

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

int FileExists(const char *fileName) {
  struct stat buffer;   
  return (stat(fileName, &buffer) == 0);
}

/*const char* ErrorToString(FileManagerError err) {
  switch(err) {
    case FM_ERROR_NONE:           return "<none>";
    case FM_ERROR_NO_MASTER_DAT:  return "Could not find master.dat / or master data directory.";
    case FM_ERROR_FILE_NOT_FOUND: return "Could not find file.";
    case FM_ERROR_NO_CRITTER_DAT: return "Could not find critter.dat / or critter data directory.";
    case FM_ERROR_NO_FONLINE_DAT: return "Could not find fonline.dat / or fonline data directory."
  }
}*/

bool IsDir(const char* path) {
  struct stat buf;
  
  // Empty path is equivalent to ".", must be true.
  if (path[0] == 0) {
      return true;
  }
  
  if (stat(path, &buf) == 0) {
      return (buf.st_mode & S_IFDIR) != 0;
  }
  
  return false;
}

}; // anonymous namespace

char pathlst[][50]=
{
	"art\\critters\\",
	"art\\intrface\\",
	"art\\inven\\",
	"art\\items\\",
	"art\\misc\\",
	"art\\scenery\\",
	"art\\skilldex\\",
	"art\\splash\\",
	"art\\tiles\\",
	"art\\walls\\",

	"maps\\",

	"proto\\items\\",
	"proto\\misc\\",
	"proto\\scenery\\",
	"proto\\tiles\\",
	"proto\\walls\\",

	"sound\\music\\",
	"sound\\sfx\\",

	"text\\english\\game\\",
	
  "maps\\",
  "maps\\wm_mask\\",
};

FileManager::FileManager(): initialized(0),file(NULL),fileSize(0),position(0),buffer(NULL) { };

int FileManager::Init() {
  FONLINE_LOG("FileManager Initialization...\n");

  strcpy(fo_dat, "./");

  FONLINE_LOG("FileManager Initialization complete\n");
  initialized=1;
  return 1;
}

FileManager::~FileManager() {
  Clear();
  initialized = 0;
}

int FileManager::Init(const char* masterDatPath, const char* critterDatPath, const char* fonlineDatPath) {
  assert(masterDatPath != NULL);
  assert(critterDatPath != NULL);
  assert(fonlineDatPath != NULL);
  
  assert(masterDatPath[0] != 0);
  assert(critterDatPath[0] != 0);
  assert(fonlineDatPath[0] != 0);
	
	FONLINE_LOG("FileManager Initialization...\n");
	
	// FIXME[12.12.2012 alex]: copypasta
	
	master_dat[0] = 0;
	strcat(master_dat, masterDatPath);
	
	if (!IsDir(master_dat)) {
    lpDAT.Init(master_dat);

    if(lpDAT.error == ERR_CANNOT_OPEN_FILE) {
      ReportErrorMessage("FileManager Init>","файл %s не найден",master_dat);
      return 0;
    }
	} else {
	  strcat(master_dat, "/");
	}

	crit_dat[0] = 0;
	strcat(crit_dat, critterDatPath);
	
	if (!IsDir(crit_dat)) {
		lpDATcr.Init(crit_dat);
		
		if(lpDATcr.error==ERR_CANNOT_OPEN_FILE) {
			ReportErrorMessage("FileManager Init>","файл %s не найден",crit_dat);
			return 0;
		}
	}
	else {
	  strcat(crit_dat, "/");
	}

  fo_dat[0] = 0;
	strcpy(fo_dat, fonlineDatPath);
  if (!IsDir(fo_dat)) {
    // FIXME[12.12.2012 alex]: not supported yet.
    assert(false);
  } else {
    strcat(fo_dat, "/");
  }

	FONLINE_LOG("FileManager Initialization complete\n");
	initialized=1;
	return 1;
}

void FileManager::Clear()
{
	FONLINE_LOG("FileManager Clear...\n");
	UnloadFile();
	FONLINE_LOG("FileManager Clear complete\n");
}

void FileManager::UnloadFile() {
  if (buffer != NULL) {
    delete [] buffer;
    buffer = NULL;
  }
}

int FileManager::LoadFile(char* fileName, int pathType)
{
  assert(fileName != NULL);
  assert(initialized);
	
	UnloadFile();

	char path[1024]="";
	char pfname[1024]="";

	strcpy(pfname,pathlst[pathType]);
	strcat(pfname,fileName);

	strcpy(path,fo_dat);
	strcat(path,pfname);

	//данные fo
  if (::LoadFile(path, (void**) &buffer, (size_t*) &fileSize)) {
    return 1;
  }

	if(pathType==PT_ART_CRITTERS) {
		if (!lpDATcr.IsLoaded()) {
		  FONLINE_LOG("Loading from normal FS.\n");
			//попрбуем загрузить из critter_dat если это каталог
			strcpy(path,crit_dat);
			strcat(path,pfname);
	
      if (::LoadFile(path, (void**) &buffer, (size_t*) &fileSize)) {
        return 1;
      } else {
        return 0; //а вот не вышло
      }
		}	

		if (lpDATcr.DATOpenFile(pfname) != false) {
			fileSize = lpDATcr.DATGetFileSize();			

			buffer = new uint8_t[fileSize+1];
			size_t br;
		
			lpDATcr.DATReadFile(buffer,fileSize,&br);

			buffer[fileSize]=0;

			position=0;
			return 1;
		}

	}
	
	if(!lpDAT.IsLoaded())
	{
	  FONLINE_LOG("Loading from normal FS.\n");
		//попрбуем загрузить из master_dat если это каталог
		strcpy(path,master_dat);
		strcat(path,pfname);

    if (::LoadFile(path, (void**) &buffer, (size_t*) &fileSize)) {
      return 1;
    } else return 0; //а вот не вышло
	}

	if (lpDAT.DATOpenFile(pfname) != false) {	
		fileSize = lpDAT.DATGetFileSize();

		buffer = new uint8_t[fileSize+1];
		size_t br;
		
		lpDAT.DATReadFile(buffer,fileSize,&br);

		buffer[fileSize]=0;

		position=0;
		return 1;
	}

	FONLINE_LOG("FileMngr - Файл %s не найден\n",pfname);//!Cvet
	return 0;
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
	strcpy(get_path,fo_dat);
	strcat(get_path,pathlst[PathType]);
	strcat(get_path,fname);

  if (FileExists(get_path)) {
    return 1;
  }

	FONLINE_LOG("Файл:|%s|\n",get_path);

	return 0;
}