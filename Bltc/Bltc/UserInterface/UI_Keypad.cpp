// Author : Gamma Chen

//##ModelId=48102AEF0260
#ifdef CONFIG_WITH_KEYPAD

#include "UI_Keypad.h"
#include "TiLib.h"
#include "TiLib_Keypad.h"
#include "bcmdbg.h"

#define MAX_PRESSED_TIME	3000	// 3000ms

UI_Keypad::UI_Keypad(UI_OutputMsg *outMsg) : UserInterface(outMsg), _pressedTime_ms(0)
{
}

//##ModelId=46FB1B0201A6
bool UI_Keypad::LongPressed()
{
    if (_pressedTime_ms > 2000) {
        return true;
    }
    else {
        return false;
    }
}

//##ModelId=46FB1B2A028F
bool UI_Keypad::GetKey(U32 interval)
{
	bool timeToRead = false;
	
	_timer.ms--;
	if (_timer.ms > 0) {
		timeToRead = false;
		return timeToRead;
	}
	timeToRead = true;
	_timer.ms = interval;	 // unit 1ms, => interval ms
	_oldKey = _key;
	_key = lib.keypad.GetKey();
	_pressedTime_ms += interval;
    if (_key != _oldKey || _key == KEYPAD_NOKEY)  _pressedTime_ms = 0;

    return timeToRead;
}

//##ModelId=46CE723E000D
U8 UI_Keypad::SelectItem()
{
	_key = KEYPAD_NOKEY;
	_tick_ms.Reset();
	while (_key != KEYPAD_SELECT) {
		if (_tick_ms.Tick()) {
		// trigger ProcessKeyIn() at every 1ms
			ProcessKeyIn();
		}
	}
    return _iter.CurrentItem();
}

//##ModelId=48102AEF0326
U8 UI_Keypad::ProcessKeyIn()
{
	static U32 sameKeyCount = 0;
	bool timeToRead;
	U8 state = SELECTING_ITEM;
	
	timeToRead = GetKey(60);	// every 60 ms put to _key
	if (!timeToRead) {
		return state;
	}
    if ( !LongPressed() ) {
		if (_key == _oldKey) {
		// Slow down scrolling, to 1/4
			sameKeyCount++;
			if (sameKeyCount < 6) {
				return state;
			}
			else {
				sameKeyCount = 3;
			}
		}
		else {
			sameKeyCount = 0;
		}
	}
	if (_state == SCROLLING) {
	// do scroll only in _state == SCROLLING
		switch((KEYTYPE)_key)
		{
		case KEYPAD_CUR_UP:
			_iter.Next();
			OutMsg()->ItemNr(_iter.CurrentItem());
			break;
		case KEYPAD_CUR_DOWN:
			_iter.Previous();
			OutMsg()->ItemNr(_iter.CurrentItem());
			break;
		case KEYPAD_SELECT:
			state = EXECUTE_ITEM;
			break;
		}
	}
	
	return state;
}
#endif	// CONFIG_WITH_KEYPAD
