#include "TiLib.h"

#include "Bcm7400GpioController.h"
#include "gpio.h"

#include "watchdog.h"
#include "userio.h"

extern WatchDog gWatchDog;
extern BOOL	 gbWatchDogServiceEnabled;
extern "C" VOID ServiceWatchDogManager();
extern "C" void HalServiceWatchDogManager();

//##ModelId=472C6983011F
U32 TiLib_Peripheral::USB_CheckVendorDeviceId(S8* str)
{
	U32	lError;
	int i = 0;
	
	// vendor ID must be hex 0..F
	for (i = 0; i < 4; i++) {
		if (!(str[i] >= '0' && str[i] <= '9'  ||
			str[i] >= 'A' && str[i] <= 'F'	||
			str[i] >= 'a' && str[i] <= 'f')) {
			return (lError = 1);
		}
	}
	
	// device ID must be hex 0..F
	for (i = 5; i < 8; i++) {
		if (!(str[i] >= '0' && str[i] <= '9'  ||
			str[i] >= 'A' && str[i] <= 'F'	||
			str[i] >= 'a' && str[i] <= 'f')) {
			return (lError = 1);
		}
	}
	
	return (lError = 0);
}

// Author : Dolphin
//##ModelId=472C69830121
U32 TiLib_Peripheral::AC_Outlet_Init()
{
	Bcm7400IoControlDriver *pGPIO = new Bcm7400IoControlDriver;
	Pin_t Ac_outlet;

//	GetPinByNum( PIN_AC_CONTROL, &Ac_outlet );	// QIP7kP2 remove the ac out let hardware

	return SUCCESS;
}

// Author : Dolphin
//##ModelId=472C69830122
U32 TiLib_Peripheral::AC_Outlet_Status(bool on)
{
	Bcm7400IoControlDriver *pGPIO = new Bcm7400IoControlDriver;
	Pin_t Ac_outlet;

	if (on) {
		SetPinState( pGPIO, &Ac_outlet, PIN_ON, TRUE );
	}
	else {
		SetPinState( pGPIO, &Ac_outlet, PIN_OFF, TRUE );
	}

	return SUCCESS;
}

//##ModelId=48114D2703C3
void TiLib_Peripheral::WatchDog_Disable()
{
	gbWatchDogServiceEnabled = FALSE;
//	DB_PRINTF(("Watchdog Timer Service Disabled\r\n"));
	gWatchDog.DisableWatchDog();
	lib.clock.Detach(this);
}

void TiLib_Peripheral::WatchDog_Enable()
{
	// Enables the Watchdog timer service
	gbWatchDogServiceEnabled = TRUE;
	
/*	bool rs232enabled = lib.rs232.EnableOutput();
	DB_PRINTF(("Watchdog Timer Service Enabled\r\n"));
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}*/
	
	ServiceWatchDogManager();
	_timeInterval = 0;
//	lib.clock.Attach(this);
}

void TiLib_Peripheral::WatchDog_Serve()
{
// Prevent reset

/*	bool rs232enabled = lib.rs232.EnableOutput();
	DB_PRINTF(("Watchdog Timer Service Served\r\n"));
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}*/
	
	HalServiceWatchDogManager();
}

void TiLib_Peripheral::Update(Subject *theChangedSubject)
{
// Prevent reset

/*	bool rs232enabled = lib.rs232.EnableOutput();
	DB_PRINTF(("TiLib_Peripheral::Update();\r\n"));
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}*/
	
	_timeInterval--;
	if (_timeInterval < 0) {
		WatchDog_Serve();
		_timeInterval = 0;
	}
}
