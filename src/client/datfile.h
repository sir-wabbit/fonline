#ifndef _DATFILE_H_
#define _DATFILE_H_

#include <map>

// Added by ABel: Seamless reading from both Fallouts' DAT
#include "cfile/cfile.h"

enum
{
   RES_OK,
   ERR_CANNOT_OPEN_FILE,
   ERR_FILE_TRUNCATED,
   ERR_FILE_NOT_SUPPORTED,
   ERR_ALLOC_MEMORY,
   ERR_FILE_NOT_SUPPORTED2,
};


class compare
{
public:
	bool operator() (const char *s, const char *t)const
	{
		return strcmp(s,t)<0;
  }
};

typedef std::map<char*, uint8_t*, compare> index_map;

struct IndexMap 
{
	index_map index;
};

typedef std::map<char*, IndexMap*, compare> find_map;

class DatArchive {
public:
  //index_map index;
  static find_map* fmap;

  char Datname[1024];

  uint8_t  FileType; //если там 1, то файл считается компрессированым(не всегда).
  uint32_t RealSize; //Размер файла без декомпрессии
  uint32_t PackedSize; //Размер сжатого файла
  uint32_t Offset; //Адрес файла в виде смещения от начала DAT-файла.

  bool lError;
  UINT ErrorType;

  HANDLE hFile; //Handles: (DAT) files

  uint8_t *m_pInBuf;

  ULONG FileSizeFromDat;
  ULONG TreeSize;
  ULONG FilesTotal;

  uint8_t* ptr, *buff,*ptr_end;
  //in buff - DATtree, ptr - pointer

  CFile* reader; // reader for current file in DAT-archive

  int ReadTree();
  void IndexingDAT();

  HANDLE DATOpenFile(char* fname);
  bool FindFile(char* fname);

  bool DATReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
                                                 LPDWORD lpNumberOfBytesRead);
  bool DATSetFilePointer(LONG lDistanceToMove, uint32_t dwMoveMethod);

  uint32_t DATGetFileSize();

  void RevDw(uint32_t *addr);
  void ShowError();

  DatArchive(char* filename);
  virtual ~DatArchive();
};


#endif