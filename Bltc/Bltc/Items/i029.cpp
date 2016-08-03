// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 IEEE1394OHCI_Test()
{
	U32 lError = 1;
	
	lError = api.IEEE1394OHCI_Test();
	
	return lError;
}

U32 Items::TI029()
{
	U32 lError = 1;
	U32 (*pf)() = &IEEE1394OHCI_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
