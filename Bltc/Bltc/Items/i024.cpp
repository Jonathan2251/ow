// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib_RS232.h"

U32 BCM7400_ChipID_Check()
{
	U32 lError = 1;
	U32 BCM7400_Version;
	
	lError=api.ChipID_Get(CT_MAIN_CHIP,&BCM7400_Version);
	PDEBUG("CPU chip ID = %.8x\r\n", BCM7400_Version);

	if ((BCM7400_Version==BCM7400_REV_E0)&&(!lError))
	     lError=0;
	else lError=1;
	
	return lError;
}

U32 Items::TI024()
{
	U32 lError = 1;
	U32 (*pf)() = &BCM7400_ChipID_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}