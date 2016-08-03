#include "TiLib_DigAV.h"
#include "TvMonIO.h"
#include "TiLib.h"

#include "audvid.h"
#include "BhlVideoDisplay.h"
#include "remod.h"

#include "bCmdChanMgr.h"	// For VIDEO_SelSrc_BrcmMenu
#include "brfm.h"

static BFMT_VideoFmt s_eVideoFmt = BFMT_VideoFmt_e1080i;

extern AudMuteCmd AudMute(AudMuteCmd Audcmd);
extern VOID SelectAudioSource(VOID);
extern "C" void DVI4Digital(void);
extern "C" void BLTC_SetComponetFormatTo480I(int stream);
extern "C" void VidOut_Disable
(
    etDisplay       eDisp,
    etSurface       eSurf
);
extern "C" void VidOut_Enable
(
    etDisplay       eDisp,
    etSurface       eSurf,
    etVideoSource   eSrc
);
extern BHDM_Handle hHDMI;
extern BRFM_Handle shRfm;
extern BOOL sbRemodState;

//##ModelId=48114CDD000F
void TiLib_DigAV::Init()
{
	char *remod_init[] = {
		"/",		// exit doCcard()
		"\0"};
		
	lib.tvMonCmd.RunCmdsInMenu(remod_init, doRemod);	// Remod menu
	_oif = 0;
}

// Enable Composite and Component interface
//##ModelId=48114CDD0010
void TiLib_DigAV::VIDEO_EnableCompositeAndComponent(int stream)
{
	if (stream == TUNER1) {
	// eDisplay0 : Composite
	// eDisplay1 : Component
    	VidOut_Disable( eDisplay0, eSurfaceVideo2);
    	VidOut_Disable( eDisplay1, eSurfaceVideo2);
	    VidOut_Enable( eDisplay0, eSurfaceVideo1, eVideoSourceMpegChannel0 );
	    VidOut_Enable( eDisplay1, eSurfaceVideo1, eVideoSourceMpegChannel0 );
    }
    else if (stream == TUNER2) {
	    VidOut_Disable( eDisplay0, eSurfaceVideo1);
    	VidOut_Disable( eDisplay1, eSurfaceVideo1);
    	VidOut_Enable( eDisplay0, eSurfaceVideo2, eVideoSourceMpegChannel1 );
    	VidOut_Enable( eDisplay1, eSurfaceVideo2, eVideoSourceMpegChannel1 );
    }
}

// Follows modified from DigVidDec_Start() which is Broadcom Menu.
//##ModelId=48114CDD002F
void TiLib_DigAV::VIDEO_SelSrc(int stream, U32 ulPcrPid, U32 ulVidPid)
{
	U32 ulPipe = (U32)(stream - 1);
    U32    ulVidType = 0;
    
    BRCM_DBG_OUT_NO_NEW_LINE(( "Enter the video type: 0 for Mpeg2, 1 for AVC, 2 for VC1 ==> " ));

    ChanStopDecode( ulPipe );
    
    ChanStartDecode( ulPipe, ulPcrPid, ulVidPid, (int)0xFFFF, 
                     (ulVidType == 0) ? HAL_VIDEO_DECODER_STREAM_TYPE_MPEG2
                                      : HAL_VIDEO_DECODER_STREAM_TYPE_AVC,
                     (etHalAudioDecoderStreamType)0 );

    //-----------------------------------------------------------------
    // Start Video Output
    //-----------------------------------------------------------------
    ChanEnableVideoOutput( ulPipe );
    
    //-----------------------------------------------------------------
    // Start Pcr/Time Base.
    //-----------------------------------------------------------------
    ChanConfigPcr( ulPipe, ulPcrPid, ulVidPid );
    _latestStream = stream;
}

// Follows for TiLib_DigAV::AUDIO_SelSrc_BrcmMenu()
#include "bCmdDigAudDec.h"
/*--------------------------------------------------------------------------
 * Platform specific includes
 *--------------------------------------------------------------------------*/
