// Author : Gamma Chen
// Graphic Test

#include "ModelConfig.h"
#include "Items.h"
#include "TiLib.h"
#include "TiLib_Glyph.h"
#include "TiLib_Char.h"
#include "TiLib_Text.h"

//#define FONT_WIDTH	30
//#define FONT_HEIGHT	60
#define FONT_WIDTH	14
#define FONT_HEIGHT	28
#define FONT_LINE_WIDTH	3
#define FONT_INTERVAL	10

U32 Items::TI013()
{
#if 0
	TiLib_Rect rect = lib.window.rect;
	TiLib_Text text = lib.window.text;
    lib.osd.ClearScreen();
	lib.window.Create(100, 100, 720-200, FONT_HEIGHT+10, FONT_LINE_WIDTH, BLACK, "", WHITE, FONT_WIDTH, FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
	lib.window.DisplayStr("0123456789ABCDEFILNOPS-.");
	lib.window.Create(100, 150, 720-200, 2*FONT_HEIGHT+10, FONT_LINE_WIDTH, BLACK, "", WHITE, 2*FONT_WIDTH, 3*FONT_HEIGHT, FONT_INTERVAL, WIN_CENTER);
	lib.window.DisplayStr("ABCDEFILNOPS-.");
	
	TiLib_Circle circle(200, 300, 40, 0*TILIB_PI/2, 3*TILIB_PI/2, 5, WHITE);
	circle.Draw();
	
	TiLib_Curve curve(400, 300, 40, 20, -1*TILIB_PI/2, 1*TILIB_PI/2, 5, WHITE);
	curve.Draw();
	
	lib.DelayMS(5000);
	lib.window.rect = rect;
	lib.window.text = text;
//    lib.osd.ClearScreen();
#endif
		
	return TI_SUCCESS;
}
