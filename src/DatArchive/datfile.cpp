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

int DatIndexTree(FILE* fd, DATArchiveInfo* archive) {
/*  find_map::iterator it = fmap.find(datFileName);

  if(it != fmap.end()) {
    //WriteLog("%s already indexed\n", datFileName.c_str());
    return;
  } else {
    fmap[datFileName] = IndexMap();
  }
  IndexMap& nmap = fmap[datFileName];

  //WriteLog("Indexing %s...",datFileName.c_str());
  //DWORD tc = GetTickCount();

  std::string path;
  std::string fname;
  std::string last_path;

  ptr = buff;

  while (true) {
    assert(sizeof(size_t) >= sizeof(uint32_t));
    size_t szFileName = *(uint32_t *)ptr;
    ptr += 4;

    fname.assign((const char*) ptr, szFileName);

    GetPath(path, fname);

    if(path != last_path) {
      std::string str = path;
      if (nmap.index.find(path) == nmap.index.end()) {
        nmap.index[path] = ptr - 4;
      }
    }

    if((ptr + szFileName + 13) >= ptr_end)
      break;
    else
      ptr += szFileName + 13;
  }
  ptr = buff;
  //WriteLog("for %d ms\n", GetTickCount() - tc);*/
  
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
  
  if (archive->treePtr != NULL) {
    free(archive->treePtr);
    archive->treePtr = NULL;
  }
  
  archive->treePtr = (uint8_t*) malloc(archive->TreeSize);
  if (archive->treePtr == NULL) {
    return ERR_ALLOC_MEMORY;
  }
  
  if (ReadAll(fd, archive->treePtr, archive->TreeSize - 4) != 0) {
    return ERR_CANNOT_OPEN_FILE;
  }
  
  return 0;
}

} // anonymous namespace

DatArchive::DatArchive() {
  error = 0;

  file.fileType = 0; //если там 1, то файл считается компрессированым(не всегда).
  file.realSize = 0; //Размер файла без декомпрессии
  file.packedSize = 0; //Размер сжатого файла
  file.offset = 0; //Адрес файла в виде смещения от начала DAT-файла.

  lError = 0;

  hFile = 0; //Handles: (DAT) files

  archive.FileSizeFromDat = 0;
  archive.TreeSize = 0;
  archive.FilesTotal = 0;

  ptr = 0;
  buff = 0;
  ptr_end = 0;
  //in buff - DATtree, ptr - pointer

  reader = 0; // reader for current file in DAT-archive

}

bool DatArchive::Init(char* fileName) {
  lError = true;
  buff = NULL;

  reader = NULL; // Initially empty reader. We don't know its type at this point

  hFile = fopen(fileName, "rb");

  if (hFile == NULL) {
    error = ERR_CANNOT_OPEN_FILE;
    return false;
  }

  if (ReadTree() != RES_OK) {
    return false;
  }

  lError = false;
  return true;
}

bool DatArchive::IsLoaded() {
  return hFile != NULL;
}

//------------------------------------------------------------------------------
DatArchive::~DatArchive()
{
   if(hFile != NULL) {
      fclose(hFile);
      hFile = NULL;  
   }
   if(buff != NULL) {
      delete[] buff;
      buff = NULL;
   }

  if (reader != NULL) {
    delete reader;
    reader = NULL;
  }
}
//------------------------------------------------------------------------------
int DatArchive::ReadTree()
{
  int err;
  
	if ((err = DatReadTree(hFile, &archive)) != 0) {
	  return err;
	}
	
	buff = archive.treePtr;
	ptr_end = archive.treePtr + archive.TreeSize - 4;
	
  IndexingDAT();
  return RES_OK;
}
//------------------------------------------------------------------------------

void DatArchive::IndexingDAT() {
  //WriteLog("Indexing %s...",datFileName.c_str());
  //DWORD tc = GetTickCount();
  
  std::string path;
  std::string fname;
  std::string last_path;
  
  ptr = buff;
  
  while (true) {
    assert(sizeof(size_t) >= sizeof(uint32_t));
    size_t szFileName = *(uint32_t *)ptr;
    ptr += 4;
    
    fname.assign((const char*) ptr, szFileName);
    
    GetPath(path, fname);
    
    if(path != last_path) {
      std::string str = path;
      if (archive.index.find(path) == archive.index.end()) {
        archive.index[path] = ptr - 4;
      }
    }
    
    if((ptr + szFileName + 13) >= ptr_end)
      break;
    else
      ptr += szFileName + 13;
  }
  ptr = buff;
  //WriteLog("for %d ms\n", GetTickCount() - tc);
}


//------------------------------------------------------------------------------
bool DatArchive::DATOpenFile(char* fname) {
  // if we still have old non-closed reader - kill it
  if (reader) {
    delete reader;
    reader = NULL;
  }

  if(hFile != NULL) {
    if(FindFile(fname)) {
      if(!file.fileType) {
        reader = new CPlainFile (hFile, file.offset, file.realSize);
      } else {
        reader = new InflatorStream (hFile, file.offset, file.realSize, file.packedSize);
      }
      return true;
    }
  }
  
  return false;
}
//------------------------------------------------------------------------------
bool DatArchive::FindFile(const std::string& fileName)
{
	
  std::string str;
  std::string fnd;
  std::string path;
  
  str = fileName;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  GetPath(path, str);

  ptr = archive.index[path];
  if (!ptr) return false;

  int difpos = str.size() - 5;
  char difchar = str[difpos];

  while (true) {
    assert(sizeof(size_t) >= sizeof(uint32_t));
	  size_t szFileName = *(uint32_t *)ptr;
    ptr += 4;
    
    fnd.assign((const char*) ptr, szFileName);
    std::transform(fnd.begin(), fnd.end(), fnd.begin(), ::tolower);
    
    if (fnd == str) {
      file.fileType = *(ptr + szFileName);
      file.realSize = *(uint32_t *)(ptr + szFileName+ 1);
      file.packedSize = *(uint32_t *)(ptr + szFileName + 5);
      file.offset = *(uint32_t *)(ptr + szFileName + 9);
	    return true;
	  }
	  
    if ((ptr + szFileName + 13) >= ptr_end)
      break;
    else
      ptr += szFileName + 13;
   }
   return false;
}
//------------------------------------------------------------------------------
bool DatArchive::DATSetFilePointer(int64_t lDistanceToMove, uint32_t dwMoveMethod) {
  if(hFile == NULL) {
    return false;
  }
  reader->seek (lDistanceToMove, dwMoveMethod);
  return true;
}
//------------------------------------------------------------------------------
uint64_t DatArchive::DATGetFileSize(void) {
  if(hFile == NULL) {
    return 0;
  }
  
  return file.realSize;
}
//------------------------------------------------------------------------------
bool DatArchive::DATReadFile(void* lpBuffer, size_t nNumberOfBytesToRead, size_t* lpNumberOfBytesRead) {
  if (hFile == NULL) {
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