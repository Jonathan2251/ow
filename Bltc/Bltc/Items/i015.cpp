// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"
#include "UI_StationTest.h"

extern int gStationId;
extern UI_StationTest			ui_stationTest;

#ifdef CONFIG_WITH_RS232
#include "UI_OutputMsg_OnRS232.h"
extern UI_OutputMsg_OnRS232	outMsg_OnRS232;
#endif	// CONFIG_WITH_RS232

U32 Items::TI015()
{
#ifdef CONFIG_WITH_RS232
	ui_stationTest.OutMsg()->Delete(&outMsg_OnRS232);
#endif	// CONFIG_WITH_RS232
	gStationId = 4;
	ui_stationTest.Run();
#ifdef CONFIG_WITH_RS232
	ui_stationTest.OutMsg()->Insert(&outMsg_OnRS232);
#endif	// CONFIG_WITH_RS232
	while (1) {
		U16 key = lib.keypad.GetKey_U16();
		key = (~key & ~KEYPAD_SELECT_U16);
		if (key != (U16)0) {	// if KEYPAD_SELECT_U16 be pressed
			break;
		}
	}
	
	return TI_SUCCESS;
}
