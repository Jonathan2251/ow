// Author : Gamma Chen

//##ModelId=48112F910348
#include "UI_Timer.h"
#include "UserInterface.h"

UI_Timer::UI_Timer()
{
//	ms = 0;
//	gTick_ms.Attach(this);
}

//##ModelId=48112F910368
void UI_Timer::Update(Subject *theChangedSubject)
{
//	downcount until ms == -1
	if (ms >= 0) {
		ms--;
	}
}

//##ModelId=48112F9103B6
UI_Timer::~UI_Timer()
{
//	gTick_ms.Detach(this);
}
