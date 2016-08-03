//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Messages.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
                   
enum TDrawButton {dtSelect, dtState, dtTransition};

class TMainForm : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileCloseItem;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *Edit1;
	TMenuItem *CutItem;
	TMenuItem *CopyItem;
	TMenuItem *PasteItem;
	TMenuItem *WindowMinimizeItem;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TMenuItem *WindowTileItem2;
        TToolBar *ToolBar2;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
    TToolButton *ToolButton12;
    TToolButton *PointButton;
    TToolButton *RectButton;
    TToolButton *LineButton;
    TMenuItem *Doc1;
    TMenuItem *ExpandItem;
    TMenuItem *ShrinkItem;
    TMenuItem *RoomInItem;
    TMenuItem *RoomOutItem;
    TMenuItem *ExWidthItem;
    TMenuItem *ExHeightItem;
    TMenuItem *ShWidthItem;
    TMenuItem *ShHeightItem;
    TMenuItem *ExBothItem;
    TMenuItem *ShBothItem;
    TMenuItem *Width1;
    TMenuItem *RIHeightItem;
    TMenuItem *RIBothItem;
    TMenuItem *ROWidthItem;
    TMenuItem *ROHeightItem;
    TMenuItem *ROBothItem;
        void __fastcall FileNew1Execute(TObject *Sender);
        void __fastcall FileOpen1Execute(TObject *Sender);
        void __fastcall HelpAbout1Execute(TObject *Sender);
        void __fastcall FileExit1Execute(TObject *Sender);
    void __fastcall PointButtonClick(TObject *Sender);
    void __fastcall RectButtonClick(TObject *Sender);
    void __fastcall LineButtonClick(TObject *Sender);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ExWidthItemClick(TObject *Sender);
    void __fastcall ExHeightItemClick(TObject *Sender);
    void __fastcall ExBothItemClick(TObject *Sender);
    void __fastcall ShWidthItemClick(TObject *Sender);
    void __fastcall ShHeightItemClick(TObject *Sender);
    void __fastcall ShBothItemClick(TObject *Sender);
private:
	void __fastcall CreateMDIChild(const String Name);
    int size;
public:
//    friend class State;
	virtual __fastcall TMainForm(TComponent *Owner);
    TMDIChild *doc[10];
    TDrawButton drawButton;
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif    
