#ifndef TIAPI_FRONTEND_H_HEADER_INCLUDED_B9049D3D
#define TIAPI_FRONTEND_H_HEADER_INCLUDED_B9049D3D

#include "BltcBasicType.h"
#include "TiApi_DigAV.h"
#include "BhlDemod.h"

//##ModelId=46EDE13601C7
class TiApi_FrontEnd
{
  public:
    //##ModelId=472C5E7400DB
	class RFU
	{
	  public:

        //##ModelId=472C68E002A1
	    int RFU_Init();

        //##ModelId=472C5E740282
	     U32 Calibration(U32 freq, U32 powerLevelDbmv, float *foffset);

        //##ModelId=472C5E740292
	     U32 TestPower(U32 freq, U32 powerLevelDbmv);

        //##ModelId=472C5E7402A1
	     U32 ClosePower();

        //##ModelId=472C5E7402A2
	     U32 SaveOffsetToNvsram(float offset);

        //##ModelId=472C5E7402B0
	     U32 SaveOffsetToPFD(float offset);
	};
	
    //##ModelId=472C54F201F4
	class CLink
	{
	   public:
	   
        //##ModelId=472C54F2039A
	    U32  DeviceProperty_Identify(void);
        //##ModelId=472C54F2039C
	    U32  ContinuousOutput_ChannelSet(U16 CO_ChNum);
        //##ModelId=472C54F203AB
	    U32  ContinuousWave_Test(U16 CW_ChNum);  

	};
	
    //##ModelId=48117B4E0117
    CLink entropic;

//	tuneMode=0 => Digital, tuneMode=1 => Analog
    //##ModelId=48117B4E0135
    U32 QAM_Set_Brcm(int tuner, U32 freqHz, int qamType, U32 tuneMode);
    //##ModelId=48117B4E03D5
//	U32 QAM_GetLockStatus_Brcm(int tuner);
	
    //##ModelId=46EDF2550156
    U32 QAM_SetFreq(int tuner, U32 freq);

    //##ModelId=46EDF2550184
    U32 QAM_SetChan(int tuner, int chan);

    //##ModelId=46EDF7E10253
    U32 QAM_SetPcrPids(int tuner, PcrPids pcrPids);

    //##ModelId=46EDF25501A4
    U32 QAM_256Acq(int tuner);

    //##ModelId=46EDF25501D3
//    U32 QAM_256LockStatus(int tuner);

    //##ModelId=46EDF2550201
    U32 QAM_64Acq(int tuner);

    //##ModelId=46EDF2550230
    U32 QAM_LockStatus(int tuner);
    U32 QAM_GetAllLockStatus(int tuner, etHalDemodIbStatus* demodIbStatus);

    //##ModelId=46F07A3C00FD
    U32 OOB_Lock(U32 rfFreqHz);

    //##ModelId=46EB46930307
    U32 VCXO_Test(U8 selFreqHz);
    
    U32 StartFactoryClkTest();
    U32 StopFactoryClkTest();
};



#endif /* TIAPI_FRONTEND_H_HEADER_INCLUDED_B9049D3D */
