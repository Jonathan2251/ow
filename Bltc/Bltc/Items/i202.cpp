// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"

#include "UI_StationTest.h"
#include "UserInterface.h"

extern UI_StationTest ui_stationTest;
extern UserInterface *ui;

#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
	extern U32 Delay_us_trace;	// Gamma debug
	extern U32 BKNI_Delay_trace;	// Gamma debug
	extern U32 TimeDelay_trace;	// Gamma debug
	extern U32 bcmKNISleep_trace;	// Gamma debug
#endif	// CONFIG_DELAY_TRACE

void Items::AI202()
{
	int i;
	char sError[20];
	bool rs232enabled = lib.rs232.EnableOutput();
	
/*	if (ui != & ui_stationTest) {
		return;
	}*/
	ErrorCode	error = ui_stationTest.CurrStation()->Error();
	for (i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4X", error.code[i]);
	}
	lib.led->DisplayStr(sError);
	
	for (i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4x", error.code[i]);
	}
	//For Auto Test Station Return to IPC with Error code format Error_Code:FFFF(CR)(LF)(CR)
	lib.rs232.Print("@Error_Code:%s\r\n\r", sError);
	lib.rs232.Print("@Error_Code:%s\r\n\r", sError);	// Second for fix some old version of SIE bug

#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
	printf("Delay_us_trace(%d)\r\n", Delay_us_trace);
	printf("BKNI_Delay_trace(%d)\r\n", BKNI_Delay_trace);
	printf("TimeDelay_trace(%d)\r\n", TimeDelay_trace);
	printf("bcmKNISleep_trace(%d)\r\n", bcmKNISleep_trace);
	printf("total delay(%d)\r\n\r\n", Delay_us_trace + BKNI_Delay_trace + TimeDelay_trace + bcmKNISleep_trace);
#endif	// CONFIG_DELAY_TRACE
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	return;
}
