/*****************************************************************************
Module:			Test Item 063
Return:			U32 lError
Author:			Gamma Chen
*****************************************************************************/

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiApi_Memory.h"

U32 FLASH_IsPFDAreaErased()
{
	U32 lError;
	
	if (api.memory.FLASH_IsPFDAreaErased()) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

U32 Items::TI043()
{
	U32 lError = 1;
	U32 (*pf)() = &FLASH_IsPFDAreaErased;
	
	int timeOut = 3;	// unit:1s, => 3 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
