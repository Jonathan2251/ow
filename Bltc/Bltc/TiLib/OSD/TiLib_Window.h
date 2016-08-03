// Author : Gamma Chen

#ifndef OSD_TILIB_WINDOW_H_HEADER_INCLUDED_B72387AB
#define OSD_TILIB_WINDOW_H_HEADER_INCLUDED_B72387AB

#ifdef CONFIG_WITH_OSD

#include "TiLib_Glyph.h"
#include "TiLib_Text.h"

#define WIN_LEFT	0
#define WIN_CENTER	1

//##ModelId=48DC4DB6033F
class TiLib_Window
{
  public:
  // LW : Line Width
  	//##ModelId=48DC6E6102B4
    void Create(U16 X, U16 Y, U16 W, U16 H, U16 LW, U32 BACKGROUND_COLOR, S8* STR, U32 TEXT_COLOR, U16 FONT_WIDTH, U16 FONT_HEIGHT, U16 FONT_INTERVAL, U8 ALIGNMENT);
    //##ModelId=48DC6E62015C
  	void DisplayStr(S8 *s);
    //##ModelId=48DC6EFC01EB
    TiLib_Rect rect;
    //##ModelId=48DC6F5102A7
    TiLib_Text text;
  private:
    //##ModelId=48DC6E610277
  	U8 _alignment;
};


#endif	// CONFIG_WITH_OSD

#endif /* OSD_TILIB_WINDOW_H_HEADER_INCLUDED_B72387AB */
