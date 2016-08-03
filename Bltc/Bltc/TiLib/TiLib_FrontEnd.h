#ifndef TILIB_FRONTEND_H_HEADER_INCLUDED_B904A2D5
#define TILIB_FRONTEND_H_HEADER_INCLUDED_B904A2D5

#include "BltcBasicType.h"
#include "RfUpstream.h"
#include "TiLib_FrontEnd_CLink.h"


//##ModelId=46EBB8E0027A
class TiLib_FrontEnd
{
  public:
    //##ModelId=472C556C00AE
	class RFU : public RfUpstreamTransmitter
	{
	  public:
        //##ModelId=472C556C02A2
	     eRfUpstreamError_t SetFrequency(U32 ulSetFreqKhz);

        //##ModelId=472C556C02B1
	     eRfUpstreamError_t SetPowerLevel(U32 fPowerDbmv);

        //##ModelId=472C556C02B3
	     eRfUpstreamError_t GetPowerLevel(U32 &fPowerDbmv);

        //##ModelId=472C556C02B5
	     eRfUpstreamError_t Calibrate(float *offset);

        //##ModelId=472C556C02C2
	     int ReadPowerFromPowerMeter(U32 freq, U32* powerLevelDbm);

        //##ModelId=472C556C02D1
	     int CheckPower(U32 freq, U32 powerLevelDbm);

	};

    //##ModelId=472C556C00AF
	class CLink
	{
	   public:         
        //##ModelId=472C556C02F0
	       U32 CLink_Device_Identify(U16 &pciVendorId, U16 &chipVersion);                 
        //##ModelId=472C556C02FF
	       U32 CLink_CO_Channel_Tune(U16 CO_ChNum);       
        //##ModelId=472C556C0301
	       U32 CLink_CW_Channel_Tune(U16 CW_ChNum);     
	};
    //##ModelId=472C556C0312
    CLink entropic;

    //##ModelId=472A819C0289
	void OOB_FrequencySet(U32 rfFreq);
    //##ModelId=472A819C0299
	U32 OOB_Acq(void); 
    //##ModelId=472A819C029B
	bool OOB_IsLock();
	
    U32 EnableFactoryClkTest();
    U32 DisableFactoryClkTest();
};

#endif /* TILIB_FRONTEND_H_HEADER_INCLUDED_B904A2D5 */