#include "BhlAudioDecoder.h"
#include "BhlAudioOutput.h"
#define INVALID_PID         0xFFFF	// copy from bcmhaldemux.h
    #define BXPT_P_MAX_PID_CHANNELS         ( 255 ) 
#define PID_CHANNEL_AUDIO_0     (BXPT_P_MAX_PID_CHANNELS - 2)
#define PID_CHANNEL_AUDIO_1     (BXPT_P_MAX_PID_CHANNELS - 5)
extern "C" {
	etGIerror DemuxAudioVideoStop(U32 ulPipe);
	etGIerror DemuxAudioVideoSetPid
	(
	    U32     ulPipe,
	    U32     ulPcrPid,
	    U32     ulVidPid,
	    U32     ulAudPid
	);
	etGIerror DemuxAudioVideoStart(U32 ulPipe);
}

// Modify from DigAudDec_Start() which is Broadcom Menu;
//##ModelId=48114CDD006D
U32 TiLib_DigAV::AUDIO_SelSrc(int stream, U32 ulPcrPid, U32 ulAudPid)
{
/*    sbRemodState = TRUE;
    BRFM_EnableRfOutput(shRfm);
    // RFM driver was initialized already
    VDEVideoOutRFMSourceSet(eDisplay1);
//    lib.DelayMS(4000);*/
	
	
    S32 lChan = (S32)(stream - 1);
    S32 lPcrPid = (S32)ulPcrPid;
    S32 lAudPid = (S32)ulAudPid;
    
    BRCM_DBG_OUT_NO_NEW_LINE(( "Enter the pcr and audio pids number in hex ==> " ));
    
    if( (lPcrPid >= 0) && (lPcrPid < 0x0000FFFF) && 
        (lAudPid >= 0) && (lAudPid < 0x0000FFFF))
    {
        /* Make sure to stop the previous channel */
    	AudioDecoderStop( (etHalAVdecoderChannel)lChan );	
        
        DemuxAudioVideoStop( lChan );

        DemuxAudioVideoSetPid( lChan, lPcrPid, INVALID_PID, lAudPid );
        DemuxAudioVideoStart( lChan );
#if 1
   //-----------------------------------------------------------------
    // Start Audio
    //-----------------------------------------------------------------
    if( lAudPid != INVALID_PID )
    {
        /* Configure Audio mixer and output before audio start */
        if (lChan == 0)
        {
            /* Only digital channel goes to FMM0 DAC and SPDIF outputs */
            AudMixerInputAdd (HAL_AUDOUT_MIXER0, HAL_AUDMIXER_OUTPUT_DAC, HAL_AUDMIXER_INPUT_DIGITAL, true);
            AudMixerInputAdd (HAL_AUDOUT_MIXER0, HAL_AUDMIXER_OUTPUT_SPDIF, HAL_AUDMIXER_INPUT_DIGITAL, true);

            /* Send FMM0 to DAC output. Unmute output */
            AudOutDacSourceSet(0, HAL_AUDOUT_MIXER0);
            AudMixerInputMuteSet (HAL_AUDOUT_MIXER0, HAL_AUDMIXER_INPUT_DIGITAL, false);
        }else
        {
            /* Only digital channel goes to FMM1 DAC and SPDIF outputs */
            AudMixerInputAdd (HAL_AUDOUT_MIXER1, HAL_AUDMIXER_OUTPUT_DAC1, HAL_AUDMIXER_INPUT_DIGITAL, true);
            AudMixerInputAdd (HAL_AUDOUT_MIXER1, HAL_AUDMIXER_OUTPUT_SPDIF, HAL_AUDMIXER_INPUT_DIGITAL, true);

            /* Send FMM1 to DAC output. Unmute digital input to FMM1 to output ADP */
            AudOutDacSourceSet(1, HAL_AUDOUT_MIXER1);
            AudMixerInputMuteSet (HAL_AUDOUT_MIXER1, HAL_AUDMIXER_INPUT_DIGITAL, false);

        }
        AudioDecoderSetDownMixMode( (etHalAVdecoderChannel)lChan, HAL_AUDIO_DECODER_OUTPUT_LEFT_RIGHT );
        AudioDecoderStart( (etHalAVdecoderChannel)lChan, 
                            (U16)lPcrPid, 
                            (U16)lAudPid, 
                            (lChan==0)? PID_CHANNEL_AUDIO_0: PID_CHANNEL_AUDIO_1, 
                            HAL_AUDIO_DECODER_STREAM_TYPE_AC3,   /* hard code for now -jh */
                            NULL, 
                            (void *)0, 
                            FALSE );
    }
	    
#else /* old code */	    
        
	    /* Set timebase PCR and PCR_OFFSET */
    	switch( lChan )
	    {
        	case 0:
    	        bcmTimebaseSourceSet((etTimebase) lChan, BXPT_PCR_TimeRef_eXpt, eIB0);  
	            bcmTimebaseConfigXptPCR((etTimebase) lChan); 
    	        bcmTimebaseSetPid((etTimebase) lChan, lPcrPid);
    	        if(DmxPidChanIsEnabled(PID_CHANNEL_PCR_0))
				    bcmTimebasePcrOffsetSetPidChannel((etTimebase)lChan, PID_CHANNEL_PCR_0);   	            	            	        
				else
				    bcmTimebasePcrOffsetSetPidChannel((etTimebase)lChan, PID_CHANNEL_VIDEO_0);   	            	            	        				
        	    break;
	        case 1:
    	        bcmTimebaseSourceSet( (etTimebase) lChan, BXPT_PCR_TimeRef_eXpt, eIB1);  
        	    bcmTimebaseConfigXptPCR((etTimebase) lChan);                      
            	bcmTimebaseSetPid((etTimebase) lChan, lPcrPid);
    	        if(DmxPidChanIsEnabled(PID_CHANNEL_PCR_1))
				    bcmTimebasePcrOffsetSetPidChannel((etTimebase)lChan, PID_CHANNEL_PCR_1);   	            	            	        
				else
				    bcmTimebasePcrOffsetSetPidChannel((etTimebase)lChan, PID_CHANNEL_VIDEO_1);   	            	            	        				
	            break;
	   }

    	
    	AudioDecoderSetDownMixMode( (etHalAVdecoderChannel)lChan, HAL_AUDIO_DECODER_OUTPUT_LEFT_RIGHT );
#if 1  /* Change fro CSP113805:Add audio playback support for DCX/QIPH TvMon */
    	AudioDecoderStart( (etHalAVdecoderChannel)lChan, 
    						(U16)lPcrPid, 
    						(U16)lAudPid, 
    						(lChan==0)? PID_CHANNEL_AUDIO_0: PID_CHANNEL_AUDIO_1, 
    						eAudType,
    						NULL, 
    						(void *)0, 
    						FALSE );
#endif    	
#endif    	
	    //bcmTimebaseBroadcastSTC(lChan, lPcrPid);
    }
    

    // Set volumn since it's different between TvMon10.10 and TvMon10.05
    // Refer following which came from bCmdChanMgr.c
    // 0802-0527  TVMon QIP7K/DCX3K - Truncation of audio for weak input levels
    // 0803-0015  MTC DCX3K - Baseband Audio Level Slightly Low
    AudOutDacVolumeSet(0, 500);
    AudOutDacVolumeSet(1, 500);
    
//    AudOutDacVolumeSet(0, 0);	// to maximum, but will be cut into triaugular shape in TvMon12.21c
//    AudOutDacVolumeSet(1, 0);
        
    return 0;
}

