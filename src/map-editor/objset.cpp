//////////////////////////////////////////////////////////////////////
// CObjSet Class
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "objset.h"
#include "mdi.h"
#include "main.h"
#include "script.h"
#include "utilites.h"
#include "macros.h"
#include "objtempl.h"
int IDlen[6] = {-1, 128, -1, 88, 0, 0};
// Для Fallout 1 длинна объектов LadderUp и LadderDown меньше на 4 байта
int SubIDlen[5][7] = {88, 88, 88, 96, 92, 92, 92, //88, 88, 88, 96, 92, 92, 92,
                      0,   0,  0,  0,  0,  0,  0,
                      92, 96, 96, 96, 96, 88,  0, //92, 96, 96, 92, 92, 88,  0,
                      0,   0,  0,  0,  0,  0,  0,
                      104, 88};
//"items", "critters", "scenery", "walls", "tiles", "misc"
//---------------------------------------------------------------------------
CObjSet::CObjSet(HANDLE h_map)
{
   lError = true;
   pScriptBuf = NULL;
   pUtil = frmMDI->pUtil;
   pProSet = frmMDI->pProSet;
   pScriptBuf = NULL;
   pObjBuf = NULL;
   pObjStart = NULL;
   pObj = NULL;
   pSelObj = NULL;
   pNewObjBuf = NULL;
   pLevel[0] = NULL;
   pLevel[1] = NULL;
   pLevel[2] = NULL;
   nLevelCount[0] = 0;
   nLevelCount[1] = 0;
   nLevelCount[2] = 0;
   nSelected = 0;
   DWORD i;
   // В Фолле1 объекты LadderUp и LadderDown на 4 байта меньше
   if (frmEnv->pMap->dwVer == VER_FALLOUT1)
   {
      SubIDlen[2][3] = 92;
      SubIDlen[2][4] = 92;
   }
   else
   {
      SubIDlen[2][3] = 96;
      SubIDlen[2][4] = 96;
   }

   pScriptBuf = new CScriptBuf(pTempBuf, dwScriptSize);
   DWORD dwPos = SetFilePointer(h_map, 0, 0, FILE_CURRENT); // Get file pointer
   DWORD filesize = GetFileSize(h_map, NULL);
   dwBufSize = filesize - dwPos;
   if ((pTempBuf = (BYTE *)malloc(dwBufSize)) == NULL) return;
   ReadFile(h_map, pTempBuf, dwBufSize, &i, NULL);
   FindObjStart();
   dwScriptSize = pObjStart - pTempBuf;
   dwObjSize = dwBufSize - dwScriptSize;
   dwFirstObjFilePos = dwPos + dwScriptSize; //added for SeaWolf  (Obj offset)
//   if ((pScriptBuf = (BYTE *)malloc(dwScriptSize)) == NULL) return;
   if ((pObjBuf = (BYTE *)malloc(dwObjSize)) == NULL) return;
//   memcpy(pScriptBuf, pTempBuf, dwScriptSize);
   memcpy(pObjBuf, pTempBuf + dwScriptSize, dwObjSize);
   pObjStart = pObjBuf;
   free(pTempBuf);
   GetObjValues();
   CloseHandle(h_map);
   lError = false;
}
//---------------------------------------------------------------------------
void CObjSet::FindObjStart(void)
{
   DWORD *ptrDW, count, blocknum, blockcount, length;
   BYTE *ptr;
   int type_count = 5;
   int ScrDescType;
   ptrDW = (DWORD *)pTempBuf;
   while (type_count)
   {
      count = pUtil->GetDW(ptrDW);
      ptrDW++;
      if (!count)
      {
         type_count--;
         continue;
      }
      blocknum = (count >> 4) + (count % 16 != 0);
      ScrDescType = 5 - type_count;
      pScriptBuf->CreateDescBlock(ScrDescType, count);
      while (blocknum)
      {
         for (int i = 0; i < 16; i++)
         {
            ptr = (BYTE *)ptrDW;
            switch (*ptr)
            {
               case 0x01:
                  length = 18;
                  break;
               case 0x02:
                  length = 17; //broken2.map use it
                  break;
               case 0x03:
                  length = 16;
                  break;
               case 0x04:
                  length = 16;
                  break;
               default:
                  length = 16;
                  break;
            }
            if (*ptr == ScrDescType && pScriptBuf->nDescCount[ScrDescType] < count)
               pScriptBuf->CopyDesc(ptrDW, ScrDescType);
            ptrDW += length;
         }
         blocknum--;
         ptrDW += blocknum ? 2 : 1;
      }
      type_count--;
      ptrDW++;
   }
   pObjStart = (char *)ptrDW;
}
//---------------------------------------------------------------------------
void CObjSet::GetObjValues(void)
{
   nObjTotal = pUtil->GetDW((DWORD *)pObjStart);
   frmEnv->panelObjCount->Text = "Objects: " + (String)nObjTotal;
//   if (nObjTotal == 0) return;
   nLevelCount[0] = pUtil->GetDW((DWORD *)(pObjStart + 4));
   pLevel[0] = (BYTE *)(pObjStart + 8);
}
//---------------------------------------------------------------------------
BYTE* CObjSet::GetFirstObj(DWORD *nObjNum, int nLevel)
{
   pObj = nLevelCount[nLevel] ? pLevel[nLevel] : NULL;
   *nObjNum = 1;
   return pObj;
}
//---------------------------------------------------------------------------
BYTE* CObjSet::GetNextObj(DWORD *nObjNum, DWORD *nChildCount, int nLevel)
{
   WORD nObjID;
   BYTE nObjType;
   int nObjLen, nSubType = -1;

   *nChildCount -= (*nChildCount ? 1 : 0);
   GetObjType(&nObjType, &nObjID);

   switch (IDlen[nObjType])
   {
      case -1 :
         nSubType = pProSet->GetSubType(nObjType, nObjID);
         nObjLen = SubIDlen[nObjType][nSubType];
         break;
      case 0 : // misc type
         nObjLen = (nObjID == 1 || nObjID == 12) ? 88 : 104;
         break;
      default :
         nObjLen = IDlen[nObjType];
   }

   if ((nObjType == 1) || (nObjType == 0 && nSubType == 1) && !*nChildCount)
      *nChildCount = ChildCount();
   pObj += (nObjLen + (*nChildCount ? 4 : 0));

   switch (nLevel)
   {
      case LEVEL1:
         if (*nObjNum == nLevelCount[nLevel] && !(*nChildCount))
         {
            if (frmEnv->pMap->Levels > 1)  // > nLevel
            {
               pLevel[nLevel + 1] = pObj + 4;
               nLevelCount[nLevel + 1] = pUtil->GetDW((DWORD *)pObj);
            }
            return NULL;
         }
         break;
      case LEVEL2:
         if (*nObjNum == nLevelCount[nLevel] && !(*nChildCount))
         {
            if (frmEnv->pMap->Levels > 2)
            {
               pLevel[nLevel + 1] = pObj + 4;
               nLevelCount[nLevel + 1] = pUtil->GetDW((DWORD *)pObj);
            }
            return NULL;
         }
         break;
      case LEVEL3:
         if (*nObjNum == nLevelCount[nLevel] && !(*nChildCount)) return NULL;
   }
   *nObjNum += (*nChildCount ? 0 : 1);
   return pObj;
}
//---------------------------------------------------------------------------
int CObjSet::GetObjLen(void)
{
   DWORD nObjNum, nChilds;
   WORD nObjID;
   BYTE nObjType;
   BYTE *pOldObj;
   int nLevel, nObjLen, nSubType = -1;
   pOldObj = pObj;
   GetObjType(&nObjType, &nObjID);
   switch (IDlen[nObjType])
   {
      case -1 :
         nSubType = pProSet->GetSubType(nObjType, nObjID);
         nObjLen = SubIDlen[nObjType][nSubType];
         break;
      case 0 : // misc type
         nObjLen = (nObjID == 1 || nObjID == 12) ? 88 : 104;
         break;
      default :
         nObjLen = IDlen[nObjType];
   }
   if (ObjIsChild()) nObjLen += 4;
   return nObjLen;
}
//---------------------------------------------------------------------------
int CObjSet::GetObjLenByID(BYTE nObjType, WORD nObjID)
{
   int nSubType;
   switch (IDlen[nObjType])
   {
      case -1 :
         nSubType = pProSet->GetSubType(nObjType, nObjID);
         return SubIDlen[nObjType][nSubType];
      case 0 : // misc type
         return (nObjID == 1 || nObjID == 12) ? 88 : 104;
      default :
         return IDlen[nObjType];
   }
}
//---------------------------------------------------------------------------
int CObjSet::GetFullType(void)
{
   WORD nObjID;
   BYTE nObjType;
   int nSubType;
   GetObjType(&nObjType, &nObjID);
   switch (nObjType)
   {
      case item_ID:
         nSubType = pProSet->GetSubType(nObjType, nObjID);
         switch (nSubType)
         {
            case OSArmor:
               return OArmor;
            case OSContainer:
               return OContainer;
            case OSDrug:
               return ODrug;
            case OSWeapon:
               return OWeapon;
            case OSAmmo:
               return OAmmo;
            case OSMiscItem:
               return OMiscItem;
            case OSKey:
               return OKey;
         }
      case critter_ID:
         return OCritter;
      case scenery_ID:
         nSubType = pProSet->GetSubType(nObjType, nObjID);
         switch (nSubType)
         {
            case OSPortal:
               return OPortal;
            case OSStairs:
               return OStairs;
            case OSElevator:
               return OElevator;
            case OSLadderBottom:
               return OLadderBottom;
            case OSLadderTop:
               return OLadderUp;
            case OSGenericScenery:
               return OGenericScenery;
         }
      case wall_ID:
         return OWalls;
      case misc_ID:
         return (nObjID == 1 || nObjID == 12) ? OGenericMisc : OExitGrid;
   }
}
//---------------------------------------------------------------------------
void CObjSet::SetObj(BYTE *pSetObj)
{
   pObj = pSetObj;
}
//---------------------------------------------------------------------------
BYTE* CObjSet::GetObj()
{
   return pObj;
}
//---------------------------------------------------------------------------
void CObjSet::SelectObj(void)
{
   pSelObj = pObj;
   if (pObj[0x3C] == OBJ_SELECTED) return;
   pObj[0x3C] = OBJ_SELECTED;
   nSelected++;
   frmEnv->panelObjSelected->Text = "Selected: " + (String) nSelected;
   frmEnv->sbar->Update();
}
//---------------------------------------------------------------------------
BYTE* CObjSet::GetSelObj()
{
   return pSelObj;
}
//---------------------------------------------------------------------------
void CObjSet::DeselectObj(void)
{
   if (pObj[0x3C] == OBJ_DESELECTED) return;
   pObj[0x3C] = OBJ_DESELECTED;
   nSelected--;
   frmEnv->panelObjSelected->Text = "Selected: " + (String) nSelected;
   frmEnv->sbar->Update();
   if (!nSelected) pSelObj = NULL;
}
//---------------------------------------------------------------------------
BYTE CObjSet::ObjSelected(void)
{
   return (BYTE)pObj[0x3C];
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetObjInfo(int nDWORD)
{
   return (DWORD)pUtil->GetDW((DWORD *)(pObj + (nDWORD << 2)));
}
//---------------------------------------------------------------------------
void CObjSet::SetObjInfo(int nDWORD, DWORD dwData)
{
   *(DWORD *)(pObj + (nDWORD << 2)) = pUtil->GetDW(&dwData);
}
//---------------------------------------------------------------------------
void CObjSet::GetObjType(BYTE *nType, WORD *nID)
{
   DWORD nPID = pUtil->GetDW((DWORD *)(pObj + 0x2C));
   *nType = (BYTE)(nPID >> 24);
   *nID = (WORD)(nPID & 0x0000FFFF);
}
//---------------------------------------------------------------------------
void CObjSet::SetObjType(BYTE nType, WORD nID)
{
   DWORD nPID = 0x00000000 | (nType << 24) | nID;
   *(DWORD *)(pObj + 0x2C) = pUtil->GetDW(&nPID);
}
//---------------------------------------------------------------------------
void CObjSet::GetObjFrm(BYTE *nType, WORD *nID)
{
   DWORD nPID = pUtil->GetDW((DWORD *)(pObj + 0x20));
   *nType = (BYTE)(nPID >> 24);
   *nID = *nType != 1 ? (WORD)(nPID & 0x0000FFFF) : (WORD)(nPID & 0x000000FF);
//   *nID = (WORD)(nPID & 0x0000FFFF);
}
//---------------------------------------------------------------------------
void CObjSet::SetObjFrm(BYTE nType, WORD nID)
{
   DWORD nPID = 0x00000000 | (nType << 24) | nID;
   *(DWORD *)(pObj + 0x20) = pUtil->GetDW(&nPID);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetObjFrmDW()
{
   return pUtil->GetDW((DWORD *)(pObj + 0x20));
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetObjLevel(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x28));
}
//---------------------------------------------------------------------------
void CObjSet::SetObjLevel(DWORD nLevel)
{
   *(DWORD *)(pObj + 0x28) = pUtil->GetDW(&nLevel);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetUnknown6(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x30));
}
//---------------------------------------------------------------------------
void CObjSet::SetUnknown6(DWORD nUnknown6)
{
   *(DWORD *)(pObj + 0x30) = pUtil->GetDW(&nUnknown6);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetScriptPID(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x40));
}
//---------------------------------------------------------------------------
void CObjSet::SetScriptPID(DWORD nScriptPID)
{
   *(DWORD *)(pObj + 0x40) = pUtil->GetDW(&nScriptPID);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetMapScriptID(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x44));
}
//---------------------------------------------------------------------------
void CObjSet::SetMapScriptID(DWORD nMapScriptID)
{
   *(DWORD *)(pObj + 0x44) = pUtil->GetDW(&nMapScriptID);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetFlags(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x24));
}
//---------------------------------------------------------------------------
void CObjSet::SetFlags(DWORD dwFlags)
{
   *(DWORD *)(pObj + 0x24) = pUtil->GetDW(&dwFlags);
}
//---------------------------------------------------------------------------
DWORD CObjSet::ChildCount(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x48));
}
//---------------------------------------------------------------------------
void CObjSet::SetChildCount(DWORD nNewCount)
{
   *(DWORD *)(pObj + 0x48) = pUtil->GetDW(&nNewCount);
}
//---------------------------------------------------------------------------
bool CObjSet::ObjIsChild(void)
{
   if (pUtil->GetDW((DWORD *)(pObj + 0x04)) == 0xFFFFFFFF ||
       pUtil->GetDW((DWORD *)(pObj + 0x04)) == 0x00000000)
      return true;
   else
      return false;
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetMaxChilds(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x4C));
}
//---------------------------------------------------------------------------
void CObjSet::SetMaxChilds(DWORD nMaxChilds)
{
   *(DWORD *)(pObj + 0x4C) = pUtil->GetDW(&nMaxChilds);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetCountInObj(void)
{
   return ObjIsChild() ? pUtil->GetDW((DWORD *)(pObj - 0x04)) : 0;
}
//---------------------------------------------------------------------------
void CObjSet::SetCountInObj(DWORD nCount)
{
   *(DWORD *)(pObj - 0x04) = pUtil->GetDW(&nCount);
}
//---------------------------------------------------------------------------
void CObjSet::GetHexXY(WORD *nX, WORD *nY)
{
   DWORD nXY = pUtil->GetDW((DWORD *)(pObj + 0x04));
   *nX = nXY % 200;
   *nY = nXY / 200;
}
//---------------------------------------------------------------------------
void CObjSet::SetHexXY(WORD nX, WORD nY)
{
   DWORD nXY;
   if (nX == 0xFFFF && nY == 0xFFFF)
      nXY = 0xFFFFFFFF;
   else
      nXY = nY * 200 + nX;
   *(DWORD *)(pObj + 0x04) = pUtil->GetDW(&nXY);
}
//---------------------------------------------------------------------------
int CObjSet::GetDWHexXY(void)
{
   return pUtil->GetDW((DWORD *)(pObj + 0x04));
}
//---------------------------------------------------------------------------
void CObjSet::SetDWHexXY(DWORD nXY)
{
}
//---------------------------------------------------------------------------
int CObjSet::GetXX(void)
{
   return (int)pUtil->GetDW((DWORD *)(pObj + 0x08));
}
//---------------------------------------------------------------------------
int CObjSet::GetYY(void)
{
   return (int)pUtil->GetDW((DWORD *)(pObj + 0x0C));
}
//---------------------------------------------------------------------------
void CObjSet::GetXYoffset(WORD *nX, WORD *nY)
{
   DWORD nXY = pUtil->GetDW((DWORD *)(pObj + 0x34));
   if (nXY != 0)
   {
      *nX = nXY % 200;
      *nY = nXY / 200;
   }
}
//---------------------------------------------------------------------------
void CObjSet::GetDirection(DWORD *nDir)
{
   *nDir = pUtil->GetDW((DWORD *)(pObj + 0x1C));
}
//---------------------------------------------------------------------------
void CObjSet::SetDirection(DWORD nDir)
{
   *(DWORD *)(pObj + 0x1C) = pUtil->GetDW(&nDir);
}
//---------------------------------------------------------------------------
DWORD CObjSet::GetFileOffset(void)
{
   return dwFirstObjFilePos + (pObj - pObjBuf);
}
//---------------------------------------------------------------------------
void CObjSet::SaveToFile(HANDLE h_map)
{
   DWORD i;
   pScriptBuf->SaveToFile(h_map);
//   WriteFile(h_map, pScriptBuf, dwScriptSize, &i, NULL);
   WriteFile(h_map, pObjBuf, dwObjSize, &i, NULL);
}
//---------------------------------------------------------------------------
void CObjSet::DeleteSelected()
{
   DWORD nObjNum, nLevel, nChildCount;
   DWORD dwObjTotal;
   DWORD dwLenght;
   BYTE *l_pObj;
   BYTE *pTemp;
   BYTE nObjType;
   bool MarkForDelete;
   DWORD dwDeletingLenght[3] = {0, 0, 0};
   DWORD dwDeletingObj[3] = {0, 0, 0};
   DWORD dwNewLevelCount[3] = {0, 0, 0};

   nLevel = 0;
   do
   {
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
      {
         if (!ObjIsChild())
         {
            MarkForDelete = ObjSelected();
            if (MarkForDelete) dwDeletingObj[nLevel] ++;
         }
         if (MarkForDelete)
            dwDeletingLenght[nLevel] += GetObjLen();
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
   } while (++nLevel < frmEnv->pMap->Levels);

   if (dwDeletingLenght[0] + dwDeletingLenght[1] + dwDeletingLenght[2])
   {
      //Приступаем к удалению объектов
      dwObjSize = dwObjSize -
              (dwDeletingLenght[0] + dwDeletingLenght[1] + dwDeletingLenght[2]);
      if ((pTempObjBuf = (BYTE *)malloc(dwObjSize)) == NULL) return;
      pTemp = pTempObjBuf;
      nObjTotal = nObjTotal -
                       (dwDeletingObj[0] + dwDeletingObj[1] + dwDeletingObj[2]);
      // Удаление...
      *(DWORD *)(pTemp) = pUtil->GetDW(&nObjTotal);
      pTemp += 4;

      nLevel = 0;
      do
      {
         dwNewLevelCount[nLevel] = nLevelCount[nLevel] - dwDeletingObj[nLevel];
         *(DWORD *)(pTemp) = pUtil->GetDW(&dwNewLevelCount[nLevel]);
         pTemp += 4;
         l_pObj = GetFirstObj(&nObjNum, nLevel);
         nChildCount = 0;
         while (l_pObj)
         {
            if (!ObjIsChild())
               MarkForDelete = ObjSelected();
            if (!MarkForDelete)
            {
               dwLenght = GetObjLen();
               if (!ObjIsChild())
                  memcpy(pTemp, l_pObj, dwLenght);
               else
                  memcpy(pTemp, l_pObj - 4, dwLenght);
               pTemp += dwLenght;
            }
            l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
         }
      } while (++nLevel < 3); //++nLevel < frmEnv->pMap->Levels
      int Offs = pTempObjBuf  + dwObjSize - pTemp;

      free(pObjBuf);
      pObjBuf = pTempObjBuf;
      pObjStart = pObjBuf;

      nSelected = 0;
      GetObjValues();
      // Проходим по объектам, чтобы вычислить начала каждого уровня
      nLevel = 0;
      do
      {
         l_pObj = GetFirstObj(&nObjNum, nLevel);
         nChildCount = 0;
         while (l_pObj)
            l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      } while (++nLevel < frmEnv->pMap->Levels);
   }
}
//---------------------------------------------------------------------------
void CObjSet::DeleteObject(BYTE *pDelObj)
{
   DWORD nObjNum, nLevel, nChildCount;
   DWORD dwObjTotal;
   DWORD dwLenght;
   BYTE *l_pObj, *pLastParent, *pParent;
   BYTE *pTemp;
   BYTE nObjType;
   bool MarkForDelete;
   DWORD dwDeletingLenght[3] = {0, 0, 0};
   DWORD dwDeletingObj[3] = {0, 0, 0};
   DWORD dwNewLevelCount[3] = {0, 0, 0};

   nLevel = 0;
   do
   {
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
      {
         if (!ObjIsChild()) pLastParent = l_pObj;
         if (l_pObj == pDelObj)
         {
            pParent = pLastParent;
            dwLenght = GetObjLen();
            if (!ObjIsChild()) dwDeletingObj[nLevel] ++;
            dwDeletingLenght[nLevel] += dwLenght;
         }
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
   } while (++nLevel < frmEnv->pMap->Levels);

   if (dwDeletingLenght[0] + dwDeletingLenght[1] + dwDeletingLenght[2])
   {
      //Приступаем к удалению объектов
      dwObjSize = dwObjSize -
              (dwDeletingLenght[0] + dwDeletingLenght[1] + dwDeletingLenght[2]);
      if ((pTempObjBuf = (BYTE *)malloc(dwObjSize)) == NULL) return;
      pTemp = pTempObjBuf;
      nObjTotal = nObjTotal -
                       (dwDeletingObj[0] + dwDeletingObj[1] + dwDeletingObj[2]);
      // Удаление...
      *(DWORD *)(pTemp) = pUtil->GetDW(&nObjTotal);
      pTemp += 4;

      nLevel = 0;
      do
      {
         dwNewLevelCount[nLevel] = nLevelCount[nLevel] - dwDeletingObj[nLevel];
         *(DWORD *)(pTemp) = pUtil->GetDW(&dwNewLevelCount[nLevel]);
         pTemp += 4;
         l_pObj = GetFirstObj(&nObjNum, nLevel);
         nChildCount = 0;
         while (l_pObj)
         {
            dwLenght = GetObjLen();
            if (l_pObj != pDelObj)
            {
               if (!ObjIsChild())
                  memcpy(pTemp, l_pObj, dwLenght);
               else
                  memcpy(pTemp, l_pObj - 4, dwLenght);
               pTemp += dwLenght;
            }
            if (l_pObj == pParent)
            {
               DWORD nNewChildCount = ChildCount() - 1;
               *(DWORD *)(pTemp - dwLenght + 0x48) = pUtil->GetDW(&nNewChildCount);
            }
            l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
         }
      } while (++nLevel < 3); //++nLevel < frmEnv->pMap->Levels
      int Offs = pTempObjBuf  + dwObjSize - pTemp;

      free(pObjBuf);
      pObjBuf = pTempObjBuf;
      pObjStart = pObjBuf;

//      nSelected = 0;
      GetObjValues();
      // Проходим по объектам, чтобы вычислить начала каждого уровня
      nLevel = 0;
      do
      {
         l_pObj = GetFirstObj(&nObjNum, nLevel);
         nChildCount = 0;
         while (l_pObj)
            l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      } while (++nLevel < frmEnv->pMap->Levels);
   }
}
//---------------------------------------------------------------------------
void CObjSet::AppendObject(int X, int Y, int nLevel, BYTE nType, WORD nID)
{
   BYTE *l_pObj = CreateObject(X, Y, nLevel, nType, nID);
   AddObject(l_pObj);
   ReleaseObject(l_pObj);
}
//---------------------------------------------------------------------------
void CObjSet::AppendChildObject(BYTE *pToObj, int nLevel, BYTE nType, WORD nID)
{
   BYTE *l_pObj = CreateObject(0xFFFF, 0xFFFF, nLevel, nType, nID);
   AddChildObject(pToObj, l_pObj);
   ReleaseObject(l_pObj);
}
//---------------------------------------------------------------------------
BYTE* CObjSet::CreateObject(int X, int Y, int nLevel, BYTE nType, WORD nID)
{
   int nObjLen = GetObjLenByID(nType, nID);
   if ((pNewObjBuf = (BYTE *)malloc(nObjLen)) == NULL) return NULL;
   memset(pNewObjBuf, 0, nObjLen);
   SetObj(pNewObjBuf);
   SetHexXY(X, Y);
   SetObjLevel(nLevel);
   SetObjType(nType, nID);
   WORD nFrmID = pProSet->pPRO[nType][nID].GetFrmID();
   SetObjFrm(nType, nFrmID);
   SetUnknown6(0xFFFFFFFF);
   SetScriptPID(0xFFFFFFFF);
   SetMapScriptID(0xFFFFFFFF);
   SetFlags(pProSet->pPRO[nType][nID].GetFlags());
   return pNewObjBuf;
}
//---------------------------------------------------------------------------
void CObjSet::ReleaseObject(BYTE *pObjForDelete)
{
   if (pObjForDelete == NULL) return;
   free(pObjForDelete);
   pObjForDelete = NULL;
}
//---------------------------------------------------------------------------
void CObjSet::AddObject(BYTE *pNewObj)
{
   DWORD dwNewObjLen, dwLenght;
   DWORD nObjNum, nLevel, nChildCount;
   BYTE nObjType;
   WORD nObjID;
   BYTE *l_pObj;
   BYTE *pTemp;
   bool Added = false;

   SetObj(pNewObj);
   GetObjType(&nObjType, &nObjID);
   dwNewObjLen = GetObjLenByID(nObjType, nObjID);

   DWORD dwAddingLenght[3] = {0, 0, 0};
   DWORD dwAddingObj[3] = {0, 0, 0};
   DWORD dwNewLevelCount[3] = {0, 0, 0};

   int dwPosition = GetDWHexXY();
   DWORD dwObjLevel = GetObjLevel();
   dwAddingLenght[dwObjLevel] = dwNewObjLen;
   dwAddingObj[dwObjLevel] = 1;

   dwObjSize = dwObjSize +
                      dwAddingLenght[0] + dwAddingLenght[1] + dwAddingLenght[2];
   if ((pTempObjBuf = (BYTE *)malloc(dwObjSize)) == NULL) return;
   pTemp = pTempObjBuf;
   nObjTotal = nObjTotal + dwAddingObj[0] + dwAddingObj[1] + dwAddingObj[2];

   *(DWORD *)(pTemp) = pUtil->GetDW(&nObjTotal);
   pTemp += 4;

   nLevel = 0;
   do
   {
      dwNewLevelCount[nLevel] = nLevelCount[nLevel] + dwAddingObj[nLevel];
      *(DWORD *)(pTemp) = pUtil->GetDW(&dwNewLevelCount[nLevel]);
      pTemp += 4;
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
      {
         DWORD dwTemp = GetDWHexXY();
         if (GetObjLevel() == dwObjLevel && GetDWHexXY() > dwPosition && !Added)
         {
            memcpy(pTemp, pNewObj, dwNewObjLen);
            pTemp += dwNewObjLen;
            Added = true;
         }
         dwLenght = GetObjLen();
         if (!ObjIsChild())
            memcpy(pTemp, l_pObj, dwLenght);
         else
            memcpy(pTemp, l_pObj - 4, dwLenght);
         pTemp += dwLenght;
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
      // Если абсолютные координаты нового объекта больше или на карте
      // нет объектов
      if (nLevel == dwObjLevel && !Added)
      {
         memcpy(pTemp, pNewObj, dwNewObjLen);
         pTemp += dwNewObjLen;
      }
   } while (++nLevel < frmEnv->pMap->Levels);

   free(pObjBuf);
   pObjBuf = pTempObjBuf;
   pObjStart = pObjBuf;

   nSelected = 0;
   GetObjValues();
   // Проходим по объектам, чтобы вычислить начала каждого уровня
   nLevel = 0;
   do
   {
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
   } while (++nLevel < frmEnv->pMap->Levels);
}
//---------------------------------------------------------------------------
void CObjSet::AddChildObject(BYTE *pToObj, BYTE *pNewObj)
{
   // Внимание !!! Добавление в инвентарь не увеличивает счётчик объектов !
   DWORD dwNewObjLen, dwLenght;
   DWORD nObjNum, nLevel, nChildCount;
   BYTE nObjType;
   WORD nObjID;
   BYTE *l_pObj;
   BYTE *pTemp;

   SetObj(pNewObj);
   GetObjType(&nObjType, &nObjID);
   dwNewObjLen = GetObjLenByID(nObjType, nObjID);

   DWORD dwAddingLenght[3] = {0, 0, 0};
   DWORD dwAddingObj[3] = {0, 0, 0};
   DWORD dwNewLevelCount[3] = {0, 0, 0};

   DWORD dwObjLevel = GetObjLevel();
   dwAddingLenght[dwObjLevel] = dwNewObjLen + 4; // Учитываем 4 байта для кол-ва
   dwAddingObj[dwObjLevel] = 0; //Не увеличиваем счётчик объектов

   dwObjSize = dwObjSize +
                      dwAddingLenght[0] + dwAddingLenght[1] + dwAddingLenght[2];
   if ((pTempObjBuf = (BYTE *)malloc(dwObjSize)) == NULL) return;
   pTemp = pTempObjBuf;
   nObjTotal = nObjTotal + dwAddingObj[0] + dwAddingObj[1] + dwAddingObj[2];

   *(DWORD *)(pTemp) = pUtil->GetDW(&nObjTotal);
   pTemp += 4;

   nLevel = 0;
   do
   {
      dwNewLevelCount[nLevel] = nLevelCount[nLevel] + dwAddingObj[nLevel];
      *(DWORD *)(pTemp) = pUtil->GetDW(&dwNewLevelCount[nLevel]);
      pTemp += 4;
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
      {
         dwLenght = GetObjLen();
         if (!ObjIsChild())
            memcpy(pTemp, l_pObj, dwLenght);
         else
            memcpy(pTemp, l_pObj - 4, dwLenght);
         pTemp += dwLenght;

         if (l_pObj == pToObj)
         {
            DWORD nNewChildCount = ChildCount() + 1;
            *(DWORD *)(pTemp - dwLenght + 0x48) = pUtil->GetDW(&nNewChildCount);
            *(DWORD *)(pTemp) = 0x01000000; //Count of new items
            *(DWORD *)(pTemp - dwLenght + 0x4C) = 0x0A000000; //Max items
            pTemp += 4;
            memcpy(pTemp, pNewObj, dwNewObjLen);
            pTemp += dwNewObjLen;
         }

         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
   } while (++nLevel < frmEnv->pMap->Levels);

   free(pObjBuf);
   pObjBuf = pTempObjBuf;
   pObjStart = pObjBuf;

//   nSelected = 0;
   GetObjValues();
   // Проходим по объектам, чтобы вычислить начала каждого уровня
   nLevel = 0;
   do
   {
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
   } while (++nLevel < frmEnv->pMap->Levels);
}
//---------------------------------------------------------------------------
void CObjSet::ClearSelection(void)
{
   if (!nSelected) return;
   BYTE *l_pObj;
   DWORD nObjNum, nChildCount = 0;
   int nLevel = 0;
   do
   {
      l_pObj = GetFirstObj(&nObjNum, nLevel);
      while (l_pObj)
      {
         if (ObjSelected())
            DeselectObj();
         l_pObj = GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
   } while (++nLevel < frmEnv->pMap->Levels);
}
//---------------------------------------------------------------------------
void CObjSet::ChangeScriptID(BYTE *pInObj, int nScriptID)
{
   BYTE *pTempObj = GetObj();
   SetObj(pInObj);
   DWORD dwCurrentScriptID = GetMapScriptID();
   if (dwCurrentScriptID != nScriptID)
   {
      DWORD dwScriptDesc = GetScriptPID();
      if (dwScriptDesc == 0xFFFFFFFF) //Добавляем описатель
      {
         BYTE nObjType;
         WORD nObjID;
         GetObjType(&nObjType, &nObjID);
         int ScrDescType = nObjType == critter_ID ? 4 : 3;
         dwScriptDesc = pScriptBuf->AppendDesc(ScrDescType);
      }
      pScriptBuf->ChangeScriptID(dwScriptDesc, (DWORD)nScriptID);
      SetScriptPID(dwScriptDesc);
      SetMapScriptID((DWORD)nScriptID);
   }
   SetObj(pTempObj);
}
//---------------------------------------------------------------------------
CObjSet::~CObjSet()
{
   if (pScriptBuf != NULL) delete pScriptBuf;
   if (pObjBuf != NULL) free(pObjBuf);
}
//---------------------------------------------------------------------------


