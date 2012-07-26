//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "properts.h"
#include "mdi.h"
#include "main.h"
#include "script.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProperties *frmProperties;
//------- ALL OBJECTS TYPE ----------
String sAllList[22] ={"[0x00] Unknown1", "[0x04] Object Position", "[0x08] X offset", "[0x0C] Y offset",
                          "[0x10] Unknown4", "[0x14] Unknown5", "[0x18] Frame Number",
                          "[0x1C] Object direction", "[0x20] Object FRM ID",
                          "[0x24] Unknown Flags", "[0x28] Level", "[0x2C] Proto ID", "[0x30] Unknown6",
                          "[0x34] Unknown7", "[0x38] Unknown8", "[0x3C] Unknown9",
                          "[0x40] PID of Map Scripts", "[0x44] Map script ID",
                          "[0x48] Sub Items", "[0x4C] Unknown10", "[0x50] Unknown11", "[0x54] Unknown12"};

String sList[17][10] =  {"", "", "", "", "", "", "", "", "", "",                //0
                         "", "", "", "", "", "", "", "", "", "",                //1
                         "", "", "", "", "", "", "", "", "", "",                //2
                         /*------- ITEMS :: WEAPON --------*/
                         "[0x58] Ammo count", "[0x5C] Ammo proto ID", "", "" ,"" ,"" ,"" ,""  //3
                         , "", "",
                         /*------- ITEMS :: AMMO ----------*/
                         "[0x58] Amount of ammo", "", "", "", "", "", "", "", "", "",  //4
                         /*------- ITEMS :: MISC ----------*/
                         "[0x58] Unknown1", "", "", "", "", "", "", "", "", "",        //5
                         /*------- ITEMS :: KEY -----------*/
                         "[0x58] Unknown1", "", "", "", "", "", "", "", "", "",        //6
                         /*------- CRITTERS ---------------*/
                         "[0x58] Unknown1", "[0x5C] Unknown2", "[0x60] Unknown3", "[0x64] Unknown4",        //7
                         "[0x68] Critter AI", "[0x6C] Group ID", "[0x70] Unknown5", "[0x74] Hit points",    //7
                         "[0x78] Unknown6", "[0x7C] Unknown7",                                //7
                         /*------- SCENERY :: PORTAL ------*/
                         "[0x58] Unknown1", "", "", "", "", "", "", "", "", "",        //8
                         /*------- SCENERY :: STAIRS ------*/
                         "[0x58] Unknown1", "[0x5C] Unknown2", "", "", "", "", "", "", "", "",//9
                         /*------- SCENERY :: ELEVATOR ----*/
                         "[0x58] Unknown1", "[0x5C] Unknown2", "", "", "", "", "", "", "", "",//10
                         /*------- SCENERY :: LADDER DOWN -*/
                         "[0x58] Unknown1", "[0x5C] Unknown2", "", "", "", "", "", "", "", "",//11
                         /*------ SCENERY :: LADDER UP ----*/
                         "[0x58] Unknown1", "[0x5C] Unknown2", "", "", "", "", "", "", "", "",//12
                         "", "", "", "", "", "", "", "", "", "",                //13
                         "", "", "", "", "", "", "", "", "", "",                //14
                         /*------- MISC :: EXIT GRIDS -----*/
                         "[0x58] Map ID", "[0x5C] Player position", "[0x60] Map elevation",          //15
                         "[0x64] Player direction", "", "", "", "", "", "",            //15
                         "", "", "", "", "", "", "", "", "", "" };              //16

String sType [17] = {"Type: ITEMS; SubType: Armor",
                     "Type: ITEMS; SubType: Container",
                     "Type: ITEMS; SubType: Drug",
                     "Type: ITEMS; SubType: Weapon",
                     "Type: ITEMS; SubType: Ammo",
                     "Type: ITEMS; SubType: Misc",
                     "Type: ITEMS; SubType: Key",
                     "Type: CRITTERS",
                     "Type: SCENERY; SubType: Portal",
                     "Type: SCENERY; SubType: Stairs",
                     "Type: SCENERY; SubType: Elevator",
                     "Type: SCENERY; SubType: Ladder Down",
                     "Type: SCENERY; SubType: Ladder Up",
                     "Type: SCENERY; SubType: Generic",
                     "Type: WALLS",
                     "Type: MISC; SubType: Exit Grid",
                     "Type: MISC; SubType: Generic"  };

