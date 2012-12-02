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

} // namespace anonymous

//------------------------------------------------------------------------------

DatArchive::DatArchive() {
  error = 0;

  fileType = 0; //если там 1, то файл считается компрессированым(не всегда).
  realSize = 0; //Размер файла без декомпрессии
  packedSize = 0; //Размер сжатого файла
  offset = 0; //Адрес файла в виде смещения от начала DAT-файла.

  lError = 0;

  hFile = 0; //Handles: (DAT) files

  m_pInBuf = 0;

  FileSizeFromDat = 0;
  TreeSize = 0;
  FilesTotal = 0;

  ptr = 0;
  buff = 0;
  ptr_end = 0;
  //in buff - DATtree, ptr - pointer

  reader = 0; // reader for current file in DAT-archive

}

bool DatArchive::Init(char* fileName) {
  lError = true;
  buff = NULL;
  m_pInBuf = NULL;

  reader = NULL; // Initially empty reader. We don't know its type at this point

  datFileName = fileName;

  hFile = CreateFile(fileName,  //В hFile находится HANDLE на DAT файл
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  if (hFile == INVALID_HANDLE_VALUE) {
    error = ERR_CANNOT_OPEN_FILE;
    return false;
  }

  if (ReadTree() != RES_OK) {
    return false;
  }

  m_pInBuf = (uint8_t*) malloc(ZLIB_BUFF_SIZE);

  if (m_pInBuf == NULL) {
    error = ERR_ALLOC_MEMORY;
    return false;
  }

  lError = false;
  return true;
}

bool DatArchive::IsLoaded() {
  return hFile != INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
DatArchive::~DatArchive()
{
   if(hFile != INVALID_HANDLE_VALUE)
      CloseHandle(hFile);
   if(m_pInBuf != NULL)
      free(m_pInBuf);
   if(buff != NULL)
      delete[] buff;

  if (reader != NULL) {
    delete reader;
    reader = NULL;
  }
  
	fmap.clear();
}
//------------------------------------------------------------------------------
int DatArchive::ReadTree()
{
	DWORD i,F1DirCount;
	bool Fallout1=false;

	
   //Проверка на то, что файл не менее 8 байт
   i = SetFilePointer(hFile, -8, NULL, FILE_END);
   if(i == 0xFFFFFFFF)
       return ERR_FILE_TRUNCATED;
	
   //Чтение информации из DAT файла
   ReadFile(hFile, &TreeSize, 4, &i, NULL);
   ReadFile(hFile, &FileSizeFromDat, 4, &i, NULL);

   i = SetFilePointer(hFile, 0, NULL, FILE_BEGIN); //Added for Fallout1
   ReadFile(hFile, &F1DirCount, 4, &i, NULL); //Added for Fallout1
   SwapBytes(&F1DirCount, 4); //Added for Fallout1
   if(F1DirCount == 0x01 || F1DirCount == 0x33) Fallout1 = true; //Added for Fallout1
   if(GetFileSize(hFile, NULL) != FileSizeFromDat && Fallout1 == false)
      return ERR_FILE_NOT_SUPPORTED;
   if(!Fallout1)
   {
      i = SetFilePointer (hFile, -((LONG)TreeSize + 8), NULL, FILE_END);
      ReadFile(hFile, &FilesTotal, 4, &i, NULL);
   }
   else //FALLOUT 1 !!!
	return ERR_FILE_NOT_SUPPORTED2;


   if(buff != NULL)
      delete[] buff;
   if((buff = new uint8_t[TreeSize]) == NULL)
      return ERR_ALLOC_MEMORY;
   ZeroMemory(buff, TreeSize);

   ReadFile(hFile, buff, TreeSize - 4, &i, NULL);
   ptr_end = buff + TreeSize;

   IndexingDAT();
   
   return RES_OK;
}
//------------------------------------------------------------------------------
void DatArchive::ShowError(void)
{
  if(lError) {
    MessageBox(NULL, *(error_types + error), "Error", MB_OK);
  }
  lError = false;
}
//------------------------------------------------------------------------------

void DatArchive::IndexingDAT() {
  find_map::iterator it = fmap.find(datFileName);
  
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
  //WriteLog("for %d ms\n", GetTickCount() - tc);
}


//------------------------------------------------------------------------------
bool DatArchive::DATOpenFile(char* fname) {
  // if we still have old non-closed reader - kill it
  if(reader) {
    delete reader;
    reader = NULL;
  }

  if(hFile != INVALID_HANDLE_VALUE) {
    if(FindFile(fname)) {
      if(!fileType) {
        reader = new CPlainFile (hFile, offset, realSize);
      } else {
        reader = new InflatorStream (hFile, offset, realSize, packedSize);
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

  index_map& index = fmap[datFileName].index; 

  ptr = index[path];
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
      fileType = *(ptr + szFileName);
      realSize = *(uint32_t *)(ptr + szFileName+ 1);
      packedSize = *(uint32_t *)(ptr + szFileName + 5);
      offset = *(uint32_t *)(ptr + szFileName + 9);
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
  if(hFile == INVALID_HANDLE_VALUE) {
    return false;
  }
  reader->seek (lDistanceToMove, dwMoveMethod);
  return true;
}
//------------------------------------------------------------------------------
uint64_t DatArchive::DATGetFileSize(void) {
  if(hFile == INVALID_HANDLE_VALUE) {
    return 0;
  }
  
  return realSize;
}
//------------------------------------------------------------------------------
bool DatArchive::DATReadFile(void* lpBuffer, size_t nNumberOfBytesToRead, size_t* lpNumberOfBytesRead) {
  if (hFile == INVALID_HANDLE_VALUE) {
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