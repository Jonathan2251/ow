#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 Jedi1394Interruptest()
{
	U32 lError = 1;
	
	lError = api.peripheral.Jedi1394Interruptest();
	
	return lError;
}

//JEDI2 1394 Interrupt test 
U32 Items::TI094()
{
	U32 lError = 0;
   	
	U32 (*pf)() = &Jedi1394Interruptest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	return lError;
}