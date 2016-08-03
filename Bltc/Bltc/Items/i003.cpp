// Author : Gamma Chen

// Function : Test Enable Timer Interrupt
#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "MipsCpu.h"

U32 loop()
{
	U32 lError = 0;
	
	while (1) {
	}
	
	return lError;
}

//#define CONFIG_NOT_USING_TIME_OUT
U32 Items::TI003()
{	
	U32 lError = 1;
	
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	lib.rs232.Print("Hello");
	lib.rs232.Print("GAMMA\r\n");
	tvMonIO.out.enable = 0;
// When did the single step test, the probe "CODE RUNNER" will disable interrupt
#ifdef CONFIG_NOT_USING_TIME_OUT
	lError = 1;	// Set stop pointer at here
	lError = 1;	// And will clear IE of SR at here
	lError = 1;
	int timerISR_Enabled = lib.clock.IsTimerInterruptEnabled();
	lib.clock.DisableTimerInterrupt();
	loop();
	if (timerISR_Enabled) {
		lib.clock.EnableTimerInterrupt();
	}
#else
	U32 (*pf)() = &loop;
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif
	
	return lError;	// It's right : it's equal to lw ra,16(sp); jr ra;
}
