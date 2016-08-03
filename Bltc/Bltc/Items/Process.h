// Author : Gamma Chen

#ifndef PROCESS_H
#define PROCESS_H

#include "Observer.h"
#include "Subject.h"
#include "BltcBasicType.h"
#include "ModelConfig.h"

#ifdef CONFIG_ENABLE_TIMER_INTERRUPT
	#define PROCESS_CREATE_IF_DEF(pf, timeOut)	process.Create(pf, timeOut)
#else
	#define PROCESS_CREATE_IF_DEF(pf, timeOut) pf()
#endif	// CONFIG_ENABLE_TIMER_INTERRUPT

#ifdef CONFIG_ENABLE_TIMER_INTERRUPT
//##ModelId=473572590232
class Process : public Observer
{
  private:
	
    //##ModelId=480FE97C0057
	AsmVoid GetPC();
    //##ModelId=480FE9C100ED
	AsmU32 RunTask(U32 (*pFunction)());
    //##ModelId=480FE91D0313
    U32 context[64];
  public:
    //##ModelId=480FEA6E01D1
	AsmVoid StopRun(U32 aReturnValue);
    //##ModelId=473572E701E6
    void Update(Subject *theChangedSubject);
    
    //##ModelId=4736B96702E2
    U32 Create(U32 (*pFunction)(), int timeOut);
    //##ModelId=473574410083
    int timeLimit;
};

extern Process process;

#endif	// CONFIG_ENABLE_TIMER_INTERRUPT

#endif /* PROCESS_H */
