//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "config.h"
#include "mdi.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmConfig *frmConfig;
//---------------------------------------------------------------------------
__fastcall TfrmConfig::TfrmConfig(TComponent* Owner)
        : TForm(Owner)
{
   pUtil = frmMDI->pUtil;
}
//---------------------------------------------------------------------------
void TfrmConfig::ShowDialog(void)
{
   pUtil = frmMDI->pUtil;
   this->edGameDir->Text = pUtil->GameDir;
   this->edDataDir->Text = pUtil->DataDir;
   this->HasChanged = false;
   InitDAT();
   this->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::btnBrowseClick(TObject *Sender)
{
   String fld;
   while (true)
   {
      fld = pUtil->GetFolder("Select Fallout directory");
      if (fld.IsEmpty()) return;
      if (!fld.IsPathDelimiter(fld.Length())) break;
      MessageBox(NULL, "Cannot be main directory", "Error", MB_OK);
   }
   edGameDir->Text = fld;
   InitDAT();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::btnBrowse2Click(TObject *Sender)
{
   String fld;
   while (true)
   {
      fld = pUtil->GetFolder("Select local data directory");
      if (fld.IsEmpty()) return;
      if (!fld.IsPathDelimiter(fld.Length())) break;
      MessageBox(NULL, "Cannot be main directory", "Error", MB_OK);
   }
   edDataDir->Text = fld;
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfig::btnOKClick(TObject *Sender)
{
   pUtil->GameDir = this->edGameDir->Text;
   pUtil->SetRegInfoS("\\SOFTWARE\\Dims\\mapper", "GamePath", pUtil->GameDir);
   pUtil->DataDir = this->edDataDir->Text;
   pUtil->SetRegInfoS("\\SOFTWARE\\Dims\\mapper", "DataPath", pUtil->DataDir);
   frmMDI->OpenDialog1->InitialDir = pUtil->DataDir;
}
//---------------------------------------------------------------------------
void TfrmConfig::InitDAT(void)
{
  TListItem  *ListItem;
  TSearchRec sr;
  int iAttributes = faAnyFile;
  this->lvDAT->Items->Clear();
  if (FindFirst(this->edGameDir->Text + "\\*.dat", iAttributes, sr) == 0)
  {
    do
    {
//      if ((sr.Attr & iAttributes) == sr.Attr)
//      {
        ListItem = this->lvDAT->Items->Add();
        ListItem->Caption = sr.Name;
        ListItem->SubItems->Add(IntToStr(sr.Size));
//      }
    } while (FindNext(sr) == 0);
    FindClose(sr);
  }
}
//---------------------------------------------------------------------------

