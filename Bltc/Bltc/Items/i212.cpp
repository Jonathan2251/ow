//Author:			Gamma Chen

#include "ModelConfig.h"
#include "TiLib.h"
#include "Items.h"
#include "UI_StationTest.h"
#include "Bltc.h"

extern UI_StationTest ui_stationTest;

void Items::AI212()
{
	U32 lError;
	
	lError = ui_stationTest.CurrStation()->LastItemError();
	if (lError == TI_SUCCESS) {
		return;
	}
	while (1) {
		lib.peripheral.WatchDog_Serve();
	}
	
	return;
}
