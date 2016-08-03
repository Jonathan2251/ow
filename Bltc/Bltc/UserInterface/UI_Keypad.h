// Author : Gamma Chen

#ifdef CONFIG_WITH_KEYPAD

#ifndef UI_KEYPAD_H_HEADER_INCLUDED_B7DC51E2
#define UI_KEYPAD_H_HEADER_INCLUDED_B7DC51E2
#include "UserInterface.h"
#include "TiLib_Keypad.h"

//##ModelId=4823B4DB01C9
class UI_Keypad : public UserInterface
{
  public:
    //##ModelId=4823B7890204
	UI_Keypad(UI_OutputMsg *outMsg);
    //##ModelId=4823B7890262
    U8 SelectItem();
    //##ModelId=4823B7890272
    U8 ProcessKeyIn();	// return SELECT_ITEM or EXECUTE_ITEM
  
  private:
    //##ModelId=4823B7890291
    bool LongPressed();
    //##ModelId=4823B78902B0
    bool GetKey(U32 interval);

    //##ModelId=4823B78901A8
    KEYTYPE _key;
    //##ModelId=4823B78901B7
    KEYTYPE _oldKey;
    //##ModelId=4823B78901C6
    U32 _pressedTime_ms;
    
    //##ModelId=4823B78901F6
    UI_Timer _timer;	// timer for key in front panel keypad
};

#endif /* UI_KEYPAD_H_HEADER_INCLUDED_B7DC51E2 */

#endif	// CONFIG_WITH_KEYPAD
