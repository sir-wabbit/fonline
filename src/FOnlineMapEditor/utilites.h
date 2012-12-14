//---------------------------------------------------------------------------
#ifndef utilitesH
#define utilitesH

#include <classes.hpp>
#include <graphics.hpp>
#include "datfile.h"

class CUtilites
{
public:
   bool lError;

   bool FirstRun;

   String DataDir;
   String GameDir;
   String MapperDir;

   TColor selColor;

   TDatFile *pPatchDAT, *pMasterDAT, *pCritterDAT;

   void InitDirectories(void);
   void MapperConfiguration(void);
   String GetFolder(String Caption);
   String GetRegInfoS(String RegKey, String KeyName);
   bool SetRegInfoS(String RegKey, String KeyName, String Value);

   HANDLE OpenFileX(String sFile);
   void SetFilePointerX(HANDLE hFile, LONG lDistanceToMove, DWORD dwMoveMethod);
   void ReadFileX(HANDLE hFile, LPVOID lpBuffer,
                       DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
   DWORD GetFileSizeX(HANDLE hFile, LPDWORD lpFileSizeHigh);
   bool CloseHandleX(HANDLE hFile);

   String GetFRMFileName(BYTE nObjType, String FileName);
   void ReverseDW(DWORD *addr);
   void ReverseW(WORD *addr);
   DWORD GetDW(DWORD *addr);
   WORD GetW(WORD *addr);
   void GetTileWorldCoord(int TileX, int TileY, int *WorldX, int *WorldY);
   void GetCursorTile(int X, int Y, int WorldX, int WorldY,
                                                        int *TileX, int *TileY);
   void GetWorldCoord(int X, int Y, int *WorldX, int *WorldY, int *iLevel);
   void GetCursorHex(int X, int Y, int WorldX, int WorldY, int *HexX, int *HexY);
   void GetHexWorldCoord(int X, int Y, int *WorldX, int *WorldY);
   bool GetListFromFile(String filename, TStringList *pList);
   String GetMsgValue(String MsgLine, int ValuePos);
   String GetName(String str);
   String GetDescription(String str);
   void RetranslateString(char *ptr);
   int GetBlockType(BYTE nProObjType, WORD nProID);
   void GetBlockFrm(int nBlockType, BYTE *nObjType, WORD *nFrmID);
   DWORD GetIndexBySuffix(String Suffix);
   String GetDxError(DWORD dwErrCode);

   CUtilites(void);
   virtual ~CUtilites();

protected:
   String IDname[7];
   TStringList *pSuffix;
};
//---------------------------------------------------------------------------
#endif
