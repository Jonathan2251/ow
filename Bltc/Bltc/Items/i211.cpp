// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "UI_StationTest.h"
#include "UserInterface.h"

extern UI_StationTest ui_stationTest;
extern UserInterface *ui;


void Items::AI211()
{
	char sError[20];
	
	ErrorCode	error = ui_stationTest.CurrStation()->Error();
	for (int i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4X", error.code[i]);
	}
	ui_stationTest.OutMsg()->DisplayStr(sError);
	
	return;
}

#endif	// CONFIG_WITH_OSD
