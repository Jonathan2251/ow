#ifndef TILIB_DIGAV_H_HEADER_INCLUDED_B904B060
#define TILIB_DIGAV_H_HEADER_INCLUDED_B904B060

#include "BltcBasicType.h"
#include "TiApi_DigAV.h"

#include "bhdm.h"

#include "BhlVideoDisplay.h"	// For VidOut_EnablePip() define

enum {OIF_COMPONENT = 0x01, OIF_COMPOSITE = 0x02, OIF_HDMI = 0x04, OIF_1394 = 0x08, OIF_SVIDEO = 0x10, OIF_RF = 0x20, OIF_802_11 = 0x40, OIF_ETHERNET = 0x80, OIF_OPTICAL = 0x100, OIF_COAXIAL = 0x200};

// copy from audout.c
//##ModelId=48114CDC006D
enum AudMuteCmd
{
	AudioMute=0,
	AudioUnMute,
	AudioStatus,
	AudioMuteInvalid
};

//##ModelId=46D3918E00FF
class TiLib_DigAV
{
  private:
    //##ModelId=4722F8650168
  	BHDM_Handle _hHDMI;
  	  	
    //##ModelId=4722F86501A5
  	int _oif;
  	
    //##ModelId=48114CDC039A
  	int _latestStream;	// last stream created. For OpenOutput_1394().
  	
	// Modified from VidOut_EnablePip()
	//##ModelId=48114CDC03B9
    void VidOut_EnablePip(etDisplay eDisp, etSurface eSurf, etVideoSource eSrc);
	
  public:

    //##ModelId=48114CDD000F
	void Init();
	
    //##ModelId=48114CDD0010
	void VIDEO_EnableCompositeAndComponent(int stream);
	
    //##ModelId=48114CDD002F
    void VIDEO_SelSrc(int stream, U32 ulPcrPid, U32 ulVidPid);

    //##ModelId=48114CDD006D
	U32 AUDIO_SelSrc(int stream, U32 ulPcrPid, U32 ulAudPid);

    //##ModelId=46D3986401B4
    U32 AUDIO_SelSrc(int stream);

    //##ModelId=46DB74210384
    U32 AUDIO_OpenOutput_Optical();

    //##ModelId=46DB745003E2
    U32 AUDIO_OpenOutput_Coaxial();

    //##ModelId=46DB76BB0253
    U32 RouteToCableCard();
    
    //##ModelId=48114CDD00BB
    U32 BypassCableCard();

    //##ModelId=48114CDD00CB
	bool IsOpen_1394(int stream);
	
    //##ModelId=46D3992A0040
    U32 OpenOutput_Composite();

    //##ModelId=46D3994D00FB
    U32 OpenOutput_Component();

    //##ModelId=46D3995C005F
    U32 OpenOutput_Hdmi();

    //##ModelId=46D3997D0254
    U32 OpenOutput_1394();

    //##ModelId=46D399A70320
    U32 OpenOutput_SVideo();

    //##ModelId=46DB73E70364
    U32 OpenOutput_RF();

    //##ModelId=46DBA45B0291
    U32 OpenOutput_802_11();

    //##ModelId=46DBA48F0021
    U32 OpenOutput_Ethernet();
    
    //##ModelId=4722F8650222
    U32 CloseOutput_Composite();
    //##ModelId=4722F8650223
    U32 CloseOutput_Component();
    //##ModelId=4722F8650232
    U32 CloseOutput_Hdmi();
    //##ModelId=4722F8650233
    U32 CloseOutput_RF();
    
    //##ModelId=4722F8650242
    U32 VIDEO_SetComponetFormatTo480I(int stream);

    //##ModelId=48114CDD0119
	U32 AUDIO_Start1KTone(U32 stream);
	
    //##ModelId=48114CDD0138
	void VIDEO_PIPDisplay();
};



#endif /* TILIB_DIGAV_H_HEADER_INCLUDED_B904B060 */
