#include "TiLib.h"
#include "ccard.h"

extern bool	mCard_hasInitialized;
extern VOID InitializeCcard();
extern VOID HwResetMenu();

//##ModelId=46F2166203C7
bool TiLib_MCard::IsInit()
{
	return mCard_hasInitialized;
}

//##ModelId=46F21736007F
U32 TiLib_MCard::DoInit()
{
	U32 lError = 0;
	
	InitializeCcard();
	
	return lError;
}


extern U8 UA;
extern U8 CV;

//##ModelId=48114D270317
U32 TiLib_MCard::OpenSession()
{
	char *open_session[] = {
	    "3",        // open session
		"/",		// exit doCcard()
		"\0"};
	U32 lError;

	lib.tvMonCmd.RunCmdsInMenu(open_session, doCcard);	// mhif menu
//	OpenSession();	// not work
	if (UA == 1 && CV == 1) {	//Card Version 01.47
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

//##ModelId=48114D270318
U32 TiLib_MCard::RouteOOBToCableCard()
{
	char *route_oob_to_mcard[] = {
		"s",		// [s] OobSwitchControl
		"1", 		// 1:  CableCard Mode
		"\t4",		// [4] Read Packet
		"/",		// exit qpsk menu
		"\0"};
	char* p;
	U32 lError;

	tvMonIO.out.enable = 1;
	lib.tvMonCmd.RunCmdsInMenu(route_oob_to_mcard, doCcard);
	tvMonIO.out.enable = 0;
	p = strstr(tvMonIO.out._ostr, "00, 12, 34, 56");	//Card Version 01.47
	if (p != 0) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

void TiLib_MCard::Reset()
{
	char *hw_reset_on[] = {
	    "1",        // 1:  On
	    "/",        // /:  exit HwResetMenu()
		"\0"};
	char *hw_reset_off[] = {
	    "2",        // 2:  Off
	    "/",        // /:  exit HwResetMenu()
		"\0"};

	tvMonIO_clear();
	tvMonIO_in_addCmds(hw_reset_on);
	HwResetMenu();
	tvMonIO_clear();
	tvMonIO_in_addCmds(hw_reset_off);
	HwResetMenu();
	//ResetMHIF();
}
