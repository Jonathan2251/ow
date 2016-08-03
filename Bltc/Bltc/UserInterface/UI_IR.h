// Author : Gamma Chen

#ifdef CONFIG_WITH_IR

#ifndef UI_IR_H_HEADER_INCLUDED_B904F6B0
#define UI_IR_H_HEADER_INCLUDED_B904F6B0
#include "UserInterface.h"
#include "TiLib_IR.h"
#include "UI_Timer.h"

#define NOTHING		0
#define CLEAR_DATA	1

//##ModelId=46CE706C02AC
class UI_IR : public UserInterface
{
  public:
    //##ModelId=48102AF00151
	UI_IR(UI_OutputMsg *outMsg);
    //##ModelId=46CE71A0037D
    U8 SelectItem();
    //##ModelId=48102AF0020D
    U8 ProcessKeyIn();	// return SELECT_ITEM or EXECUTE_ITEM

  private:
    //##ModelId=48102AF0022C
    KEYTYPE_IR GetKey();
    //##ModelId=48102AF0025B
    bool IsKeyNr(KEYTYPE_IR key);
    //##ModelId=48102AF00307
    void Scroll();
    //##ModelId=48102AF00345
    void InputItemNr();
    
    //##ModelId=48102AF00086
    KEYTYPE_IR _key;
    //##ModelId=48102AF000C4
    KEYTYPE_IR _oldKey;
    //##ModelId=48102AF00103
    U8	_event;
    //##ModelId=48102AF00142
    UI_Timer _timer;	// timer for ir key in
};

#endif /* UI_IR_H_HEADER_INCLUDED_B904F6B0 */

#endif	// CONFIG_WITH_IR
