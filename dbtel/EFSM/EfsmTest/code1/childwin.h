//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <StdCtrls.hpp> 
#include "GPage.h"
//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
	TMemo *Memo1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormPaint(TObject *Sender);
private:
public: 
#define PAGE_SIZE 10
	virtual __fastcall TMDIChild(TComponent *Owner);  
/*    int PageCount();
    int ActivePageIndex();
    TPageControl* ppc;
    TTabSheet* pts[PAGE_SIZE];  */
    TGPageForm* page[PAGE_SIZE];
    TPanel *Panel;
};
//----------------------------------------------------------------------------
#endif	
