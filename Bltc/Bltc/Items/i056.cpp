// Author : Rogers Liu

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 Entropic_ContinuousWave_Test()
{
   U32 lError = 1;
   
   lError=api.frontEnd.entropic.ContinuousWave_Test(28);  // 1500MHz
   
   return lError;
}

U32 Items::TI056()
{
	U32 lError = 0;
	U32 (*pf)() = &Entropic_ContinuousWave_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}