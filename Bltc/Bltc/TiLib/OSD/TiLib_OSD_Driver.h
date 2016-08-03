// Author : Gamma Chen

#ifndef OSD_TILIB_OSD_DRIVER_H_HEADER_INCLUDED_B723B0BD
#define OSD_TILIB_OSD_DRIVER_H_HEADER_INCLUDED_B723B0BD

#ifdef CONFIG_WITH_OSD

#include "BltcBasicType.h"
#include "TiLib_Glyph.h"

#define GFX_UPDATE  { }

#define RED		0xffff0000
#define GREEN	0xFFFFFF00
#define PURPLE	0xffff0000
#define BLUE	0xff000080
#define LIGHT_BLUE	0xFF0000FF
#define WHITE	0xffffffff
#define BLACK	0xff000000

//##ModelId=48DC4C9A003D
class TiLib_OSD_Driver
{
  public:
    //##ModelId=48DC5D4603A0
    bool Init();
    //##ModelId=48DC5D4603B0
    void ClearScreen();
    //##ModelId=48DC5D4603C0
    void Test();
    //##ModelId=48DC5D4603DF
    void Draw();
    //##ModelId=48DC5D470007
    void SetPoint(U16 x, U16 y, U32 color);
//    void DrawRect(TiLib_Rect *rect);
//    void DrawLine(U16 x1, U16 y1, U16 x2, U16 y2, U32 color);
};


#endif	// CONFIG_WITH_OSD

#endif /* OSD_TILIB_OSD_DRIVER_H_HEADER_INCLUDED_B723B0BD */
