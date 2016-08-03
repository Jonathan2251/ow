// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 NVSRAM_CopyMtcSudbToNvram_Test()
{
	int i;
	U32 lError = 1;
	
	for (i = 0; i < 3 && lError != 0; i++) {
		lError = api.memory.NVSRAM_CopyMtcSudbToNvram();
	}
	
	return lError;
}

#define CONFIG_NOT_USING_TIME_OUT

U32 Items::TI038()
{
	U32 lError = 1;
	
#ifdef CONFIG_NOT_USING_TIME_OUT
	int timerISR_Enabled = lib.clock.IsTimerInterruptEnabled();
	lib.clock.DisableTimerInterrupt();
	lError = NVSRAM_CopyMtcSudbToNvram_Test();
	if (timerISR_Enabled) {
		lib.clock.EnableTimerInterrupt();
	}
#else
	U32 (*pf)() = &NVSRAM_CopyMtcSudbToNvram_Test;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif
	
	return lError;
}
