#ifndef TIMER_ISR_H_HEADER_INCLUDED_B7EEC501
#define TIMER_ISR_H_HEADER_INCLUDED_B7EEC501
#include "Subject.h"

#include "BltcBasicType.h"

extern "C" asm void TimerInterruptHandler();
extern "C" void BKNI_Delay_R(unsigned int microsec);
extern asm void EnableTimerInterrupt();
extern asm void ClearEXLBit();
extern asm void ClearEXLBit_If_OnlyTimerInterrupt();
extern asm void DisableTimerInterrupt();

//##ModelId=4735A0D00366
class Timer_ISR : public Subject
{
  public:
    //##ModelId=48117853032E
    void Tick();
    //##ModelId=48117853033E
    AsmVoid EnableTimerInterrupt();
    //##ModelId=48117853035D
    AsmVoid DisableTimerInterrupt();
    //##ModelId=48117853037C
    AsmS32 IsTimerInterruptEnabled();
  private:
    //##ModelId=4811785302FF
  	int us;
};

#endif /* TIMER_ISR_H_HEADER_INCLUDED_B7EEC501 */
