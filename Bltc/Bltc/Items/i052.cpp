// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "Bltc.h"
#include "UI_StationTest.h"

extern UI_StationTest ui_stationTest;

U32 Items::TI052()
{
	U8  SIE_Reply = 0;
	U32 lError = 0;
	bool rs232enabled = lib.rs232.EnableOutput();
	
//	"@1\r\n\r" : message for tell SIE to measure Power Value
	lib.rs232.Print("@1\r\n\r");

	while (1) {
		if ( lib.rs232.RxByte(&SIE_Reply) == SUCCESS ) {
	    	if((SIE_Reply =='G') || (SIE_Reply =='W')) {
	    		break;
	    	}   	
	    }

	}

// SIE return 'G' : Good;
//			  'W' : Worse
	if (SIE_Reply == 'W') {
		lError = 1;
	}
	else {
		lError = 0;
	}
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	return lError;
}
