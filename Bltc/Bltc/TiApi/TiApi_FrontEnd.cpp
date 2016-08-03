#include "TiApi_FrontEnd.h"
#include "TiLib.h"

#include "userio.h"
#include "qamqpsk.h"
#include "audvid.h"
#include "audout.h"

#include "BhlDemod.h"	// For QAM_Set_Brcm()

#define	bool	unsigned int
#include "bqds_3250.h"	// For struct _tDEMOD_PRIV_DATA
#include "bqob_3250.h"	// For struct _tDEMOD_PRIV_DATA
#include "bqus_3250.h"	// For struct _tDEMOD_PRIV_DATA
#include "btnr_3250ob.h"	// For struct _tDEMOD_PRIV_DATA
#undef bool

#include "cputimer.h"
#include "NxpTuner.h"

// Copy from HalDemod_dct64p4.c
#define INBAND_MAX 2
extern "C" {
typedef //##ModelId=481136F70039
struct _tDEMOD_PRIV_DATA
{
    //##ModelId=481136F70039
    BTNR_Handle         hTunerIb[INBAND_MAX];
    //##ModelId=481136F70162
    BTNR_Handle         hTunerOob;

    //##ModelId=481136F701EF
    BQDS_Handle         hQamDs;
    //##ModelId=481136F701FE
    BQOB_Handle         hQamOob;
    //##ModelId=481136F701FF
    BQUS_Handle         hQamUs;
    //##ModelId=481136F7020E
    BQDS_ChannelHandle  hChanIb[INBAND_MAX];

    //##ModelId=481136F7021D
    BCHP_Handle         hChip325x;
    //##ModelId=481136F7022D
    BREG_Handle         hReg325x;
    //##ModelId=481136F7025C
    BINT_Handle         hInt325x;
    //##ModelId=481136F7025D
    BMEM_Handle         hMem;
    //##ModelId=481136F7027B
    BREG_I2C_Handle     hI2c;
    
    //##ModelId=481136F7029A
    unsigned long		aulIbFreq[INBAND_MAX]; /* store the freq for a band */
    //##ModelId=481136F702AA
    unsigned long       ulOobFreq;
}
tDEMOD_PRIV_DATA;
}

// Copy from BhlDemod_3520.c
#define BCM3520_DEMOD_1		NXP_TUNER_1		// Match up Demod 1 with Tuner 1
#define BCM3520_DEMOD_2		NXP_TUNER_2		// Match up Demod 2 with Tuner 2

extern etHalDemodIb gSelectedDemod;
extern VOID SetVideoAudioPids();

extern "C" unsigned long bcmDemodGetTuner( unsigned long ulInband );
extern "C" tDEMOD_PRIV_DATA s_DemodPrivData;

// tuneMode=0 => Digital, tuneMode=1 => Analog
//##ModelId=48117B4E0135
U32 TiApi_FrontEnd::QAM_Set_Brcm(int tuner, U32 freqHz, int qamType, U32 tuneMode)
{
	U32 ulBand, qamMode, symRate;
    etHalDemodLockStatus    eLockStatus;
	U32 lError = 0;
              
	if (tuner == TUNER1 || tuner == TUNER2) {
		ulBand = (U32)tuner;
	}
	else {
		return (lError = 1);
	}
    if( qamType == 0 )
    {
        qamMode = HAL_DEMOD_QAM_TYPE_64;
        symRate = 5056900;
    }
    else if( qamType == 1 )
    {
        qamMode = HAL_DEMOD_QAM_TYPE_256;
        symRate = 5365000;
    }
    else {
    	return (lError = 1);
    }
    ulBand = bcmDemodGetTuner( ulBand );
    DemodIbAcquire( (etHalDemodIb)ulBand,
                    freqHz,
                    0,
                    symRate,
                    (etHalDemodQamType)qamMode,
                    &eLockStatus, FALSE );
    
    if( eLockStatus == HAL_DEMOD_LOCK_STATUS_LOCKED )
    {
        return( 1 );
    }
    else
    {
        return( 0 );
    } 
	return lError;
}

