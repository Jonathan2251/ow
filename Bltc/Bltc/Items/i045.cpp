// Author : Gamma

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 DMA_DDR0ToDDR2_Test()
{
	U32 lError = 1;
	
	lError=api.memory.DMA_DDRToPCITransfer(DDR2);

	return lError;
}

U32 Items::TI045()
{
	U32 lError = 1;
	U32 (*pf)() = &DMA_DDR0ToDDR2_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
