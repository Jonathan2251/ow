// Author : Gamma Chen

#ifndef USERINTERFACE_H_HEADER_INCLUDED_B904D59B
#define USERINTERFACE_H_HEADER_INCLUDED_B904D59B

#include "BltcBasicType.h"
#include "ModelConfig.h"
#include "Items.h"
#include "ItemsIterator.h"
#include "UI_OutputMsg.h"
#include "cputimer.h"
#include "Timer_MsTick.h"
#include "UI_Timer.h"

#define EXIT_UI	255

#define SELECTING_ITEM	0
#define EXECUTE_ITEM	1

#define SCROLLING		0
#define INPUT_ITEM_NO	1

#define INPUT_ITEM_NO_MODE_TIME_INTERVAL	5000 // unit 1ms, => 5 seconds

extern UI_Timer gUI_timer; 
extern Timer_MsTick gTick_ms;

//##ModelId=46CBAB08025D
class UserInterface
{
  public:
    //##ModelId=48102AF20103
  	UserInterface();
    //##ModelId=48102AF20151
  	UserInterface(UI_OutputMsg *outMsg);
    //##ModelId=46CBAB58003C
    virtual U8 SelectItem();
    //##ModelId=48102AF203A3
    virtual U8 ProcessKeyIn();	// return SELECT_ITEM or EXECUTE_ITEM
    //##ModelId=46CE717302D3
    virtual U32 RunItem(U8 itemNr, U32 arg) const;
	//##ModelId=46DCEEC30236
    virtual void Run();
    //##ModelId=48102AF300C4
    void ChangeOutMsg(UI_OutputMsg* outMsg);
    
    //##ModelId=48102AF301ED
    UI_OutputMsg* OutMsg();
    
  protected:
    //##ModelId=48102AF102C8
    U8& _state;	// reference gUI_state, => alias
    //##ModelId=48102AF10384
    U32& _usrInputNr;	// reference gUI_usrInputNr
    //##ModelId=481673E60292
	UI_Timer& _timeout;
    //##ModelId=481673940268
    Timer_MsTick& _tick_ms;
    //##ModelId=481669390390
    ItemsIterator& _iter;	// reference gIterator
  
  private:
    //##ModelId=48167A8A036C
  	UI_OutputMsg  *_outMsg;
};

#endif /* USERINTERFACE_H_HEADER_INCLUDED_B904D59B */
