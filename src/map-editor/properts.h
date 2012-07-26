//---------------------------------------------------------------------------

#ifndef propertsH
#define propertsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "objset.h"
#include "script.h"
#include "lists.h"
#include "utilites.h"
#include "proset.h"
#include "msg.h"
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TfrmProperties : public TForm
{
__published:	// IDE-managed Components
        TListView *lv;
        TStatusBar *sb;
        TPanel *pn;
        TEdit *ed;
        TSpeedButton *sb1;
        TSpeedButton *sb2;
        TScrollBox *scrbox;
        TBevel *Bevel1;
        TLabel *lblMSG1;
        TComboBox *cbScript;
        TCheckBox *flag1;
        TCheckBox *flag2;
        TCheckBox *flag3;
        TCheckBox *flag4;
        TCheckBox *flag5;
        TCheckBox *flag6;
        TCheckBox *flag7;
        TCheckBox *flag8;
        TCheckBox *flag9;
        TCheckBox *flag10;
        TCheckBox *flag11;
        TCheckBox *flag12;
        TCheckBox *flag13;
        TCheckBox *flag14;
        TCheckBox *flag15;
        TCheckBox *flag16;
        TPageControl *pages;
        TCheckBox *flag17;
        TCheckBox *flag18;
        TCheckBox *flag19;
        TCheckBox *flag20;
        TCheckBox *flag21;
        TCheckBox *flag22;
        TCheckBox *flag23;
        TCheckBox *flag24;
        TCheckBox *flag25;
        TCheckBox *flag26;
        TCheckBox *flag27;
        TCheckBox *flag28;
        TCheckBox *flag29;
        TCheckBox *flag30;
        TCheckBox *flag31;
        TCheckBox *flag32;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TTabSheet *TabSheet5;
        TTabSheet *TabSheet6;
        TTabSheet *TabSheet7;
        TTabSheet *TabSheet8;
        TTabSheet *TabSheet9;
        TTabSheet *TabSheet10;
        TTabSheet *TabSheet11;
        TTabSheet *TabSheet12;
        TTabSheet *TabSheet13;
        TTabSheet *TabSheet14;
        TTabSheet *TabSheet15;
        TTabSheet *TabSheet16;
        TTabSheet *TabSheet17;
        TMemo *lblScriptDesc;
        TLabel *lbl1;
        TLabel *Label1;
        TBevel *Bevel2;
        TMemo *lblMSG2;
        void __fastcall lvSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
        void __fastcall edKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall sb2Click(TObject *Sender);
        void __fastcall sb1Click(TObject *Sender);
        void __fastcall cbScriptChange(TObject *Sender);
        void __fastcall FlagClick(TObject *Sender);
private:	// User declarations
        CObjSet *pObjSet;
        CScriptBuf *pScriptBuf;
        CProSet *pProSet;
        CMsg *pMsg;
        CListFiles *pList;
        CUtilites *pUtil;
        bool bFlagsUpdating;
        void FillView(String *pStr, int nLen);
        void RefreshNames(void);
        void RefreshValues(void);
        void RefreshProperties(void);
        void ChangeButtonsState(bool State);
public:		// User declarations
        void AssignValues(void);
        void ListRefresh(void);
        void ShowProperties(void);
        void SetControlsTo(bool Enabled);
        void UpdateScriptDesc(void);
        void UpdateFlags(void);
        __fastcall TfrmProperties(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmProperties *frmProperties;
//---------------------------------------------------------------------------
#endif
