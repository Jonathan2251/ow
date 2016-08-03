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
	extern U32 QAM_lock_delay_trace;	// Gamma debug
#endif	// CONFIG_DELAY_TRACE

void Items::AI201()
{
	char sError[20];
	
/*	if (ui != & ui_stationTest) {
		return;
	}*/
	ErrorCode	error = ui_stationTest.CurrStation()->Error();
	for (int i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4X", error.code[i]);
	}
	lib.led->DisplayStr(sError);
	
#ifdef CONFIG_DELAY_TRACE	// BLTC modify : Gamma
	bool rs232enabled = lib.rs232.EnableOutput();
	printf("Delay_us_trace(%d)\r\n", Delay_us_trace);
	printf("BKNI_Delay_trace(%d)\r\n", BKNI_Delay_trace);
	printf("TimeDelay_trace(%d)\r\n", TimeDelay_trace);
	printf("bcmKNISleep_trace(%d)\r\n", bcmKNISleep_trace);
	printf("total delay(%d)\r\n\r\n", Delay_us_trace + BKNI_Delay_trace + TimeDelay_trace + bcmKNISleep_trace);
	printf("QAM_lock_delay_trace(%d)\r\n", QAM_lock_delay_trace);
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
#endif	// CONFIG_DELAY_TRACE
	
	return;
}