//##ModelId=46D3986401B4
U32 TiLib_DigAV::AUDIO_SelSrc(int stream)
{
	char *select_audio_source[] = {
//		"aud",  	// enter from TvMon menu to audio menu
//		"0",		// [0] Select the Audio Source
		"0",		// [0] Digital Path 1
		"/", 		// exit [0] Select the Audio Source
//		"/", 		// exit audio menu
//		"exit",		// exit TvMon menu
		"\0"};
		
	U32 lError = 0;
		
// copy from AudioOutputTests() of audout.c
	BOOL sbAudioInit=FALSE;

	/*
	 * Initialize the audio paramters
	*/
	if (sbAudioInit==FALSE)
	{
		AudMute(AudioUnMute);
		
		sbAudioInit = TRUE;
	}
	
	sprintf(select_audio_source[0], "%d", stream-1);
	tvMonIO_clear();
	tvMonIO_in_addCmds(select_audio_source);
	SelectAudioSource();
	
	return lError;
}

#include "ccard.h"

//##ModelId=46DB76BB0253
U32 TiLib_DigAV::RouteToCableCard()
{
	U32 lError = 0;
	char *route_to_mcard[] = {
	    "t",        // [t] Enable/Bypass M-Card Transport Routing
	    "1",        // 1:  Route X-ports through M-Card interface
		"/",		// exit doCcard()
		"\0"};
//	char* p;

//	tvMonIO.out.enable = 1;
	lib.tvMonCmd.RunCmdsInMenu(route_to_mcard, doCcard);	// mhif menu
//	tvMonIO.out.enable = 0;
/*		
	p = strstr(tvMonIO.out._ostr, "Unit Address 0x");//Card Version 01.47
	if (p != 0) {
		lError = 0;
	}
	else {
		lError = 1;
	}
*/	
	return lError;
}

 //##ModelId=48114CDD00BB
