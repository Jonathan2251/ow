// Author : Gamma Chen

#ifdef CONFIG_WITH_RS232

#ifndef UI_RS232_H_HEADER_INCLUDED_B904BC24
#define UI_RS232_H_HEADER_INCLUDED_B904BC24
#include "UserInterface.h"

#define NOTHING		0
#define CLEAR_DATA	1

//##ModelId=46CE71AC031F
class UI_RS232 : public UserInterface
{
  public:
    //##ModelId=48102AF100A5
	UI_RS232(UI_OutputMsg *outMsg);
    //##ModelId=46CE722D003D
    U8 SelectItem();
    //##ModelId=48102AF10170
    U8 ProcessKeyIn();	// return SELECT_ITEM or EXECUTE_ITEM

  private:
    //##ModelId=48102AF1019F
  	U8 GetKey();
    //##ModelId=48102AF101DE
  	void InputItemNr();
    //##ModelId=48102AF10019
	U8 _oldKey;
    //##ModelId=48102AF10057
    U8 _key;
    U8	_event;
};

#endif /* UI_RS232_H_HEADER_INCLUDED_B904BC24 */

#endif	// CONFIG_WITH_RS232
