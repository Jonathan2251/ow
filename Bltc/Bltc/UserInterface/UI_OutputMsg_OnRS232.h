
// Decorator pattern :
// Author : Gamma Chen
// Forward these virtual function to UI_OutputMsg_Decorator, and output message to RS232.

//##ModelId=480EE3760258
#ifdef CONFIG_WITH_RS232

#ifndef UI_OUTPUT_MSG_ON_RS232_H_HEADER_INCLUDED
#define UI_OUTPUT_MSG_ON_RS232_H_HEADER_INCLUDED

#include "UI_OutputMsg.h"

class UI_OutputMsg_OnRS232 : public UI_OutputMsg
{
public:
    //##ModelId=48112C8A01E4
    virtual void ItemNr(U8 itemNr) const;
    //##ModelId=48112C8A0242
    virtual void RunningItem(U8 itemNr) const;
    //##ModelId=48112C8A02B0
    virtual void TestResult(U32 lError) const;
    //##ModelId=48112C8A031D
    virtual void UsrInputItemNr(U32 itemNr) const;
    //##ModelId=48112C8A039A
    virtual void NewLine() const;
	virtual void DisplayStr(char *str) const;
};

#endif /* UI_OUTPUT_MSG_ON_RS232_H_HEADER_INCLUDED */

#endif	// CONFIG_WITH_RS232
