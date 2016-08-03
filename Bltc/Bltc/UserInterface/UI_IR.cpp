// Author : Gamma Chen

#ifdef CONFIG_WITH_IR

#include "UI_IR.h"
#include "TiLib.h"
#include "Items.h"

#define SCROLLING		0
#define INPUT_ITEM_NO	1

//##ModelId=48102AF00151
UI_IR::UI_IR(UI_OutputMsg *outMsg) : UserInterface(outMsg)
{
}

//##ModelId=48102AF0022C
KEYTYPE_IR UI_IR::GetKey()
{
    _key = lib.ir.GetKey();
    
    return _key;
}

//##ModelId=48102AF0025B
bool UI_IR::IsKeyNr(KEYTYPE_IR key)
{
	if (key >= IR_0 && key <= IR_9) {
		return true;
	}
	else {
		return false;
	}
}

//##ModelId=48102AF00307
void UI_IR::Scroll()
{
	switch(_key)
	{
	case IR_UP:
		_iter.Next();
		OutMsg()->ItemNr(_iter.CurrentItem());
		break;
	case IR_DOWN:
		_iter.Previous();
		OutMsg()->ItemNr(_iter.CurrentItem());
		break;
	}
}

//##ModelId=48102AF00345
void UI_IR::InputItemNr()
{
	if (IsKeyNr(_key)) {
		_usrInputNr = _usrInputNr*10 + _key;
		if (_usrInputNr <= MAX_TEST_ITEMS) {
	    	OutMsg()->UsrInputItemNr(_usrInputNr);
	    }
	    else {
			_usrInputNr = _usrInputNr/10;
	    }
	}
}

//##ModelId=46CE71A0037D
U8 UI_IR::SelectItem()
{
	_tick_ms.Reset();
	_key = IR_NOKEY;
	while (_key != IR_OK) {
		if (_tick_ms.Tick()) {
		// trigger ProcessKeyIn() at every 1ms
			ProcessKeyIn();
		}
	}
    return _iter.CurrentItem();
}

//##ModelId=48102AF0020D
U8 UI_IR::ProcessKeyIn()
{
	GetKey();
	_timer.ms--;
	if (_timer.ms > 0) {
		return SELECTING_ITEM;
	}
	// _timer.ms <= 0
	if (_event == CLEAR_DATA) {
		lib.ir.ClearData();
		_event = NOTHING;
	}
	if (_state == SCROLLING) {
		if (_key != IR_NOKEY) {
			if (IsKeyNr(_key)) {
				_usrInputNr = _key;
				_state = INPUT_ITEM_NO;
		    	OutMsg()->UsrInputItemNr(_usrInputNr);
				_timer.ms = 500;
				_event = CLEAR_DATA;
				_timeout.ms = INPUT_ITEM_NO_MODE_TIME_INTERVAL;
				_tick_ms.Attach(&_timeout);
			}
			else {
				Scroll();
				_timer.ms = 150;
				_event = CLEAR_DATA;
			}
		}
	}
	else if (_state == INPUT_ITEM_NO) {
		if (_timeout.ms <= 0) {
			_iter._itemNr = _usrInputNr;
			_usrInputNr = 0;
			_state = SCROLLING;
			OutMsg()->NewLine();
		    OutMsg()->ItemNr(_iter.CurrentItem());
			lib.ir.ClearData();
			_tick_ms.Detach(&_timeout);
		}
		if (_key == IR_OK) {
			if (_usrInputNr <= MAX_TEST_ITEMS) {
				OutMsg()->NewLine();
				_iter._itemNr = _usrInputNr;
			}
			_state = SCROLLING;
			_tick_ms.Detach(&_timeout);
			_timer.ms = 150;
			_event = CLEAR_DATA;
		}
		else {
			InputItemNr();
			if (_key != IR_NOKEY) {
				_timer.ms = 200;
				_event = CLEAR_DATA;
			}
		}
	}
		
	if (_key == IR_OK) {
		_state = SCROLLING;
		return EXECUTE_ITEM;
	}
	else {
	    return SELECTING_ITEM;
	}
}

#endif	// CONFIG_WITH_IR
