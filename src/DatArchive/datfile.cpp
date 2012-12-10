#include "datfile.hpp"

#include <algorithm>
#include <assert.h>

#include "cfile/cfile.hpp"

//#include <SimpleLeakDetector/SimpleLeakDetector.hpp>

#define VER_FALLOUT1 0x00000013
#define VER_FALLOUT2 0x10000014

#define ZLIB_BUFF_SIZE 0x10000
#define GZIP_MODE1 0x0178
#define GZIP_MODE2 0xDA78

char *error_types[] = {
   "Cannot open file.",                       // ERR_CANNOT_OPEN_FILE
   "File invalid or truncated.",              // ERR_FILE_TRUNCATED
   "This file not supported.",                // ERR_FILE_NOT_SUPPORTED
   "Not enough memory to allocate buffer.",   // ERR_ALLOC_MEMORY
   "Fallout1 dat files are not supported.",   // ERR_FILE_NOT_SUPPORTED2
};

namespace {

bool IsLittleEndian() {
  uint32_t x = 1;
  return *((uint8_t*) &x) == 1;
}

void SwapBytes(void* ptr, size_t size) {
  assert(ptr != NULL);

  char* bytePtr = (char*) ptr;

  for (size_t i = 0; i < size / 2; i++) {
    char temp;
    temp = bytePtr[i];
    bytePtr[i] = bytePtr[size - 1 - i];
    bytePtr[size - 1 - i] = temp;    
  }
}

bool GetPath(std::string& result, const std::string& path) {
  size_t pos = path.find_last_of('\\');
  
  if (pos == path.npos) {
    result = "";
  }
  
  result = path.substr(0, pos + 1);
  
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);

  return true;
}

uint64_t FileSize(FILE* fd) {
  assert(fd != NULL);

  long original = ftell(fd);
  fseek(fd, 0, SEEK_END);
  long result = ftell(fd);
  fseek(fd, original, SEEK_SET);
  
  if (result < 0) {
    return UINT64_MAX;
  }
  
  return result;
}

int ReadAll(FILE* fd, void* buf, size_t size) {
  assert(fd != NULL);
  assert(buf != NULL);

  size_t read = 0;

  while (read < size) {
    int r = fread(buf, 1, size, fd);
    if (r <= 0) return -1;
    read += r;
  }

  return 0;
}

int ReadLittleEndian(FILE* fd, void* buf, size_t size) {
  assert(fd != NULL);
  assert(buf != NULL);
  
  int err = ReadAll(fd, buf, size);
  
  if (err != 0) {
    return err;
  }
  
  if (!IsLittleEndian()) {
    SwapBytes(buf, size);
  }
  
  return 0;
}

int ReadBigEndian(FILE* fd, void* buf, size_t size) {
  assert(fd != NULL);
  assert(buf != NULL);

  int err = ReadAll(fd, buf, size);

  if (err != 0) {
    return err;
  }

  if (IsLittleEndian()) {
    SwapBytes(buf, size);
  }
  
  return 0;
}

int DatReadTree(FILE* fd, DATArchiveInfo* archive) {
  assert(fd != NULL);
  assert(archive != NULL);
  
  assert(sizeof(archive->TreeSize) == 4);
  assert(sizeof(archive->FileSizeFromDat) == 4);
  assert(sizeof(archive->FilesTotal) == 4);
  
  uint64_t fileSize = FileSize(fd);
  if (fileSize == UINT64_MAX) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  if (fileSize < 8) {
    return ERR_FILE_TRUNCATED;
  }
  
  fseek(fd, -8, SEEK_END);
  
  // Read tree size.
  if (ReadLittleEndian(fd, &archive->TreeSize, 4) != 0) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  // Read total file size.
  if (ReadLittleEndian(fd, &archive->FileSizeFromDat, 4) != 0) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  if (fileSize != archive->FileSizeFromDat) {
    return ERR_FILE_NOT_SUPPORTED;
  }

  fseek(fd, -((long) archive->TreeSize + 8), SEEK_END);
  
  // Read total file count.
  if (ReadLittleEndian(fd, &archive->FilesTotal, 4) != 0) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  // Minus file count size.
  archive->TreeSize -= 4;
  
  if (archive->treePtr != NULL) {
    free(archive->treePtr);
    archive->treePtr = NULL;
  }
  
  archive->treePtr = (uint8_t*) malloc(archive->TreeSize);
  if (archive->treePtr == NULL) {
    return ERR_ALLOC_MEMORY;
  }
  
  if (ReadAll(fd, archive->treePtr, archive->TreeSize) != 0) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  return 0;
}

