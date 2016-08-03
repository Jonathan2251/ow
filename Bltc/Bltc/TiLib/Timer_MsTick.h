#ifndef TIMER_MS_TICK_H_HEADER_INCLUDED
#define TIMER_MS_TICK_H_HEADER_INCLUDED
#include "Subject.h"
#include "cputimer.h"

// Actually, the cpu clock rate is around 400MHz, not 297MHz.
//#define ADJUST_CPU_CLOCK_RATE	 4/3
#define ADJUST_CPU_CLOCK_RATE	 1	// It's correct from TvMon10.08

//##ModelId=480EE7080206
class Timer_MsTick : public Subject
{
  public:
    //##ModelId=481161DB032C
    bool Tick();
    //##ModelId=481161DB033C
    void Reset();
  	
  private:
    //##ModelId=481161DB031D
	TimeStruct _timer; 
};

#endif /* TIMER_MS_TICK_H_HEADER_INCLUDED */
