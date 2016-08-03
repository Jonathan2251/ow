// Author : Rogers Liu

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 OOB_Lock_75_25MHz()
{
	U32 lError = 0;
	
    lError=api.frontEnd.OOB_Lock(75250000);  //OOB 75.25 MHz     
   	
	return lError;
}

U32 Items::TI053()
{
	U32 lError = 0;
	U32 (*pf)() = &OOB_Lock_75_25MHz;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}