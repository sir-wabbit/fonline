//---------------------------------------------------------------------------

#ifndef msgH
#define msgH

#include "utilites.h"

class CMsg
{
public:
   bool lError;

   TStringList *pMSGlst[6];

   String GetMSG(BYTE nType, DWORD nID);

   CMsg(void);
   virtual ~CMsg();

protected:
   CMsg *pMsg;
   CUtilites *pUtil;

   void LoadDefaultMsg(void);
};
//---------------------------------------------------------------------------
#endif
 