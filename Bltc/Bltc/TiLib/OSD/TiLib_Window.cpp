// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "TiLib_Window.h"
#include "TiLib_OSD_Driver.h"

//##ModelId=48DC6E6102B4
void TiLib_Window::Create(U16 X, U16 Y, U16 W, U16 H, U16 LW, U32 BACKGROUND_COLOR, S8* STR, U32 TEXT_COLOR, U16 FONT_WIDTH, U16 FONT_HEIGHT, U16 FONT_INTERVAL, U8 ALIGNMENT)
{
    rect.x = X;
    rect.y = Y;
    rect.w = W;
    rect.h = H;
    rect.color = BACKGROUND_COLOR;
    
	text.x = rect.x+10;
	text.y = rect.y+10;
	text.lw = LW;
	text.ChangeStr(STR);
	text.color = TEXT_COLOR;
	text.w = FONT_WIDTH;
	text.h = FONT_HEIGHT;
	text.gap = FONT_INTERVAL;
	
	_alignment = ALIGNMENT;
}

//##ModelId=48DC6E62015C
void TiLib_Window::DisplayStr(S8* s)
{
	rect.Draw();
	text.ChangeStr(s);
	int length = text.Width();
	if (_alignment == WIN_LEFT) {
		text.x = rect.x + 10;
	}
	else if (_alignment == WIN_CENTER) {
		text.x = rect.x + (rect.w-length) / 2;
	}
	text.Draw();
}

#endif	// CONFIG_WITH_OSD
