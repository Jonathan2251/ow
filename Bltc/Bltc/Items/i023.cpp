// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

//#define CONFIG_NOT_USING_TIME_OUT

U32 BCM3217_Revision_Check()
{
	U32 lError = 1;
	U32 bcm3517_revision;
	
	lError=api.ChipID_Demod_Check();
		
	return lError;
}

U32 Items::TI023()
{
	U32 lError = 1;
	
#ifdef CONFIG_NOT_USING_TIME_OUT
	lError = BCM3217_Revision_Check();
#else
	U32 (*pf)() = &BCM3217_Revision_Check;
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif
	
	return lError;
}