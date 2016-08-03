// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 TI_066_AV_Test()
{
	TS_Msg tsMsg = {TUNER1, true, 183000000, QAM64, {0x880, 0x880, 0x881}, ROUTE_TO_MCARD};
	U32 lError = 0;
	
	lError = api.digAV.CreateTransportStream(tsMsg);
	if (lError) {
		return lError;
	}
	lib.digAV.AUDIO_SelSrc(tsMsg.tuner, tsMsg.pcrPids.pcr, tsMsg.pcrPids.aPids);
//	After CreateTransportStream(), no output interface is opened.
	api.digAV.OpenOutput(OIF_COMPOSITE|OIF_COMPONENT|OIF_HDMI|OIF_RF|OIF_1394);
//	lib.digAV.Start1KTone(TUNER1);

	return lError;
}

//#define CONFIG_NOT_USING_TIME_OUT
U32 Items::TI066()
{
	U32 lError = 0;
	
// When did the single step test, the probe "CODE RUNNER" will disable interrupt.
#ifdef CONFIG_NOT_USING_TIME_OUT
	int timerISR_Enabled = lib.clock.IsTimerInterruptEnabled();
	lib.clock.DisableTimerInterrupt();
	lError = TI_066_AV_Test();
	if (timerISR_Enabled) {
		lib.clock.EnableTimerInterrupt();
	}
#else
	U32 (*pf)() = &TI_066_AV_Test;
	int timeOut = 25;	// unit:1s, => 25 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif	
	
	
	return lError;
}
