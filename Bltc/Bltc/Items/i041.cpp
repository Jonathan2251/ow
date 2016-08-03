// Author : Alan

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 FLASH_Protect_Test()
{
	U32 lError = 0;
	
	lError = api.memory.FLASH_Protect();
	
	return lError;
}

U32 Items::TI041()
{
	U32 lError = 1;
	U32 (*pf)() = &FLASH_Protect_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
