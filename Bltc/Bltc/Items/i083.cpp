// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"

U32 FrontPanelLedTest_ProductionLine()
{
	U32 lError = 0;

	TiLib_Led::OnOffInfo onOffInfo = lib.led->GetOnOffInfo();
	lib.led->AllOff();
	lib.led->FastSpin(10);
	lib.led->AllOn();
	lib.DelayMS(500);
	lib.led->SetOnOff(onOffInfo);
 
	return lError;
}

U32 Items::TI083()
{
	U32 lError = 1;
	U32 (*pf)() = &FrontPanelLedTest_ProductionLine;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
