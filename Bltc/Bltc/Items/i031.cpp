// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 CACHE_ModeChange()
{
	U32 lError = 0;
	
	DisableHalDmaCode();	// Avoid conflict with OSD (possible trigger exception)
	lError = api.memory.CACHE_ModeChangeTest();
    EnableHalDmaCode();		// Without this, the OSD will dispear
	lib.DelayMS(200);
	
	return lError;
}

U32 Items::TI031()
{
	U32 lError = 1;
	U32 (*pf)() = &CACHE_ModeChange;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
