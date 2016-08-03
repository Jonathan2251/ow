// Author : Gamma Chen

#ifndef OSD_TILIB_TEXT_H_HEADER_INCLUDED_B723DE48
#define OSD_TILIB_TEXT_H_HEADER_INCLUDED_B723DE48

#ifdef CONFIG_WITH_OSD

#include "BltcBasicType.h"
#include "TiLib_Char.h"

//##ModelId=48C4970F0273
class TiLib_Text
{
  public:
    //##ModelId=48C4C1150398
    TiLib_Text();

    //##ModelId=48C4C11503C7
    TiLib_Text(U16 X, U16 Y, U16 W, U16 H, U16 LW, U16 FONT_INTERVAL, U32 COLOR, S8* STR);

    //##ModelId=48C4C1160359
    void ChangeStr(S8* STR);

    //##ModelId=48C4C11603E6
    U16 Width();

    //##ModelId=48C4C117003D
    U16 Height();

    //##ModelId=48C4C117007B
    void Draw();

    //##ModelId=48C4C1150185
    U16 x;
    //##ModelId=48C4C11501A4
    U16 y;
    //##ModelId=48C4C11501D3
    U16 w;
    //##ModelId=48C4C1150202
    U16 h;
    //##ModelId=48C4C1150240
    U16 gap;
    //##ModelId=48C4C115027F
    U16 lw;
    //##ModelId=48C4C11502BD
    U32 color;
  private:
    //##ModelId=48C4C11502FC
    S8 str[100];
    //##ModelId=48C4C115034A
    AsciiFont font;
};


#endif	// CONFIG_WITH_OSD

#endif /* OSD_TILIB_TEXT_H_HEADER_INCLUDED_B723DE48 */
