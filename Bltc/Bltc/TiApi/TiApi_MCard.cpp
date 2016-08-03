#include "TiApi_MCard.h"
#include "TiApi.h"
#include "TiLib.h"
#include "ccard.h"
#include "Timer_MsTick.h"

#define OFF 0
#define ON	1

#define POLLING_PERIOD	100	// 100 ms

extern Timer_MsTick gUI_tick_ms;
extern bool	mCard_hasInitialized;

//##ModelId=46EBA19F0221
/*int TiApi_MCard::Init()
{
	lib.mCard.DoInit();
		
	return 0;
}*/

extern VOID PowerCableCardMenu();

//##ModelId=46EBAA2D017F
U32 TiApi_MCard::PowerOn()
{
	char *power_on[] = {
	    "2",        // 2:  Power On
	    "/",        // /:  exit PowerCableCardMenu()
		"\0"};

	tvMonIO_clear();
	tvMonIO_in_addCmds(power_on);
	PowerCableCardMenu();
			
	return 0;
}

//##ModelId=481125BE01BC
U32 TiApi_MCard::PowerOff()
{
	char *power_off[] = {
	    "1",        // 1:  Power Off
	    "/",        // /:  exit PowerCableCardMenu()
		"\0"};

	tvMonIO_clear();
	tvMonIO_in_addCmds(power_off);
	PowerCableCardMenu();
			
	return 0;
}

//##ModelId=472A817C03A3
U32 TiApi_MCard::IsLoaded() {
	char *is_MCard_loaded[] = {
		"/",		// exit doCcard()
		"\0"};
	char* p;
	bool loaded;

	tvMonIO.out.enable = 1;
	lib.tvMonCmd.RunCmdsInMenu(is_MCard_loaded, doCcard);	// mhif menu
	tvMonIO.out.enable = 0;
		
	p = strstr(tvMonIO.out._ostr, "CC Detect Status                           [M-CARD]");
	if (p != 0) {
		loaded = true;
	}
	else {
		loaded = false;
	}
	
	return loaded;
}

//##ModelId=472C54F3005E
U32 TiApi_MCard::IsLoadedCardVersion() {
	return lib.mCard.OpenSession();
}

//##ModelId=481125BE01BD
U32 TiApi_MCard::OOBDownStreamTest(U32 rfFreqHz) {
	U32 lError = 0;
	
	// lock the oob signal at 125MHz -- begin	
	lError = api.frontEnd.OOB_Lock(rfFreqHz);
	if (lError)
	{
	   return lError;	
	}
	// lock the oob signal at 125MHz -- end
    	
	// enter the ccard menu - begin
	lib.mCard.DoInit();
	// enter the ccard menu - end
	lib.mCard.DoInit();	
	// enter the ccard menu twice - end
	
//	lib.mCard.OpenSession();
	PowerOn();
//	lib.mCard.Reset();
	
	// switch to cable card oob mode - begin
	lError = lib.mCard.RouteOOBToCableCard();
	// switch to cable card oob mode - end

	return lError;
}

// Take Power On/Off MCard automatically by check MCard Attached/Detached for every 100ms.
//##ModelId=481125BE01CC
void TiApi_MCard::AutoPowerOnOff()
{
	static bool firstTime = true;
	S32 newMCardStatus;
	
	if (!mCard_hasInitialized) {
		return;
	}
	if (firstTime) {
		if (IsLoaded()) {
			_mCardStatus = ON;
		}
		else {
			_mCardStatus = OFF;
		}
		_count_ms = POLLING_PERIOD;
		firstTime = false;
	}
	
	if (_ms.Tick()) {
		_count_ms--;
		if (_count_ms < 0) {
		// Trigger at every 100ms. It must. If check MCard IsLoaded() too frequently, the insert MCard action seems will reset STB.
			bool rs232Enabled = lib.rs232.DisableOutput();

			if (IsLoaded()) {
				newMCardStatus = ON;
			}
			else {
				newMCardStatus = OFF;
			}
			if (newMCardStatus != _mCardStatus) {
				if (newMCardStatus == ON) {
					PowerOn();
				}
				else {	// mCardStatus == OFF
					PowerOff();
				}
				_mCardStatus = newMCardStatus;
			}
			_count_ms = POLLING_PERIOD;
			if (rs232Enabled) {
				lib.rs232.EnableOutput();
			}
		}
	}
}
