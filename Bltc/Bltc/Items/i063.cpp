// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 TI_063_AV_Test()
{
	TS_Msg tsMsg = {TUNER1, true, 85000000, QAM256, {0x800, 0x800, 0x801}, BYPASS_MCARD};
	U32 lError = 0;
	
	lError = api.digAV.CreateTransportStream(tsMsg);
	if (lError) {
		return lError;
	}
	lib.digAV.AUDIO_SelSrc(tsMsg.tuner, tsMsg.pcrPids.pcr, tsMsg.pcrPids.aPids);
//	After CreateTransportStream(), no output interface is opened.
	api.digAV.OpenOutput(OIF_COMPOSITE|OIF_COMPONENT|OIF_HDMI|OIF_RF);

	return lError;
}

//#define CONFIG_NOT_USING_TIME_OUT
U32 Items::TI063()
{
	U32 lError = 0;
	
// When did the single step test, the probe "CODE RUNNER" will disable interrupt.
#ifdef CONFIG_NOT_USING_TIME_OUT
	int timerISR_Enabled = lib.clock.IsTimerInterruptEnabled();
	lib.clock.DisableTimerInterrupt();
	lError = TI_063_AV_Test();
	if (timerISR_Enabled) {
		lib.clock.EnableTimerInterrupt();
	}
#else
	U32 (*pf)() = &TI_063_AV_Test;
	int timeOut = 15;	// unit:1s, => 15 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif

	return lError;
}
