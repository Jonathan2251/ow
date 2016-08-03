//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop 
#include <mmsystem.hpp> // for SDK function sndPlaySound(), waveOutGetVolume(), waveOutSetVolume().
USERES("Project1.res");
USEFORM("Unit1.cpp", Form1);
USERC("extrares.rc");
USEUNIT("Phone.cpp");
USE("Group.h", File);
USEUNIT("Network.cpp");
USEUNIT("Group.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
         sndPlaySound( NULL, NULL); // stop that audio playing voice
    }
    return 0;
}
//---------------------------------------------------------------------------
