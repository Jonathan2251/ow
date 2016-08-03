#include "TiApi_DigAV.h"
#include "TiApi.h"
#include "TiLib.h"

extern "C" void DigAudDec_Start(long lChan);

#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
	U32 QAM_lock_delay_trace = 0;
#endif	// CONFIG_DELAY_TRACE

//##ModelId=481125BE018E
U32 TiApi_DigAV::CreateTransportStream(TS_Msg ts)
{
	int i;
	U32 lError = 1;
	
	if ( (ts.qamType != QAM64 && ts.qamType != QAM256) || 
		 (ts.tuner != TUNER1 && ts.tuner != TUNER2) ) {
		return (lError = 1);
	}
	if (ts.path == ROUTE_TO_MCARD) {
		lib.mCard.DoInit();
	}
	api.frontEnd.QAM_SetFreq(ts.tuner, ts.freq);
//	api.frontEnd.QAM_64Acq(ts.tuner);
//	lError = api.frontEnd.QAM_LockStatus(ts.tuner);
	for (i = 0; i < 5 && lError > 0; i++) {
		if (ts.qamType == QAM64) {
			api.frontEnd.QAM_64Acq(ts.tuner);
		}
		else if (ts.qamType == QAM256) {
			api.frontEnd.QAM_256Acq(ts.tuner);
		}
		lib.DelayMS(500);
	#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
		QAM_lock_delay_trace += 100*1000;
	#endif	// CONFIG_DELAY_TRACE
		lError = api.frontEnd.QAM_LockStatus(ts.tuner);
	}
	if (lError) {
		PDEBUG("QAM Lock fail!!\r\n");
		goto out;
	}
	// After QAM_SetPcrPids() the audio is open
	lib.digAV.VIDEO_SelSrc(ts.tuner, ts.pcrPids.pcr, ts.pcrPids.vPids);
	
	// Enable Composite and Component interface
    lib.digAV.VIDEO_EnableCompositeAndComponent(ts.tuner);
	lib.digAV.VIDEO_SetComponetFormatTo480I(ts.tuner);
//	No Audio_SelSrc() since in PIP only need call it once.
//	lib.digAV.AUDIO_SelSrc(ts.tuner, ts.pcrPids.pcr, ts.pcrPids.aPids);
	if (ts.path == BYPASS_MCARD) {
		lib.digAV.BypassCableCard();
	}
	else if (ts.path == ROUTE_TO_MCARD) {
		lib.digAV.RouteToCableCard();
		api.mCard.PowerOn();
	}

out:
	return lError;
}

#if 0
U32 TiApi_DigAV::CreateTransportStream(TS_Msg ts)
{
	int i;
	U32 lError = 0;
	
	if ( (ts.qamType != QAM64 && ts.qamType != QAM256) || 
		 (ts.tuner != TUNER1 && ts.tuner != TUNER2) ) {
		return (lError = 1);
	}
	if (ts.path == ROUTE_TO_MCARD) {
		lError = api.mCard.Init();
		if (lError) {
			PDEBUG("Missing M Card\r\n");
			goto out;
		}
		lError = api.mCard.PowerOn();
	    lib.DelayMS(10);
	    lib.mCard.Reset();
//		lError = lib.mCard.OpenSession();
		if (lError) {
			PDEBUG("Missing M Card\r\n");
			goto out;
		}
	}
	api.frontEnd.QAM_Set_Brcm(ts.tuner, ts.freq, ts.qamType, (U32)0);	// tuneMode=0 => Digital, tuneMode=1 => Analog
	lError = api.frontEnd.QAM_GetLockStatus_Brcm(ts.tuner);
	for (i = 0; i < 5 && lError > 0; i++) {
		lError = api.frontEnd.QAM_GetLockStatus_Brcm(ts.tuner);
		lib.DelayMS(100);
	#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
		QAM_lock_delay_trace += 100*1000;
	#endif	// CONFIG_DELAY_TRACE
	}
	if (lError) {
		PDEBUG("QAM Lock fail!!\r\n");
		goto out;
	}
	// After QAM_SetPcrPids() the audio is open
	lib.digAV.VIDEO_SelSrc(ts.tuner, ts.pcrPids.pcr, ts.pcrPids.vPids);
	
	// Enable Composite and Component interface
    lib.digAV.VIDEO_EnableCompositeAndComponent(ts.tuner);
	lib.digAV.VIDEO_SetComponetFormatTo480I(ts.tuner);
//	No Audio_SelSrc() since in PIP only need call it once.
//	lib.digAV.AUDIO_SelSrc(ts.tuner, ts.pcrPids.pcr, ts.pcrPids.aPids);
	if (ts.path == BYPASS_MCARD) {
		lib.digAV.BypassCableCard();
	}
	else if (ts.path == ROUTE_TO_MCARD) {
		lib.digAV.RouteToCableCard();
	}

out:
	return lError;
}
#endif

//##ModelId=46EB4693021D
U32 TiApi_DigAV::PIP_CreateTransportStream(TS_Msg tuner1, TS_Msg tuner2)
{
	U32 lError = 0;
	
	lError = CreateTransportStream(tuner2);
	if (lError) {
		return lError;
	}
	lError = CreateTransportStream(tuner1);
	if (lError) {
		return lError;
	}
	lib.digAV.VIDEO_PIPDisplay();
	
	return lError;
}

//##ModelId=46EB4693022C
U32 TiApi_DigAV::ChangeChanMap(ChanMap chanMap)
{
	U32 lError = 0;
	
	return lError;
}

//##ModelId=46EB4693023C
U32 TiApi_DigAV::OpenOutput(int flag)
{
	U32 lError = 0;
	
	if (flag & OIF_COMPONENT) {
		lib.digAV.OpenOutput_Component();
	}
	if (flag & OIF_COMPOSITE) {
		lib.digAV.OpenOutput_Composite();
	}
	if (flag & OIF_HDMI) {
		lib.digAV.OpenOutput_Hdmi();
	}
	if (flag & OIF_1394) {
//		lib.digAV.OpenOutput_1394();
	}
	if (flag & OIF_SVIDEO) {
		lib.digAV.OpenOutput_SVideo();
	}
	if (flag & OIF_RF) {
		lib.digAV.OpenOutput_RF();
	}
	if (flag & OIF_802_11) {
		lib.digAV.OpenOutput_802_11();
	}
	if (flag & OIF_ETHERNET) {
		lib.digAV.OpenOutput_Ethernet();
	}
	
	return lError;
}

//##ModelId=46EB4693024C
U32 TiApi_DigAV::OpenOutput_All()
{
	U32 lError = 0;
	
	return lError;
}

//##ModelId=4722F8510186
U32 TiApi_DigAV::CloseOutput(int flag)
{
	U32 lError = 0;
	
	if (flag & OIF_COMPONENT) {
		lib.digAV.CloseOutput_Component();
	}
	if (flag & OIF_COMPOSITE) {
		lib.digAV.CloseOutput_Composite();
	}
	if (flag & OIF_HDMI) {
		lib.digAV.CloseOutput_Hdmi();
	}
/*	if (flag & OIF_1394) {
		lib.digAV.CloseOutput_1394();
	}
	if (flag & OIF_SVIDEO) {
		lib.digAV.CloseOutput_SVideo();
	}
	*/
	if (flag & OIF_RF) {
		lib.digAV.CloseOutput_RF();
	}
/*	if (flag & OIF_802_11) {
		lib.digAV.CloseOutput_802_11();
	}
	if (flag & OIF_ETHERNET) {
		lib.digAV.CloseOutput_Ethernet();
	}*/
	
	return lError;
}

