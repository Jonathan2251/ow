// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib_RS232.h"

U32 PSOC_Version_Check()
{
	U32 lError = 1;
	U32 PSoC_Version;
	
	lError=api.ChipID_Get(CT_PSOC_FAN,&PSoC_Version);
	PDEBUG("psoc version = 0x%.2X\r\n", PSoC_Version);
   
    if ((PSoC_Version==PSoC_FW_Version)&&(!lError))
	     lError=0;
	else lError=1;
	
	return lError;
}

U32 Items::TI022()
{
	U32 lError = 1;
	U32 (*pf)() = &PSOC_Version_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}