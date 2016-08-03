// Author : Gamma Chen

#ifndef OSD_TILIB_CHAR_H_HEADER_INCLUDED_B723C7E7
#define OSD_TILIB_CHAR_H_HEADER_INCLUDED_B723C7E7

#ifdef CONFIG_WITH_OSD

#include "BltcBasicType.h"

//##ModelId=48C496ED034E
class TiLib_Char
{
  public:
    //##ModelId=48C4CCBA0297
    TiLib_Char();

    //##ModelId=48C4CCBA02C6
    virtual  void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

    //##ModelId=48DC549F0317
	virtual void Draw_Matrix_5_7(const U8 *font, U16 x, U16 y, U16 w, U16 h, U32 color);
};




//##ModelId=48C49AA903A9
class TiLib_Char_0 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBB0278
    TiLib_Char_0();

    //##ModelId=48DC54A0026B
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A10019
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBB0297
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B2E0213
class TiLib_Char_1 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBC0094
    TiLib_Char_1();

    //##ModelId=48DC54A1025B
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A2000A
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBC00A3
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B3E031F
class TiLib_Char_2 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBC02A7
    TiLib_Char_2();

    //##ModelId=48DC54A2020D
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A203A3
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBC02B6
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B490364
class TiLib_Char_3 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBD00C2
    TiLib_Char_3();

    //##ModelId=48DC54A301BF
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A30346
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBD00D2
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B550399
class TiLib_Char_4 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBD02C6
    TiLib_Char_4();

    //##ModelId=48DC54A40152
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A402E8
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBD02D6
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B610025
class TiLib_Char_5 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBE00E2
    TiLib_Char_5();

    //##ModelId=48DC54A500F4
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A5027B
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBE00F1
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B84018E
class TiLib_Char_6 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBE02E5
    TiLib_Char_6();

    //##ModelId=48DC54A60058
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A601EE
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBE02F5
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49B910155
class TiLib_Char_7 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBF0101
    TiLib_Char_7();

    //##ModelId=48DC54A603B3
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A70161
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBF0111
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49BC203BD
class TiLib_Char_8 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCBF02E5
    TiLib_Char_8();

    //##ModelId=48DC54A70317
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A800C5
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCBF02F5
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49BCE023C
class TiLib_Char_9 : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC000E2
    TiLib_Char_9();

    //##ModelId=48DC54A8027B
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A90039
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC00101
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49BEC0234
class TiLib_Char_A : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC002C6
    TiLib_Char_A();

    //##ModelId=48DC54A901EE
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54A90375
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC002D6
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49C130229
class TiLib_Char_B : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC100F1
    TiLib_Char_B();

    //##ModelId=48DC54AA01BF
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AA0355
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC10101
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49C1D03BC
class TiLib_Char_C : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC102F5
    TiLib_Char_C();

    //##ModelId=48DC54AB01A0
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AB0375
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC10314
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49C27036B
class TiLib_Char_D : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC20120
    TiLib_Char_D();

    //##ModelId=48DC54AC01B0
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AC0336
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC20130
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B40800ED
class TiLib_Char_Dash : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC20333
    TiLib_Char_Dash();

    //##ModelId=48DC54AD0161
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AD02F8
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC20353
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B4130002
class TiLib_Char_Dot : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC3019D
    TiLib_Char_Dot();

    //##ModelId=48DC54AE0133
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AE02C9
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC301BC
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C49C34028D
class TiLib_Char_E : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC303DF
    TiLib_Char_E();

    //##ModelId=48DC54AF00F4
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54AF028A
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC40007
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};


//##ModelId=48C49C4001ED
class TiLib_Char_F : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC4021A
    TiLib_Char_F();

    //##ModelId=48DC54B000A6
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B0023C
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC4022A
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B39101D5
class TiLib_Char_I : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC50017
    TiLib_Char_I();

    //##ModelId=48DC54B1000A
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B101A0
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC50026
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B3AE02D0
class TiLib_Char_L : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC50249
    TiLib_Char_L();

    //##ModelId=48DC54B10356
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B20104
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC50259
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B3B8032E
class TiLib_Char_N : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC60084
    TiLib_Char_N();

    //##ModelId=48DC54B202B9
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B30077
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC60094
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B3C402C0
class TiLib_Char_O : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC60297
    TiLib_Char_O();

    //##ModelId=48DC54B3022D
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B303C3
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC602B7
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B3D601B7
class TiLib_Char_P : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC700C3
    TiLib_Char_P();

    //##ModelId=48DC54B401DF
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B40375
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC700D2
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

//##ModelId=48C4B3E201C7
class TiLib_Char_S : public TiLib_Char
{
  public:
    //##ModelId=48C4CCC702A7
    TiLib_Char_S();

    //##ModelId=48DC54B501CF
  	void Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48DC54B50356
  	void Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);
    //##ModelId=48C4CCC702C6
     void Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color);

};

struct AsciiFont {
	TiLib_Char_0 _0;
	TiLib_Char_1 _1;
	TiLib_Char_2 _2;
	TiLib_Char_3 _3;
	TiLib_Char_4 _4;
	TiLib_Char_5 _5;
	TiLib_Char_6 _6;
	TiLib_Char_7 _7;
	TiLib_Char_8 _8;
	TiLib_Char_9 _9;
	TiLib_Char_A A;
	TiLib_Char_B B;
	TiLib_Char_C C;
	TiLib_Char_D D;
	TiLib_Char_E E;
	TiLib_Char_F F;
	TiLib_Char_I I;
	TiLib_Char_L L;
	TiLib_Char_N N;
	TiLib_Char_P P;
	TiLib_Char_O O;
	TiLib_Char_S S;
	TiLib_Char_Dash dash;
	TiLib_Char_Dot dot;
};

extern AsciiFont	font;

#endif	// CONFIG_WITH_OSD

#endif /* OSD_TILIB_CHAR_H_HEADER_INCLUDED_B723C7E7 */
