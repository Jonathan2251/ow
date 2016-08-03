#ifndef UI_OUTPUTMSG_COMPOSITE_H_HEADER_INCLUDED_B7BAE38E
#define UI_OUTPUTMSG_COMPOSITE_H_HEADER_INCLUDED_B7BAE38E
#include "UI_OutputMsg.h"

// Composite pattern :
// Author : Gamma Chen
// Forward these virtual function to UI_OutputMsg.

//##ModelId=48450B4302A7
class UI_OutputMsg_Composite : public UI_OutputMsg
{
  public:
    //##ModelId=48450C1602D9
  	UI_OutputMsg_Composite();
    //##ModelId=48450C1602F9
	virtual bool Insert(UI_OutputMsg* outMsg);
	virtual bool Delete(UI_OutputMsg* outMsg);
    //##ModelId=48450C160366
    virtual void ItemNr(U8 itemNr) const;
    //##ModelId=48450C1603D3
    virtual void RunningItem(U8 itemNr) const;
    //##ModelId=48450C170078
    virtual void TestResult(U32 lError) const;
    //##ModelId=48450C170105
    virtual void UsrInputItemNr(U32 itemNr) const;
    //##ModelId=48450C1701A1
    virtual void NewLine() const;
	virtual void DisplayStr(char *str) const;
    
  private:
    //##ModelId=48450C16024D
    int _size;
    //##ModelId=48450C6F01F0
    UI_OutputMsg *_outMsg[10];
};



#endif /* UI_OUTPUTMSG_COMPOSITE_H_HEADER_INCLUDED_B7BAE38E */
