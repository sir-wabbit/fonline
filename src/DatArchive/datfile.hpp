#pragma once

#include <map>
#include <string>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#if ULONG_MAX == 4294967295UL
# ifndef UINT64_C
#   define UINT64_C(n) n ## ULL
# endif
# ifndef PRIu64
#   define PRIu64 "llu"
# endif
# ifndef PRIx64
#   define PRIx64 "llx"
# endif
# ifndef PRIX64
#   define PRIX64 "llX"
# endif
#else
# ifndef UINT64_C
#   define UINT64_C(n) n ## UL
# endif
# ifndef PRIu64
#   define PRIu64 "lu"
# endif
# ifndef PRIx64
#   define PRIx64 "lx"
# endif
# ifndef PRIX64
#   define PRIX64 "lX"
# endif
#endif
#ifndef UINT64_MAX
# define UINT64_MAX UINT64_C(18446744073709551615)
#endif

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
