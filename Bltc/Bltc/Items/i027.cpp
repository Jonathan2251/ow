// Author : Rogers

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 Entropic2210_ChipID_Check()
{
   U32 lError = 1;
   
   lError=api.frontEnd.entropic.DeviceProperty_Identify();
   
   return lError;
}

U32 Items::TI027()
{
	U32 lError = 1;
	U32 (*pf)() = &Entropic2210_ChipID_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}