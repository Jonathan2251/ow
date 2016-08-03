//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "Main.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"

extern TMainForm *MainForm;
           
int TMDIChild::PageCount() {
    return ppc->PageCount;
};

int TMDIChild::ActivePageIndex() {
    return ppc->ActivePageIndex;
};

//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
    ppc = new TPageControl(this);
	ppc->Parent = this;
    ppc->Width = MainForm->Width;
    ppc->Height = 810;
	for (int i = 0; i < 2; i++) {
		pts[i] = new TTabSheet(ppc);
		pts[i]->PageControl = ppc;
        pts[i]->Width = ppc->Width;
        pts[i]->ClientHeight = 805;
        pts[i]->Caption = "p " + IntToStr(i + 1);

        page[i] = new TGPageForm(pts[i]);
        page[i]->Parent = pts[i];
    }
}
//--------------------------------------------------------------------- 
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormPaint(TObject *Sender)
{
static int k;
k++;
    MainForm->StatusBar->SimpleText = "FormPaint" + IntToStr(k);
}
//---------------------------------------------------------------------------

