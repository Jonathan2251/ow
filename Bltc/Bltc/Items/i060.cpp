// Author : Rogers Liu

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 StopFactoryClkTest()
{
	U32 lError = 0;
	
	lError = api.frontEnd.StopFactoryClkTest();
	
	return lError;
}

U32 Items::TI060()
{
	U32 lError = 0;
	U32 (*pf)() = &StopFactoryClkTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
  