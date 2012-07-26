//---------------------------------------------------------------------------
#ifndef mapH
#define mapH

#include <classes.hpp>
#include "utilites.h"

class CMap
{
public:
   struct
   {
      DWORD ver;            // 0x0000
      char mapname[16];         // 0x0004
      DWORD DefPlPos;           // 0x0014 :: Default player position [0..39999]
      DWORD DefMapElev;         // 0x0018 :: Default map elevation [0..2]
      DWORD DefPlDirection;     // 0x001C :: Defuult player direction [0..5]
      DWORD Num1;               // 0x0020 :: #Map Local Vars
      DWORD MapScriptID;        // 0x0024
      DWORD TilesCountID;       // 0x0028
      DWORD Unknown4;           // 0x002C
      DWORD Num2;               // 0x0030 :: #Map Global Vars (*.gam)
      DWORD MapID;              // 0x0034
      DWORD GameStartDate;      // 0x0038 :: Time since the epoch.
      DWORD Unknown6[44];       // 0x003C -> 0x00EB
   } mapvars;
   DWORD *pMapGVars;
   DWORD *pMapLVars;

   DWORD dwVer;
   DWORD dwLocalVars;
   DWORD dwGlobalVars;
   DWORD dwTilesCount;

   int TilesSizeX;
   int TilesSizeY;
   int Levels;

   bool lError;

   CMap(HANDLE h_map);
   void SaveToFile(HANDLE h_map);
   virtual ~CMap();

protected:
   CUtilites *pUtil;
};
//---------------------------------------------------------------------------

#endif
