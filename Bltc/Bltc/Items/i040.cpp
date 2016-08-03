// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 FLASH_TC_MTC_SudbCrcCheck()
{
	U32 lError = 1;
	
	lError = api.memory.FLASH_TCMTC_SudbCrcCheck();
	
	return lError;
}

U32 Items::TI040()
{
	U32 lError = 1;
	U32 (*pf)() = &FLASH_TC_MTC_SudbCrcCheck;
	
	int timeOut = 20;	// unit:1s, => 20 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
