// Author : Gamma Chen

#ifndef OSD_TILIB_GLYPH_H_HEADER_INCLUDED_B723D9B7
#define OSD_TILIB_GLYPH_H_HEADER_INCLUDED_B723D9B7

#ifdef CONFIG_WITH_OSD

#include "BltcBasicType.h"
#define TILIB_PI	3.1416

//##ModelId=48C4970203D6
class TiLib_Glyph
{
  public:
    //##ModelId=48C4BEF30016
    void Draw();

};

//##ModelId=48C4BC190136
class TiLib_Pen
{
  public:
    //##ModelId=48C4BEF40101
    U32 color;
};



//##ModelId=48C4BC4F0108
class TiLib_Rect
{
  public:
    //##ModelId=48C4BEF6014F
    TiLib_Rect();

    //##ModelId=48C4BEF6017E
    TiLib_Rect(U16 X, U16 Y, U16 W, U16 H, U32 COLOR);

    //##ModelId=48C4BEF60362
    void Draw();

    //##ModelId=48C4BEF503A1
    U16 x;
    //##ModelId=48C4BEF503CF
    U16 y;
    //##ModelId=48C4BEF60093
    U16 w;
    //##ModelId=48C4BEF600D2
    U16 h;
    //##ModelId=48C4BEF60110
    U32 color;
};

//##ModelId=48C4BC200184
class TiLib_Point
{
  public:
    //##ModelId=48C4BEF401EB
    TiLib_Point();

    //##ModelId=48C4BEF4020A
    TiLib_Point(U16 X, U16 Y, U32 COLOR);

    //##ModelId=48C4BEF40304
    void Draw();

    //##ModelId=48C4BEF4014F
    U16 x;
    //##ModelId=48C4BEF4017E
    U16 y;
    //##ModelId=48C4BEF401AD
    U32 color;
};

//##ModelId=48C4BC560146
class TiLib_PolyLine
{
  public:
    //##ModelId=48C4BEF500D2
    TiLib_PolyLine();

    //##ModelId=48C4BEF500F1
    TiLib_PolyLine(U16 POINT_NUM, U16 *X, U16* Y, U16 LW, U32 COLOR);

    //##ModelId=48C4BEF502F5
    void Draw();

    //##ModelId=48C4BEF40372
    U16 pointNum;
    //##ModelId=48C4BEF403B0
    U16 x[100];
    //##ModelId=48C4BEF403DF
    U16 y[100];
    //##ModelId=48C4BEF50036
    U16 lw;
    //##ModelId=48C4BEF50084
    U32 color;
};

//##ModelId=48C4BC390194
class TiLib_Line
{
  public:
    //##ModelId=48C4BEF3019D
    TiLib_Line();

    //##ModelId=48C4BEF301CC
    TiLib_Line(U16 X1, U16 Y1, U16 X2, U16 Y2, U16 LW, U32 COLOR);

    //##ModelId=48C4BEF40055
    void Draw();

    //##ModelId=48C4BEF30036
    U16 x1;
    //##ModelId=48C4BEF30064
    U16 y1;
    //##ModelId=48C4BEF30093
    U16 x2;
    //##ModelId=48C4BEF300D2
    U16 y2;
    //##ModelId=48C4BEF30110
    U16 lw;
    //##ModelId=48C4BEF3015E
    U32 color;
  private:
    //##ModelId=48C4BEF40084
    void DrawSlopeLine_DDA_Algorithm();

};

//##ModelId=48C4BC6702BD
class TiLib_Circle
{
  public:
    //##ModelId=48C4BEF10007
    TiLib_Circle();

    //##ModelId=48C4BEF10036
    TiLib_Circle(U16 CX, U16 CY, U16 R, double FROM, double TO, U16 LW, U32 COLOR);

    //##ModelId=48C4BEF10381
    void SetAngle(double from, double to);

    //##ModelId=48C4BEF200C2
    void Draw();

    //##ModelId=48C4BEF000B3
    U16 cx;
    //##ModelId=48C4BEF001BC
    U16 cy;
    //##ModelId=48C4BEF001FB
    U16 r;
    //##ModelId=48C4BEF0022A
    U16 bx;
    //##ModelId=48C4BEF00268
    U16 by;
    //##ModelId=48C4BEF002B6
    U16 ex;
    //##ModelId=48C4BEF002F5
    U16 ey;
    double from;
    double to;
    //##ModelId=48C4BEF00343
    U16 lw;
    //##ModelId=48C4BEF00391
    U32 color;
    
  private:
    //##ModelId=48C4BEF200F1
    void SetPoint_IfInRange(int x, int y);

    //##ModelId=48C4BEF2022A
    void PlotCirclePoints(int x, int y);

    //##ModelId=48C4BEF20381
    void BressCircle();
	U32 _xBound[8];
};

//##ModelId=48DC5A9502AD
class TiLib_Curve
{
  public:
  	U16
    //##ModelId=48DC5AE20399
    cx,
    //##ModelId=48DC5AE203C8
    cy;
  	U16
    //##ModelId=48DC5AE3000E
    r1,
    //##ModelId=48DC5AE3003D
    r2;
  	double
    //##ModelId=48DC5AE3007C
    from,
    //##ModelId=48DC5AE300AB
    to;
    //##ModelId=48DC5AE300F9
  	U16 lw;
    //##ModelId=48DC5AE30137
  	U32 color;
  	
    //##ModelId=48DC5AE30185
  	TiLib_Curve();
    //##ModelId=48DC5AE301A5
  	TiLib_Curve(U16 CX, U16 CY, U16 R1, U16 R2, double FROM, double TO, U16 LW, U32 COLOR);
    //##ModelId=48DC5AE400E9
  	void Draw();
};

#endif	// CONFIG_WITH_OSD

#endif /* OSD_TILIB_GLYPH_H_HEADER_INCLUDED_B723D9B7 */
