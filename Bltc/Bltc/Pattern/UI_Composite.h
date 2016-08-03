#ifndef UI_COMPOSITE_H_HEADER_INCLUDED
#define UI_COMPOSITE_H_HEADER_INCLUDED

// Composite pattern :
// Author : Gamma Chen
// Scheduling by round robin for multi UI.

#include "UserInterface.h"
#include "UI_OutputMsg.h"

//##ModelId=480EA489012B
class UI_Composite : public UserInterface
{
  public:
    //##ModelId=480EA75D0371
	UI_Composite(UI_OutputMsg *outMsg);
    //##ModelId=480EA75D03BF
    virtual bool Insert(UserInterface *ui);
    //##ModelId=480EA75E0054
    virtual U8 SelectItem();
    //##ModelId=480EA75E0083
    virtual U8 ProcessKeyIn();	// return SELECT_ITEM or EXECUTE_ITEM
    
  private:
    //##ModelId=480EA75D0334
    UserInterface* _ui[10];
    //##ModelId=480EA75D0342
    int _size;
};


#endif /* UI_COMPOSITE_H_HEADER_INCLUDED */