U32 TiLib_DigAV::BypassCableCard()
{
	U32 lError = 0;
	char *route_to_mcard[] = {
	    "t",        // [t] Enable/Bypass M-Card Transport Routing
	    "2",        // 2:  Bypass M-Card interface
		"/",		// exit doCcard()
		"\0"};

	lib.tvMonCmd.RunCmdsInMenu(route_to_mcard, doCcard);	// mhif menu
	
	return lError;
}


extern VOID DisplaySourceMenu();

//##ModelId=48114CDD00CB
bool TiLib_DigAV::IsOpen_1394(int stream) {
/*	char *is_1394_enable[] = { 
		"video", 	// enter from TvMon menu to video menu
		"\t0", 		// [0] Select the Video Source : [Current NO VIDEO SRC Src Selected]
		"/",		// exit to Video Source
		"/",		// exit video menu
		"exit",		// exit TvMon menu
		"\0"};*/
	char answer[100] = "";
	bool enabled;
		
//	runCmds(is_1394_enable);
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	DisplaySourceMenu();
	tvMonIO.out.enable = 0;
	
	if (stream == TUNER1) {
		tvMonIO_out_getResultN(tvMonIO.out._ostr, "[A] Toggle RMX PORT 0 1394 (Transmit) Path 1 Source   [Currently : ", answer, strlen("Enabled"));
	}
	else if (stream == TUNER2) {
		tvMonIO_out_getResultN(tvMonIO.out._ostr, "[B] Toggle RMX PORT 0 1394 (Transmit) Path 2 Source   [Currently : ", answer, strlen("Enabled"));
	}
	else {
		enabled = false;
		goto out;
	}
	if (strcmp(answer, "Enabled") == 0) {
		enabled = true;
	}
	else {
		enabled = false;
	}
	
out:
	return enabled;
}

//##ModelId=46DB74210384
U32 TiLib_DigAV::AUDIO_OpenOutput_Optical()
{
	U32 lError = 0;
	
	if (_oif & OIF_OPTICAL) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	_oif |= OIF_OPTICAL;
	
	return lError;
}

