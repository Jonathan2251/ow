// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "Bltc.h"

// defines (taken from OtpMenu.c)
#define DEVICE_PROGRAMMED										0
#define DEVICE_ALREADY_PROGRAMMED								1

// extern 
extern U32 ProgramAllOtpBits();
extern void GetDeviceSerialNumber(U32 &ru32DevSnHi, U32 &ru32DevSnLo);

U32 OTP_Lock()
{
	U32 lError = 1;
	U32 ulStatus=0;
	U32 u32DevSnHi, u32DevSnLo;
//	bool rs232enabled = lib.rs232.EnableOutput();

	// Uses TVMon Based Function to Program and verify OTP Security bits
	ulStatus = ProgramAllOtpBits();

	GetDeviceSerialNumber(u32DevSnHi,u32DevSnLo);
	lib.rs232.Print("Bcm74xx Device ID = 0x%08X%08X\r\n", u32DevSnHi, u32DevSnLo);
	// Return status can determine specific error
	
	if (ulStatus == DEVICE_PROGRAMMED)
	{
		// It's in unlock status before run. => First time to do lock.
		// Bltc only wants 0 or 1 for return codes
		lib.rs232.Print("Program OK...Code:0x%8x\r\n", ulStatus);
		lError = 0;
	}
	else if (ulStatus == DEVICE_ALREADY_PROGRAMMED)
	{
		// Rework unit
		// Bltc only wants 0 or 1 for return codes
		lib.rs232.Print("Already Programmed...Code:0x%8x\r\n", ulStatus);
		lError = 0;
	}
	else
	{
		// Otherwise is wrong status, we should treat it as error and find out why.
		// Print out some error information
		lib.rs232.Print("Program Failure...Error_Code:0x%8x\r\n", ulStatus);
		lError = 1;
	}
//	if (!rs232enabled) {	// restore RS232 status
//		lib.rs232.DisableOutput();
//	}

	return lError;
}

U32 Items::TI028()
{
	U32 lError = 1;
	U32 (*pf)() = &OTP_Lock;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
