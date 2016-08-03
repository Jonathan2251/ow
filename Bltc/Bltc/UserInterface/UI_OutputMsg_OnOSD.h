
// Decorator pattern :
// Author : Gamma Chen
// Forward these virtual function to UI_OutputMsg_Decorator, and output message to OSD.

//##ModelId=480EE35D02D5
#ifdef CONFIG_WITH_OSD

#ifndef UI_OUTPUT_MSG_ON_OSD_H_HEADER_INCLUDED
#define UI_OUTPUT_MSG_ON_OSD_H_HEADER_INCLUDED

#include "UI_OutputMsg.h"
#include "TiLib_Glyph.h"
#include "TiLib_Text.h"

class UI_OutputMsg_OnOSD : public UI_OutputMsg
{
public:
	UI_OutputMsg_OnOSD();
    //##ModelId=480EE35D02D5
    virtual void ItemNr(U8 itemNr) const;
    //##ModelId=48112DC7026C
    virtual void RunningItem(U8 itemNr) const;
    //##ModelId=48112DC702CA
    virtual void TestResult(U32 lError) const;
    //##ModelId=48112DC70337
    virtual void UsrInputItemNr(U32 itemNr) const;
	virtual void DisplayStr(char *str) const;
};

#endif /* UI_OUTPUT_MSG_ON_OSD_H_HEADER_INCLUDED */

#endif	// CONFIG_WITH_OSD
