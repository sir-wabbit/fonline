//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "utilites.h"
#include "log.h"
#include "map.h"
#include "tileset.h"
#include "frmset.h"
#include "lists.h"
#include "msg.h"
#include "proset.h"
#include "objset.h"
#include "pal.h"
#include "properts.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <ddraw.h>
//---------------------------------------------------------------------------
class TfrmEnv : public TForm
{
__published:	// IDE-managed Components
        TPanel *imgMap;
        TShape *shp;
        TPanel *imgObj;
        TTabControl *tabc;
        TImage *imgMiniMap;
        TShape *Shape2;
        TShape *Shape3;
        TShape *Shape4;
        TLabel *lblMM;
        TLabel *lblInfo;
        TShape *Shape5;
        TScrollBar *sb;
        TPopupMenu *popupMap;
        TMenuItem *Mapperv09byDims1;
        TMenuItem *N1;
        TSpeedButton *btnWorld;
        TShape *Shape6;
        TSpeedButton *btnLocal;
        TImageList *ImageList1;
        TMenuItem *piProperties;
        TMenuItem *piDelete;
        TMenuItem *N2;
        TStatusBar *sbar;
        TShape *Shape1;
        TShape *Shape7;
        TLabel *lblinv;
        TScrollBar *sbINV;
        TPanel *imgInv;
        TShape *Shape8;
        TSpeedButton *btnAdd;
        TSpeedButton *btnRemove;
        TSpeedButton *btnChange;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall imgMapMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imgMapMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall imgMapMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall imgMiniMapMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imgMiniMapMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall imgMiniMapMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imgMapClick(TObject *Sender);
        void __fastcall popupMapPopup(TObject *Sender);
        void __fastcall piPropertiesClick(TObject *Sender);
        void __fastcall tabcChange(TObject *Sender);
        void __fastcall imgObjMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall btnWorldClick(TObject *Sender);
        void __fastcall btnLocalClick(TObject *Sender);
        void __fastcall sbScroll(TObject *Sender, TScrollCode ScrollCode,
          int &ScrollPos);
        void __fastcall piDeleteClick(TObject *Sender);
        void __fastcall imgInvMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnChangeClick(TObject *Sender);
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnRemoveClick(TObject *Sender);
        void __fastcall sbINVChange(TObject *Sender);
private:	// User declarations
        CUtilites *pUtil;
        CLog *pLog;
        CFrmSet *pFrmSet;
        CProSet *pProSet;
        CListFiles *pLstFiles;
        CMsg *pMsg;
        CPal *pPal;
        bool mouseBLeft, mouseBLeft2;
        bool mouseBRight, mouseBRight2;
        int downX, downY, upX, upY; //for Mouse at Map
        int downHexX, downHexY;
        int downTileX, downTileY;
        int cursorX, cursorY;
        int prevX, prevY, have_sel; // for selection-frame drawing
        int TileX, TileY, tile_sel;
        int HexX, HexY;
        int OldX, OldY; // for Locator
        int OldShowObjX, OldShowObjY; // for indicating obj
        HANDLE h_map;
        Graphics::TBitmap *pBMPlocator;
        Controls::TWndMethod OldMapWndProc;
        Controls::TWndMethod OldNavWndProc;
        Controls::TWndMethod OldInvWndProc;
        struct OBJNAVIGATOR
        {
           BYTE nObjType;
           DWORD nSelID;
           DWORD nNavID[8];
           BYTE nShowMode;
           int nCount;
           int nMaxID;
        } Navigator;
        struct OBJINVENTORY
        {
           BYTE *pObj;
           BYTE *pChildObj[3];
           int nItemNum;
           int nInvStartItem;
        } Inventory;
        void __fastcall NewMapWndProc(TMessage &Msg);
        void __fastcall NewNavWndProc(TMessage &Msg);
        void __fastcall NewInvWndProc(TMessage &Msg);
        void DrawMiniMap(void);
        void RedrawFloor(void);
        void RerawObjects(void);
        void RedrawRoof(void);
        void RedrawObjects(void);
        void RedrawBlockers(void);
        void RedrawHex(void);
        void RedrawLocator(void);
        void SelectObjXY(int X, int Y);
        bool SelectObjRegion(int X, int Y, int X2, int Y2);
        void SelectionChanged(void);
        bool SelectTileRegion(int TileMode, int X, int Y, int X2, int Y2);
        void LogSelected(void);
public:		// User declarations
        TfrmProperties *frmProp;
        int SelectMode;
        CMap *pMap;
        CTileSet *pTileSet;
        CObjSet *pObjSet;
        int WorldX, OldWorldX;
        int WorldY, OldWorldY;
        int iLevel;
        bool bShowObj[14];
        LPDIRECTDRAWSURFACE7 dds, dds2Map, dds2Nav, dds2Inv;
        LPDIRECTDRAWCLIPPER ddc, ddcMap, ddcNav, ddcInv;
        TStatusPanel *panelHEX;
        TStatusPanel *panelTILE;
        TStatusPanel *panelObjCount;
        TStatusPanel *panelObjSelected;
        TStatusPanel *panelMSG;

        void ClearSelection(void);
        void ClearFloorSelection(bool NeedRedrawMap);
        void ClearRoofSelection(bool NeedRedrawMap);
        void ClearObjSelection(bool NeedRedrawMap);
        void MoveSelectedObjects(int offsetHexX, int offsetHexY);
        void RotateSelectedObjects(int direction);
        void SetButtonSave(bool State);
        void RedrawMap(bool StaticRedraw);
        void PrepareNavigator(BYTE nObjType);
        void RedrawNavigator(void);
        void ResetInventoryInfo(void);
        void RedrawInventory(void);

       	void TransBltFrm(CFrame* frm, TControl*, short nDir, short nFrame,
                                       int x, int y, LPDIRECTDRAWSURFACE7 dds2);
	void TransBltMask(CFrame* frm, TControl*, short nDir, short nFrame,
                int x, int y, LPDIRECTDRAWSURFACE7 dds2, int width, int height);
        void TransBltTileRegion(LPDIRECTDRAWSURFACE7 dds2,
                                int TileX1, int TileY1, int TileX2, int TileY2);
	void RepaintDDrawWindow(TWinControl *win, LPDIRECTDRAWSURFACE7 dds,
                          LPDIRECTDRAWSURFACE7 dds2, LPDIRECTDRAWCLIPPER ddc);
	void AttachDDraw(TControl *win, LPDIRECTDRAWSURFACE7 *dds, int primary);

        __fastcall TfrmEnv(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEnv *frmEnv;
//---------------------------------------------------------------------------
#endif
