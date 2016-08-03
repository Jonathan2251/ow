//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("ChildWin.cpp", MDIChild);
USERES("mdiapp.res");
USEFORM("about.cpp", AboutBox);
USEFORM("Page.cpp", GPage);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TAboutBox), &AboutBox);
         Application->CreateForm(__classid(TGPage), &GPage);
         Application->Run();

	return 0;
}
//---------------------------------------------------------------------
