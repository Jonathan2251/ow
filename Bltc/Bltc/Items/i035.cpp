// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

#define BCM325x 0

U32 DMA_DDRToDDRTransfer_Test()
{
	U32 lError = 1;
	
	lError=api.memory.DMA_DDRToDDRTransfer();

	return lError;
}

U32 Items::TI035()
{
	U32 lError = 1;
	U32 (*pf)() = &DMA_DDRToDDRTransfer_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
