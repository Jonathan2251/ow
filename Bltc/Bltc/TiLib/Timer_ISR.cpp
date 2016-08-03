#include "Timer_ISR.h"

#include "MipsCpu.h"
#include "TiLib_RS232.h"
#include "TiLib.h"

AsmVoid Timer_ISR::EnableTimerInterrupt()
{
	addiu   sp,sp,-32
	sw   t0,(0*8)(sp)
	sw   t1,(1*8)(sp)
      
	// Set Compare register to Count+200,000,000 around 1 s.
	mfc0		t0,Count
	addi		t1,$0,200
	sll			t1,t1,20
	addu		t1,t0,t1
	mtc0		t1,Compare	// set Compare register to Count+200M
	
	// clear Cause register timer interrupt pending bit
	mfc0		t0,Cause
	andi		t0,t0,0x7fff
	mfc0		t1,Cause
	srl			t1,t1,16
	sll			t1,t1,16
	or			t1,t1,t0
	mtc0		t1,Cause
	
	// Enable timer interrupt (set bit 15, clear bit 1 (EXL), 
	// and set bit 0 in SR register)
	//		set t0 = 0s(31..16) | ((SR(15..0)&(0xFFFD)|(0x8001))
	mfc0		t0,SR
	andi		t0,t0,0xFFFD
	ori			t0,t0,0x8001
	//		set t1 = SR(31..16)|0s(15:0)
	mfc0		t1,SR
	srl			t1,t1,16
	sll			t1,t1,16
	//		set SR = t0|t2
	or			t1,t1,t0
	mtc0		t1,SR
	
	lw   t0,(0*8)(sp)
	lw   t1,(1*8)(sp)
	addiu   sp,sp,32

	jr	ra
	nop
}

AsmVoid Timer_ISR::DisableTimerInterrupt()
{
	addiu   sp,sp,-32
	sw   t0,(0*8)(sp)
	sw   t1,(1*8)(sp)
	
	// Disable timer interrupt (clear bit 15, clear bit 1 (EXL), in SR register)
	//		set t0 = 0s(31..16) | ((SR(15..0)&(0x00FD))
	mfc0		t0,SR
	andi		t0,t0,0x7FFD
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

AsmS32 Timer_ISR::IsTimerInterruptEnabled()
{
	mfc0		v0,SR
	andi		v0,v0,0x8000
	jr			ra
	nop
}

//##ModelId=4735A32700EE
void Timer_ISR::Tick()
{
//	PDEBUG("Tick\r\n");	// Will crash system
//	PDEBUG("T");	// Wont's crash system
	Notify();
}


