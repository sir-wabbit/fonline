#ifndef _DATFILE_H_
#define _DATFILE_H_

#include <map>
#include <string>

#include "DatArchive.hpp"

class IOStream;

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
  DATARCHIVE_API DatArchive();
  DATARCHIVE_API virtual ~DatArchive();
  
  DATARCHIVE_API bool Init(char* filename);
  DATARCHIVE_API bool IsLoaded();

  DATARCHIVE_API int ReadTree();
  DATARCHIVE_API void IndexingDAT();

  DATARCHIVE_API HANDLE DATOpenFile(char* fname);
  DATARCHIVE_API bool FindFile(const std::string& fname);

  DATARCHIVE_API bool DATReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
                                                 LPDWORD lpNumberOfBytesRead);
  DATARCHIVE_API bool DATSetFilePointer(LONG lDistanceToMove, uint32_t dwMoveMethod);

  DATARCHIVE_API uint32_t DATGetFileSize();

  DATARCHIVE_API void ShowError();
  
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