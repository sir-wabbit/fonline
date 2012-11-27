#ifndef _DATFILE_H_
#define _DATFILE_H_

#include <map>
#include <string>

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

typedef std::map<std::string, uint8_t*> index_map;

struct IndexMap {
	index_map index;
};

typedef std::map<std::string, IndexMap> find_map;

class DatArchive {
public:
  DatArchive();
  virtual ~DatArchive();
  
  bool Init(char* filename);
  bool IsLoaded();

  int ReadTree();
  void IndexingDAT();

  HANDLE DATOpenFile(char* fname);
  bool FindFile(const std::string& fname);

  bool DATReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
                                                 LPDWORD lpNumberOfBytesRead);
  bool DATSetFilePointer(LONG lDistanceToMove, uint32_t dwMoveMethod);

  uint32_t DATGetFileSize();

  void ShowError();
  
  UINT error;
  
private:
  //index_map index;
  find_map fmap;

  std::string datFileName;

  uint8_t  fileType; //если там 1, то файл считается компрессированым(не всегда).
  uint32_t realSize; //Размер файла без декомпрессии
  uint32_t packedSize; //Размер сжатого файла
  uint32_t offset; //Адрес файла в виде смещения от начала DAT-файла.

  bool lError;

  HANDLE hFile; //Handles: (DAT) files

  uint8_t *m_pInBuf;

  ULONG FileSizeFromDat;
  ULONG TreeSize;
  ULONG FilesTotal;

  uint8_t* ptr, *buff,*ptr_end;
  //in buff - DATtree, ptr - pointer

  IOStream* reader; // reader for current file in DAT-archive
};


#endif