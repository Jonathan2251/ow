// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "UI_OutputMsg_OnOSD.h"
#include "TiLib.h"

#define FONT_WIDTH	50
#define FONT_HEIGHT	100
#define FONT_LINE_WIDTH	10
#define FONT_INTERVAL	20

UI_OutputMsg_OnOSD::UI_OutputMsg_OnOSD()
{
// Font Line Width = 10
//	lib.window.Create(150, 300, 400, FONT_HEIGHT+20, FONT_LINE_WIDTH, BLUE, "", RED, FONT_WIDTH, FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
	lib.window.Create(150, 330, 400, FONT_HEIGHT+20, FONT_LINE_WIDTH, BLACK, "", WHITE, FONT_WIDTH, FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
}

void UI_OutputMsg_OnOSD::ItemNr(U8 itemNr) const
{
	char str[10];
	
	if (itemNr < 100) {
		sprintf(str, "-%.2d-", itemNr);
		lib.window.DisplayStr(str);
	}
	else {
		sprintf(str, "-%.3d-", itemNr);
		lib.window.DisplayStr(str);
	}
}

void UI_OutputMsg_OnOSD::RunningItem(U8 itemNr) const
{
	char str[10];
	
	if (itemNr < 100) {
		sprintf(str, "..%.2d", itemNr);
	}
	else {
		sprintf(str, "..%.3d", itemNr);
	}
	lib.window.DisplayStr(str);
}

void UI_OutputMsg_OnOSD::TestResult(U32 lError) const
{	
	if (lError == 0) {
		lib.window.DisplayStr("PASS");
	}
	else if (lError == 1) {
		lib.window.DisplayStr("FAIL");
	}
	else if (lError == 2) {
		lib.window.DisplayStr("NONE");
	}
}

void UI_OutputMsg_OnOSD::UsrInputItemNr(U32 itemNr) const
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
	lib.window.DisplayStr(buf);
}

void  UI_OutputMsg_OnOSD::DisplayStr(char *str) const
{
	lib.window.DisplayStr(str);
}

#endif	// CONFIG_WITH_OSD
