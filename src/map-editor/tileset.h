//---------------------------------------------------------------------------
#ifndef tilesetH
#define tilesetH

#include <classes.hpp>
#include "lists.h"
#include "frmset.h"
#include "utilites.h"

class CTileSet
{
public:
   DWORD dwTileSetSize;
   DWORD dwSelection;
   int SelectedFloorX1, SelectedFloorY1, SelectedFloorX2, SelectedFloorY2;
   int SelectedRoofX1, SelectedRoofY1, SelectedRoofX2, SelectedRoofY2;

   WORD GetRoofID(int x, int y);
   WORD GetFloorID(int x, int y);
   void SetFloor(int x, int y, int iLevel, WORD tileID);
   void SetRoof(int x, int y, int iLevel, WORD tileID);
   void SetFloorRegion(int X1, int Y1, int X2, int Y2, WORD tileID);
   void SetRoofRegion(int X1, int Y1, int X2, int Y2, WORD tileID);
   void SelectTiles(int TileMode, int iLevel, int TileX1, int TileY1,
                                              int TileX2, int TileY2);

   bool FloorIsSelected(int TileX, int TileY);
   bool RoofIsSelected(int TileX, int TileY);
   void ClearFloorSelection(void);
   void ClearRoofSelection(void);
   CTileSet(HANDLE h_map, DWORD dwSize);
   void SaveToFile(HANDLE h_map);
   virtual ~CTileSet();

protected:
   struct TILES
   {
      WORD roof;
      WORD floor;
   } tls;
   TILES *pTile;
   CUtilites *pUtil;
   CListFiles *pLstFiles;
   CFrmSet *pFrmSet;
};
//---------------------------------------------------------------------------
#endif