//##ModelId=46DB745003E2
U32 TiLib_DigAV::AUDIO_OpenOutput_Coaxial()
{
	U32 lError = 0;
		
	if (_oif & OIF_COAXIAL) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	_oif |= OIF_COAXIAL;
	
	return lError;
}

// OpenOutput_Composite() allows to do twice, immediate repeatedly
//##ModelId=46D3992A0040
U32 TiLib_DigAV::OpenOutput_Composite()
{
	U32 lError = 0;
		
	if (_oif & OIF_COMPOSITE) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	VDEVideoOutStateSet(eVideoOutPutComposite, true);
	_oif |= OIF_COMPOSITE;
	
	return lError;
}

// OpenOutput_Component() allows to do twice, immediate repeatedly
//##ModelId=46D3994D00FB
U32 TiLib_DigAV::OpenOutput_Component()
{
	U32 lError = 0;

	if (_oif & OIF_COMPONENT) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	VDEVideoOutStateSet(eVideoOutPutComponent, true);
	_oif |= OIF_COMPONENT;
	
	return lError;
}

// It's work
//##ModelId=46D3995C005F
U32 TiLib_DigAV::OpenOutput_Hdmi()
{
	U32 lError = 0;
			
	if (_oif & OIF_HDMI) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	DVI4Digital();
	_oif |= OIF_HDMI;
	
	return lError;
}

//##ModelId=46D3997D0254
U32 TiLib_DigAV::OpenOutput_1394()
{
	char *enable_1394_path1[] = { 
//		"video", 	// enter from TvMon menu to video menu
		"0", 		// [0] Select the Video Source : [Current NO VIDEO SRC Src Selected]
		"A", 		// [A] Toggle 1394 HSX PORT 0 (Transmit) Path 1 Source
		"/",		// exit to Video Source
		"/",		// exit video menu
//		"exit",		// exit TvMon menu
		"\0"};
	char *enable_1394_path2[] = { 
//		"video", 	// enter from TvMon menu to video menu
		"0", 		// [0] Select the Video Source : [Current NO VIDEO SRC Src Selected]
		"B", 		// [B] Toggle 1394 HSX PORT 0 (Transmit) Path 1 Source
		"/",		// exit to Video Source
		"/",		// exit video menu
//		"exit",		// exit TvMon menu
		"\0"};
	U32 lError = 0;

	if ( !IsOpen_1394(_latestStream) ) {
		if (_latestStream == TUNER1) {
			lib.tvMonCmd.RunCmdsInMenu(enable_1394_path1, doAudioVideo);
		}
		else if (_latestStream == TUNER2) {
			lib.tvMonCmd.RunCmdsInMenu(enable_1394_path2, doAudioVideo);
		}
		else {
			lError = 1;
			goto out;
		}
		if ( !IsOpen_1394(_latestStream) ) {
			lError = 1;
		}
	}
out:
	
	return lError;
}

//##ModelId=46D399A70320
U32 TiLib_DigAV::OpenOutput_SVideo()
{
	U32 lError = 0;
			
	if (_oif & OIF_SVIDEO) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	_oif |= OIF_SVIDEO;
	
	return lError;
}

#include "bcmsysinfo.h"	// For tSysInfo
#include "brfm.h"	// For BRFM_Settings

static  BCHP_Handle s_hChip74xx;
static  BREG_Handle s_hReg74xx;
static  BINT_Handle s_hInt74xx;
static  BRFM_Handle s_hRfm;

