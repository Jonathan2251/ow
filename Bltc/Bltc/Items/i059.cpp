// Author : Gamma Chen

// QIP7100P2: TP104:16Mhz, TP1300:25Mhz(on back plan), TP211:54Mhz (on back plan), TP16:54Mhz(on back plan)
// QIP7232P2: TP104:16Mhz, TP1300:25Mhz(on back plan), TP1801:54Mhz, TP1701:54Mhz, TP16:54Mhz(on back plan)

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 StartFactoryClkTest()
{
	U32 lError = 0;
	
	lError = api.frontEnd.StartFactoryClkTest();
	
	return lError;
}

U32 Items::TI059()
{
	U32 lError = 0;
	U32 (*pf)() = &StartFactoryClkTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
  