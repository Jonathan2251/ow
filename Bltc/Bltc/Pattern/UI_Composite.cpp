// Author : Gamma Chen

//##ModelId=480EA75D0371
#include "UI_Composite.h"
#include "Timer_MsTick.h"
    
//##ModelId=480EA75D0371
UI_Composite::UI_Composite(UI_OutputMsg *outMsg) : UserInterface(outMsg), _size(0)
{
}

//##ModelId=480EA75D03BF
bool UI_Composite::Insert(UserInterface *ui)
{
	if (_size < 10) {
		ui->ChangeOutMsg(OutMsg());	// Using the original _outMsg
		_ui[_size] = ui;
		_size++;
		return true;
	}
	else {
		return false;
	}
}

//##ModelId=480EA75E0054
U8 UI_Composite::SelectItem()
{
	U8 state = SELECTING_ITEM;
	
	// Round robin to Call GetKey for each UI input, 
	// and call ProcessKeyIn(
	_tick_ms.Reset();
	while (state != EXECUTE_ITEM) {
		if (_tick_ms.Tick()) {
		// trigger ProcessKeyIn() at every 1ms
			state = ProcessKeyIn();
		}
	}
	
	return _iter.CurrentItem();
}


//##ModelId=480EA75E0083
U8 UI_Composite::ProcessKeyIn()
{
	U8 state = SELECTING_ITEM;
	
	for (int i = 0; i < _size; i++) {
		state = _ui[i]->ProcessKeyIn();
		if (state == EXECUTE_ITEM) {
			return state;
		}
	}
	
	return state;
}

