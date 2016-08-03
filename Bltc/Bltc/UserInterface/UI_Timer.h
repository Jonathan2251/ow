#ifndef UI_TIMER_H_HEADER_INCLUDED
#define UI_TIMER_H_HEADER_INCLUDED

#include "BltcBasicType.h"
#include "Subject.h"
#include "cputimer.h"

// Author : Gamma Chen
// Used by UserInterface class

//##ModelId=480EE592034A
class UI_Timer : public Observer
{
  public:
    //##ModelId=48112F910348
  	UI_Timer();
    //##ModelId=48112F910368
	void Update(Subject *theChangedSubject);
    //##ModelId=48112F9103B6
  	~UI_Timer();
    //##ModelId=48118013028D
	S32 ms;
    //##ModelId=48112F91031A
	TimeStruct _timer; 
};

#endif /* UI_TIMER_H_HEADER_INCLUDED */
