//---------------------------------------------------------------------------

#ifndef scriptH
#define scriptH

#include "utilites.h"

class CScriptBuf
{
public:
   bool lError;

   int nDescCount[5];
   int nMaxDescID[5];

   CScriptBuf(BYTE *pTemp, DWORD dwSize);
   void CreateDescBlock(int ScrDescType, int count);
   void CopyDesc(DWORD *pDesc, int ScrDescType);
   DWORD AppendDesc(int ScrDescType); //DWORD HexPos, DWORD Level, DWORD Radius
   void ChangeScriptID(DWORD dwScriptDesc, DWORD dwScriptID);
   DWORD GetDescID(DWORD *pDW);
   void SetDescID(DWORD *pDW, DWORD dwDescID);
   DWORD GetScriptID(DWORD *pDW);
   void SetScriptID(DWORD *pDW, DWORD dwScriptID);
   void SaveToFile(HANDLE h_map);

   virtual ~CScriptBuf();
protected:
   CUtilites *pUtil;
   DWORD dwScriptSize;
   BYTE *pScriptBuf;
   BYTE *pScriptDesc;

   DWORD **pDescBuf;
};
//---------------------------------------------------------------------------
#endif
