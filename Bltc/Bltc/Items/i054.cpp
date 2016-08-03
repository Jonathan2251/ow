// Author : Rogers Liu

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib_FrontEnd.h"

U32 OOB_Lock_72_75MHz()
{
	U32 lError = 0;
	
	lError=api.frontEnd.OOB_Lock(72750000);  //OOB 72.75 MHz 
	
	return lError;
}

U32 Items::TI054()
{
	U32 lError = 0;
	U32 (*pf)() = &OOB_Lock_72_75MHz;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
  