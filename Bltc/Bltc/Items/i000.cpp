// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "bcmdbg.h"
#include "CreateStation.h"
#include "TiLib.h"
#include "Bltc.h"
#include "TiApi.h"

#ifdef CONFIG_DISPLAY_STATION_INFO

extern const char* stationKey[MAX_NR_STATIONS];

void DisplayItems() {
	int i = 0;
	
	lib.rs232.Print("---------- ITEM LIST ----------\r\n\r\n");
	for (i = 0; i < 256; i++) {
		char sType[20];
		
		if (gItem[i].type != NO_ITEM_TYPE) {
			if (gItem[i].type == TEST_ITEM_TYPE) {
				strcpy(sType, "TEST_ITEM_TYPE");
			}
			else if (gItem[i].type == ACTION_ITEM_TYPE) {
				strcpy(sType, "ACTION_ITEM_TYPE");
			}
			else if (gItem[i].type == CONTROL_ITEM_TYPE) {
				strcpy(sType, "CONTROL_ITEM_TYPE");
			}
			lib.rs232.Print("%d  %s  \"%s\"\r\n", gItem[i].id, sType, gItem[i].name);
		}
	}
	lib.rs232.Print("\r\n\r\n");
}

void DisplayStationList() {
	int i;
	
	lib.rs232.Print("---------- STATION LIST ----------\r\n\r\n");
	for (i = 1; i < MAX_NR_STATIONS; i++) {
		if (station[i].Id() != 0) { // _id != 0 meaning this station exists
			// output station information to out which is file "station_list.txt"
			lib.rs232.Print("\r\n- STATION %d -\r\n", i);
			station[i].OutputStationList();
		}
	}
	lib.rs232.Print("\r\n\r\n");
}

void DisplayErrCodeList() {
	int i;
	
	lib.rs232.Print("--------- ERROR CODE LIST ---------\r\n\r\n");
	for (i = 1; i < MAX_NR_STATIONS; i++) {
		if (station[i].Id() != 0) { // _id != 0 meaning this station exists
			// output station information to out which is file "station_list.txt"
			lib.rs232.Print("\r\n- STATION %d -\r\n", i);
			station[i].OutputErrCodeList();
		}
	}
	lib.rs232.Print("\r\n\r\n");
}

U32 Items::TI000()
{
	U32 HWBoardID;
	bool rs232enabled = lib.rs232.EnableOutput();
	U32 lError = TI_SUCCESS;
	
	StationsInit();
	lib.led->DisplayStr(BLTC_VERSION_SHORT);
#ifdef CONFIG_WITH_OSD
	lib.window.DisplayStr(BLTC_VERSION_SHORT);
#endif	// CONFIG_WITH_OSD
	lib.rs232.Print("@D %s compile date:%s time:%s\r\n\r\n", BLTC_VERSION_FULL, __DATE__, __TIME__);
	api.ChipID_Get(CT_BOARD_ID,&HWBoardID);
	lib.rs232.Print("Board ID = %x\r\n", HWBoardID);
	DisplayItems();
	DisplayStationList();
	DisplayErrCodeList();
#ifdef CONFIG_WITH_OSD
	lib.osd.ClearScreen();
#endif	// CONFIG_WITH_OSD
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	return lError;
}

#endif // CONFIG_DISPLAY_STATION_INFO
