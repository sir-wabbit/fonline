//////////////////////////////////////////////////////////////////////
// CProSet Class
//////////////////////////////////////////////////////////////////////

#include "proset.h"
#include "objset.h"
#include "main.h"
#include "mdi.h"
#include "utilites.h"
#include "pro.h"
#include "macros.h"
//---------------------------------------------------------------------------
CProSet::CProSet(void)
{
   lError = true;
   pUtil = frmMDI->pUtil;
   pLog = frmMDI->pLog;

   InitPROArrays();
   lError = false;
}
//---------------------------------------------------------------------------
void CProSet::InitPROArrays(void)
{
   for (int i = 0; i < 6; i++)
      pPRO[i] = new CPro[frmMDI->pLstFiles->pPROlst[i]->Count + 1];
}
//---------------------------------------------------------------------------
void CProSet::ClearLocals(void)
{
   int i, j;
   for (i = 0; i < 6; i++)
      for (j = 0; j < frmMDI->pLstFiles->pPROlst[i]->Count + 1; j++)
         pPRO[i][j].bLocal = false;
   memset(nProCount, 0, 24);
}
//---------------------------------------------------------------------------
void CProSet::LoadLocalPROs(void)
{
   CObjSet *pObjSet = frmEnv->pObjSet;
   DWORD x, y, nObjNum, nLevel, nChildCount = 0;
   BYTE *pObj;
   BYTE nObjType;
   WORD nProID;
   nLevel = 0;
   do
   {
      pObj = pObjSet->GetFirstObj(&nObjNum, nLevel);
      while (pObj)
      {
//         if (nObjNum == 600) // bug in junkcsno.map [fallout1]
//            nObjNum = nObjNum;
//         pLog->LogX("Object Num:" + String(nObjNum) + " Child Count:" + String(nChildCount));
         pObjSet->GetObjType(&nObjType, &nProID);
         LoadPRO(nObjType, nProID, true);
         pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, nLevel);
         frmMDI->iPos++;
         Application->ProcessMessages();
      }
   } while (++nLevel < frmEnv->pMap->Levels);
}
//---------------------------------------------------------------------------
void CProSet::LoadPRO(BYTE nObjType, WORD nObjID, bool bLocal)
{
   CPro *l_pPRO = &pPRO[nObjType][nObjID];
   nProCount[nObjType] += !l_pPRO->bLocal && bLocal ? 1 : 0;
   l_pPRO->bLocal = l_pPRO->bLocal ? l_pPRO->bLocal : bLocal;
   if (l_pPRO->data != NULL) return;

   String IDname[6] = {"items", "critters", "scenery", "walls", "tiles", "misc"};
   ULONG i;
   DWORD filesize, filesize2;

   pLog->LogX("Need PRO file for ObjType = " + String(nObjType) +
              ", ObjID = \"" + String(nObjID));
   if (nObjID > frmMDI->pLstFiles->pPROlst[nObjType]->Count) return;
   String profile = "proto\\" + IDname[nObjType]+ "\\" +
                     frmMDI->pLstFiles->pPROlst[nObjType]->Strings[nObjID - 1];
   HANDLE h_pro = pUtil->OpenFileX(profile);
   if (h_pro == INVALID_HANDLE_VALUE)
   {
      pLog->LogX("Cannot open file \"" + profile + "\"");
      return;
   }
   filesize = pUtil->GetFileSizeX(h_pro, &filesize2);
   pPRO[nObjType][nObjID].LoadData(filesize);     //nObjID + 1
   pUtil->ReadFileX(h_pro, l_pPRO->data, filesize, &i);
   pLog->LogX("Loaded PRO file \"" + profile + "\" load " + String(i) + " of " +
              String(filesize) + " bytes.");
   pUtil->CloseHandleX(h_pro);
}
//---------------------------------------------------------------------------
DWORD CProSet::GetSubType(BYTE nObjType, WORD nObjID)
{
   LoadPRO(nObjType, nObjID, true);
   return pPRO[nObjType][nObjID].GetSubType();
}
//---------------------------------------------------------------------------
CProSet::~CProSet()
{
   for (int i = 0; i < 6; i++)
      delete []pPRO[i];
}
//---------------------------------------------------------------------------

