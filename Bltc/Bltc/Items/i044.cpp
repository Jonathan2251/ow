// Author : Gamma

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 DMA_DDR0ToDDR1_Test()
{
	U32 lError = 1;
	
	lError=api.memory.DMA_DDRToPCITransfer(DDR1);

	return lError;
}

U32 Items::TI044()
{
	U32 lError = 1;
	U32 (*pf)() = &DMA_DDR0ToDDR1_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
