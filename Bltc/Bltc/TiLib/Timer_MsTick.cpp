#include "Timer_MsTick.h"

//##ModelId=481161DB032C
bool Timer_MsTick::Tick()
{
	if (Check_timer(&_timer) != 1) {
		return false;
	}
	Init_timer(&_timer, 1000*ADJUST_CPU_CLOCK_RATE);	 // unit 1us, => 1 ms
	Notify();
	return true;
}

//##ModelId=481161DB033C
void Timer_MsTick::Reset()
{
	Init_timer(&_timer, 1000* ADJUST_CPU_CLOCK_RATE);	 // unit 1us, => 1 ms
}

