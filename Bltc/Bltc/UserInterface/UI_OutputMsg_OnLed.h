
// Decorator pattern :
// Author : Gamma Chen
// Forward these virtual function to UI_OutputMsg_Decorator, and output message to LED.

//##ModelId=480EE34D014E
#ifdef CONFIG_WITH_LED

#ifndef UI_OUTPUT_MSG_ON_LED_H_HEADER_INCLUDED
#define UI_OUTPUT_MSG_ON_LED_H_HEADER_INCLUDED

#include "UI_OutputMsg_Composite.h"

class UI_OutputMsg_OnLed : public UI_OutputMsg
{
public:
    //##ModelId=48112DC70049
    virtual void ItemNr(U8 itemNr) const;
    //##ModelId=48112DC70097
    virtual void RunningItem(U8 itemNr) const;
    //##ModelId=48112DC70105
    virtual void TestResult(U32 lError) const;
    //##ModelId=48112DC70172
    virtual void UsrInputItemNr(U32 itemNr) const;
	virtual void DisplayStr(char *str) const;
};

#endif /* UI_OUTPUT_MSG_ON_LED_H_HEADER_INCLUDED */

#endif	// CONFIG_WITH_LED
