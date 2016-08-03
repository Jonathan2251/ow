// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

#define IR_TimeOut 5000000	// 5 seconds

U32 FrontPanelIR_Test()
{
	U32 lError = 1;

	U8  TestPattern='1';

	lError=api.peripheral.FPIR_Test(&TestPattern,IR_TimeOut);

	return lError;
}

U32 Items::TI085()
{
	U32 lError = 1;
	U32 (*pf)() = &FrontPanelIR_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
