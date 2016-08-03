// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"

U32 FrontPanelLedTest_Lab()
{
	U32 lError = 0;

	TiLib_Led::OnOffInfo onOffInfo = lib.led->GetOnOffInfo();
	lib.led->AllOff();
	lib.led->SlowSpin();
	lib.led->AllOn();
	lib.DelayMS(2000);
	lib.led->SetOnOff(onOffInfo);
 
	return lError;
}

U32 Items::TI082()
{
	U32 lError = 1;
	U32 (*pf)() = &FrontPanelLedTest_Lab;
	
	int timeOut = 30;	// unit:1s, => 30 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
