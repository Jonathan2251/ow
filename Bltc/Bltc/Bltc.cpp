
#include "gitypes.h"
#include "fpcontrol.h"

#include "UI_OutputMsg_Composite.h"
#include "UI_OutputMsg_OnLed.h"
#include "UI_OutputMsg_OnRS232.h"
#include "UI_OutputMsg_OnOSD.h"

//#include "UI_AllInOneTest.h"
#include "UI_AllInOneTest_Gamma.h"
#include "UI_StationTest.h"
#include "UI_IR.h"
#include "UI_RS232.h"
#include "UI_Keypad.h"
#include "UI_Composite.h"
#include "UI_DebugTest.h"

#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

#include "CreateStation.h"

Items	items;

TiApi	api;
TiLib	lib;

int gStationId;

#ifdef CONFIG_WITH_LED
UI_OutputMsg_OnLed		outMsg_OnLed;
#endif	// CONFIG_WITH_LED

#ifdef CONFIG_WITH_RS232
UI_OutputMsg_OnRS232	outMsg_OnRS232;
#endif	// CONFIG_WITH_RS232

#ifdef CONFIG_WITH_OSD
UI_OutputMsg_OnOSD		outMsg_OnOSD;
#endif	// CONFIG_WITH_OSD

UI_OutputMsg_Composite	outMsg;

//UI_AllInOneTest 		ui_aio(&outMsg);
UI_AllInOneTest1 		ui_aio1(&outMsg);

#ifdef CONFIG_UI_STATION_TEST
UI_StationTest			ui_stationTest(&outMsg);
#endif	// CONFIG_UI_STATION_TEST

#ifdef CONFIG_WITH_IR
UI_IR					ui_ir(&outMsg);
#endif	// CONFIG_WITH_IR

#ifdef CONFIG_WITH_RS232
UI_RS232				ui_rs232(&outMsg);
#endif	// CONFIG_WITH_RS232

#ifdef CONFIG_WITH_KEYPAD
UI_Keypad				ui_keypad(&outMsg);
#endif	// CONFIG_WITH_KEYPAD

UI_Composite			ui_multiInput(&outMsg);
UI_Decorator			ui_decorator(&ui_multiInput);
UI_DebugTest			ui_debugTest(&ui_decorator);

UserInterface* 			ui;

void UI_DebugTest_Config()
{
	if (ui == &ui_debugTest) {
		ui_multiInput.Insert(&ui_keypad);
		ui_multiInput.Insert(&ui_ir);
		ui_multiInput.Insert(&ui_rs232);
	}
}

void OutMsg_Config()
{
	#ifdef CONFIG_WITH_LED
	outMsg.Insert(&outMsg_OnLed);
	#endif	// CONFIG_WITH_LED

	#ifdef CONFIG_WITH_RS232
	if (ui == &ui_debugTest) {
		outMsg.Insert(&outMsg_OnRS232);
	}
	#endif	// CONFIG_WITH_RS232

	#ifdef CONFIG_WITH_OSD
#ifdef CONFIG_UI_STATION_TEST
	if (ui == &ui_debugTest || ui == &ui_stationTest) {
#else
	if (ui == &ui_debugTest) {
#endif	// CONFIG_UI_STATION_TEST
		outMsg.Insert(&outMsg_OnOSD);
	}
	#endif	// CONFIG_WITH_OSD
}

bool IsProductModel(S8* model) {
	return lib.IsProductModel(model);
}

int WhichStation(U16 key) {
	int stationId;
	
    switch(key)
	{
     	case KEYPAD_CUR_UP_U16:
     		stationId = 1;
			break;
		
      	case KEYPAD_CUR_LEFT_U16:
     		stationId = 2;
			break;
		
		case (KEYPAD_CUR_UP_U16 & KEYPAD_CUR_LEFT_U16):
     		stationId = 3;
			break;
		
        case KEYPAD_CUR_DOWN_U16:
     		stationId = 4;
			break;
		
     	case (KEYPAD_CUR_DOWN_U16 & KEYPAD_CUR_UP_U16):
     		stationId = 5;
			break;
		
      	case (KEYPAD_CUR_DOWN_U16 & KEYPAD_CUR_LEFT_U16):
     		stationId = 6;
			break;

      	case (KEYPAD_CUR_DOWN_U16 & KEYPAD_CUR_LEFT_U16 & KEYPAD_CUR_UP_U16):
     		stationId = 7;
			break;

		default:
     		stationId = -1;
			break;
    }
    return stationId;   
}

bool HaveFrontKeypadHardware() {
	return true;
}

UserInterface* UserInterfaceSelect() {
	U16 key;
	
//	return &ui_aio1;	// Gamma debug
	key = lib.keypad.GetKey_U16();
	if (key != KEYPAD_NOKEY) {
		gStationId = WhichStation(key);
		if (gStationId > 0) {
			if (gStationId == 7) {
				return &ui_aio1;
	      	}
#ifdef CONFIG_UI_STATION_TEST
	      	else {
			// station test
				return &ui_stationTest;
			}
#endif	// CONFIG_UI_STATION_TEST
		}
	}
#ifdef CONFIG_MUTI_INPUT
	return &ui_debugTest;
#else
	if (WaitIR(KEY_ENTER_IR_MODE)) {
		return &ui_only_ir;
	}
	if (HaveFrontKeypadHardware()) {
		return &ui_only_keypad;
	}
	return &ui_only_rs232;
#endif

}

int bltc_main()
{
	lib.Init();
	ui = UserInterfaceSelect();
	UI_DebugTest_Config();
	OutMsg_Config();
	if (lib.IsProductModel("QIP7100P2")) {
		lib.led->AllOn();	// PE request: indicate the BLTC is up.
	}
	// From PPR3 both QIP7100P2 and QIP7232P2 removed JP1 pin and keep pad. So, production line can short JP1 pad and Debug Test UI have no chance.
	lib.peripheral.WatchDog_Enable();
	ui->Run();
	
	return 0;
}

