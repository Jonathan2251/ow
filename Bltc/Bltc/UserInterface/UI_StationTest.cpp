// Author : Gamma Chen

#ifdef CONFIG_UI_STATION_TEST

#include "UI_StationTest.h"
#include "TiLib.h"
#include "CreateStation.h"
#include "gitypes.h"

extern int gStationId;

//##ModelId=48102AF102A9
UI_StationTest::UI_StationTest(UI_OutputMsg *outMsg) : UserInterface(outMsg)
{
}

//##ModelId=46FA2134003A
void UI_StationTest::Run()
{
	U32 result = FAILURE;
	
	if (gStationId != -1) {
		result = CreateStation(gStationId);
		if (result == SUCCESS) {
		    UartMode oldUartMode = lib.rs232.SetUartMode(BLT_SIE);
			bool rs232enabled = lib.rs232.DisableOutput();
			_station->Test();
			if (rs232enabled) {	// restore RS232 status
				lib.rs232.EnableOutput();
			}
			lib.DelayMS(1000);
		    lib.rs232.SetUartMode(oldUartMode);
		}
		else {
			bool rs232enabled = lib.rs232.EnableOutput();
			lib.rs232.Print("No this station #%d\r\n", gStationId);
			if (!rs232enabled) {	// restore RS232 status
				lib.rs232.DisableOutput();
			}
		}
	}
}

//##ModelId=46FCDA4F010A
U32 UI_StationTest::CreateStation(int stationId)
{
	StationsInit();
	if (station[stationId].Id() > 0) {
		_station = &station[stationId];
		return SUCCESS;
	}
	else {
		return FAILURE;
	}
}

//##ModelId=4705B0F7017B
Station* UI_StationTest::CurrStation()
{
	return _station;
}

#endif	// CONFIG_UI_STATION_TEST

