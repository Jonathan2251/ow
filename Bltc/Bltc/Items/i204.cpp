// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"

#if 1
#include "UI_StationTest.h"
#include "UserInterface.h"
#include "TiLib_Glyph.h"
#include "TiLib_Text.h"

extern UI_StationTest ui_stationTest;
extern UserInterface *ui;

#define FONT_WIDTH	30
#define FONT_HEIGHT	60
#define FONT_LINE_WIDTH	6
#define FONT_INTERVAL	10
#endif // #if 0

void Items::AI204()
{
#if 1
	char sError[20];
	
	// Save original setting of window.
	TiLib_Rect rect = lib.window.rect;
	TiLib_Text text = lib.window.text;
/*	if (ui != &ui_stationTest) {
		return;
	}*/
	ErrorCode	error = ui_stationTest.CurrStation()->Error();
	for (int i = 0; i < error.size; i++) {
		sprintf(sError+i*4, "%.4X", error.code[i]);
	}
    lib.osd.ClearScreen();
//		lib.window.Create(0, 380, 720, 200, FONT_LINE_WIDTH, BLUE, "", RED, FONT_WIDTH, FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
	lib.window.Create(0, 380, 720, 200, FONT_LINE_WIDTH, BLACK, "", WHITE, FONT_WIDTH, FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
	lib.window.DisplayStr(sError);
	
	// Restore original setting of window.
	lib.window.rect = rect;
	lib.window.text = text;
#endif // #if 0
	
	return;
}

#endif	// CONFIG_WITH_OSD
