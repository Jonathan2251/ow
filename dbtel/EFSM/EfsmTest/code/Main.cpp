//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "About.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
TMDIChild *currDoc;
//---------------------------------------------------------------------------


__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateMDIChild(String Name)
{
	//--- create a new MDI child window ----
	currDoc = new TMDIChild(Application);
	currDoc->Caption = Name;
    currDoc->WindowState = wsMaximized;
	if (FileExists (Name)) {
		currDoc->Memo1->Lines->LoadFromFile(Name);
    }
    doc[size] = currDoc;
    size++;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{
	CreateMDIChild("NONAME" + IntToStr(MDIChildCount + 1));
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileOpen1Execute(TObject *Sender)
{
	if (OpenDialog->Execute())
		CreateMDIChild(OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PointButtonClick(TObject *Sender)
{
    drawButton = dtPoint;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RectButtonClick(TObject *Sender)
{
    drawButton = dtRectangle;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LineButtonClick(TObject *Sender)
{
    drawButton = dtLine;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TVarRec tempvar[2] = {X, Y};
    StatusBar->SimpleText = Format("Current: (%d, %d)", tempvar, 2);
}
//---------------------------------------------------------------------------


