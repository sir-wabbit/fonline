//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "main.h"
#include "mdi.h"
#include "utilites.h"
#include "log.h"
#include "map.h"
#include "lists.h"
#include "frmset.h"
#include "frame.h"
#include "tileset.h"
#include "pal.h"
#include "macros.h"
#include "properts.h"
#include "msg.h"
#include "change.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmEnv *frmEnv;
//---------------------------------------------------------------------------
__fastcall TfrmEnv::TfrmEnv(TComponent* Owner)
        : TForm(Owner)
{
   pUtil = frmMDI->pUtil;
   pLog = frmMDI->pLog;
   pFrmSet = frmMDI->pFrmSet;
   pProSet = frmMDI->pProSet;
   pLstFiles = frmMDI->pLstFiles;
   pMsg = frmMDI->pMsg;
   pPal = frmMDI->pPal;
   pMap = NULL;
   pTileSet = NULL;
   pObjSet = NULL;
   frmProp = frmMDI->frmProp;

   dds = NULL; dds2Map = NULL; dds2Nav = NULL; dds2Inv = NULL;
   ddcMap = NULL; ddcNav = NULL; ddcInv = NULL;

   panelTILE = (TStatusPanel *)sbar->Panels->Items[0];
   panelHEX = (TStatusPanel *)sbar->Panels->Items[1];
   panelObjCount = (TStatusPanel *)sbar->Panels->Items[2];
   panelObjSelected = (TStatusPanel *)sbar->Panels->Items[3];
   panelMSG = (TStatusPanel *)sbar->Panels->Items[4];

   pLog->LogX("Attach DDraw primary surface ...");
   AttachDDraw (this, &dds, 1);
   pLog->LogX("Attach DDraw map surface ...");
   AttachDDraw (imgMap, &dds2Map, 0);
   pLog->LogX("Attach DDraw navigator surface ...");
   AttachDDraw (imgObj, &dds2Nav, 0);
   pLog->LogX("Attach DDraw inventory surface ...");
   AttachDDraw (imgInv, &dds2Inv, 0);

   imgMap->Cursor = (TCursor)crHandCursor;
   OldMapWndProc = imgMap->WindowProc;
   imgMap->WindowProc = NewMapWndProc;
   OldNavWndProc = imgObj->WindowProc;
   imgObj->WindowProc = NewNavWndProc;
   OldInvWndProc = imgInv->WindowProc;
   imgInv->WindowProc = NewInvWndProc;

   OldX = -100;
   OldY = -100;
   WorldX = 3640;
   WorldY = 1560;
   iLevel = 0;

   bShowObj[item_ID] = frmMDI->btnItems->Down;
   bShowObj[critter_ID] = frmMDI->btnCritters->Down;
   bShowObj[scenery_ID] = frmMDI->btnScenery->Down;
   bShowObj[wall_ID] = frmMDI->btnWalls->Down;
   bShowObj[floor_ID] = frmMDI->btnFloor->Down;
   bShowObj[misc_ID] = frmMDI->btnMisc->Down;
   bShowObj[roof_ID] = frmMDI->btnRoof->Down;
   bShowObj[EG_blockID] = frmMDI->btnEGBlock->Down;
   bShowObj[SAI_blockID] = frmMDI->btnSaiBlock->Down;
   bShowObj[wall_blockID] = frmMDI->btnWallBlock->Down;
   bShowObj[obj_blockID] = frmMDI->btnObjBlock->Down;
   bShowObj[light_blockID] = frmMDI->btnLightBlock->Down;
   bShowObj[scroll_blockID] = frmMDI->btnScrollBlock->Down;
   bShowObj[obj_self_blockID] = frmMDI->btnObjSelfBlock->Down;
   SelectMode = SELECT_NONE;

   ULONG i;
   h_map = CreateFile(frmMDI->OpenDialog1->FileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
   if (h_map == INVALID_HANDLE_VALUE)
   {
      pLog->LogX("Cannot open file.");
      return;
   }

   frmMDI->OpenPBarForm();
   frmMDI->frmPBar->NewTask("Loading map data", "Load header ...",
                                                           0, 0, &frmMDI->iPos);
   pMap = new CMap(h_map); //passed
   frmMDI->frmPBar->NewTask(NULL, "Load tileset ...", 0, 0, &frmMDI->iPos);
   pTileSet = new CTileSet(h_map, pMap->TilesSizeX * pMap->TilesSizeY); //passed
   pObjSet = new CObjSet(h_map); //passed
   this->Caption = frmMDI->OpenDialog1->FileName;
   this->lblInfo->Caption =
       (pMap->dwVer == VER_FALLOUT1 ? "[FALLOUT 1] " : "[FALLOUT 2] ") +
       (String)pMap->mapvars.mapname +
       " :: " + (String)(pUtil->GetDW(&pMap->mapvars.Num1)) + " local var(s)" +
       " :: " + (String)(pUtil->GetDW(&pMap->mapvars.Num2)) + " global var(s)";

   frmMDI->frmPBar->NewTask(NULL, "Load pro files ...", 0,
                                             pObjSet->nObjTotal, &frmMDI->iPos);
   Application->ProcessMessages();
   pProSet->ClearLocals();
   pProSet->LoadLocalPROs();

   frmMDI->frmPBar->NewTask(NULL, "Load frm files ...", 0,
       pMap->TilesSizeX * pMap->TilesSizeY + pObjSet->nObjTotal, &frmMDI->iPos);
   Application->ProcessMessages();
   pFrmSet->ClearLocals();
   pFrmSet->LoadLocalFRMs();

   frmMDI->DeletePBarForm();

   pBMPlocator = new Graphics::TBitmap();
   pBMPlocator->LoadFromResourceName((UINT)HInstance, "locator");
   DrawMiniMap();
   RedrawMap(true);

   frmMDI->tbcurs->Visible = true;
   frmMDI->tbVis->Visible = true;
   frmMDI->btnSaveAs->Enabled = true;
   btnLocalClick(this);
   RedrawInventory();
   SetButtonSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::FormDestroy(TObject *Sender)
{
   imgMap->WindowProc = OldMapWndProc;
   imgObj->WindowProc = OldNavWndProc;
   if (pBMPlocator) delete pBMPlocator;
   if (pObjSet) delete pObjSet;
   if (pTileSet) delete pTileSet;
   if (pMap) delete pMap;
   if (dds) dds->Release();
   if (ddc) ddc->Release();
   if (dds2Map) dds2Map->Release();
   if (dds2Nav) dds2Nav->Release();
   if (dds2Inv) dds2Inv->Release();
   frmMDI->tbcurs->Visible = false;
   frmMDI->tbVis->Visible = false;
   frmMDI->btnhand->Down = true;
   frmProp->Hide();
}
//---------------------------------------------------------------------------
void TfrmEnv::SetButtonSave(bool State)
{
   frmMDI->btnSave->Enabled = State;
}
//---------------------------------------------------------------------------
void TfrmEnv::DrawMiniMap(void)
{
   Graphics::TBitmap *OffMiniMap = new Graphics::TBitmap;
   OffMiniMap->PixelFormat = pf32bit;
   OffMiniMap->IgnorePalette = true;
   OffMiniMap->Width = 100;
   OffMiniMap->Height = 300;
   PatBlt(OffMiniMap->Canvas->Handle, 0, 0, 100, 300, BLACKNESS);
   for (int y = 0; y < pMap->Levels * 100 ; y++)
   {
      unsigned int *LinePtr = (unsigned int *)OffMiniMap->ScanLine[y];
      for (int x = 0; x < 100; x++)
      {
         int FloorId = pTileSet->GetFloorID(x, y);
         if (FloorId != 1)
         {
            LinePtr[99 - x] =
             pPal->GetEntry(pFrmSet->pFRM[tile_ID][FloorId].GetCenterPix(0, 0));
         }
      }
   }
  imgMiniMap->Canvas->Draw(0, 0, OffMiniMap);
  delete OffMiniMap;
}
//---------------------------------------------------------------------------
void TfrmEnv::PrepareNavigator(BYTE nObjType)
{
   sb->Max = 1; // Обходим ошибку, когда sb->Min > sbMax 
   // Подготавливаем scrollbar в навигаторе для работы с объектами
   // Минимум равен 1, т.к. ПРО и ФРМ начинаются с индекса 1
   sb->Min = nObjType == tile_ID ? 0 : 1;
   sb->Position = sb->Min;
   // Проверяем режим показа объектов : локальный или глобальный

   switch (nObjType)
   {
      case tile_ID:
         if (Navigator.nShowMode == SHOW_WORLD)
            Navigator.nCount = pLstFiles->pFRMlst[nObjType]->Count - 1;
         else
            Navigator.nCount = pFrmSet->nFrmCount[nObjType];
         Navigator.nMaxID = pLstFiles->pFRMlst[nObjType]->Count;
         break;
      case inven_ID:
         if (Navigator.nShowMode == SHOW_WORLD)
            Navigator.nCount = pLstFiles->pPROlst[item_ID]->Count;
         else
            Navigator.nCount = pProSet->nProCount[item_ID];
         Navigator.nMaxID = pLstFiles->pPROlst[item_ID]->Count;
         break;
      default:
         if (Navigator.nShowMode == SHOW_WORLD)
            Navigator.nCount = pLstFiles->pPROlst[nObjType]->Count;
         else
            Navigator.nCount = pProSet->nProCount[nObjType];
         Navigator.nMaxID = pLstFiles->pPROlst[nObjType]->Count;
   }
   sb->Max = Navigator.nCount > 8 ? Navigator.nCount - 7 : sb->Min;
   sb->Enabled = sb->Min != sb->Max;
   // Устанавливаем тип отображаемых элементов для навигатора
   Navigator.nObjType = nObjType;
   // Сбрасываем селектирование элементов в навигаторе
   Navigator.nSelID = -1;
   // Установка стиля фонта отображаемого в навигаторе
   imgObj->Font->Name = "Tahoma";
   imgObj->Font->Height = 11;
   imgObj->Font->Style = TFontStyles()<< fsBold;
   imgObj->Brush->Color = clBlack;
   imgObj->Font->Color = clWhite;
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawNavigator(void)
{
   int navX, navY, nID, newWidth, newHeight, nPos, nTemp;
   WORD nFrmID;
   CFrame *frm;
   String frmfile;
   navX = 45;
   navY = 5;
   nPos = 0;

   nID = 0;
   if (Navigator.nShowMode == SHOW_LOCAL)
   {
      nTemp = Navigator.nObjType == tile_ID ? sb->Position + 1 : sb->Position;
      while (nTemp)
      {
         nID ++;
         switch (Navigator.nObjType)
         {
            case tile_ID:
               if (pFrmSet->pFRM[Navigator.nObjType][nID - 1].bLocal)
                  nTemp --;
               break;
            case inven_ID:
               if (pProSet->pPRO[item_ID][nID].bLocal)
                  nTemp --;
               break;
            default:
               if (pProSet->pPRO[Navigator.nObjType][nID].bLocal)
                  nTemp --;
         }
      }
   }
   else
      nID = sb->Position;

   memset(Navigator.nNavID, -1, 32);
   HDC dc;
   RECT rcs, rcd;
   dds2Nav->GetDC (&dc);
   PatBlt(dc, 0, 0, imgObj->Width,imgObj->Height, BLACKNESS);
   dds2Nav->ReleaseDC (dc);
   while (nID <= Navigator.nMaxID && ((navX + 80) < 735))
   {
      if (Navigator.nShowMode == SHOW_LOCAL)
      {
         if ((Navigator.nObjType == tile_ID &&
             !pFrmSet->pFRM[Navigator.nObjType][nID].bLocal) ||
             (Navigator.nObjType == inven_ID &&
             !pProSet->pPRO[item_ID][nID].bLocal) ||
             (Navigator.nObjType != tile_ID &&  Navigator.nObjType != inven_ID &&
             !pProSet->pPRO[Navigator.nObjType][nID].bLocal) )
         {
            nID++;
            continue;
         }
      }
      switch (Navigator.nObjType)
      {
         case tile_ID:
            nFrmID = nID;
            break;
         case inven_ID :
            pProSet->LoadPRO(item_ID, nID, false);
            nFrmID = pProSet->pPRO[item_ID][nID].GetInvFrmID();
            break;
         default :
            pProSet->LoadPRO(Navigator.nObjType, nID, false);
            nFrmID = pProSet->pPRO[Navigator.nObjType][nID].GetFrmID();
      }
      if (nFrmID != 0xFFFF)
      {
         frmfile = pUtil->GetFRMFileName(Navigator.nObjType,
                       pLstFiles->pFRMlst[Navigator.nObjType]->Strings[nFrmID]);
         if (Navigator.nObjType == critter_ID)
         {
            pFrmSet->GetCritterFName(&frmfile,
                  pProSet->pPRO[Navigator.nObjType][nID].GetFrmIDDW(), &nFrmID);
         }
         pFrmSet->LoadFRM(frmfile, Navigator.nObjType, nFrmID, false);
         frm = &pFrmSet->pFRM[Navigator.nObjType][nFrmID];
      }
      else
      {
         frmfile = pUtil->GetFRMFileName(item_ID,
                                       pLstFiles->pFRMlst[item_ID]->Strings[0]);
         pFrmSet->LoadFRM(frmfile, item_ID, 0, false);
         frm = &pFrmSet->pFRM[item_ID][0];
      }
      if (Navigator.nObjType == inven_ID)
      {
         pProSet->LoadPRO(item_ID, nID, false);
         nFrmID = pProSet->pPRO[item_ID][nID].GetFrmID();
         frmfile = pUtil->GetFRMFileName(item_ID,
                                  pLstFiles->pFRMlst[item_ID]->Strings[nFrmID]);
         pFrmSet->LoadFRM(frmfile, item_ID, nFrmID, false);
      }
      newWidth = frm->GetWi(0, 0);
      newWidth = newWidth > NAV_SIZE_X ? NAV_SIZE_X : newWidth;
      newHeight = frm->GetHe(0, 0);
      newHeight = newHeight > NAV_SIZE_Y ? NAV_SIZE_Y : newHeight;

      double ar; // aspect ratio of the frame
      if (newWidth && newHeight)
      {
         double ax = (double)frm->GetWi(0,0) / newWidth,
                ay = (double)frm->GetHe(0,0) / newHeight;
         ar = max (ax, ay);
         if (ar > .001)
         {
            newWidth = (int)frm->GetWi (0,0) / ar;
            newHeight = (int)frm->GetHe (0,0) / ar;
         }
      }

      int x = navX - (newWidth >> 1);
      int y = 35 - (newHeight >> 1);
      rcs = Rect (0, 0, frm->GetWi(0, 0), frm->GetHe(0, 0));
      rcd = Bounds (x, y, newWidth, newHeight);
      dds2Nav->Blt(&rcd, frm->GetBMP(), &rcs, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

      Navigator.nNavID[nPos] = nID;
      if (Navigator.nSelID == nID)
         TransBltMask (frm, imgObj, 0, 0, x, y, dds2Nav, newWidth, newHeight);

      String id_str =
                     "ID: " + String(Navigator.nObjType == tile_ID ? nID : nID);
      dds2Nav->GetDC(&dc);
      SelectObject (dc, imgObj->Font->Handle);
      SelectObject (dc, Brush->Handle);
      SetBkMode (dc, TRANSPARENT);
      SetTextColor (dc,
                     ColorToRGB( (Navigator.nSelID==nID) ? clYellow : clWhite));
      SetBkColor (dc, ColorToRGB(clBlack));
      TextOutA (dc, navX - 40, 66, id_str.c_str(), id_str.Length());
      dds2Nav->ReleaseDC(dc);

      nID++;
      navX += 85;
      nPos++;
   }
   imgObj->Repaint();
}
//---------------------------------------------------------------------------
void TfrmEnv::ResetInventoryInfo(void)
{
   Inventory.pObj = NULL;
   Inventory.pChildObj[0] = NULL;
   Inventory.pChildObj[1] = NULL;
   Inventory.pChildObj[2] = NULL;
   Inventory.nItemNum = -1;
   Inventory.nInvStartItem = -1;
   btnChange->Enabled = false;
//   btnAdd->Enabled = false;
   btnRemove->Enabled = false;
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawInventory(void)
{
   CFrame *frm;
   HDC dc;
   dds2Inv->GetDC (&dc);
   PatBlt(dc, 0, 0, imgInv->Width, imgInv->Height, BLACKNESS); //BLACKNESS
   dds2Inv->ReleaseDC (dc);
   imgInv->Repaint();

//   Inventory.pObj = NULL;

   if (pObjSet->nSelected != 1) return;

   DWORD nObjNum, nChildCount = 0;
   WORD nID, nFrmID, hexX, hexY;
   BYTE nType, *pObj;
   String frmfile;
   int nPos, navY = 0;
   int newWidth, newHeight;
   RECT rcs, rcd;

   nPos = 0;
   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      if (pObjSet->ObjSelected())
      {
         Inventory.pObj = pObj;
         if (!pObjSet->ChildCount()) break;
         sbINV->Max = pObjSet->ChildCount() > 3 ? pObjSet->ChildCount() : 3;
         sbINV->Min = 3;
         sbINV->Enabled = (sbINV->Max > 3);

         pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
         for (int nP = 0; nP < Inventory.nInvStartItem; nP++)
            pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
         while (pObjSet->ObjIsChild() && nPos < 3)
         {
            pObjSet->GetObjType(&nType, &nID);
            pProSet->LoadPRO(item_ID, nID, false);
            nFrmID = pProSet->pPRO[item_ID][nID].GetInvFrmID();
            if (nFrmID != 0xFFFF)
            {
               frmfile = pUtil->GetFRMFileName(inven_ID,
                                 pLstFiles->pFRMlst[inven_ID]->Strings[nFrmID]);
               pFrmSet->LoadFRM(frmfile, inven_ID, nFrmID, true);
               frm = &pFrmSet->pFRM[inven_ID][nFrmID];
            }
            else
            {
               frmfile = pUtil->GetFRMFileName(item_ID,
                                       pLstFiles->pFRMlst[item_ID]->Strings[0]);
               pFrmSet->LoadFRM(frmfile, item_ID, 0, false);
               frm = &pFrmSet->pFRM[item_ID][0];
            }
            newWidth = frm->GetWi(0, 0);
            newWidth = newWidth > NAV_SIZE_X ? NAV_SIZE_X : newWidth;
            newHeight = frm->GetHe(0, 0);
            newHeight = newHeight > NAV_SIZE_Y ? NAV_SIZE_Y : newHeight;

            double ar; // aspect ratio of the frame
            if (newWidth && newHeight)
            {
               double ax = (double)frm->GetWi(0,0) / newWidth,
                      ay = (double)frm->GetHe(0,0) / newHeight;
               ar = max (ax, ay);
               if (ar > .001)
               {
                  newWidth = (int)frm->GetWi (0,0) / ar;
                  newHeight = (int)frm->GetHe (0,0) / ar;
               }
            }
            int x = 42 - (newWidth >> 1);
            int y = navY + (31 - (newHeight >> 1));
            rcs = Rect (0,0, frm->GetWi(0,0), frm->GetHe(0,0));
            rcd = Bounds (x, y, newWidth, newHeight);
            dds2Inv->Blt(&rcd, frm->GetBMP(), &rcs,
                                               DDBLT_WAIT | DDBLT_KEYSRC, NULL);
            if (Inventory.nItemNum == nPos)
            {
               dds2Inv->GetDC(&dc);
               Canvas->Brush->Color = clLime; //
               Canvas->Brush->Style = bsSolid; //
               RECT rc = Rect(0, navY, imgInv->Width, navY + NAV_SIZE_Y);
               FrameRect (dc, &rc, Canvas->Brush->Handle);
               dds2Inv->ReleaseDC (dc);
            }
            Inventory.pChildObj[nPos] = pObj;

            String count_str = "x" + String(pObjSet->GetCountInObj());
            dds2Inv->GetDC(&dc);
            SelectObject (dc, imgInv->Font->Handle);
            SelectObject (dc, Brush->Handle);
            SetBkMode (dc, TRANSPARENT);
            SetTextColor (dc, ColorToRGB(clLime));
            SetBkColor (dc, ColorToRGB(clBlack));
            TextOutA (dc, 5, navY + 5, count_str.c_str(), count_str.Length());
            dds2Inv->ReleaseDC(dc);


            navY += NAV_SIZE_Y;
            nPos ++;

            pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
         }
         break;
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
   imgInv->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::btnChangeClick(TObject *Sender)
{
   if (Inventory.nItemNum == -1) return;
   pObjSet->SetObj(Inventory.pChildObj[Inventory.nItemNum]);
   int nOld = pObjSet->GetCountInObj();
   DWORD nNew = fmChange->ChangeValue(nOld);
   if (nNew != nOld)
   {
      pObjSet->SetCountInObj(nNew);
      RedrawInventory();
      SetButtonSave(true);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::TransBltFrm(CFrame* frm, TControl* win, short nDir, short nFrame,
                                        int x, int y, LPDIRECTDRAWSURFACE7 dds2)
{
   RECT rcs, rcd;
   rcs = Bounds(frm->GetSprX(nDir, nFrame), frm->GetSprY(nDir, nFrame),
                            frm->GetWi(nDir, nFrame), frm->GetHe(nDir, nFrame));
   rcd = Bounds(x, y, frm->GetWi(nDir, nFrame), frm->GetHe(nDir, nFrame));
   HRESULT res = dds2->Blt(&rcd, frm->GetBMP(), &rcs,
                                               DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}
//---------------------------------------------------------------------------
void TfrmEnv::TransBltMask(CFrame* frm, TControl* win, short nDir, short nFrame,
                 int x, int y, LPDIRECTDRAWSURFACE7 dds2, int width, int height)
{
   RECT rcs, rcd;
   rcs = Bounds(0, 0, width, height);
   rcd = Bounds(x, y, width, height);
   frm->InitBorder(nDir, nFrame, width, height);
   HRESULT res = dds2->Blt(&rcd, frm->GetBorder(), &rcs,
                                               DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}
//---------------------------------------------------------------------------
void TfrmEnv::TransBltTileRegion(LPDIRECTDRAWSURFACE7 dds2,
                                 int TileX1, int TileY1, int TileX2, int TileY2)
{
      int v1x, v1y, v2x, v2y, v3x, v3y, v4x, v4y;
      TileX1 = TileX1 % 100;
      TileY1 = TileY1 % 100;
      TileX2 = TileX2 % 100;
      TileY2 = TileY2 % 100;
      pUtil->GetTileWorldCoord(TileX1 , TileY1, &v1x, &v1y);
      v1x += TILE_UP_CORNER_X - WorldX;
      v1y += TILE_UP_CORNER_Y - WorldY;
      pUtil->GetTileWorldCoord(TileX1, TileY2, &v2x, &v2y);
      v2x += TILE_RIGHT_CORNER_X - WorldX;
      v2y += TILE_RIGHT_CORNER_Y - WorldY;
      pUtil->GetTileWorldCoord(TileX2, TileY2, &v3x, &v3y);
      v3x += TILE_DOWN_CORNER_X - WorldX;
      v3y += TILE_DOWN_CORNER_Y - WorldY;
      pUtil->GetTileWorldCoord(TileX2, TileY1, &v4x, &v4y);
      v4x += TILE_LEFT_CORNER_X - WorldX;
      v4y += TILE_LEFT_CORNER_Y - WorldY;

      Windows::TPoint points[4];
      points[0] = Point(v1x, v1y);
      points[1] = Point(v2x, v2y);
      points[2] = Point(v3x, v3y);
      points[3] = Point(v4x, v4y);

      Canvas->Brush->Style = bsClear; //
      Canvas->Pen->Color = clWhite; //
      HDC dc;
      dds2->GetDC(&dc);
      HPEN pen = Canvas->Pen->Handle;
      HPEN oldpen = SelectObject(dc, pen);
      HBRUSH brush = Canvas->Brush->Handle;
      HBRUSH oldbrush = SelectObject(dc, brush);
      Polygon(dc, points, 4);
      SelectObject(dc, oldpen);
      SelectObject(dc, oldbrush);
      dds2->ReleaseDC (dc);
}
//---------------------------------------------------------------------------
void TfrmEnv::AttachDDraw(TControl *win, LPDIRECTDRAWSURFACE7 *dds, int primary)
{
   HRESULT res;
   DDSURFACEDESC2 ddSurfaceDesc;
   if (primary)
   {
      ZeroMemory(&ddSurfaceDesc, sizeof(ddSurfaceDesc));
      ddSurfaceDesc.dwSize = sizeof(ddSurfaceDesc);
      ddSurfaceDesc.dwFlags = DDSD_CAPS;
      ddSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
      res = frmMDI->pDD->CreateSurface (&ddSurfaceDesc, dds, NULL);
      res = frmMDI->pDD->CreateClipper (0, &ddcMap, NULL);
      res = ddcMap->SetHWnd (0, imgMap->Handle);
      res = frmMDI->pDD->CreateClipper (0, &ddcNav, NULL);
      res = ddcNav->SetHWnd (0, imgObj->Handle);
      res = frmMDI->pDD->CreateClipper (0, &ddcInv, NULL);
      res = ddcInv->SetHWnd (0, imgInv->Handle);
   }
   else
   {
      ZeroMemory(&ddSurfaceDesc, sizeof(ddSurfaceDesc));
      ddSurfaceDesc.dwSize = sizeof(ddSurfaceDesc);
      ddSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
      ddSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
      ddSurfaceDesc.dwWidth = win->Width;
      ddSurfaceDesc.dwHeight = win->Height;
      res = frmMDI->pDD->CreateSurface(&ddSurfaceDesc, dds, NULL);

      struct
      {
	 RGNDATAHEADER rdh;
	 RECT rect;
      } clip_rgn;
      memset(&clip_rgn, 0, sizeof(clip_rgn));
      clip_rgn.rect = win->ClientRect;
      clip_rgn.rdh.dwSize = sizeof(RGNDATAHEADER);
      clip_rgn.rdh.iType = RDH_RECTANGLES;
      clip_rgn.rdh.nCount = 1;
      clip_rgn.rdh.nRgnSize = sizeof(RECT);
      clip_rgn.rdh.rcBound = clip_rgn.rect;

      res = frmMDI->pDD->CreateClipper (0, &ddc, NULL);
      res = ddc->SetClipList ((LPRGNDATA)&clip_rgn, 0);
      res = (*dds)->SetClipper (ddc);

      DDCOLORKEY ck;
      ck.dwColorSpaceLowValue = 0;
      ck.dwColorSpaceHighValue = 0;
      res = (*dds)->SetColorKey(DDCKEY_SRCBLT, &ck);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawMap(bool StaticRedraw)
{
   if ((OldWorldX == WorldX) && (OldWorldY == WorldY) && !StaticRedraw) return;
   int w1 = 640;
   int h1 = 480;
   HDC dc;
   dds2Map->GetDC (&dc);
   PatBlt(dc, 0, 0, w1, h1, BLACKNESS);
   dds2Map->ReleaseDC (dc);
   if (bShowObj[floor_ID]) RedrawFloor();

   RedrawObjects();
   
   if (bShowObj[roof_ID]) RedrawRoof();

   if (bShowObj[EG_blockID] || bShowObj[SAI_blockID] ||
       bShowObj[wall_blockID] || bShowObj[obj_blockID] ||
       bShowObj[light_blockID] || bShowObj[scroll_blockID] ||
       bShowObj[obj_self_blockID])
          RedrawBlockers();

//   RedrawHex();
   if (have_sel)
   {
      dds2Map->GetDC(&dc);
      Canvas->Brush->Color = clAqua; //
      Canvas->Brush->Style = bsSolid; //
      RECT rc = Rect(min(downX, cursorX), min(downY, cursorY),
                     max(downX, cursorX), max(downY, cursorY));
      FrameRect (dc, &rc, Canvas->Brush->Handle);
      dds2Map->ReleaseDC (dc);
   }
   imgMap->Repaint();
   RedrawLocator();
   OldWorldX = WorldX;
   OldWorldY = WorldY;
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawFloor(void)
{
   int x, y, prev_xx, prev_yy;
   WORD TileId;
   int xx = 4752; //
   int yy = 0; //
   int w1 = 640;
   int h1 = 480;
   for (y = iLevel * 100; y < (iLevel + 1) * 100; y++)
   {
      prev_xx = xx;
      prev_yy = yy;
      for (x = 0; x < 100; x++)
      {
         TileId = pTileSet->GetFloorID(x, y);
         TileId &= 0x0FFF;
         if (TileId != 1) //несуществующие тайлы пола
         {
            if ((xx + pFrmSet->pFRM[tile_ID][TileId].GetWi(0, 0) > WorldX && xx < WorldX + w1) &&
               (yy + pFrmSet->pFRM[tile_ID][TileId].GetHe(0, 0) > WorldY && yy < WorldY + h1))
            {
               TransBltFrm(&pFrmSet->pFRM[tile_ID][TileId], imgMap, 0, 0,
                                             xx - WorldX, yy - WorldY, dds2Map);
            }
         }
         xx -= 48;
         yy += 12;
      }
      xx = prev_xx + 32;
      yy = prev_yy + 24;
   }
   if (pTileSet->dwSelection == floor_ID)
   {
      TransBltTileRegion(dds2Map,
                         pTileSet->SelectedFloorX1,
                         pTileSet->SelectedFloorY1,
                         pTileSet->SelectedFloorX2,
                         pTileSet->SelectedFloorY2);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawObjects(void)
{
   int w1 = 640;
   int h1 = 480;
   int X, Y;
   BYTE *pObj, nObjType, nProObjType;
   WORD nX, nY, nXX, nYY, nFrmID, nProID;
   DWORD nDir, nObjNum, nChildCount = 0;
   int nMapX, nMapY, nLevel;
   short xx, yy;

   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      pObjSet->GetObjFrm(&nObjType, &nFrmID);
      if (nObjType == critter_ID)
      {
         String filename = pUtil->GetFRMFileName(nObjType,
                                 pLstFiles->pFRMlst[nObjType]->Strings[nFrmID]);
         pFrmSet->GetCritterFName(&filename, pObjSet->GetObjFrmDW(), &nFrmID);
      }
      pObjSet->GetObjType(&nProObjType, &nProID);
      if (bShowObj[nObjType] && !pUtil->GetBlockType(nProObjType, nProID))
      {
         pObjSet->GetHexXY(&nX, &nY);
         pUtil->GetHexWorldCoord(nX, nY, &nMapX, &nMapY);
         pObjSet->GetDirection(&nDir);
//         pLog->LogX("Draw (" + String(nObjType) + ", " + String(nFrmID) + ") : (" +
//                     String(pFrmSet->pFRM[nObjType][nFrmID].nDirTotal) + ", " +
//                     String(pFrmSet->pFRM[nObjType][nFrmID].nFrames) + ") " +
//                     "dir:" + String(nDir) + " filename: " +
//                     pFrmSet->pFRM[nObjType][nFrmID].FileName);

         X = nMapX - WorldX -
             (pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) >> 1) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffX(nDir) +
             pObjSet->GetXX();
         Y = nMapY - WorldY -
             pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffY(nDir) +
             pObjSet->GetYY();
         if ((X + pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) > 0 && X < w1)
            && (Y + pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0) > 0 && Y < h1))
         {
	     TransBltFrm(&(pFrmSet->pFRM[nObjType][nFrmID]), imgMap, nDir, 0,
                                   X, Y, dds2Map);
             if (pObjSet->ObjSelected())
             {
		TransBltMask(&pFrmSet->pFRM[nObjType][nFrmID], imgMap, nDir, 0,
                                X, Y, dds2Map,
                                pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0),
                                pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0));
             }
         }
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawRoof(void)
{
   int x, y, prev_xx, prev_yy;
   WORD TileId;
   int xx = 4752; //4752              TileX = 0
   int yy = -96; //0                    TileY = 0
   int w1 = 640;
   int h1 = 480;
   for (y = iLevel * 100; y < (iLevel + 1) * 100; y++)
   {
      prev_xx = xx;
      prev_yy = yy;
      for (x = 0; x < 100; x++)
      {
         TileId = pTileSet->GetRoofID(x, y);
         TileId &= 0x0FFF;
         if (TileId != 1) {//несуществующие тайлы потолка
            if ((xx + pFrmSet->pFRM[tile_ID][TileId].GetWi(0, 0) > WorldX && xx < WorldX + w1) &&
               (yy + pFrmSet->pFRM[tile_ID][TileId].GetHe(0, 0) > WorldY && yy < WorldY + h1))
            {
               TransBltFrm(&pFrmSet->pFRM[tile_ID][TileId], imgMap, 0, 0,
                                             xx - WorldX, yy - WorldY, dds2Map);
            }
         }
         xx -= 48;
         yy += 12;
      }
      xx = prev_xx + 32;
      yy = prev_yy + 24;
   }
   if (pTileSet->dwSelection == roof_ID)
   {
      TransBltTileRegion(dds2Map,
                         pTileSet->SelectedRoofX1 - 2,
                         pTileSet->SelectedRoofY1 - 3,
                         pTileSet->SelectedRoofX2 - 2,
                         pTileSet->SelectedRoofY2 - 3);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawBlockers(void)
{
   int w1 = 640;
   int h1 = 480;
   int X, Y;
   BYTE *pObj, nObjType, nProObjType;
   WORD nX, nY, nXX, nYY, nFrmID, nProID;
   DWORD nObjNum, nChildCount = 0;
   int nMapX, nMapY, nLevel;
   short xx, yy;

   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      pObjSet->GetObjFrm(&nObjType, &nFrmID);
      pObjSet->GetObjType(&nProObjType, &nProID);
      int nBlockType = pUtil->GetBlockType(nProObjType, nProID);
      if (bShowObj[obj_self_blockID] && !(pObjSet->GetFlags() & 0x00000010))
         nBlockType = obj_self_blockID;
      if (nBlockType && bShowObj[nBlockType])
      {
         pUtil->GetBlockFrm(nBlockType, &nObjType, &nFrmID);
         pObjSet->GetHexXY(&nX, &nY);
         pUtil->GetHexWorldCoord(nX, nY, &nMapX, &nMapY);
         X = nMapX - WorldX -
             (pFrmSet->pFRM[nObjType][nFrmID].GetWi(0, 0) >> 1) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffX(0) +
             pObjSet->GetXX();
         Y = nMapY - WorldY -
             pFrmSet->pFRM[nObjType][nFrmID].GetHe(0, 0) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffY(0) +
             pObjSet->GetYY();
         if ((X + pFrmSet->pFRM[nObjType][nFrmID].GetWi(0, 0) > 0 && X < w1) &&
                (Y + pFrmSet->pFRM[nObjType][nFrmID].GetHe(0, 0) > 0 && Y < h1))
         {
            TransBltFrm(&pFrmSet->pFRM[nObjType][nFrmID], imgMap, 0, 0,
                                                                 X, Y, dds2Map);
            if (pObjSet->ObjSelected())
            {
               TransBltMask(&pFrmSet->pFRM[nObjType][nFrmID], imgMap, 0, 0,
                                   X, Y, dds2Map,
                                   pFrmSet->pFRM[nObjType][nFrmID].GetWi(0, 0),
                                   pFrmSet->pFRM[nObjType][nFrmID].GetHe(0, 0));
            }
         }
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawHex(void)
{
   int w1 = 640;
   int h1 = 480;
   int nMapX, nMapY;
   int x, y;
   for (y = 0; y < 200; y++)
   {
      for (x = 0; x < 200; x++)
      {
         pUtil->GetHexWorldCoord(x, y, &nMapX, &nMapY);
         if ((nMapX + 32 > WorldX && nMapX < WorldX + w1) &&
                                   (nMapY + 16 > WorldY && nMapY < WorldY + h1))
         {
	    TransBltFrm(&pFrmSet->pFRM[2][21], imgMap, 0, 0, 
                                  nMapX - WorldX, nMapY - WorldY, dds2Map);
         }
      }
   }
}
//---------------------------------------------------------------------------
void TfrmEnv::RedrawLocator(void)
{
   int x, y;
   pUtil->GetCursorTile(0, 0, WorldX, WorldY, &x, &y);
   if (x == OldX && y == OldY) return;
   BitBlt(imgMiniMap->Canvas->Handle, OldX, OldY, 21, 21,
                                  pBMPlocator->Canvas->Handle, 0, 0, SRCINVERT);
   x = 89 - x;
   y = y + (100 * iLevel);
   BitBlt(imgMiniMap->Canvas->Handle, x, y, 21, 21,
                                  pBMPlocator->Canvas->Handle, 0, 0, SRCINVERT);
   OldX = x;
   OldY = y;
   imgMiniMap->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::FormClose(TObject *Sender, TCloseAction &Action)
{
   Action = caNone;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   downX = X;
   downY = Y;
   pUtil->GetCursorHex(X, Y, WorldX, WorldY, &downHexX, &downHexY);
   pUtil->GetCursorTile(X, Y, WorldX, WorldY, &downTileX, &downTileY);
   switch (Button)
   {
      case mbLeft:
         switch (SelectMode)
         {
            case SELECT_NONE:
               Screen->Cursor = (TCursor)crHandTakeCursor;
               break;
            case SELECT_OBJ:
               ClearObjSelection(true);
               break;
         }
         mouseBLeft = true;
         break;
      case mbRight:
         mouseBRight = true;
         if (SelectMode == DRAW_ROOF)
         {
             frmMDI->btnselect1->Click();
             frmMDI->btnselect1->Down = true;
         }
         if (SelectMode == DRAW_OBJ)
         {
             frmMDI->btnselect2->Click();
             frmMDI->btnselect2->Down = true;
         }
         if (SelectMode == DRAW_FLOOR)
         {
             frmMDI->btnselect3->Click();
             frmMDI->btnselect3->Down = true;
         }
         break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMapClick(TObject *Sender)
{
   int TileX, TileY;
   int HexX, HexY;
   switch (SelectMode)
   {
      case SELECT_OBJ:
         if (upX != downX || upY != downY) return;
         SelectObjXY(downX, downY);
         RedrawMap(true);
         break;
      case DRAW_FLOOR:
         if (Navigator.nObjType != tile_ID || Navigator.nSelID == NONE_SELECTED)
            return;
         pUtil->GetCursorTile(downX, downY, WorldX, WorldY, &TileX, &TileY);
         pTileSet->SetFloor(TileX, TileY, iLevel, Navigator.nSelID);
         RedrawMap(true);
         SetButtonSave(true);
         break;
      case DRAW_ROOF:
         if (Navigator.nObjType != tile_ID || Navigator.nSelID == NONE_SELECTED)
            return;
         pUtil->GetCursorTile(downX, downY, WorldX, WorldY, &TileX, &TileY);
         pTileSet->SetRoof(TileX, TileY, iLevel, Navigator.nSelID);
         RedrawMap(true);
         SetButtonSave(true);
         break;
      case DRAW_OBJ:
         if (Navigator.nObjType == tile_ID || Navigator.nSelID == NONE_SELECTED)
            return;
         pUtil->GetCursorHex(downX, downY, WorldX, WorldY, &HexX, &HexY);
         BYTE ObjType = Navigator.nObjType == inven_ID ? item_ID : Navigator.nObjType;
         pObjSet->AppendObject(HexX, HexY, iLevel, ObjType, Navigator.nSelID);
         RedrawMap(true);
         SetButtonSave(true);
         break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMapMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   upX = X;
   upY = Y;
   switch (Button)
   {
      case mbLeft:
         mouseBLeft = false;
         break;
      case mbRight:
         mouseBRight = false;
   }
   switch (SelectMode)
   {
      case SELECT_NONE:
         Screen->Cursor = crDefault;
         break;
      case SELECT_OBJ:
         if (upX - downX && upY - downY)
         {
            have_sel = 0;
            RedrawMap(true);
         }
         else
            imgMapClick(Sender);
         break;
      case SELECT_FLOOR:
      case SELECT_ROOF:
         if (upX - downX && upY - downY)
         {
            tile_sel = 0;
            RedrawMap(true);
         }
         else
         {
            SelectTileRegion(SelectMode, downX, downY, downX, downY);
            RedrawMap(true);
         }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   pUtil->GetCursorTile(X, Y, WorldX, WorldY, &TileX, &TileY);
   panelTILE->Text = "TILE: (" + (String)TileX + ", " + TileY + ")";
   pUtil->GetCursorHex(X, Y, WorldX, WorldY, &HexX, &HexY);
   panelHEX->Text = "HEX: (" + (String)HexX + ", " + (String)HexY + ")";
   cursorX = X;
   cursorY = Y;
   switch (SelectMode)
   {
      case SELECT_NONE:
         if (mouseBLeft)
         {
            int changeX = X - downX;
            int changeY = Y - downY;
            WorldX -= changeX;
            WorldY -= changeY;
            RedrawMap(false);
            downX = X;
            downY = Y;
         }
         break;
      case SELECT_FLOOR:
      case SELECT_ROOF:
         if ((X - downX) && (Y - downY) && (mouseBLeft || mouseBRight))
         {
            SelectTileRegion(SelectMode, downX, downY, X, Y);
            RedrawMap(true);
            if (!tile_sel || prevX != X || prevY != Y)
            {
               tile_sel = 1;
               prevX = X;
               prevY = Y;
            }
         }
         break;
      case SELECT_OBJ:
         if ((X - downX) && (Y - downY) &&
             (X - prevX) && (Y - prevY) && (mouseBLeft || mouseBRight))
         {
            SelectObjRegion(min(downX, X) , min(downY, Y),
                            max(downX, X) , max(downY, Y));
            RedrawMap(true);

            if (!have_sel || prevX != X || prevY != Y)
            {
               have_sel = 1;
               prevX = X;
               prevY = Y;
            }
         }
         break;
      case DRAW_OBJ:
         if (Navigator.nSelID != -1 && Navigator.nObjType != tile_ID)
         {
            int HexX, HexY, objX, objY;
            BYTE ObjType;
            ObjType = Navigator.nObjType == inven_ID ? item_ID : Navigator.nObjType;
            WORD nFrmID = pProSet->pPRO[ObjType][Navigator.nSelID].GetFrmID();
            CFrame *frm = &pFrmSet->pFRM[ObjType][nFrmID];
            pUtil->GetCursorHex(X, Y, WorldX, WorldY, &HexX, &HexY);
            pUtil->GetHexWorldCoord(HexX, HexY, &objX, &objY);

            objX = objX - WorldX - (frm->GetWi(0, 0) >> 1) + frm->GetDoffX(0);
            objY = objY - WorldY - frm->GetHe(0, 0) + frm->GetDoffY(0);
            if (!(objX == OldShowObjX && objY == OldShowObjY))
            {
               RedrawMap(true);
               TransBltFrm(frm, imgMap, 0, 0, objX, objY, dds2Map);
               imgMap->Repaint();
               OldShowObjX = objX;
               OldShowObjY = objY;
            }
         }
         break;
      case DRAW_FLOOR:
      case DRAW_ROOF:
         if (Navigator.nSelID != -1 && Navigator.nObjType == tile_ID)
         {
            int TileX, TileY, objX, objY;
            CFrame *frm = &pFrmSet->pFRM[tile_ID][Navigator.nSelID];
            pUtil->GetCursorTile(X, Y, WorldX, WorldY, &TileX, &TileY);
            pUtil->GetTileWorldCoord(TileX, TileY, &objX, &objY);

            if (!(objX == OldShowObjX && objY == OldShowObjY))
            {
               RedrawMap(true);
               TransBltFrm(frm, imgMap, 0, 0, objX - WorldX, objY - WorldY, dds2Map);
               imgMap->Repaint();
               OldShowObjX = objX;
               OldShowObjY = objY;
            }
         }
         break;
      case MOVE_OBJ:
         int offsetHexX = HexX - downHexX;
         int offsetHexY = HexY - downHexY;
         if ((offsetHexX || offsetHexY) && mouseBLeft && pObjSet->nSelected)
         {
            MoveSelectedObjects(offsetHexX, offsetHexY);
            RedrawMap(true);
            downHexX = HexX;
            downHexY = HexY;
         }
         break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMiniMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Y / 100 >= pMap->Levels) return;
   pUtil->GetWorldCoord(X, Y, &WorldX, &WorldY, &iLevel);
   switch (Button)
   {
      case mbLeft:
         mouseBLeft2 = true;
         break;
      case mbRight:
         mouseBRight2 = true;
         break;
   }
   RedrawMap(false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMiniMapMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   switch (Button)
   {
      case mbLeft:
         mouseBLeft2 = false;
         break;
      case mbRight:
         mouseBRight2 = false;
         break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgMiniMapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   X = (X <= 99 ? X : 99) * (X >= 0);
   Y = (Y <= 299 ? Y : 299) * (Y >= 0);
   panelTILE->Text =
                  "TILE: (" + (String)(99 - X) + ", " + (String)(Y % 100) + ")";
   if (mouseBLeft2)
      imgMiniMapMouseDown(Sender, mbRight, Shift, X, Y);
}
//---------------------------------------------------------------------------
void TfrmEnv::SelectObjXY(int X, int Y)
{
   DWORD nObjNum, nChildCount = 0;
   WORD nFrmID, nProID, hexX, hexY;
   BYTE nObjType, nProObjType, *pObj, *pSelObj;
   int nMapX, nMapY, objX, objY;
   DWORD nDir;

   ClearObjSelection(false);
   pSelObj = NULL;
   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      pObjSet->GetObjFrm(&nObjType, &nFrmID);
      if (nObjType == critter_ID)
      {
         String filename = pUtil->GetFRMFileName(nObjType,
                                 pLstFiles->pFRMlst[nObjType]->Strings[nFrmID]);
         pFrmSet->GetCritterFName(&filename, pObjSet->GetObjFrmDW(), &nFrmID);
      }
      pObjSet->GetObjType(&nProObjType, &nProID);
      pObjSet->GetDirection(&nDir);
      int nBlockType = pUtil->GetBlockType(nProObjType, nProID);
      if ((!nBlockType && bShowObj[nProObjType]) ||
          (nBlockType && bShowObj[nBlockType]))
      {
         pUtil->GetBlockFrm(nBlockType, &nObjType, &nFrmID);
         pObjSet->GetHexXY(&hexX, &hexY);
         pUtil->GetHexWorldCoord(hexX, hexY, &nMapX, &nMapY);
         objX = nMapX - WorldX -
             (pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) >> 1) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffX(nDir) +
             pObjSet->GetXX();
         objY = nMapY - WorldY -
             pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffY(nDir) +
             pObjSet->GetYY();
         if (X >= objX &&
             X <= objX + pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) &&
             Y >= objY &&
             Y <= objY + pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0))
         {
            if (pFrmSet->pFRM[nObjType][nFrmID].GetPixel(X - objX, Y - objY, nDir, 0))
               pSelObj = pObj;
         }
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
   if (pSelObj)
   {
      pObjSet->SetObj(pSelObj);
      pObjSet->SelectObj();
   }
   SelectionChanged();
}
//---------------------------------------------------------------------------
bool TfrmEnv::SelectObjRegion(int X, int Y, int X2, int Y2)
{
   DWORD nObjNum, nChildCount = 0;
   WORD nFrmID, nProID, hexX, hexY;
   BYTE nObjType, nProObjType, *pObj;
   int nMapX, nMapY, objX, objY;
   DWORD nDir;

   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      pObjSet->GetObjFrm(&nObjType, &nFrmID);
      if (nObjType == critter_ID)
      {
         String filename = pUtil->GetFRMFileName(nObjType,
                                 pLstFiles->pFRMlst[nObjType]->Strings[nFrmID]);
         pFrmSet->GetCritterFName(&filename, pObjSet->GetObjFrmDW(), &nFrmID);
      }
      pObjSet->GetObjType(&nProObjType, &nProID);
      pObjSet->GetDirection(&nDir);
      int nBlockType = pUtil->GetBlockType(nProObjType, nProID);
      if ((!nBlockType && bShowObj[nProObjType]) ||
          (nBlockType && bShowObj[nBlockType]))
      {
         pUtil->GetBlockFrm(nBlockType, &nObjType, &nFrmID);
         pObjSet->GetHexXY(&hexX, &hexY);
         pUtil->GetHexWorldCoord(hexX, hexY, &nMapX, &nMapY);
         objX = nMapX - WorldX -
             (pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) >> 1) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffX(nDir) +
             pObjSet->GetXX();
         objY = nMapY - WorldY -
             pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0) +
             pFrmSet->pFRM[nObjType][nFrmID].GetDoffY(nDir) +
             pObjSet->GetYY();
         if ((objX + pFrmSet->pFRM[nObjType][nFrmID].GetWi(nDir, 0) > X &&
               objX < X2) &&
             (objY + pFrmSet->pFRM[nObjType][nFrmID].GetHe(nDir, 0) > Y &&
               objY < Y2))
         {
            pObjSet->SelectObj();
         }
         else
         {
            pObjSet->DeselectObj();
         }
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
   SelectionChanged();
   return true;
}
//---------------------------------------------------------------------------
void TfrmEnv::SelectionChanged(void)
{
   frmProp->ListRefresh();
   ResetInventoryInfo();
   RedrawInventory();
   panelObjSelected->Text = "Selected: " + (String)pObjSet->nSelected;
   btnAdd->Enabled =
        ((Navigator.nObjType == inven_ID || Navigator.nObjType == item_ID) &&
         pObjSet->nSelected == 1 && Navigator.nSelID != 0xFFFFFFFF);
}
//---------------------------------------------------------------------------
bool TfrmEnv::SelectTileRegion(int TileMode, int X, int Y, int X2, int Y2)
{
   int TileX1, TileY1, TileX2, TileY2;
   pUtil->GetCursorTile(X, Y, WorldX, WorldY, &TileX1, &TileY1);
   pUtil->GetCursorTile(X2, Y2, WorldX, WorldY, &TileX2, &TileY2);
   TileY1 = (iLevel * 100) + TileY1;
   TileY2 = (iLevel * 100) + TileY2;
   pTileSet->SelectTiles(TileMode, iLevel,
                         min(TileX1, TileX2), min(TileY1, TileY2),
                         max(TileX1, TileX2), max(TileY1, TileY2));
}
//---------------------------------------------------------------------------
void TfrmEnv::ClearSelection(void)
{
   ClearFloorSelection(false);
   ClearRoofSelection(false);
   ClearObjSelection(false);
   RedrawMap(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::ClearFloorSelection(bool NeedRedrawMap)
{
   pTileSet->ClearFloorSelection();
   if (NeedRedrawMap) RedrawMap(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::ClearRoofSelection(bool NeedRedrawMap)
{
   pTileSet->ClearRoofSelection();
   if (NeedRedrawMap) RedrawMap(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::ClearObjSelection(bool NeedRedrawMap)
{
   pObjSet->ClearSelection();
//   frmProp->ListRefresh();
//   RedrawInventory();
   SelectionChanged();
   if (NeedRedrawMap) RedrawMap(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::MoveSelectedObjects(int offsetHexX, int offsetHexY)
{
   DWORD nObjNum, nChildCount = 0;
   BYTE nObjType, nProObjType, *pObj;
   WORD objHexX, objHexY;
   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      if (pObjSet->ObjSelected() == OBJ_SELECTED)
      {
         pObjSet->GetHexXY(&objHexX, &objHexY);
         objHexX += offsetHexX;
         objHexY += offsetHexY;
         pObjSet->SetHexXY(objHexX, objHexY);
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
   SetButtonSave(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::RotateSelectedObjects(int direction)
{
   DWORD nDir, nObjNum, nChildCount = 0;
   WORD nProID, nFrmID, nMaxDir;
   BYTE nProObjType, nFrmType, *pObj;
   pObj = pObjSet->GetFirstObj(&nObjNum, iLevel);
   while (pObj)
   {
      if (pObjSet->ObjSelected() == OBJ_SELECTED)
      {
         pObjSet->GetObjType(&nProObjType, &nProID);
         pObjSet->GetDirection(&nDir);
         pObjSet->GetObjFrm(&nFrmType, &nFrmID);
         if (nFrmType == critter_ID)
         {
            String filename = pUtil->GetFRMFileName(nProObjType,
                               pLstFiles->pFRMlst[nProObjType]->Strings[nFrmID]);
            pFrmSet->GetCritterFName(&filename, pObjSet->GetObjFrmDW(), &nFrmID);
         }
         nMaxDir = pFrmSet->pFRM[nFrmType][nFrmID].nDirTotal;
         if (nMaxDir)
            if (direction == ROTATE_CW)
               pObjSet->SetDirection(++nDir == nMaxDir ? 0 : nDir);
            else
               pObjSet->SetDirection((int)--nDir < 0 ? nMaxDir : nDir);
      }
      pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, iLevel);
   }
   RedrawMap(true);
   SetButtonSave(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::popupMapPopup(TObject *Sender)
{
   piProperties->Enabled = pObjSet->nSelected ? true : false;
   piDelete->Enabled =
     pObjSet->nSelected | pTileSet->dwSelection != NONE_SELECTED ? true : false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::piDeleteClick(TObject *Sender)
{
   if (pTileSet->dwSelection == floor_ID)
   {
      pTileSet->SetFloorRegion(pTileSet->SelectedFloorX1,
                               pTileSet->SelectedFloorY1,
                               pTileSet->SelectedFloorX2,
                               pTileSet->SelectedFloorY2, 1);
   }
   if (pTileSet->dwSelection == roof_ID)
   {
      pTileSet->SetRoofRegion(pTileSet->SelectedRoofX1,
                              pTileSet->SelectedRoofY1,
                              pTileSet->SelectedRoofX2,
                              pTileSet->SelectedRoofY2, 1);
   }
   if (pObjSet->nSelected)
   {
      pObjSet->DeleteSelected();
   }
   SelectionChanged();
   RedrawMap(true);
   SetButtonSave(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::piPropertiesClick(TObject *Sender)
{
   frmProp->ShowProperties();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::tabcChange(TObject *Sender)
{
   PrepareNavigator(tabc->TabIndex);
   RedrawNavigator();
   btnAdd->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgObjMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int nPos = X / 85;
   nPos = nPos > 7 ? 7 : nPos;
   if (Navigator.nNavID[nPos] == -1) return;
   Navigator.nSelID = Navigator.nNavID[nPos];
   if (Navigator.nObjType != tile_ID)
   {
      BYTE ObjType = Navigator.nObjType == inven_ID ? item_ID : Navigator.nObjType;
      DWORD nMsgID = pProSet->pPRO[ObjType][Navigator.nSelID].GetMsgID();
      panelMSG->Text = pMsg->GetMSG(ObjType, nMsgID);
   }
   else
   {
      panelMSG->Text = "";
      switch (pTileSet->dwSelection)
      {
         case floor_ID:
            pTileSet->SetFloorRegion(pTileSet->SelectedFloorX1,
                                     pTileSet->SelectedFloorY1,
                                     pTileSet->SelectedFloorX2,
                                     pTileSet->SelectedFloorY2,
                                     Navigator.nSelID);
            RedrawMap(true);
            break;
         case roof_ID:
            pTileSet->SetRoofRegion(pTileSet->SelectedRoofX1,
                                    pTileSet->SelectedRoofY1,
                                    pTileSet->SelectedRoofX2,
                                    pTileSet->SelectedRoofY2,
                                    Navigator.nSelID);
            RedrawMap(true);
            break;
      }
   }
   btnAdd->Enabled =
        ((Navigator.nObjType == inven_ID || Navigator.nObjType == item_ID) &&
         pObjSet->nSelected == 1 && Navigator.nSelID != 0xFFFFFFFF);
   RedrawNavigator();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::imgInvMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int nPos = Y / NAV_SIZE_Y;
   Inventory.nItemNum = nPos;
   RedrawInventory();
   btnChange->Enabled = (Inventory.pChildObj[nPos] != NULL);
   btnRemove->Enabled = (Inventory.pChildObj[nPos] != NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   switch(Key)
   {
      case VK_UP:
         WorldY -= 32;
         RedrawMap(false);
         break;
      case VK_DOWN:
         WorldY += 32;
         RedrawMap(false);
         break;
      case VK_LEFT:
         if (Shift.Contains(ssCtrl) && pObjSet->nSelected)
         {
            RotateSelectedObjects(ROTATE_CCW);
            break;
         }
         WorldX -= 32;
         RedrawMap(false);
         break;
      case VK_RIGHT:
         if (Shift.Contains(ssCtrl) && pObjSet->nSelected)
         {
            RotateSelectedObjects(ROTATE_CW);
            break;
         }
         WorldX += 32;
         RedrawMap(false);
         break;
      case VK_RETURN:
         if (Shift.Contains(ssAlt))
            frmProp->ShowProperties();
         break;
      case VK_DELETE:
         if (pObjSet->nSelected)
            piDeleteClick(Sender);
         break;
      case VK_F5:
         if (pObjSet->nSelected)
            LogSelected();
         break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::btnWorldClick(TObject *Sender)
{
   Navigator.nShowMode = SHOW_WORLD;
   PrepareNavigator(tabc->TabIndex);
   RedrawNavigator();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::btnLocalClick(TObject *Sender)
{
   Navigator.nShowMode = SHOW_LOCAL;
   PrepareNavigator(tabc->TabIndex);
   RedrawNavigator();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::sbScroll(TObject *Sender, TScrollCode ScrollCode,
      int &ScrollPos)
{
   RedrawNavigator();
}
//---------------------------------------------------------------------------
void TfrmEnv::RepaintDDrawWindow(TWinControl *win, LPDIRECTDRAWSURFACE7 dds,
                           LPDIRECTDRAWSURFACE7 dds2, LPDIRECTDRAWCLIPPER ddc)
{
   RECT rcs, rcd;
//   GetUpdateRect (win->Handle, &rcs, false);
   rcs = win->ClientRect;
   rcd = rcs;
   OffsetRect(&rcd, win->ClientOrigin.x, win->ClientOrigin.y);
   dds->SetClipper(ddc);
   HRESULT res = dds->Blt(&rcd, dds2, &rcs, DDBLT_ASYNC, NULL);
   if (res == DDERR_SURFACELOST)
   {
      if (dds->IsLost())
         res = dds->Restore();
      if (dds2->IsLost())
         res = dds2->Restore();
      res = dds->Blt(&rcd, dds2, &rcs, DDBLT_ASYNC, NULL);
   }
   ValidateRect (win->Handle, &rcs);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::NewMapWndProc(TMessage &Msg)
{
   switch (Msg.Msg)
   {
      case WM_PAINT:
         RepaintDDrawWindow(imgMap, dds, dds2Map, ddcMap);
         Msg.Result = 0;
         break;
      case CM_MOUSELEAVE:
         RedrawMap(true);
      default:
         OldMapWndProc(Msg);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::NewNavWndProc(TMessage &Msg)
{
   if (Msg.Msg == WM_PAINT)
   {
      RepaintDDrawWindow(imgObj, dds, dds2Nav, ddcNav);
      Msg.Result = 0;
   }
   else
   {
      OldNavWndProc(Msg);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::NewInvWndProc(TMessage &Msg)
{
   if (Msg.Msg == WM_PAINT)
   {
      RepaintDDrawWindow(imgInv, dds, dds2Inv, ddcInv);
      Msg.Result = 0;
   }
   else
   {
      OldInvWndProc(Msg);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::btnAddClick(TObject *Sender)
{
   BYTE ObjType = Navigator.nObjType == inven_ID ? item_ID : Navigator.nObjType;
   pObjSet->AppendChildObject(Inventory.pObj, iLevel, ObjType, Navigator.nSelID);
   ResetInventoryInfo();
   RedrawInventory();
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::btnRemoveClick(TObject *Sender)
{
   pObjSet->DeleteObject(Inventory.pChildObj[Inventory.nItemNum]);
   ResetInventoryInfo();
   RedrawInventory();
   SetButtonSave(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEnv::sbINVChange(TObject *Sender)
{
   Inventory.nInvStartItem = sbINV->Position - 3;
   RedrawInventory();
   SetButtonSave(true);
}
//---------------------------------------------------------------------------
void TfrmEnv::LogSelected(void)
{
   DWORD nObjNum, nLevel, nChildCount;
   BYTE nObjType;
   WORD nObjID, nX, nY;
   BYTE *l_pObj;
   nLevel = 0;
   do
   {
      l_pObj = pObjSet->GetFirstObj(&nObjNum, nLevel);
      nChildCount = 0;
      while (l_pObj)
      {
         if (pObjSet->ObjSelected())
         {
            pObjSet->GetObjType(&nObjType, &nObjID);
            pObjSet->GetHexXY(&nX, &nY);
            pLog->LogX("Object type=" + String(nObjType) + ", ID=" +
                       String(nObjID) + "[" + String(nX) + ", " + String(nY) +
                       "]");
         }
         l_pObj = pObjSet->GetNextObj(&nObjNum, &nChildCount, nLevel);
      }
   } while (++nLevel < pMap->Levels);
}
//---------------------------------------------------------------------------

