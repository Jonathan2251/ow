// Author : Gamma Chen

//##ModelId=48112DC603E3
#ifdef CONFIG_WITH_LED

#include "UI_OutputMsg_OnLed.h"

#include "Items.h"
#include "TiLib.h"

//##ModelId=48112DC70049
void UI_OutputMsg_OnLed::ItemNr(U8 itemNr) const
{
	char buf[6];
	
	if (itemNr < 100) {
		sprintf(buf, "-%.2d-",itemNr);
	}
	else {
		sprintf(buf, "-%.3d",itemNr);
	}
	lib.led->DisplayStr(buf);
}

//##ModelId=48112DC70097
void UI_OutputMsg_OnLed::RunningItem(U8 itemNr) const
{
	char buf[6];
	
//	lib.led->AllOff();
	if (itemNr < 100) {
		sprintf(buf, "''%.2d", itemNr);
	}
	else {
		sprintf(buf, "'%.3d", itemNr);
	}
	lib.led->DisplayStr(buf);
}

//##ModelId=48112DC70105
void UI_OutputMsg_OnLed::TestResult(U32 lError) const
{
//	lib.led->AllOff();
	if (lError == 0) {
		lib.led->DisplayStr("PASS");
	}
	else if (lError == 1) {
		lib.led->DisplayStr("FAIL");
	}
	else if (lError == 2) {
		lib.led->DisplayStr("NONE");
	}
}

//##ModelId=48112DC70172
void UI_OutputMsg_OnLed::UsrInputItemNr(U32 itemNr) const
{
	char buf[6];
	
	if (itemNr < 10) {
		sprintf(buf, "   %.1d",itemNr);
	}
	else if (itemNr < 100) {
		sprintf(buf, "  %.2d",itemNr);
	}
	else {
		sprintf(buf, " %.3d",itemNr);
	}
	lib.led->DisplayStr(buf);
}

void  UI_OutputMsg_OnLed::DisplayStr(char *str) const
{
	lib.led->DisplayStr(str);
}

#endif	// CONFIG_WITH_LED