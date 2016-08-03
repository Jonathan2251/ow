// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"
#include "userio.h"

// defines (taken from OtpMenu.c)
#define DEVICE_PROGRAMMED										0
#define DEVICE_ALREADY_PROGRAMMED								1
#define NON_VIRGIN_DEVICE_PROGRAMMED							20

// extern 
extern U32 ProgramAllOtpBitsRegardsOfInitState();
extern VOID DisplayOtpProgramError(U32 ulOtpProgrammingStatus);

U32 OTP_Lock_Regardless_InitStatus()
{
	U32 lError = 1;
	U32 ulOtpProgrammingStatus;
	bool rs232enabled = lib.rs232.EnableOutput();

	DB_PRINTF(("Permanently program ALL OTP bits regardless of the device's initial state.\r\n"));
	ulOtpProgrammingStatus = ProgramAllOtpBitsRegardsOfInitState();

	// Tell the user in detail what error occurred...
	DisplayOtpProgramError(ulOtpProgrammingStatus);
	if (ulOtpProgrammingStatus == DEVICE_PROGRAMMED ||
		ulOtpProgrammingStatus == DEVICE_ALREADY_PROGRAMMED ||
		ulOtpProgrammingStatus == NON_VIRGIN_DEVICE_PROGRAMMED) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}

	return lError;
}

U32 Items::TI017()
{
	U32 lError = 1;
	U32 (*pf)() = &OTP_Lock_Regardless_InitStatus;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
