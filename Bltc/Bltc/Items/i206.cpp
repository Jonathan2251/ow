// Author:			Gamma Chen

#include "ModelConfig.h"
#include "TiLib.h"
#include "Items.h"
#include "TiApi.h"

void Items::AI206()
{
	while (1) {
		api.mCard.AutoPowerOnOff();
		U16 key = lib.keypad.GetKey_U16();
		key = (~key & (~ KEYPAD_CH_UP_U16 | ~ KEYPAD_CH_DOWN_U16) );
		PDEBUG("~key & (~ KEYPAD_CH_UP_U16 | ~ KEYPAD_CH_DOWN_U16) = %4X\n", key);
		if (key != (U16)0) {	// if KEYPAD_CH_UP_U16 or KEYPAD_CH_DOWN_U16 be pressed
			break;
		}
		lib.peripheral.WatchDog_Serve();
	}
	
	return;
}
