//---------------------------------------------------------------------------
#ifndef prosetH
#define prosetH

#include <classes.hpp>
#include "utilites.h"
#include "log.h"
#include "pro.h"

class CProSet
{
public:
   bool lError;

   CPro *pPRO[6];

   int nProCount[6];
   void ClearLocals(void);
   void LoadLocalPROs(void);
   void LoadPRO(BYTE nObjType, WORD nObjID, bool bLocal);
   DWORD GetSubType(BYTE nObjType, WORD nObjID);
   CProSet(void);
   virtual ~CProSet();

protected:
   CUtilites *pUtil;
   CLog *pLog;
   void  InitPROArrays(void);
};
//---------------------------------------------------------------------------
#endif
