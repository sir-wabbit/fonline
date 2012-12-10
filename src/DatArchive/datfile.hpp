#ifndef _DATFILE_H_
#define _DATFILE_H_

#include <map>
#include <string>
#include <stdio.h>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

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

typedef std::map<std::string, const uint8_t*> IndexMap;

struct DATArchiveInfo {
  DATArchiveInfo() : FileSizeFromDat(0), TreeSize(0), FilesTotal(0), treePtr(0) { }

  uint32_t FileSizeFromDat;
  uint32_t TreeSize;
  uint32_t FilesTotal;
  
  uint8_t* treePtr;
  
  IndexMap index;
};

struct DATFileInfo {
  DATFileInfo() : fileType(0), realSize(0), packedSize(0), offset(0) { }
  // Eсли там 1, то файл считается компрессированым (не всегда).
  uint8_t  fileType;
  // Размер файла без декомпрессии.
  uint32_t realSize;
  // Размер сжатого файла.
  uint32_t packedSize;
  // Адрес файла в виде смещения от начала DAT-файла.
  uint32_t offset;
};

class DatArchive {
public:
  DATARCHIVE_API DatArchive();
  DATARCHIVE_API virtual ~DatArchive();
  
  DATARCHIVE_API bool Init(char* filename);
  DATARCHIVE_API bool IsLoaded();

  DATARCHIVE_API bool DATOpenFile(char* fname);
  DATARCHIVE_API uint64_t DATGetFileSize();
  DATARCHIVE_API bool DATReadFile(void* buffer, size_t numberOfBytesToRead, size_t* numberOfBytesRead);
  DATARCHIVE_API bool DATSetFilePointer(int64_t offset, uint32_t moveMethod);
  
  unsigned int error;
  
private:  
  FILE* fileStream;
  
  DATArchiveInfo archive;
  DATFileInfo file;

  bool lError;

  IOStream* reader;
};


#endif