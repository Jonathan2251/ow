//Author:			Gamma Chen

#include "ModelConfig.h"
#include "TiLib.h"
#include "Items.h"

void Items::AI209()
{
	while (1) {
		U16 key = lib.keypad.GetKey_U16();
		key = (~key & ~KEYPAD_SELECT_U16);
		PDEBUG("~key & ~KEYPAD_SELECT_U16 = %4X\n", key);
		if (key != (U16)0) {	// if KEYPAD_SELECT_U16 be pressed
			break;
		}
		lib.peripheral.WatchDog_Serve();
	}
	
	return;
}
