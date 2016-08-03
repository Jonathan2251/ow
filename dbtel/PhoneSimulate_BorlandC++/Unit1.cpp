/*
Image1 : The handset image of no key be pressed.
Image2 : The image of single Key Button be pressed.
Image3 : The handset image of the Key Button Color is colored by Red or Black.
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Group.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "extrares.RES"
TForm1 *Form1;
//---------------------------------------------------------------------------

extern PT *pt;
extern FT *ft;
      
int timeOut; // used by OnTimer() and PTMMI.Run()
int timeCount;

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
/*    Form1->Top = 0;
    Form1->Left = 0;
    Form1->Height = 553 + 30;
    Form1->Width = Image1->Width + 500;*/
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    char s[20];

    pt->kp->pressTime = 0;
    pt->ptxx_dr_key = pt->kp->WhichKeyPressed(X, Y); // return -1 when no key to click
    if (pt->ptxx_dr_key != NO_KEY) {
        pt->kp->keyPressed = true;
        pt->kp->key[pt->ptxx_dr_key].ShowKeyPressed(Image2);
        if (pt->mmi->State() != PTMMI::POWER_OFF) {
            pt->buzzar->PlayKeyPressedVoice(pt->ptxx_dr_key);
        }
    }
    else {
        pt->act_event = NO_EVENT;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    if (ParamCount() != 2) {
        Form1->Close();
        return;
    }
    ft = new FT(ServerSocket, ClientSocket);
    pt = new PT;
    StatusBar->Panels->Items[0]->Text = "Power Off : Please press key PowerOn for 3 seconds to Power On.";
    timeCount = 0;
    Form1->Timer1->Interval = 500;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{       
    Image2->Visible = false;// back to the image that before Key be pressed.
    if (pt->kp->pressTime < 4 && pt->ptxx_dr_key != NO_KEY) { // when not long press and key be pressed
        pt->act_event = DR_KEYPAD_IND;
        pt->mmi->Run();
    }           
    pt->kp->keyPressed = false;
    pt->kp->pressTime = 0;
}
//---------------------------------------------------------------------------
/* OnTimer() : trigger at every 0.5 second. */
void __fastcall TForm1::OnTimer(TObject *Sender)
{
    pt->kp->pressTime++;
    pt->act_event = TIME_OUT_500MS;
    pt->lcd->Trigger(); // trigger lcd to do blink at every 0.5 second.
    timeOut++;
    pt->mmi->Run(); // fire event TIME_OUT_500MS for MMI
    if (pt->mmi->State() == PTMMI::SHOW_MESSAGE && timeOut > 2) {
        pt->mmi->ExitShowMessageForOneSec();
        pt->act_event = EXIT_SHOW_MESSAGE;
        pt->mmi->Run();
    }
    if (pt->kp->keyPressed && pt->kp->pressTime > 3) {  // over 2 seconds is long press
        if (pt->ptxx_dr_key == KEY_ON_OFF) { // fire event DR_KEYPAD_IND for MMI because the KEY_ON_OFF long pressed.
            pt->ptxx_dr_key = KEY_ON_OFF_LONG_PRESS;
            pt->act_event = DR_KEYPAD_IND;
            pt->mmi->Run();
        }
    }
    timeCount = (timeCount + 1) % 2000;
    if (timeCount % 2 == 0) { // update time variables of configure file at every 1 second.
        ft->cfg.Time.Second = (ft->cfg.Time.Second + 1) % 60;
        if (ft->cfg.Time.Second == 0) {
            ft->cfg.Time.Minute = (ft->cfg.Time.Minute + 1) % 60;
            if (ft->cfg.Time.Minute == 0) {
                ft->cfg.Time.Hour = (ft->cfg.Time.Hour + 1) % 24;
            }
        }
    }

    Timer1->Interval = 500; // trigger every 0.5 second
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    delete ft;
    delete pt;
    ServerSocket->Close();
    ClientSocket->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
  StatusBar->Panels->Items[0]->Text = "Connect to: " + Socket->RemoteHost;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
  StatusBar->Panels->Items[0]->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    CID cid;
    Socket->ReceiveBuf((void*)&cid, 80);
    ft->IncomingCall(cid);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    char sPort[20];
    itoa(ft->cfg.sendPort, sPort, 10);
    string port(sPort);
    string temp = "Error connecting to: (" + ft->cfg.sendIP + ", " + port + ")";
    StatusBar->Panels->Items[0]->Text = temp.c_str();
    ErrorCode = 0;
}
//---------------------------------------------------------------------------

