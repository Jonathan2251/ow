#ifndef UI_DECORATOR_H_HEADER_INCLUDED
#define UI_DECORATOR_H_HEADER_INCLUDED

// Decorator pattern :
// Author : Gamma Chen

#include "UserInterface.h"

//##ModelId=480EA5000156
class UI_Decorator : public UserInterface
{
  public:
    //##ModelId=480EA75F0114
  	UI_Decorator(UserInterface *ui);
    virtual U8 SelectItem();
    //##ModelId=480EA75F017D
    virtual U8 ProcessKeyIn();
  private:
    //##ModelId=481528940120
    UserInterface *_ui;
    
};


#endif /* UI_MONO_H_HEADER_INCLUDED */