int DatIndexTree(DATArchiveInfo* archive) {
  std::string path;
  std::string fname;
  std::string last_path;

  const uint8_t* ptr = archive->treePtr;
  const uint8_t* endPtr = archive->treePtr + archive->TreeSize;

  while (true) {
    uint32_t szFileName = *(uint32_t *)ptr;
    ptr += 4;

    fname.assign((const char*) ptr, szFileName);

    GetPath(path, fname);

    if(path != last_path) {
      std::string str = path;
      if (archive->index.find(path) == archive->index.end()) {
        archive->index[path] = ptr - 4;
      }
    }

    ptr += szFileName + 13;

    if(ptr >= endPtr) {
      break;
    }
  }

  return 0;
}

bool DatFindFile(DATArchiveInfo* archive, DATFileInfo* file, const std::string& fileName) {
  std::string str;
  std::string fnd;
  std::string path;

  str = fileName;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  GetPath(path, str);

  const uint8_t* ptr = archive->index[path];
  const uint8_t* endPtr = archive->treePtr + archive->TreeSize;
  
  if (!ptr) {
    return false;
  }

  while (true) {
    uint32_t szFileName = *(uint32_t *)ptr;
    ptr += 4;

    fnd.assign((const char*) ptr, szFileName);
    std::transform(fnd.begin(), fnd.end(), fnd.begin(), ::tolower);

    if (fnd == str) {
      file->fileType = *(ptr + szFileName);
      file->realSize = *(uint32_t *)(ptr + szFileName+ 1);
      file->packedSize = *(uint32_t *)(ptr + szFileName + 5);
      file->offset = *(uint32_t *)(ptr + szFileName + 9);
      return true;
    }

    ptr += szFileName + 13;

    if(ptr >= endPtr) {
      break;
    }
  }
  return false;
}

} // anonymous namespace

DatArchive::DatArchive() {
  error = 0;

  file.fileType = 0;
  file.realSize = 0;
  file.packedSize = 0;
  file.offset = 0;

  lError = 0;

  fileStream = 0; //Handles: (DAT) files

  archive.FileSizeFromDat = 0;
  archive.TreeSize = 0;
  archive.FilesTotal = 0;

  reader = 0; // reader for current file in DAT-archive

}

bool DatArchive::Init(char* fileName) {
  lError = true;
  //buff = NULL;

  reader = NULL; // Initially empty reader. We don't know its type at this point

  fileStream = fopen(fileName, "rb");

  if (fileStream == NULL) {
    error = ERR_CANNOT_OPEN_FILE;
    return false;
  }
  
  if ((error = DatReadTree(fileStream, &archive)) != 0) {
    return false;
  }

  if ((error = DatIndexTree(&archive)) != 0) {
    return false;
  }

  lError = false;
  return true;
}

bool DatArchive::IsLoaded() {
  return fileStream != NULL;
}

//------------------------------------------------------------------------------
DatArchive::~DatArchive()
{
   if(fileStream != NULL) {
      fclose(fileStream);
      fileStream = NULL;  
   }
   
   if (archive.treePtr != NULL) {
      delete[] archive.treePtr;
      archive.treePtr = NULL;
   }

  if (reader != NULL) {
    delete reader;
    reader = NULL;
  }
}

bool DatArchive::DATOpenFile(char* fname) {
  // if we still have old non-closed reader - kill it
  if (reader) {
    delete reader;
    reader = NULL;
  }

  if(fileStream != NULL) {
    if (DatFindFile(&archive, &file, fname)) {
      if(!file.fileType) {
        reader = new CPlainFile (fileStream, file.offset, file.realSize);
      } else {
        reader = new InflatorStream (fileStream, file.offset, file.realSize, file.packedSize);
      }
      return true;
    }
  }
  
  return false;
}
//------------------------------------------------------------------------------
bool DatArchive::DATSetFilePointer(int64_t lDistanceToMove, uint32_t dwMoveMethod) {
  if(fileStream == NULL) {
    return false;
  }
  reader->seek (lDistanceToMove, dwMoveMethod);
  return true;
}
//------------------------------------------------------------------------------
uint64_t DatArchive::DATGetFileSize(void) {
  if(fileStream == NULL) {
    return 0;
  }
  
  return file.realSize;
}
//------------------------------------------------------------------------------
bool DatArchive::DATReadFile(void* lpBuffer, size_t nNumberOfBytesToRead, size_t* lpNumberOfBytesRead) {
  if (fileStream == NULL) {
    return false;
  }
  
  if (!lpBuffer) {
    return false;
  }
  
  if (!nNumberOfBytesToRead) {
    lpNumberOfBytesRead = 0;
    return true;
  }
  
  reader->read(lpBuffer, nNumberOfBytesToRead, (long*)lpNumberOfBytesRead);
  return true;
}
//------------------------------------------------------------------------------