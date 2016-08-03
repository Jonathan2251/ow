//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <MPlayer.hpp>
#include <ScktComp.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TImage *Image2;
    TTreeView *TreeView1;
    TImage *Image3;
    TTimer *Timer1;
    TClientSocket *ClientSocket;
    TServerSocket *ServerSocket;
    TStatusBar *StatusBar;
    void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall OnTimer(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
private:	// User declarations  
    Graphics::TBitmap*    Icon_Select;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