// Modify from RemodEnableToggle() of bCmdVidOut.c
// OpenOutput_RF() allows to do twice, immediate repeatedly
U32 TiLib_DigAV::OpenOutput_RF()
{
    if (_oif & OIF_RF) {
    	return 0;
    }
    
    tSysInfo        *pSysInfo = bcmGetSysInfo();
    BRFM_Settings RFM_Settings;

	s_hReg74xx = pSysInfo->hReg74xx;
    s_hInt74xx = pSysInfo->hInt74xx;	
    s_hChip74xx = pSysInfo->hChip74xx;       
    
    BRFM_GetDefaultSettings(&RFM_Settings ,s_hChip74xx);

    // ntsc cable, channel 3, stereo, 18db
    BRFM_Open(&s_hRfm, s_hChip74xx, s_hReg74xx, s_hInt74xx, &RFM_Settings); 

    BRFM_EnableRfOutput(s_hRfm);    
    
    // RFM driver was initialized already
    VDEVideoOutRFMSourceSet(eDisplay1);
    
    // Set volumn since it's different between TvMon10.10 and TvMon10.05
	S8   scVolume = 30;
	BRFM_SetAudioVolume( s_hRfm,scVolume );
	
    _oif |= OIF_RF;
    
	return 0;
}

//##ModelId=46DBA45B0291
U32 TiLib_DigAV::OpenOutput_802_11()
{
	U32 lError = 0;
			
	if (_oif & OIF_802_11) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	_oif |= OIF_802_11;
	
	return lError;
}

//##ModelId=46DBA48F0021
U32 TiLib_DigAV::OpenOutput_Ethernet()
{
	U32 lError = 0;
	
	if (_oif & OIF_ETHERNET) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	_oif |= OIF_ETHERNET;
	
	return lError;
}


// CloseOutput_Composite() allows to do twice, immediate repeatedly
//##ModelId=4722F8650222
U32 TiLib_DigAV::CloseOutput_Composite()
{
	U32 lError = 0;

	if (!(_oif & OIF_COMPOSITE)) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	VDEVideoOutStateSet(eVideoOutPutComposite, false);
	_oif &= ~(OIF_COMPOSITE);
	
	return lError;
}

// CloseOutput_Component() allows to do twice, immediate repeatedly
//##ModelId=4722F8650223
U32 TiLib_DigAV::CloseOutput_Component()
{
	U32 lError = 0;

	if (!(_oif & OIF_COMPONENT)) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	VDEVideoOutStateSet(eVideoOutPutComponent, false);
	_oif &= ~(OIF_COMPONENT);
	
	return lError;
}

// It's work
//##ModelId=4722F8650232
U32 TiLib_DigAV::CloseOutput_Hdmi()
{
	U32 lError = 0;
			
	if (!(_oif & OIF_HDMI)) {
		return (lError = 0);	// Optical output interface has been opend already
	}
	    
	BHDM_DisableDisplay(hHDMI);
	_oif &= ~(OIF_HDMI);
	
	return lError;
}

// Not work.
//##ModelId=4722F8650233
U32 TiLib_DigAV::CloseOutput_RF()
{    
    if (!(_oif & OIF_RF)) {
    	return 0;
    }

    tSysInfo        *pSysInfo = bcmGetSysInfo();

	s_hReg74xx = pSysInfo->hReg74xx;
    s_hInt74xx = pSysInfo->hInt74xx;	
    s_hChip74xx = pSysInfo->hChip74xx; 
    
    BRFM_DisableRfOutput(s_hRfm);
    BRFM_Close(s_hRfm);

	_oif &= ~(OIF_RF);
	
	return 0;
}

//##ModelId=4722F8650242
U32 TiLib_DigAV::VIDEO_SetComponetFormatTo480I(int stream)
{
	U32 lError = 0;
	
	BLTC_SetComponetFormatTo480I(stream);

	return lError;
}

#include "BhlAudioOutput.h"

