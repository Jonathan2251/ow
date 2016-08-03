// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "Bltc.h"
#include "UI_StationTest.h"

extern UI_StationTest ui_stationTest;

void Items::AI051()
{
	U8	RXdata=0; 
	U32 i, j;
	bool rs232enabled = lib.rs232.EnableOutput();
	
  	//For Auto Test Station Return to IPC with Error code format Error_Code:FFFF(CR)(LF)(CR) 
	for (j=0; (RXdata!='R')&&(j<15); j++) {
		// Notify SIE starting to measure RF frequency, VCXO test
		lib.rs232.Print("@D %s\r\n\r", BLTC_VERSION_FULL);

		for(i=0;(RXdata!='R')&&(i<900000);i++) {
			lib.rs232.RxByte(&RXdata);
		}
		lib.peripheral.WatchDog_Serve();
	}

	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	if(j == 15) {
		lib.led->DisplayStr("E  1");     
		ui_stationTest.CurrStation()->StopTest(); 
	}
	
	return;
}
