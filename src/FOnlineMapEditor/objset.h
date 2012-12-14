//---------------------------------------------------------------------------
#ifndef objsetH
#define objsetH

#include <classes.hpp>
#include "utilites.h"
#include "proset.h"
#include "script.h"

class CObjSet
{
public:
   bool lError;

   CScriptBuf *pScriptBuf;
   DWORD dwBufSize;
   DWORD dwScriptSize;
   DWORD dwObjSize;
   DWORD nObjTotal, nLevelCount[3];
   DWORD nSelected;

   CObjSet(HANDLE h_map);
   BYTE* GetFirstObj(DWORD *nObjNum, int nLevel);
   BYTE* GetNextObj(DWORD *nObjNum, DWORD *nChildCount, int nLevel);
   void SetObj(BYTE *pSetObj);
   BYTE* GetObj(void);
   void SelectObj(void);
   BYTE* GetSelObj(void);
   void DeselectObj(void);
   BYTE ObjSelected(void);
   DWORD GetObjInfo(int nDWORD);
   void SetObjInfo(int nDWORD, DWORD dwData);
   int GetObjLen(void);
   int GetObjLenByID(BYTE nObjType, WORD nObjID);
   int GetFullType(void);
   void GetObjType(BYTE *nType, WORD *nID);
   void SetObjType(BYTE nType, WORD nID);
   void GetObjFrm(BYTE *nType, WORD *nID);
   void SetObjFrm(BYTE nType, WORD nID);
   DWORD GetObjFrmDW();
   DWORD GetObjLevel(void);
   void SetObjLevel(DWORD nLevel);
   DWORD GetUnknown6(void);
   void SetUnknown6(DWORD nUnknown6);
   DWORD GetScriptPID(void);
   void SetScriptPID(DWORD nScriptPID);
   DWORD GetMapScriptID(void);
   void SetMapScriptID(DWORD nMapScriptID);
   DWORD GetFlags(void);
   void SetFlags(DWORD dwFlags);
   DWORD ChildCount(void);
   void SetChildCount(DWORD nNewCount);
   bool ObjIsChild(void);
   DWORD GetMaxChilds(void);
   void SetMaxChilds(DWORD nChildsCount);
   DWORD GetCountInObj(void);
   void SetCountInObj(DWORD nCount);
   void GetHexXY(WORD *nX, WORD *nY);
   void SetHexXY(WORD nX, WORD nY);
   int GetDWHexXY();
   void SetDWHexXY(DWORD nXY);
   int GetXX(void);
   int GetYY(void);
   void GetXYoffset(WORD *nX, WORD *nY);
   void GetDirection(DWORD *nDir);
   void SetDirection(DWORD nDir);
   DWORD GetFileOffset(void);
   void SaveToFile(HANDLE h_map);
   void DeleteSelected();
   void DeleteObject(BYTE *pDelObj);
   void AppendObject(int X, int Y, int nLevel, BYTE nType, WORD nID);
   void AppendChildObject(BYTE *pToObj, int nLevel, BYTE nType, WORD nID);
   BYTE* CreateObject(int X, int Y, int nLevel, BYTE nType, WORD nID);
   void ReleaseObject(BYTE *pObjForDelete);
   void AddObject(BYTE *pNewObj);
   void AddChildObject(BYTE *pToObj, BYTE *pNewObj);
   void ClearSelection(void);
   void ChangeScriptID(BYTE *pInObj, int nScriptID);
   virtual ~CObjSet();

protected:
   CUtilites *pUtil;
   CProSet *pProSet;
   DWORD dwFirstObjFilePos;
   BYTE *pTempBuf, *pObjBuf, *pObj, *pSelObj, *pObjStart;
   BYTE *pTempObjBuf, *pNewObjBuf;
   BYTE *pLevel[3];
   void FindObjStart(void);
   void GetObjValues(void);
};
//---------------------------------------------------------------------------
#endif

