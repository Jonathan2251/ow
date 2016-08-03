#include "TiApi_Peripheral.h"
#include "ModelConfig.h"
#include "TiLib.h"
#include <string.h>

#include "usb.h"
#include "sata.h"
#include "cputimer.h"
#include "ethernet.h"
#include "TiLib.h"

// Follows copy from file TvMon\Libs\Broadcom_TvMon\src\usb\inc\usb.h
#define USB_FRONTPANEL_BUS		0
#define USB_FRONTPANEL_PORT		2
#define USB_REARPANEL_BUS		0
#define USB_REARPANEL_PORT		1

extern "C" void DetectDevice(long lBus, long lPort);

//##ModelId=472C57B402D8
U32 TiApi_Peripheral::USB_Test(U8 port)
{
	char answer[100] = "";
	U32 lError = 1;
	
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	switch (port) {
	case 1:
		DetectDevice(USB_FRONTPANEL_BUS,USB_FRONTPANEL_PORT);
		break;
	case 2:
		DetectDevice(USB_REARPANEL_BUS,USB_REARPANEL_PORT);
		break;
	default:
		return (lError = 1);
	}
	tvMonIO.out.enable = 0;
				
	tvMonIO_out_getResult(tvMonIO.out._ostr, "Vendor/Device ID:   ", answer);
	
	lError = lib.peripheral.USB_CheckVendorDeviceId(answer);
	if (!lError) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

//##ModelId=472C57B402DA
U32 TiApi_Peripheral::ETHERNET_SingleInterLoopTest()
{
	char *ethernet_single_loop_test[] = { 
//		"ethernet", 	// enter from TvMon menu to ethernet menu
		"\t0",  	// 0) LoopBack Test (Single Iteration)
		"/",	// exit ethernet menu
//		"exit",	// exit TvMon menu
		"\0"};	// command end of mark
	char *p;
	U32 lError = 0;
		
	lib.tvMonCmd.RunCmdsInMenu(ethernet_single_loop_test, doEthernet);
	p = strstr(tvMonIO.out._ostr, "Test Successful");
	if (p != 0) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

//##ModelId=472C57B402DB
U32 TiApi_Peripheral::SATA_DiskTest(U8 port)
{
	char *test_sata_port_0[] = { 
//		"sata", 	// enter from TvMon menu to sata menu
		"\tg0",  	// g?     : Get drive ?=0|1|2|3 parameters
		"",		// 'Enter' to continue...
		"x",	// exit sata menu
//		"exit",	// exit TvMon menu
		"\0"};	// command end of mark

	char *test_sata_port_2[] = { 
//		"sata", 	// enter from TvMon menu to sata menu
		"\tg2",  	// g?     : Get drive ?=0|1|2|3 parameters
		"",		// 'Enter' to continue...
		"x",	// exit sata menu
//		"exit",	// exit TvMon menu
		"\0"};	// command end of mark
		
	int i;
    char	diskID[3][50] = {"ST3120026AS ", "ST3160811AS ", "ST3160212SCE"};
	char* p = 0;
	U32 lError = 0;

	tvMonIO_clear();
	if (port == QIP7232_SATA_INTERNAL_PORT) {
		lib.tvMonCmd.RunCmdsNInMenu(test_sata_port_0, 3, doSata);
	}
	else if (port == QIP7232_SATA_EXTERNAL_PORT) {
		lib.tvMonCmd.RunCmdsNInMenu(test_sata_port_2, 3, doSata);
//		lib.tvMonCmd.RunCmdsN(test_sata_port_2, 5);
	}
	else {
		return (lError = 1);
	}
	
//	tvMonIO_out_print();	// gamma trace
	
	lError = 1;
	for (i = 0; i < 3; i++) {
		p = strstr(tvMonIO.out._ostr, diskID[i]);
		if (p != 0) {
			lError = 0;
			break;
		}
	}
	return lError;
}

//##ModelId=472C57B402E8
U32 TiApi_Peripheral::FPIR_Test(U8* key, U32 timeout)
{
	U32 lError = 0;
	TimeStruct IR_RxTimer; 
	U8  KeyCode=0;

	lib.ir.ClearData();
	Init_timer(&IR_RxTimer,timeout);
	do 
	{
		KeyCode = lib.ir.GetKey();
	} while ((KeyCode!=IR_1)&&(Check_timer(&IR_RxTimer)!=1));
	if (KeyCode==IR_1)
		lError=0;
	else
		lError=1;
//	lib.ir.ClearData();

	return lError;
}

//##ModelId=472C57B402F9
U32 TiApi_Peripheral::ACOUTLET_Test(bool on)
{
	U32 lError = 0;
	
	lib.peripheral.AC_Outlet_Init();
		
	lError = lib.peripheral.AC_Outlet_Status(on);
	
	return lError;
}

//##ModelId=472C57B402F7
U32 TiApi_Peripheral::FPKEYPAD_Test()
{
	U32 lError = 0;
	
	return lError;
}

//##ModelId=472C57B402F8
U32 TiApi_Peripheral::FPLED_Test()
{
	U32 lError = 0;
	
	return lError;
}

extern VOID Jedi1394InterruptLineTest();

#include "addrutil.h"
#include "userio.h"

extern BOOL bJedi1394InterruptActive;
extern U32 ulBaseAddress1394;
extern U32 GetPciDeviceAddress(U32 ulDeviceNumber);

// From bcminit.c
typedef void (*MotIntFunc) ();
extern MotIntFunc pJedi1394IntFunc;

extern VOID Jedi1394InterruptServiceFunction();
extern VOID Configure1394Iso2ForTransmit(U32 ulBaseAddr);
extern VOID ResetTheJediDevice();

U32 TiApi_Peripheral::Jedi1394Interruptest()
{
	U32 ulError = 1;
	
#if 0	// Use TvMon IO Redirect
	char* p = 0;
	
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	Jedi1394InterruptLineTest();
	tvMonIO.out.enable = 0;
				
	p = strstr(tvMonIO.out._ostr, "Interrupt detected");
	if (p != 0) {
		ulError = 0;
	}
	else {
		ulError = 1;
	}

#else

	bJedi1394InterruptActive = FALSE;

	// Fetch the BCM3250 PCI address
	ulBaseAddress1394 = GetPciDeviceAddress(IDSEL_1394);
	
	if (ulBaseAddress1394 == 0)
	{
		DB_PRINTF(( "\r\nCould not obtain Bcm3250 Pci address\r\n" ));
		return ulError;			// Error...Quick out
	}

	// Attach TVMon Interrupt routine to Broadcom Hal interrupt service routine...
	pJedi1394IntFunc = Jedi1394InterruptServiceFunction;
	
	Configure1394Iso2ForTransmit(ulBaseAddress1394);


	DB_PRINTF(( "Assert 1394 Interrupt Line..." ));

	// Enables wakeup & External interrupt to 7400...
	//  If System interrupt is enabled, should generate a system interrupt...
	*( (U32*)(ulBaseAddress1394+0x00c) ) = 0x81000000;

	Delay_ms(10);
	
	// The System Jedi Interrupt sets a flag...
	//  So if interrupt was rec'd (trace ok) then interrupt should occur...
	//   otherwise somethings wrong....
	if (bJedi1394InterruptActive)
	{
		DB_PRINTF(( "Interrupt detected...Line ok\r\n" ));
		bJedi1394InterruptActive = FALSE;
		ulError = 0;
	}
	else
	{
		DB_PRINTF(( "Interrupt NOT detected...\r\n"));
		bJedi1394InterruptActive = FALSE;
		ulError = 1;
	}
	
#endif

	// Reset to "before testing state", without this, the TI089 easy to hang on.
//	ResetTheJediDevice();
	
	return ulError;
}

