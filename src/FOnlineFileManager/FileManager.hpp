#ifndef FONLINE_FILEMANAGER_HPP_
#define FONLINE_FILEMANAGER_HPP_

#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
  #ifdef FONLINE_FILEMANAGER_EXPORTS
    #define FONLINE_FILEMANAGER_API __declspec(dllexport)
  #else
    #define FONLINE_FILEMANAGER_API __declspec(dllimport)
  #endif
#else
  #define FONLINE_FILEMANAGER_API
#endif

#include <DatArchive/DatArchive.hpp>

#define PT_ART_CRITTERS 0
#define PT_ART_INTRFACE 1
#define PT_ART_INVEN    2
#define PT_ART_ITEMS    3
#define PT_ART_MISC     4
#define PT_ART_SCENERY  5
#define PT_ART_SKILLDEX 6
#define PT_ART_SPLASH   7
#define PT_ART_TILES    8
#define PT_ART_WALLS    9

#define PT_MAPS         10

#define PT_PRO_ITEMS    11
#define PT_PRO_MISC     12
#define PT_PRO_SCENERY  13
#define PT_PRO_TILES    14
#define PT_PRO_WALLS    15

#define PT_SND_MUSIC    16
#define PT_SND_SFX      17

#define PT_TXT_GAME     18

#define PT_SERVER_MAPS  19
#define PT_SERVER_MASKS 20

FONLINE_FILEMANAGER_API extern const char* pathlst[50];


class FileManager {
public:
  FONLINE_FILEMANAGER_API int Init();
  FONLINE_FILEMANAGER_API int Init(const char* masterDatPath, const char* critterDatPath, const char* fonlineDatPath);
  FONLINE_FILEMANAGER_API void Clear();
  FONLINE_FILEMANAGER_API void UnloadFile();
  FONLINE_FILEMANAGER_API int LoadFile(const char* fname, int PathType);

  FONLINE_FILEMANAGER_API int GetFullPath(char* fname, int PathType, char* get_path); //!Cvet полный путь к файлу

  FONLINE_FILEMANAGER_API void SetCurrentPosition(uint32_t pos);
  FONLINE_FILEMANAGER_API void GoForward(uint32_t offs);

  FONLINE_FILEMANAGER_API int GetStr(char* str,uint32_t len);
  FONLINE_FILEMANAGER_API uint8_t GetByte();
  FONLINE_FILEMANAGER_API uint16_t GetWord();
  FONLINE_FILEMANAGER_API uint16_t GetRWord();
  FONLINE_FILEMANAGER_API uint32_t GetDWord();
  FONLINE_FILEMANAGER_API uint32_t GetRDWord();
  FONLINE_FILEMANAGER_API int Read(void* ptr, size_t size);

  FONLINE_FILEMANAGER_API uint8_t* GetBufferPtr();
  FONLINE_FILEMANAGER_API uint32_t GetFileSize();


  FONLINE_FILEMANAGER_API FileManager();
  FONLINE_FILEMANAGER_API ~FileManager();

private:
  bool initialized;

  void* file;
  uint32_t fileSize;
  uint32_t position;
  uint8_t* buffer;

  DatArchive lpDAT;
  DatArchive lpDATcr;

  char master_dat[1024];
  char crit_dat[1024];
  char fo_dat[1024];
};

#endif//FONLINE_FILEMANAGER_HPP_
