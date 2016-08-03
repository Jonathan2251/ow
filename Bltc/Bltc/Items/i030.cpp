// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

// defines (taken from OtpMenu.c)
#define DEVICE_PROGRAMMED										0
#define DEVICE_ALREADY_PROGRAMMED								1

extern extern U32 VerifyLock();

U32 OTP_LockVerify()
{
	U32 lError = 1;
	U32 ulStatus;
	
	// Uses TVMon Based Function to verify OTP Security bits
	ulStatus = VerifyLock();
	if (ulStatus == DEVICE_PROGRAMMED)
	{
		// Bltc only wants 0 or 1 for return codes
		lError = 0;
	}
	else
	{
		// Failed Verification
		// Print out some error information
		PDEBUG("Error_Code:0x%8x\r\n", ulStatus);
		lError = 1;
	}
	
	api.CPU_OTP_DisplayProcessorSecurityStatus();
	
	return lError;
}

U32 Items::TI030()
{
	U32 lError = 1;
	U32 (*pf)() = &OTP_LockVerify;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
