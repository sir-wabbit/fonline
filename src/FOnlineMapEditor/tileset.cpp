//////////////////////////////////////////////////////////////////////
// CTileSet Class
//////////////////////////////////////////////////////////////////////

#include "tileset.h"
#include "mdi.h"
#include "lists.h"
#include "frmset.h"
#include "utilites.h"
#include "macros.h"
//---------------------------------------------------------------------------
CTileSet::CTileSet(HANDLE h_map, DWORD dwSize)
{
   ULONG i;
   pTile = NULL;
   pUtil = frmMDI->pUtil;
   pFrmSet = frmMDI->pFrmSet;
   pLstFiles = frmMDI->pLstFiles;
   ClearFloorSelection();
   ClearRoofSelection();
   dwTileSetSize = dwSize;
   pTile = new TILES [dwTileSetSize];
   ReadFile(h_map, pTile, dwTileSetSize * 4, &i, NULL);
}
//---------------------------------------------------------------------------
WORD CTileSet::GetRoofID(int x, int y)
{
   return pUtil->GetW(&pTile[y*100+x].roof);
}
//---------------------------------------------------------------------------
WORD CTileSet::GetFloorID(int x, int y)
{
   return pUtil->GetW(&pTile[y*100+x].floor);
}
//------------------------------------------------------------------------------
void CTileSet::SetFloor(int x, int y, int iLevel, WORD tileID)
{
   pFrmSet->LoadFRM(pUtil->GetFRMFileName(tile_ID,
                                pLstFiles->pFRMlst[tile_ID]->Strings[tileID]),
                                                         tile_ID, tileID, true);
   pTile[(iLevel * 100 + y) * 100 + x].floor = pUtil->GetW(&tileID);
}
//------------------------------------------------------------------------------
void CTileSet::SetRoof(int x, int y, int iLevel, WORD tileID)
{
   pFrmSet->LoadFRM(pUtil->GetFRMFileName(tile_ID,
                                pLstFiles->pFRMlst[tile_ID]->Strings[tileID]),
                                                         tile_ID, tileID, true);
   pTile[(iLevel * 100 + y + 3) * 100 + x + 2].roof = pUtil->GetW(&tileID);
}
//------------------------------------------------------------------------------
void CTileSet::SetFloorRegion(int X1, int Y1, int X2, int Y2, WORD tileID)
{
   pFrmSet->LoadFRM(pUtil->GetFRMFileName(tile_ID,
                                pLstFiles->pFRMlst[tile_ID]->Strings[tileID]),
                                                         tile_ID, tileID, true);
   for (int y = Y1; y <= Y2; y++)
      for (int x = X1; x <= X2; x++)
         pTile[(y * 100) + x].floor = pUtil->GetW(&tileID);
}
//------------------------------------------------------------------------------
void CTileSet::SetRoofRegion(int X1, int Y1, int X2, int Y2, WORD tileID)
{
   pFrmSet->LoadFRM(pUtil->GetFRMFileName(tile_ID,
                                pLstFiles->pFRMlst[tile_ID]->Strings[tileID]),
                                                         tile_ID, tileID, true);
   for (int y = Y1; y <= Y2; y++)
      for (int x = X1; x <= X2; x++)
         pTile[(y * 100) + x].roof = pUtil->GetW(&tileID);
}
//------------------------------------------------------------------------------
void CTileSet::SelectTiles(int TileMode, int iLevel, int TileX1, int TileY1,
                                                     int TileX2, int TileY2)
{
   if (TileMode == SELECT_FLOOR)
   {
      SelectedFloorX1 = TileX1;
      SelectedFloorY1 = TileY1;
      SelectedFloorX2 = TileX2;
      SelectedFloorY2 = TileY2;
      dwSelection = floor_ID;
   }
   else
   {
      SelectedRoofX1 = TileX1 + 2;
      SelectedRoofY1 = TileY1 + 3;
      SelectedRoofX2 = TileX2 + 2;
      SelectedRoofY2 = TileY2 + 3;
      dwSelection = roof_ID;
   }
}
//------------------------------------------------------------------------------
bool CTileSet::FloorIsSelected(int TileX, int TileY)
{
   return ((TileX >= SelectedFloorX1 && TileX <= SelectedFloorX2) &&
           (TileY >= SelectedFloorY1 && TileY <= SelectedFloorY2));
}
//------------------------------------------------------------------------------
bool CTileSet::RoofIsSelected(int TileX, int TileY)
{
   return ((TileX >= SelectedRoofX1 && TileX <= SelectedRoofX2) &&
           (TileY >= SelectedRoofY1 && TileY <= SelectedRoofY2));
}
//------------------------------------------------------------------------------
void CTileSet::ClearFloorSelection(void)
{
   SelectedFloorX1 = -1;
   SelectedFloorY1 = -1;
   SelectedFloorX2 = -1;
   SelectedFloorY2 = -1;
   dwSelection = NONE_SELECTED;
}
//------------------------------------------------------------------------------
void CTileSet::ClearRoofSelection(void)
{
   SelectedRoofX1 = -1;
   SelectedRoofY1 = -1;
   SelectedRoofX2 = -1;
   SelectedRoofY2 = -1;
   dwSelection = NONE_SELECTED;
}
//------------------------------------------------------------------------------
void CTileSet::SaveToFile(HANDLE h_map)
{
   DWORD i;
   WriteFile(h_map, pTile, dwTileSetSize << 2, &i, NULL);
}
//---------------------------------------------------------------------------
CTileSet::~CTileSet()
{
   if (pTile != NULL)
      delete []pTile;
}
//---------------------------------------------------------------------------

