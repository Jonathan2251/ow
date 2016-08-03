#include "TiLib.h"
#include "MipsCpu.h"

extern "C" {
	AsmVoid TimerInterruptHandler();
//	void BKNI_Delay_R(unsigned int microsec);
}
/*
asm void BusyWaiting(U32 microsec)
{
loop_downcount:
	addi	t0, $0, 150
repeat_150_times:
	bne		t0, $0, repeat_150_times
	addi	t0, t0, -1
	bne		a0, $0, loop_downcount
	addi	a0, a0, -1
	jr		ra
	nop
}

void BKNI_Delay_R(unsigned int microsec)
{
	BusyWaiting(microsec);
}
*/
static void PrintTrace_TimerInterruptHandler()
{
//	PDEBUG("%s\r\n", __FUNCTION__);
}

// Trigger every 1 second
void Timer_Tick()
{
	lib.clock.Tick();
}

// Trigger every 10 ms
extern "C" void Timer_Tick_10ms()
{
	static int count_10ms = 0;
	
	count_10ms++;
	if (count_10ms >= 100) { 
		lib.clock.Tick();
		count_10ms = 0;
	}
}

AsmVoid ClearEXLBit()
{
	addiu   sp,sp,-32
	sw   t0,(0*8)(sp)
	sw   t1,(1*8)(sp)
	
	// clear bit 1 (EXL), 
	//		set t0 = 0s(31..16) | ((SR(15..0)&(0x00FD))
	mfc0		t0,SR
	andi		t0,t0,0xFFFD
	//		set t1 = SR(31..16)|0s(15:0)
	mfc0		t1,SR
	srl			t1,t1,16
	sll			t1,t1,16
	//		set SR = t0|t1
	or			t1,t1,t0
	mtc0		t1,SR

	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	addiu   sp,sp,32

	jr	ra
	nop
}

AsmVoid ClearEXLBit_If_OnlyTimerInterrupt()
{
	addiu   sp,sp,-32
//	sw   t0,(0*8)(sp)	// For optimization
	sw   t1,(1*8)(sp)
	sw   t3,(2*8)(sp)
	
	// t2 = Cause(15..10)|SR(15..10)
	// ref. InterruptHandler() of BSPUserExceptionMIPS.c
	andi		t3, t2, 0xDF	// Cause(10) is enable by Code Runner, so don't care
	// if has other interrupt, then  goto out, else clear SR register EXL bit.
	bne			t3, $0, out
//	nop
	sw   t0,(0*8)(sp)	// For optimization, move from previous instruction to delay slot
	// clear bit 1 (EXL), 
	//		set t0 = 0s(31..16) | ((SR(15..0)&(0x00FD))
	mfc0		t0,SR
	andi		t0,t0,0xFFFD
	//		set t1 = SR(31..16)|0s(15:0)
	mfc0		t1,SR
	srl			t1,t1,16
	sll			t1,t1,16
	//		set SR = t0|t1
	or			t1,t1,t0
	mtc0		t1,SR
		
out:
	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	lw   t3,(2*8)(sp)
	addiu   sp,sp,32

	jr	ra
	nop
}


// For TvMon10.05 and before (it's triggered by every 1 second)
AsmVoid TimerInterruptHandler()
{
	.set noreorder
	addiu   sp,sp,-32
//	sw   t0,(0*8)(sp)	// For optimization
	sw   t1,(1*8)(sp)
	sw   ra,(3*8)(sp)

	// trace
	jal		PrintTrace_TimerInterruptHandler
//	nop
	sw   t0,(0*8)(sp)	// For optimization, move from previous instruction to delay slot
	jal		Timer_Tick
	nop
		
	// Set Compare register to Count+200,000,000 around 1 s (came from try).
	mfc0		t0,Count
	addi		t1,$0,200
	sll			t1,t1,20
	addu		t1,t0,t1
//	mtc0		t1,Compare	// set Compare register to Count+200M,	// For optimization
	
	jal			ClearEXLBit_If_OnlyTimerInterrupt
//	nop
	mtc0		t1,Compare	// set Compare register to Count+200M,	// For optimization, move from previous instruction to delay slot
	

	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	lw   ra,(3*8)(sp)
	addiu   sp,sp,32
	
	jr			ra
	nop    
}
