// Author : Gamma Chen

//##ModelId=480FE97C0057
#include "Process.h"
#include "TiLib.h"
#include "MipsCpu.h"

#ifdef CONFIG_ENABLE_TIMER_INTERRUPT

// save PC to ra
AsmVoid Process::GetPC()
{
	jr	ra
	nop
}

//##ModelId=480FEA6E01D1
AsmVoid Process::StopRun(U32 aReturnValue)
{
//  C++ compiler using a0 point to "this", a1 to first argument
	addu	v0, a1, $0	// 	a1 = aReturnValue
	lw	sp, 8(a0)	// 	lw	sp, context[1] = returnSP
	lw	ra, 12(a0)	// 	lw	sp, context[2] = returnAddr
	jr	ra
	nop
}

//##ModelId=480FE9C100ED
AsmU32 Process::RunTask(U32 (*pFunction)())
{
   .set noreorder
	
	addiu   sp,sp,-216

	sw  s0,(0*8)(sp)
	sw  s1,(1*8)(sp)
	sw  s2,(2*8)(sp)
	sw  s3,(3*8)(sp)
	sw  s4,(4*8)(sp)
	sw  s5,(5*8)(sp)
	sw  s6,(6*8)(sp)
	sw  s7,(7*8)(sp)
	sw  s8,(8*8)(sp)
	sw  ra,(9*8)(sp)

	addiu	v0, $0, 1
	// C++ compiler using a0 pointer to "this", context[] is first member variable in Process, so a0 point to context[0].
	sw	v0, 4(a0)	// 	sw	v0, context[0] = returnValue
	sw	sp, 8(a0)	// 	sw	sp, context[1] = returnSP
	jal	GetPC
	nop
	addi	ra, ra, 16
	sw		ra, 12(a0)	// context[2] = returnAddr
	
	jalr	ra, a1	// For C++ compiler, a1 = pFunction, so jump to pFunction.
					// Note, C compiler using a0 = first argument; C++ using a0 point to "this", a1 to first argument
	nop
	
back_addr:
	
	lw  s0,(0*8)(sp)
	lw  s1,(1*8)(sp)
	lw  s2,(2*8)(sp)
	lw  s3,(3*8)(sp)
	lw  s4,(4*8)(sp)
	lw  s5,(5*8)(sp)
	lw  s6,(6*8)(sp)
	lw  s7,(7*8)(sp)
	lw  s8,(8*8)(sp)
	lw  ra,(9*8)(sp)
	nop
	
	addiu   sp,sp,216
	
	jr ra
	nop
}						

//##ModelId=473572E701E6
void Process::Update(Subject *theChangedSubject)
{
//	PDEBUG("Items_TimeOut::Update()\r\n");
	timeLimit--;
	if (timeLimit <= 0) {
		PDEBUG("Time Out\r\n");
		PDEBUG("returnVaule = %x, returnSP = %x, returnAddr = %x\r\n", context[0], context[1], context[2]);
		StopRun(1);
	}
}

//##ModelId=4736B96702E2
U32 Process::Create(U32 (*pFunction)(), int timeOut)
{
//	lib.clock.EnableTimerInterrupt();	// From TvMon10.09b, the Timer Interrupt is enable
	timeLimit = timeOut;
	lib.clock.Attach(this);
	ClearEXLBit();
	U32 lError = RunTask(pFunction);
	lib.clock.Detach(this);
//	lib.clock.DisableTimerInterrupt();
	
	return lError;
}

Process process;

#endif	// CONFIG_ENABLE_TIMER_INTERRUPT
