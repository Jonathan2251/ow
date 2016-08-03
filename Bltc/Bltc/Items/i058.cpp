// Author : Rogers Liu

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 Entropic_ContinuousOutput_ChannelSet_Chan14()
{
   U32 lError = 1;
   
   lError=api.frontEnd.entropic.ContinuousOutput_ChannelSet(14);  // 1150MHz
   
   return lError;
}

U32 Items::TI058()
{
	U32 lError = 0;
	U32 (*pf)() = &Entropic_ContinuousOutput_ChannelSet_Chan14;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}