//##ModelId=48114CDD0119
U32 TiLib_DigAV::AUDIO_Start1KTone(U32 stream)
{
	if (stream == TUNER1) {
		AudMixerInputAdd (HAL_AUDOUT_MIXER0, 
							HAL_AUDMIXER_OUTPUT_DAC, 
							HAL_AUDMIXER_INPUT_DIGITAL,
							true);
            AudMixerInputAdd (HAL_AUDOUT_MIXER0, HAL_AUDMIXER_OUTPUT_SPDIF, HAL_AUDMIXER_INPUT_DIGITAL, true);
	    AudioTestToneStart(HAL_AUDMIXER_OUTPUT_DAC0);
	}
	else if (stream == TUNER2) {
		AudMixerInputAdd (HAL_AUDOUT_MIXER1, 
							HAL_AUDMIXER_OUTPUT_DAC1, 
							HAL_AUDMIXER_INPUT_DIGITAL,
							true);
            AudMixerInputAdd (HAL_AUDOUT_MIXER1, HAL_AUDMIXER_OUTPUT_SPDIF, HAL_AUDMIXER_INPUT_DIGITAL, true);
	    AudioTestToneStart(HAL_AUDMIXER_OUTPUT_DAC1);
	}
	
	return 0;
}

// This function enable the video pip output
//##ModelId=48114CDC03B9
void TiLib_DigAV::VidOut_EnablePip	// Modified from VidOut_EnablePip()
(
    etDisplay       eDisp,
    etSurface       eSurf,
    etVideoSource   eSrc
)
{
    VideoSurfaceConfig_t    SurfaceConfig;
    DispParam_t             DispPara;
    
    //-----------------------------------------------------------------
	// Get the dislay parameters
    //-----------------------------------------------------------------
    VDEDisplayParametersGet( eDisp, &DispPara );            

    //-----------------------------------------------------------------
	// Set the source to display
    //-----------------------------------------------------------------
    SurfaceConfig.eSrc = eSrc;
	VDEVideoSurfaceConfigSet( eDisp, eSurf, &SurfaceConfig, true, false, false, false );

    //-----------------------------------------------------------------
	// Display the video by unmuting.
    //-----------------------------------------------------------------
    #if 0
	SurfaceConfig.tDestRect.uwXSize   = DispPara.ulWidth  / 5 * 2;
	SurfaceConfig.tDestRect.uwYSize   = DispPara.ulHeight / 5 * 2;
	SurfaceConfig.tDestRect.uwXOffset = DispPara.ulWidth - SurfaceConfig.tDestRect.uwXSize - 50;				
	SurfaceConfig.tDestRect.uwYOffset = 0 + (DispPara.ulHeight >> 3);;
	#else
	SurfaceConfig.tDestRect.uwXOffset = 0 + (DispPara.ulWidth  >> 3);				
	SurfaceConfig.tDestRect.uwYOffset = 0 + (DispPara.ulHeight >> 3);
	SurfaceConfig.tDestRect.uwXSize   = DispPara.ulWidth  / 3;
	SurfaceConfig.tDestRect.uwYSize   = DispPara.ulHeight / 3;
	#endif
    VDEVideoSurfaceConfigSet( eDisp, eSurf, &SurfaceConfig, false, false, true, false );	
	VDEDisplayVideoSurfaceUnMute( eDisp, eSurf );
}

//##ModelId=48114CDD0138
void TiLib_DigAV::VIDEO_PIPDisplay()
{
	// Follows modified from BcmVidOut_PIP().
    //-----------------------------------------------------------------
	// Enable main display.
    //-----------------------------------------------------------------
    VidOut_Disable( eDisplay0, eSurfaceVideo1 );
	VidOut_Disable( eDisplay1, eSurfaceVideo1 );
    VidOut_Enable(  eDisplay0, eSurfaceVideo1, eVideoSourceMpegChannel0 );
    VidOut_Enable(  eDisplay1, eSurfaceVideo1, eVideoSourceMpegChannel0 );
    
    //-----------------------------------------------------------------
	// Enable PIP display.
    //-----------------------------------------------------------------
    VidOut_Disable( eDisplay0, eSurfaceVideo2 );
    VidOut_Disable( eDisplay1, eSurfaceVideo2 );
    VidOut_EnablePip( eDisplay0, eSurfaceVideo2, eVideoSourceMpegChannel1 );
    VidOut_EnablePip( eDisplay1, eSurfaceVideo2, eVideoSourceMpegChannel1 );
}
