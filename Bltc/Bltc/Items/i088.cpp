// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 USB_RearFanelPortTest()
{
	U32 lError = 1;
	
	lError = api.peripheral.USB_Test(QIP7232_USB_REARPANEL_PORT);
	
	return lError;
}

U32 Items::TI088()
{
	U32 lError = 1;
	U32 (*pf)() = &USB_RearFanelPortTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
