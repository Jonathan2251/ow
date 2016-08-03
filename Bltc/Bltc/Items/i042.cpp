// Author : Alan

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 FLASH_Check()
{
	U32 lError = 0;
	U16 flashID;
	
	lError = api.memory.FLASH_Check();
	
	return lError;
}

U32 Items::TI042()
{
	U32 lError = 1;
	U32 (*pf)() = &FLASH_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
