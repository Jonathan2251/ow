// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "UI_StationTest.h"
#include "UserInterface.h"

extern UI_StationTest ui_stationTest;
extern UserInterface *ui;

void Items::AI203()
{
	U32 lError;
	
/*	if (ui != &ui_stationTest) {
		return;
	}*/
	lError = ui_stationTest.CurrStation()->LastItemError();
	ui_stationTest.OutMsg()->TestResult(lError);
/*	if (lError == 0) {
		lib.led.DisplayStr("PASS");
	}
	else if (lError == 1) {
		lib.led.DisplayStr("FAIL");
	}
	else if (lError == 2) {
		lib.led.DisplayStr("NONE");
	}*/
	
	return;
}

#endif	// CONFIG_WITH_OSD
