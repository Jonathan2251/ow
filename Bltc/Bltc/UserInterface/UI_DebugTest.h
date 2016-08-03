// Author : Gamma Chen

#ifndef UI_DEBUG_TEST_H_HEADER_INCLUDED
#define UI_DEBUG_TEST_H_HEADER_INCLUDED

#include "UI_Decorator.h"

class UI_DebugTest : public UI_Decorator
{
  public:
  	UI_DebugTest(UI_Decorator *ui_decorator);
    virtual U8 SelectItem();
};


#endif /* UI_DEBUG_TEST_H_HEADER_INCLUDED */
