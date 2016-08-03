// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 TI_065_AV_Test()
{
	TS_Msg tsMsg1 = {TUNER1, true, 85000000, QAM256, {0x1d0, 0x1d0, 0x1d1}, BYPASS_MCARD};
	TS_Msg tsMsg2 = {TUNER2, true, 183000000, QAM64, {0x880, 0x880, 0x881}, BYPASS_MCARD};
	U32 lError = 0;
	
	lError = api.digAV.PIP_CreateTransportStream(tsMsg1, tsMsg2);
	if (lError) {
		return lError;
	}
	lib.digAV.AUDIO_SelSrc(tsMsg1.tuner, tsMsg1.pcrPids.pcr, tsMsg1.pcrPids.aPids);
//	After CreateTransportStream(), no output interface is opened.
	api.digAV.OpenOutput(OIF_COMPOSITE|OIF_COMPONENT|OIF_HDMI|OIF_RF);

	return lError;
}

//#define CONFIG_NOT_USING_TIME_OUT
U32 Items::TI065()
{
	U32 lError = 0;
	
// When did the single step test, the probe "CODE RUNNER" will disable interrupt.
#ifdef CONFIG_NOT_USING_TIME_OUT
	int timerISR_Enabled = lib.clock.IsTimerInterruptEnabled();
	lib.clock.DisableTimerInterrupt();
	lError = TI_065_AV_Test();
	if (timerISR_Enabled) {
		lib.clock.EnableTimerInterrupt();
	}
#else
	U32 (*pf)() = &TI_065_AV_Test;
	int timeOut = 25;	// unit:1s, => 25 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif

	return lError;
}