//------- FULL TYPE OBJ LENGTH ----------
int nLen [17] = {0, 0, 0, 2, 1, 1, 1, 10, 1 , 2, 2, 2, 2, 0, 0, 4, 0};
//---------------------------------------------------------------------------
__fastcall TfrmProperties::TfrmProperties(TComponent* Owner)
        : TForm(Owner)
{
   pUtil = frmMDI->pUtil;
   pList = frmMDI->pLstFiles;
}
//---------------------------------------------------------------------------
void TfrmProperties::AssignValues(void)
{
   cbScript->Items->Clear();
   cbScript->Items->Add("None");
   for (int i = 0; i < pList->pScript->Count; i++)
   {
      cbScript->Items->Add(pUtil->GetName(pList->pScript->Strings[i]));
   }
}
//---------------------------------------------------------------------------
void TfrmProperties::RefreshProperties(void)
{
   pObjSet = frmEnv->pObjSet;
   pProSet = frmMDI->pProSet;
   pMsg = frmMDI->pMsg;

   int MapScriptID;
   BYTE *pTempObj;
   BYTE nProType;
   WORD nProID;
   DWORD nMsgID;

   switch(pObjSet->nSelected)
   {
      case 0:
         SetControlsTo(false);
         lblMSG1->Caption = "None selected";
         break;
      case 1:
         pTempObj = pObjSet->GetObj();
         pObjSet->SetObj(pObjSet->GetSelObj());
         SetControlsTo(true);
         pObjSet->GetObjType(&nProType, &nProID);

         MapScriptID = pObjSet->GetMapScriptID();
         cbScript->ItemIndex = MapScriptID + 1;
         UpdateScriptDesc();
//         UpdateFlags();
         nMsgID = pProSet->pPRO[nProType][nProID].GetMsgID();
         lblMSG1->Caption = pMsg->GetMSG(nProType, nMsgID);
         lblMSG2->Lines->Text = pMsg->GetMSG(nProType, nMsgID + 1);

         pObjSet->SetObj(pTempObj);
         break;
      default:
         SetControlsTo(false);
         lblMSG1->Caption = "Multiple selection";
         break;
   }
}
//---------------------------------------------------------------------------
void TfrmProperties::ShowProperties(void)
{
   this->Show();
}
//---------------------------------------------------------------------------
void TfrmProperties::SetControlsTo(bool Enabled)
{
   lblMSG1->Caption = "";
   lblMSG2->Lines->Text = "";
   cbScript->ItemIndex = 0;
   UpdateScriptDesc();
   cbScript->Enabled = Enabled;
   bFlagsUpdating = true;
   flag1->Enabled = Enabled;
   flag2->Enabled = Enabled;
   flag3->Enabled = Enabled;
   flag4->Enabled = Enabled;
   flag5->Enabled = Enabled;
   flag6->Enabled = Enabled;
   flag7->Enabled = Enabled;
   flag8->Enabled = Enabled;
   flag9->Enabled = Enabled;
   flag10->Enabled = Enabled;
   flag11->Enabled = Enabled;
   flag12->Enabled = Enabled;
   flag13->Enabled = Enabled;
   flag14->Enabled = Enabled;
   flag15->Enabled = Enabled;
   flag16->Enabled = Enabled;
   flag17->Enabled = Enabled;
   flag18->Enabled = Enabled;
   flag19->Enabled = Enabled;
   flag20->Enabled = Enabled;
   flag21->Enabled = Enabled;
   flag22->Enabled = Enabled;
   flag23->Enabled = Enabled;
   flag24->Enabled = Enabled;
   flag25->Enabled = Enabled;
   flag26->Enabled = Enabled;
   flag27->Enabled = Enabled;
   flag28->Enabled = Enabled;
   flag29->Enabled = Enabled;
   flag30->Enabled = Enabled;
   flag31->Enabled = Enabled;
   flag32->Enabled = Enabled;
   bFlagsUpdating = false;
   UpdateFlags();
}
//---------------------------------------------------------------------------
void TfrmProperties::UpdateScriptDesc(void)
{
   if (cbScript->ItemIndex)
      lblScriptDesc->Lines->Text =
        pUtil->GetDescription(pList->pScript->Strings[cbScript->ItemIndex - 1]);
   else
      lblScriptDesc->Lines->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::cbScriptChange(TObject *Sender)
{
   UpdateScriptDesc();
   pObjSet->ChangeScriptID(pObjSet->GetSelObj(), cbScript->ItemIndex - 1);
//   RefreshValues();
}
//---------------------------------------------------------------------------
void TfrmProperties::UpdateFlags(void)
{
   DWORD dwFlags;
   dwFlags =  pObjSet->nSelected == 1 ? pObjSet->GetFlags() : 0;
   bFlagsUpdating = true;
   flag1->Checked = dwFlags &  0x00000001;
   flag2->Checked = dwFlags &  0x00000002;
   flag3->Checked = dwFlags &  0x00000004;
   flag4->Checked = dwFlags &  0x00000008;
   flag5->Checked = dwFlags &  0x00000010;
   flag6->Checked = dwFlags &  0x00000020;
   flag7->Checked = dwFlags &  0x00000040;
   flag8->Checked = dwFlags &  0x00000080;
   flag9->Checked = dwFlags &  0x00000100;
   flag10->Checked = dwFlags & 0x00000200;
   flag11->Checked = dwFlags & 0x00000400;
   flag12->Checked = dwFlags & 0x00000800;
   flag13->Checked = dwFlags & 0x00001000;
   flag14->Checked = dwFlags & 0x00002000;
   flag15->Checked = dwFlags & 0x00004000;
   flag16->Checked = dwFlags & 0x00008000;
   flag17->Checked = dwFlags & 0x00010000;
   flag18->Checked = dwFlags & 0x00020000;
   flag19->Checked = dwFlags & 0x00040000;
   flag20->Checked = dwFlags & 0x00080000;
   flag21->Checked = dwFlags & 0x00100000;
   flag22->Checked = dwFlags & 0x00200000;
   flag23->Checked = dwFlags & 0x00400000;
   flag24->Checked = dwFlags & 0x00800000;
   flag25->Checked = dwFlags & 0x01000000;
   flag26->Checked = dwFlags & 0x02000000;
   flag27->Checked = dwFlags & 0x04000000;
   flag28->Checked = dwFlags & 0x08000000;
   flag29->Checked = dwFlags & 0x10000000;
   flag30->Checked = dwFlags & 0x20000000;
   flag31->Checked = dwFlags & 0x40000000;
   flag32->Checked = dwFlags & 0x80000000;
   bFlagsUpdating = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::FlagClick(TObject *Sender)
{
   if ((!pObjSet->nSelected) | bFlagsUpdating) return;
   DWORD dwFlags = 0;
   pObjSet->SetObj(pObjSet->GetSelObj());

   dwFlags =
   (flag1->Checked ? 0x00000001 : 0) |
   (flag2->Checked ? 0x00000002 : 0) |
   (flag3->Checked ? 0x00000004 : 0) |
   (flag4->Checked ? 0x00000008 : 0) |
   (flag5->Checked ? 0x00000010 : 0) |
   (flag6->Checked ? 0x00000020 : 0) |
   (flag7->Checked ? 0x00000040 : 0) |
   (flag8->Checked ? 0x00000080 : 0) |
   (flag9->Checked ? 0x00000100 : 0) |
   (flag10->Checked ? 0x00000200 : 0) |
   (flag11->Checked ? 0x00000400 : 0) |
   (flag12->Checked ? 0x00000800 : 0) |
   (flag13->Checked ? 0x00001000 : 0) |
   (flag14->Checked ? 0x00002000 : 0) |
   (flag15->Checked ? 0x00004000 : 0) |
   (flag16->Checked ? 0x00008000 : 0) |
   (flag17->Checked ? 0x00010000 : 0) |
   (flag18->Checked ? 0x00020000 : 0) |
   (flag19->Checked ? 0x00040000 : 0) |
   (flag20->Checked ? 0x00080000 : 0) |
   (flag21->Checked ? 0x00100000 : 0) |
   (flag22->Checked ? 0x00200000 : 0) |
   (flag23->Checked ? 0x00400000 : 0) |
   (flag24->Checked ? 0x00800000 : 0) |
   (flag25->Checked ? 0x01000000 : 0) |
   (flag26->Checked ? 0x02000000 : 0) |
   (flag27->Checked ? 0x04000000 : 0) |
   (flag28->Checked ? 0x08000000 : 0) |
   (flag29->Checked ? 0x10000000 : 0) |
   (flag30->Checked ? 0x20000000 : 0) |
   (flag31->Checked ? 0x40000000 : 0) |
   (flag32->Checked ? 0x80000000 : 0);
   pObjSet->SetFlags(dwFlags);
   RefreshValues();
}
//---------------------------------------------------------------------------
void TfrmProperties::ListRefresh(void)
{
   RefreshNames();
   RefreshValues();
   RefreshProperties();
}
//---------------------------------------------------------------------------
void TfrmProperties::RefreshNames(void)
{
   BYTE *pTempObj;
   BYTE nProType;
   WORD nProID;
   pObjSet = frmEnv->pObjSet;
   int i, nFullType, MapScriptID;

   ed->Text = "";
   sb->Panels->Items[0]->Text = "";
   lv->Items->BeginUpdate();
   lv->Items->Clear();

   if (pObjSet->nSelected == 1)
   {
      pTempObj = pObjSet->GetObj();
      pObjSet->SetObj(pObjSet->GetSelObj());
      nFullType = pObjSet->GetFullType();
      pObjSet->GetObjType(&nProType, &nProID);

      TListItem  *ListItem;
      for (i = 0; i < 22; i++) //----- Add const names
      {
         ListItem = lv->Items->Add();
         ListItem->Caption = sAllList[i];
      }
      for (i = 0; i < nLen[nFullType]; i++) //----- Add type names
      {
         ListItem = lv->Items->Add();
         ListItem->Caption = sList[nFullType][i];
      }
      sb->Panels->Items[0]->Text = sType [nFullType];
      this->Caption = "Properties [" +
          Format("0x%.8x", ARRAYOFCONST(((int)pObjSet->GetFileOffset()))) + "]";
      pObjSet->SetObj(pTempObj);
   }
   lv->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void TfrmProperties::RefreshValues(void)
{
   BYTE *pTempObj;
   pObjSet = frmEnv->pObjSet;
   int i, Val;
   ChangeButtonsState(false);
   lv->Items->BeginUpdate();
   if (pObjSet->nSelected)
   {
      pTempObj = pObjSet->GetObj();
      pObjSet->SetObj(pObjSet->GetSelObj());
      TListItem  *ListItem;
      for (i = 0; i < lv->Items->Count; i++) //----- Add const names
      {
         ListItem = lv->Items->Item[i];
         ListItem->SubItems->Clear();
//         ListItem->SubItems->Add((String)(int)pObjSet->GetObjInfo(i));
         Val = (int)pObjSet->GetObjInfo(i);
         ListItem->SubItems->Add(Format("0x%.8x", ARRAYOFCONST(((int)Val))));
      }
      pObjSet->SetObj(pTempObj);
   }
   lv->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::lvSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
   ChangeButtonsState(false);
   ed->Text = Item->SubItems->Strings[0];
}
//---------------------------------------------------------------------------
void TfrmProperties::ChangeButtonsState(bool State)
{
   if (sb1->Enabled == State && sb2->Enabled == State) return;
   sb1->Enabled = State;
   sb2->Enabled = State;
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::edKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if (!lv->SelCount) return;
   switch(Key)
   {
      case VK_RETURN:
         sb1->Click();
         break;
      case VK_ESCAPE:
         sb2->Click();
         break;
   }
   ChangeButtonsState(true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::sb2Click(TObject *Sender)
{
   if (lv->SelCount)
      lvSelectItem(Sender, lv->Selected, true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmProperties::sb1Click(TObject *Sender)
{
   int Value;
   try
   {
      Value = ed->Text.ToInt();
   }
   catch (Exception &exception)
   {
      Application->ShowException(&exception);
      return;
   }
   pObjSet->SetObj(pObjSet->GetSelObj());
   pObjSet->SetObjInfo(lv->Selected->Index, Value);
   RefreshValues();
   frmEnv->RedrawMap(true);
   frmEnv->SetButtonSave(true);
}
//---------------------------------------------------------------------------
