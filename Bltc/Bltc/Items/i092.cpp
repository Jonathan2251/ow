// Author : Dolphin Tzeng

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 ACOUTLET_On()
{
	U32 lError = 1;
	
	lError=api.peripheral.ACOUTLET_Test(AC_OUTLET_ON);
	
	return lError;
}

U32 Items::TI092()
{
	U32 lError = 0;
	U32 (*pf)() = &ACOUTLET_On;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
