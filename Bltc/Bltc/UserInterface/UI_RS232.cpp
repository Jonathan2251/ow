// Author : Gamma Chen

//##ModelId=48102AF100A5
#ifdef CONFIG_WITH_RS232

#include "UI_RS232.h"
#include "bcmdbg.h"
#include "TiLib.h"

UI_RS232::UI_RS232(UI_OutputMsg *outMsg) : UserInterface(outMsg)
{
}

//##ModelId=48102AF1019F
U8 UI_RS232::GetKey()
{
	_key = lib.rs232.GetKey();
	
	return _key;
}

//##ModelId=46CE722D003D
U8 UI_RS232::SelectItem()
{
	_tick_ms.Reset();
	_key = RS232_NOKEY;
	while (_key != '\r') {
		if (_tick_ms.Tick()) {
		// trigger ProcessKeyIn() at every 1ms
			ProcessKeyIn();
		}
	}
    return _iter.CurrentItem();
}

//##ModelId=48102AF101DE
void UI_RS232::InputItemNr()
{
	S8 str[2];
	
	if (isdigit(_key) != 0) {
		str[0] = (S8)_key;
		str[1] = '\0';
		_usrInputNr = _usrInputNr*10 + atoi(str);
		if (_usrInputNr <= MAX_TEST_ITEMS) {
	    	OutMsg()->UsrInputItemNr(_usrInputNr);
	    }
	    else {
			_usrInputNr = _usrInputNr/10;
	    }
	}
}

//##ModelId=48102AF10170
U8 UI_RS232::ProcessKeyIn()
{
	S8 str[2];
	U32 status = SUCCESS;

	if (_event == CLEAR_DATA) {
		lib.rs232.ClearData();
		_usrInputNr = 0;
		_event = NOTHING;
	}
	GetKey();
	if (_state == SCROLLING) {
		if (isdigit(_key) != 0) {
			str[0] = (S8)_key;
			str[1] = '\0';
			_usrInputNr = atoi(str);
	    	OutMsg()->UsrInputItemNr(_usrInputNr);
			_state = INPUT_ITEM_NO;
			_timeout.ms = INPUT_ITEM_NO_MODE_TIME_INTERVAL;
			_tick_ms.Attach(&_timeout);
		}
		else {
			switch(_key)
			{
			case RS232_ARROW_UP:
				_iter.Next();
				OutMsg()->ItemNr(_iter.CurrentItem());
				break;
			case RS232_ARROW_DOWN:
				_iter.Previous();
				OutMsg()->ItemNr(_iter.CurrentItem());
				break;
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
			_tick_ms.Detach(&_timeout);
		}
		if (_key == '\r') {
			if (_usrInputNr <= MAX_TEST_ITEMS) {
				OutMsg()->NewLine();
				_iter._itemNr = _usrInputNr;
			}
			_state = SCROLLING;
			_tick_ms.Detach(&_timeout);
		}
		else if (isdigit(_key) != 0) {
			InputItemNr();
		}
	}	// if (_state == SCROLLING)
		
	if (_key == '\r') { // '\r' : 'Enter' key
		_event = CLEAR_DATA;
		_state = SCROLLING;
		return EXECUTE_ITEM;
	}
	else {
	    return SELECTING_ITEM;
	}
}

#endif	// CONFIG_WITH_RS232
