//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "mdi.h"
#include "main.h"
#include "properts.h"
#include "pbar.h"
#include "log.h"
#include "macros.h"
#include <ddraw.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMDI *frmMDI;
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//---------------------------------------------------------------------------
__fastcall TfrmMDI::TfrmMDI(TComponent* Owner)
        : TForm(Owner)
{
   pLog = NULL;
   frmEnv = NULL;
   frmPBar = NULL;
   pDD = NULL;
   pUtil = new CUtilites();
   pUtil->InitDirectories();
   pLog = new CLog(ExtractFilePath(Application->ExeName) + "\\mapper.log");
   this->OpenDialog1->InitialDir = pUtil->DataDir;
   Screen->Cursors[crHandCursor] = LoadCursor(HInstance, "hand");
   Screen->Cursors[crHandTakeCursor] = LoadCursor(HInstance, "handtake");
   Screen->Cursors[crCrossCursor] = LoadCursor(HInstance, "cross");
   Screen->Cursors[crMyPenCursor] = LoadCursor(HInstance, "mypen");
   Screen->Cursors[crMoveCursor] = LoadCursor(HInstance, "move");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::FormCreate(TObject *Sender)
{
   this->WindowState = wsMaximized;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::FormDestroy(TObject *Sender)
{
   if (pPal) delete pPal;
   if (pUtil) delete pUtil;
   if (pFrmSet) delete pFrmSet;
   if (pProSet) delete pProSet;
   if (pLstFiles) delete pLstFiles;
   if (pMsg) delete pMsg;
   if (pDD) pDD->Release();
   if (pLog) delete pLog;
}
//---------------------------------------------------------------------------
bool TfrmMDI::InitClasses(void)
{
   HRESULT res;
   if (!pPal) pPal = new CPal(); //passed
   if (pPal->lError)
   {
      delete pPal;
      pPal = NULL;
      pLog->LogX("Failed to init palette.");
      return false;
   }
   else
      pLog->LogX("Palette init ... OK.");
   if (!pLstFiles) pLstFiles = new CListFiles(); //passed
   if (pLstFiles->lError)
   {
      delete pLstFiles;
      pLstFiles = NULL;
      pLog->LogX("Failed to load LST files");
      return false;
   }
   else
      pLog->LogX("Load LST files ... OK.");

   frmProp = new TfrmProperties(this);
   frmProp->AssignValues();

   if (!pProSet) pProSet = new CProSet(); //passed
   if (pProSet->lError)
   {
      delete pProSet;
      pProSet = NULL;
      pLog->LogX("Failed to create PRO structure.");
      return false;
   }
   else
      pLog->LogX("Create PRO structure ... OK.");
   if (!pFrmSet) pFrmSet = new CFrmSet(); //
   if (pFrmSet->lError == true)
   {
      delete pFrmSet;
      pFrmSet = NULL;
      pLog->LogX("Failed to create FRM structure.");
      return false;
   }
   else
      pLog->LogX("Create FRM structure ... OK.");

   if (!pMsg) pMsg = new CMsg(); //
   if (pMsg->lError == true)
   {
      delete pMsg;
      pMsg = NULL;
      pLog->LogX("Failed to load MSG files.");
      return false;
   }
   else
      pLog->LogX("Load MSG files ... OK.");

   if (!pDD)
   {
      res = DirectDrawCreateEx(NULL, (void**)&pDD, IID_IDirectDraw7, NULL);
      if (res == DD_OK)
         pLog->LogX("DDraw create ... OK.");
      else
      {
         pLog->LogX("DDraw create failed.");
         pLog->LogX(pUtil->GetDxError((DWORD)res));
         return false;
      }
      res = pDD->SetCooperativeLevel (0, DDSCL_NORMAL);
      if (res == DD_OK)
         pLog->LogX("Set DirectX cooperative level ... OK.");
      else
      {
         pLog->LogX("Failed to set DirectX cooperative level");
         pLog->LogX(pUtil->GetDxError((DWORD)res));
         return false;
      }
   }
   return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnOpenClick(TObject *Sender)
{
   if (OpenDialog1->Execute())
   {
      if (frmEnv != NULL) delete frmEnv;
      if (!InitClasses())
      {
         Application->MessageBox("Internal error.\n"
                                 "See mapper.log for details", "Error", MB_OK);
         return;
      }
      pLog->LogX("Load map \"" + OpenDialog1->FileName + "\".");
      Application->CreateForm(__classid(TfrmEnv), &frmEnv);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnSaveClick(TObject *Sender)
{
   HANDLE h_out = CreateFile(OpenDialog1->FileName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
   frmEnv->ClearSelection();
   frmEnv->pMap->SaveToFile(h_out);
   frmEnv->pTileSet->SaveToFile(h_out);
   frmEnv->pObjSet->SaveToFile(h_out);
   CloseHandle(h_out);
   frmEnv->SetButtonSave(false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnSaveAsClick(TObject *Sender)
{
   if (SaveDialog1->Execute())
   {
      OpenDialog1->FileName = SaveDialog1->FileName;
      frmEnv->Caption = OpenDialog1->FileName;
      btnSaveClick(Sender);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnhandClick(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crHandCursor;
   frmEnv->SelectMode = SELECT_NONE;
}
//---------------------------------------------------------------------------
void TfrmMDI::OpenPBarForm(void)
{
   frmPBar = new TfrmPBar(this);
}
//---------------------------------------------------------------------------
void TfrmMDI::DeletePBarForm(void)
{
   delete frmPBar;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnFloorClick(TObject *Sender)
{
   frmEnv->bShowObj[floor_ID] = btnFloor->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnRoofClick(TObject *Sender)
{
   frmEnv->bShowObj[roof_ID] = btnRoof->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnItemsClick(TObject *Sender)
{
   frmEnv->bShowObj[item_ID] = btnItems->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnCrittersClick(TObject *Sender)
{
   frmEnv->bShowObj[critter_ID] = btnCritters->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnSceneryClick(TObject *Sender)
{
   frmEnv->bShowObj[scenery_ID] = btnScenery->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnWallsClick(TObject *Sender)
{
   frmEnv->bShowObj[wall_ID] = btnWalls->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnMiscClick(TObject *Sender)
{
   frmEnv->bShowObj[misc_ID] = btnMisc->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnEGBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[EG_blockID] = btnEGBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnScrollBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[scroll_blockID] = btnScrollBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnmoveClick(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crMoveCursor;
   frmEnv->SelectMode = MOVE_OBJ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnselect1Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crCrossCursor;
   frmEnv->SelectMode = SELECT_ROOF;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnselect2Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crCrossCursor;
   frmEnv->SelectMode = SELECT_OBJ;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnselect3Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crCrossCursor;
   frmEnv->SelectMode = SELECT_FLOOR;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnpen1Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crMyPenCursor;
   frmEnv->SelectMode = DRAW_FLOOR;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnpen2Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crMyPenCursor;
   frmEnv->SelectMode = DRAW_OBJ;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnpen3Click(TObject *Sender)
{
   frmEnv->imgMap->Cursor = (TCursor)crMyPenCursor;
   frmEnv->SelectMode = DRAW_ROOF;
   frmEnv->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnConfigClick(TObject *Sender)
{
   pUtil->MapperConfiguration();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnAboutClick(TObject *Sender)
{
   Application->MessageBox(
       "Mapper Version 0.98c (24.02.2003)\n"
       "Code by Dims <dims@hot.ee>\n"
       "DirectX support by ABel <abel@krasu.ru>\n"
       "Upped interface to DirectX 7 by Dims <dims@hot.ee>\n",
       "About mapper", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnSaiBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[SAI_blockID] = btnSaiBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnWallBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[wall_blockID] = btnWallBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnObjBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[obj_blockID] = btnObjBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnLightBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[light_blockID] = btnLightBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMDI::btnObjSelfBlockClick(TObject *Sender)
{
   frmEnv->bShowObj[obj_self_blockID] = btnObjSelfBlock->Down;
   frmEnv->RedrawMap(true);
}
//---------------------------------------------------------------------------

