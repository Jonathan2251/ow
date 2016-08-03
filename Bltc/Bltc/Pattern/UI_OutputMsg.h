#ifndef UI_OUTPUT_MSG_H_HEADER_INCLUDED
#define UI_OUTPUT_MSG_H_HEADER_INCLUDED

// Decorator pattern :
// Author : Gamma Chen
// Output message to nowhere.

#include "BltcBasicType.h"

//##ModelId=480EA4D401D5
class UI_OutputMsg
{
  public:
    //##ModelId=480EA75E00B2
    virtual void ItemNr(U8 itemNr) const;
    //##ModelId=480EA75E0100
    virtual void RunningItem(U8 itemNr) const;
    //##ModelId=480EA75E015E
    virtual void TestResult(U32 lError) const;
    //##ModelId=480EA75E01BB
    virtual void UsrInputItemNr(U32 itemNr) const;
    //##ModelId=480EA75E0229
    virtual void NewLine() const;
	virtual bool Insert(UI_OutputMsg* outMsg);
	virtual bool Delete(UI_OutputMsg* outMsg);
	virtual void DisplayStr(char *str) const;
};


#endif /* UI_OUTPUT_MSG_H_HEADER_INCLUDED */
