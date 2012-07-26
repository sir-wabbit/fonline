//////////////////////////////////////////////////////////////////////
// CMap Class
//////////////////////////////////////////////////////////////////////

#include "map.h"
#include "mdi.h"
#include "utilites.h"
//------------------------------------------------------------------------------
CMap::CMap(HANDLE h_map)
{
   lError = true;
   ULONG i;
   pUtil = frmMDI->pUtil;
   pMapGVars = NULL;
   pMapLVars = NULL;


   ReadFile(h_map, &mapvars, 236, &i, NULL);

   dwVer = pUtil->GetDW(&mapvars.ver); // 0x0000 #Map Version

   dwLocalVars = pUtil->GetDW(&mapvars.Num1); // 0x0020 #Map Local Vars
   dwGlobalVars = pUtil->GetDW(&mapvars.Num2); // 0x0030 #Map Global Vars(*.gam)

   if (dwGlobalVars != 0)
   {
      pMapGVars = new DWORD[dwGlobalVars];
      ReadFile(h_map, pMapGVars, dwGlobalVars * 4, &i, NULL);
   }
   if (dwLocalVars != 0)
   {
      pMapLVars = new DWORD[dwLocalVars];
      ReadFile(h_map, pMapLVars, dwLocalVars * 4, &i, NULL);
   }

//   dwTilesCount = pUtil->GetDW(&mapvars.TilesCountID);
   dwTilesCount = pUtil->GetDW(&mapvars.TilesCountID) &0xE;
   switch (dwTilesCount)
   {
      case 0x0c :
         TilesSizeX = 100;
         TilesSizeY = 100;
         Levels = 1;
         break;
      case 0x08 :
         TilesSizeX = 100;
         TilesSizeY = 200;
         Levels = 2;
         break;
      case 0x00 :
         TilesSizeX = 100;
         TilesSizeY = 300;
         Levels = 3;
         break;
      default :
         return;
   }
   lError = false;
}
//---------------------------------------------------------------------------
void CMap::SaveToFile(HANDLE h_map)
{
   DWORD i;
   WriteFile(h_map, &mapvars, 236, &i, NULL);
   WriteFile(h_map, pMapGVars, dwGlobalVars << 2, &i, NULL);
   WriteFile(h_map, pMapLVars, dwLocalVars << 2, &i, NULL);
}
//---------------------------------------------------------------------------
CMap::~CMap()
{
   if (pMapLVars != NULL)
      delete pMapLVars;
   if (pMapGVars != NULL)
      delete pMapGVars;
}
//---------------------------------------------------------------------------