//##ModelId=48117B4E03D5
// QIP7kP2
#include "bvsb.h"
// Modify from DemodIbLockStatusGet() of BhlDemod_3520.c
//extern "C" BVSB_Handle                hVSB[MAX_NXP_TUNERS];
/*
U32 TiApi_FrontEnd::QAM_GetLockStatus_Brcm(int tuner)
{
    BVSB_QamStatus    QamStatus;
    U32						lError;
                     
	if (tuner == TUNER1) {
        // Query the current QAM status
        BVSB_GetQamStatus(hVSB, &QamStatus);
        if (QamStatus.bFecLocked && QamStatus.bPllLocked)
	    	lError = 0;
        else
	    	lError = 1;
	}
    else {
    	lError = 1;
    }
    
    return lError;
}
*/

//##ModelId=46EDF2550156
U32 TiApi_FrontEnd::QAM_SetFreq(int tuner, U32 freqHz)
{
	char *set_tuner1_freq[] = {
		"1",
		"183000000", 		// set tuner 1 frequency to 183000000
		"0",				// [0] Exit qam menu
		"\0"};
	char *set_tuner2_freq[] = {
		"p",	// [P] Selected Demod Path, change to tuner 2 (Inband 2)
		"1",
		"183000000", 		// set tuner 1 frequency to 183000000
		"0",				// [0] Exit qam menu
		"\0"};
		
	U32 lError = 0;

	if (tuner == TUNER1) {
		sprintf(set_tuner1_freq[1], "%d", freqHz);
		lib.tvMonCmd.RunCmdsInMenu(set_tuner1_freq, doQAM);
	}
	else if (tuner == TUNER2) {
		sprintf(set_tuner2_freq[2], "%d", freqHz);
		lib.tvMonCmd.RunCmdsInMenu(set_tuner2_freq, doQAM);
	}
	else {
	  	  lib.rs232.Print("Error!!! TiApi_FrontEnd::QAM_SetFreq::tuner=%d\r\n", tuner);
	}
	return lError;
}

//##ModelId=46EDF2550184
U32 TiApi_FrontEnd::QAM_SetChan(int tuner, int chan)
{
	return 0;
}

//##ModelId=46EDF7E10253
U32 TiApi_FrontEnd::QAM_SetPcrPids(int tuner, PcrPids pcrPids)
{
	S8 *set_pcr_pids[] = {
		"8c0 8c0 8c1           ",
		"0",				// [0] Exit qam menu
		"\0"};

	U32 lError = 0;
	
	tvMonIO_clear();
	if (tuner == TUNER1) {
		gSelectedDemod = HAL_DEMOD_IB0;
	}
	else if (tuner == TUNER2) {
		gSelectedDemod = HAL_DEMOD_IB1;
	}
	else {
		lib.rs232.Print("Error!!! TiApi_FrontEnd::QAM_SetPcrPids::tuner=%d\r\n", tuner);
		return (lError = 1);
	}
	sprintf(set_pcr_pids[0], "%x %x %x", pcrPids.pcr, pcrPids.vPids, pcrPids.aPids);
	tvMonIO_in_addCmds(set_pcr_pids);
	SetVideoAudioPids();
	
	return lError;
}

//##ModelId=46EDF25501A4
U32 TiApi_FrontEnd::QAM_256Acq(int tuner)
{
	char *qam_acquire_tuner1[] = {
		"4",				// [4] QAM 256 acquisition
		"0",				// [0] Exit qam menu
		"\0"};
	char *qam_acquire_tuner2[] = {
		"p",				// [P] Selected Demod Path [Current Path  : Inband1], after "p" the Current Path => Inband2
		"4",				// [4] QAM 256 acquisition
		"0",				// [0] Exit qam menu
		"\0"};

	U32 lError = 0;
	tvMonIO_clear();
	if (tuner == TUNER1) {
		lib.tvMonCmd.RunCmdsInMenu(qam_acquire_tuner1, doQAM);
	}
	else if (tuner == TUNER2) {
		lib.tvMonCmd.RunCmdsInMenu(qam_acquire_tuner2, doQAM);
	}
	else {
	  	  lib.rs232.Print("Error!!! TiApi_FrontEnd::QAM_256Acq::tuner=%d\r\n", tuner);
	}
	return lError;
}

//##ModelId=46EDF25501D3
/*U32 TiApi_FrontEnd::QAM_256LockStatus(int tuner)
{
}*/

