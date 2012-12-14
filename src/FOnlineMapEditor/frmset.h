//---------------------------------------------------------------------------
#ifndef frmsetH
#define frmsetH

#include <classes.hpp>
#include <graphics.hpp>
#include "frame.h"
#include "utilites.h"
#include "log.h"
#include "objset.h"
#include "lists.h"
//---------------------------------------------------------------------------
class CFrmSet
{
public:
   bool lError;

   CFrame *pFRM[8];
//   CFrame *pFRM_HEX;
   int nFrmCount[8]; //include intrface_ID
   DWORD dwDirectionOffset[6];
   signed short doffX[6];
   signed short doffY[6];

   void ClearLocals(void);
   void LoadLocalFRMs(void);
   void LoadFRM(String sFile, BYTE nFrmType, WORD nFrmID, bool bLocal);
   void FreeUpFRM(CFrame *l_pFRM);
   void GetCritterFName(String* filename, DWORD frmPID, WORD *frmID);
   bool getSuffixes(DWORD ID1, DWORD ID2, char& Suffix1, char& Suffix2);

   CFrmSet(void);
   virtual ~CFrmSet();

protected:
   CUtilites *pUtil;
   CLog *pLog;
   CObjSet *pObjSet;
   CListFiles *pLstFiles;

};
//---------------------------------------------------------------------------
#endif
