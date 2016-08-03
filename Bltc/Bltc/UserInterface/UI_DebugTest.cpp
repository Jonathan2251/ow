// Author : Gamma Chen

#include "UI_DebugTest.h"
#include "TiApi.h"
#include "TiLib.h"

UI_DebugTest::UI_DebugTest(UI_Decorator *ui_decorator) : UI_Decorator(*ui_decorator)
{
}

U8 UI_DebugTest::SelectItem()
{
	U8 state = SELECTING_ITEM;
	
	// Round robin to Call GetKey for each UI input, 
	// and call ProcessKeyIn(
	_tick_ms.Reset();
	while (state != EXECUTE_ITEM) {
		if (_tick_ms.Tick()) {
		// trigger ProcessKeyIn() at every 1ms
			state = UI_Decorator::ProcessKeyIn();
		}
		// Take Power On/Off MCard automatically by check MCard Attached/Detached for every 100ms.
		api.mCard.AutoPowerOnOff();
		lib.peripheral.WatchDog_Serve();
	}
	
	return _iter.CurrentItem();
}