//##ModelId=46EDF2550201
U32 TiApi_FrontEnd::QAM_64Acq(int tuner)
{
	char *qam_acquire_tuner1[] = {
		"3",				// [3] QAM 64 acquisition
		"0",				// [0] Exit qam menu
		"\0"};
	char *qam_acquire_tuner2[] = {
		"p",				// [P] Selected Demod Path [Current Path  : Inband1], after "p" the Current Path => Inband2
		"3",				// [3] QAM 64 acquisition
		"0",				// [0] Exit qam menu
		"\0"};

	U32 lError = 0;

	tvMonIO_clear();
	if (tuner == TUNER1) {
		lib.tvMonCmd.RunCmdsInMenu(qam_acquire_tuner1, doQAM);
	}
	else if (tuner == TUNER2) {
		lib.tvMonCmd.RunCmdsInMenu(qam_acquire_tuner2, doQAM);
	}
	else {
	  	  lib.rs232.Print("Error!!! TiApi_FrontEnd::QAM_64Acq::tuner=%d\r\n", tuner);
	}

	return lError;
}

//##ModelId=46EDF2550230
U32 TiApi_FrontEnd::QAM_LockStatus(int tuner)
{
	U32 lError = 0;

	char *read_qam_status[] = {
		"\t5",	// [5] Read QAM Status, and "enable output for this command"
		"0", 	// [0] exit qam menu
		"\0"};

	char *p1, *p2;
	
	tvMonIO_clear();
	
	if (tuner != TUNER1 && tuner != TUNER2) {
		return (lError = 1);
	}
	tvMonIO.out.enable = 1;
	if (tuner == TUNER1) {
        DemodIbStatusDisplay (HAL_DEMOD_IB0);
	}
	else if (tuner == TUNER2) {
        DemodIbStatusDisplay (HAL_DEMOD_IB1);
	}
	else {
	  	  lib.rs232.Print("Error!!! TiApi_FrontEnd::QAM_LockStatus::tuner=%d\r\n", tuner);
	  	  return (lError = 0);
	}
	tvMonIO.out.enable = 0;
	p1 = strstr(tvMonIO.out._ostr, "FecLock=LOCKED");
	p2 = strstr(tvMonIO.out._ostr, "QamLock=LOCKED");
	if (p1 != 0 && p2 != 0) {
		lError = 0;
	}
	else {
		lError = 1;
	}
		
	return lError;
}

U32 TiApi_FrontEnd::QAM_GetAllLockStatus(int tuner, etHalDemodIbStatus* demodIbStatus)
{
	U32 lError = 0;
	
    /* Currently Supports only a Single Inband Channel (Single 3517 chip) */
    if (tuner == TUNER1)
    {
        /* Query the current QAM status */
		DemodIbStatusGet(HAL_DEMOD_IB0, demodIbStatus);
    }
    else if (tuner == TUNER2)
    {
    	/* Query the current QAM status */
		DemodIbStatusGet(HAL_DEMOD_IB1, demodIbStatus);
    }
    else 
    {
        lError = 1;
    }
    
    return lError;
}

//##ModelId=46F07A3C00FD
U32 TiApi_FrontEnd::OOB_Lock(U32 rfFreqHz)
{
//   U32 TestResult;
	bool lock;
	U32 lError = 0;
	int i;
	
	lib.frontEnd.OOB_FrequencySet(rfFreqHz);
	lib.frontEnd.OOB_Acq();
	for (i = 0; i < 10; i++) {
		if (lib.frontEnd.OOB_IsLock()) {
			return (lError = 0);
		}
		lib.DelayMS(100);
	}
	
	return (lError = 1);
}


//##ModelId=46EB46930307
U32 TiApi_FrontEnd::VCXO_Test(U8 selFreq)
{
	return 0;
}

U32 TiApi_FrontEnd::StartFactoryClkTest()
{
	U32 lError = 0;
	
	lError = lib.frontEnd.EnableFactoryClkTest();
/*	if (lError == 0) {
		if (lib.IsProductModel("QIP72XXP2")) {
			tErr = NxpWriteTunerReg(HAL_DEMOD_IB1, 0x19, (U8)0x00);	// 0x00: Disable Inband1
			if (tErr != TM_OK) {
				DB_PRINTF(("NxpWriteTunerReg write ERROR\r\n"));
				lError = 1;
			}
			else {
				DB_PRINTF(("NxpWriteTunerReg write OK\r\n"));
				lError = 0;
			}
		}
	}*/
	return lError;
}

U32 TiApi_FrontEnd::StopFactoryClkTest()
{
	U32 lError = 0;
		
//	lError = lib.frontEnd.EnableFactoryClkTest();
	lError = lib.frontEnd.DisableFactoryClkTest();
	
	return lError;
}
