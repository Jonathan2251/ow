// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
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

void Items::AI207()
{
	char sError[50];
	int i;
	U16 keyMatch[4];
	
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

/*	if (ui != &ui_stationTest) {
		return;
	}*/
	ErrorCode	error = ui_stationTest.CurrStation()->Error();
	for (i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4X", error.code[i]);
	}
	keyMatch[0] = KEYPAD_CH_UP_U16;
	keyMatch[1] = KEYPAD_CH_DOWN_U16;
	keyMatch[2] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_UP_U16);
	keyMatch[3] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_DOWN_U16);
	sprintf(sError+i*4, "    ");
	api.menu.DisplayLongStr_By4_UntilKeyInMatch(sError, keyMatch, 4);
	
	return;
}
