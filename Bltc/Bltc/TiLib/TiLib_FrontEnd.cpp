#include "TiLib_FrontEnd.h"
#include "TiLib.h"
#include "qamqpsk.h" // From TvMon
#include "NxpTuner.h"
#include "userio.h"

extern U32 gulObIfFreq;
extern U32 gulObLoFreq;
extern U32 gulObRfFreq;
extern U32 gulObSymb;

extern etHalDemodIb gSelectedDemod;
extern BOOL bFactoryClkTest;
extern BOOL PerformFactoryClkTest(BOOL bFactoryClkTest);

//##ModelId=472A819C0289
void TiLib_FrontEnd::OOB_FrequencySet(U32 rfFreq)
{
	gulObRfFreq = rfFreq;
	gulObLoFreq = gulObRfFreq+gulObIfFreq;		// Perform Acquisition of set script
}

//##ModelId=472A819C0299
U32 TiLib_FrontEnd::OOB_Acq()
{
	etHalDemodLockStatus eStatus;
	
	DemodOobAcquire( HAL_DEMOD_OOB0,
					 gulObRfFreq,		//(gulObLoFreq-gulObIfFreq), // DecimalToBcd((gulObLoFreq-gulObIfFreq)/100),
					 gulObSymb,			// DecimalToBcd(gulObSymb/100),
					 &eStatus );

	return 0;
}

//##ModelId=472A819C029B
bool TiLib_FrontEnd::OOB_IsLock()
{
	bool lock;
	etHalDemodLockStatus eStatus;
	
	DemodOobLockStatusGet(HAL_DEMOD_OOB0, &eStatus);
	if (eStatus == HAL_DEMOD_LOCK_STATUS_LOCKED) {
		lock = true;
	}
	else {
		lock = false;
	}
	
	return lock;
}

U32 TiLib_FrontEnd::EnableFactoryClkTest()
{
	U32 lError = 0;
	
	bFactoryClkTest = TRUE;
	if (lib.IsProductModel("QIP7100P2")) {
		if (!PerformFactoryClkTest(bFactoryClkTest)) {
			DB_PRINTF(( "Error occurred\r\n" ));
			return (lError = 1);
		}
		gSelectedDemod = HAL_DEMOD_IB0;
		AcquireQamOption(HAL_DEMOD_QAM_TYPE_64);
	}
	else if (lib.IsProductModel("QIP72XXP2")) {
		if (!PerformFactoryClkTest(bFactoryClkTest)) {
			DB_PRINTF(( "Error occurred\r\n" ));
			return (lError = 1);
		}
		gSelectedDemod = HAL_DEMOD_IB0;
		AcquireQamOption(HAL_DEMOD_QAM_TYPE_64);
		gSelectedDemod = HAL_DEMOD_IB1;
		AcquireQamOption(HAL_DEMOD_QAM_TYPE_64);
		gSelectedDemod = HAL_DEMOD_IB0;
	}
	else {
		return (lError = 1);
	}
	
	return (lError = 0);
}

U32 TiLib_FrontEnd::DisableFactoryClkTest()
{
	U32 lError = 0;
	
	bFactoryClkTest = FALSE;
	if (!PerformFactoryClkTest(bFactoryClkTest))
		DB_PRINTF(( "Error occurred\r\n" ));
		
	return 0;
}
