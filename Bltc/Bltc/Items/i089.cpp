
// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 USB_FrontFanelPortTest()
{
	U32 lError = 1;
	
	lError = api.peripheral.USB_Test(QIP7232_USB_FRONTPANEL_PORT);
	
	return lError;
}

U32 Items::TI089()
{
	U32 lError = 1;
	U32 (*pf)() = &USB_FrontFanelPortTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
