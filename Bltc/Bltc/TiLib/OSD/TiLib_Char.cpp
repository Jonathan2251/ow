// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "OSD/TiLib_Char.h"

#include "TiLib_Glyph.h"
#include "TiLib.h"
#include <math.h>

const U8 font_5_7_0[8] = {
  0b11111000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b11111000,
  0b00000000
};
 
const U8 font_5_7_1[8] = {
  0b00100000,
  0b01100000,
  0b10100000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_2[8] = {
  0b11111000,
  0b10001000,
  0b00001000,
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_3[8] = {
  0b11111000,
  0b00001000,
  0b00001000,
  0b11111000,
  0b00001000,
  0b00001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_4[8] = {
  0b00111000,
  0b01010000,
  0b10010000,
  0b10010000,
  0b11111000,
  0b00010000,
  0b00010000,
  0b00000000
};

const U8 font_5_7_5[8] = {
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b00001000,
  0b00001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_6[8] = {
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b10001000,
  0b10001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_7[8] = {
  0b11111000,
  0b10001000,
  0b00001000,
  0b00010000,
  0b00010000,
  0b00100000,
  0b00100000,
  0b00000000
};

const U8 font_5_7_8[8] = {
  0b11111000,
  0b10001000,
  0b10001000,
  0b11111000,
  0b10001000,
  0b10001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_9[8] = {
  0b11111000,
  0b10001000,
  0b10001000,
  0b11111000,
  0b00001000,
  0b00001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_A[8] = {
  0b00100000,
  0b01010000,
  0b01010000,
  0b10001000,
  0b11111000,
  0b10001000,
  0b10001000,
  0b00000000
};

const U8 font_5_7_B[8] = {
  0b11110000,
  0b10001000,
  0b10001000,
  0b11110000,
  0b10001000,
  0b10001000,
  0b11110000,
  0b00000000
};

const U8 font_5_7_C[8] = {
  0b00110000,
  0b01001000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b01001000,
  0b00110000,
  0b00000000
};

const U8 font_5_7_D[8] = {
  0b11100000,
  0b10010000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b10010000,
  0b11100000,
  0b00000000
};

const U8 font_5_7_E[8] = {
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_F[8] = {
  0b11111000,
  0b10000000,
  0b10000000,
  0b11110000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b00000000
};

const U8 font_5_7_I[8] = {
  0b01110000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b01110000,
  0b00000000
};

const U8 font_5_7_L[8] = {
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_N[8] = {
  0b10001000,
  0b11001000,
  0b11001000,
  0b10101000,
  0b10101000,
  0b10011000,
  0b10001000,
  0b00000000
};

const U8 font_5_7_O[8] = {
  0b01110000,
  0b11011000,
  0b10001000,
  0b10001000,
  0b10001000,
  0b11011000,
  0b01110000,
  0b00000000
};

const U8 font_5_7_P[8] = {
  0b11110000,
  0b10001000,
  0b10001000,
  0b11110000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b00000000
};

const U8 font_5_7_S[8] = {
  0b11111000,
  0b10000000,
  0b10000000,
  0b11111000,
  0b00001000,
  0b00001000,
  0b11111000,
  0b00000000
};

const U8 font_5_7_Dash[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const U8 font_5_7_Dot[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011000,
  0b00011000,
  0b00000000
};

//##ModelId=48C4CCBA0297
TiLib_Char::TiLib_Char()
{
}

//##ModelId=48C4CCBA02C6
 void TiLib_Char::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
}

//##ModelId=48DC549F0317
void TiLib_Char::Draw_Matrix_5_7(const U8 *font, U16 x, U16 y, U16 w, U16 h, U32 color)
{
	int i, j, m, n;
	float dx = w/5;
	float dy = h/7;
	
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 5; j++) {
			if (font[i] & (0b10000000 >> j)) {
				for (m = ceil(i*dy); m <= floor((i+1)*dy-1); m++) {
					for (n = ceil(j*dx); n <= floor((j+1)*dx-1); n++) {
						lib.osd.SetPoint(x+n, y+m, color);
					}
				}
			}
		}
	}
}

//##ModelId=48DC54A0026B
void TiLib_Char_0::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w-lw, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+(h-lw);
	line.Draw();	//     |
	line.x1 = x;
	line.y1 = y+(h-lw);
	line.x2 = x+w;
	line.y2 = y+(h-lw);
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	// |
}

//##ModelId=48DC54A10019
void TiLib_Char_0::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 1*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Line line1(x, y+1*h/4, x, y+3*h/4, lw, color);
	line1.Draw();	// |
	TiLib_Line line2(x+w-lw, y+1*h/4, x+w-lw, y+3*h/4, lw, color);
	line2.Draw();	// |
	TiLib_Curve curve2(x+w/2, y+3*h/4, w/2, h/4, -1*TILIB_PI, 0*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCBB0278
TiLib_Char_0::TiLib_Char_0()
{
}

//##ModelId=48C4CCBB0297
 void TiLib_Char_0::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_0, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBC0094
TiLib_Char_1::TiLib_Char_1()
{
}

//##ModelId=48DC54A1025B
void TiLib_Char_1::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x+(w-lw)/2, y, x+(w-lw)/8, y+h/4, lw, color);
	
	line.Draw();	//   /
	line.x1 = x+(w-lw)/2;
	line.y1 = y;
	line.x2 = x+(w-lw)/2;
	line.y2 = y+h-lw;
	line.Draw();	//  |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A2000A
void TiLib_Char_1::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBC00A3
 void TiLib_Char_1::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_1, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBC02A7
TiLib_Char_2::TiLib_Char_2()
{
}

//##ModelId=48DC54A2020D
void TiLib_Char_2::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w-lw, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+(h+lw)/2;
	line.Draw();	//     |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w-lw;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y+h/2;
	line.x2 = x;
	line.y2 = y+h-lw;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A203A3
void TiLib_Char_2::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, -1*TILIB_PI/4, TILIB_PI, lw, color);
	curve1.Draw();
	int x1 = curve1.cx+curve1.r1*cos(-1*TILIB_PI/4)+lw/2;
	int y1 = curve1.cy-curve1.r2*sin(-1*TILIB_PI/4)-lw/2;
	TiLib_Line line(x1-lw, y1, x, y+h-lw, lw, color);
	line.Draw();	// /
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48C4CCBC02B6
 void TiLib_Char_2::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_2, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBD00C2
TiLib_Char_3::TiLib_Char_3()
{
}

//##ModelId=48DC54A301BF
void TiLib_Char_3::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w-lw, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h/2;
	line.Draw();	//    |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w-lw;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y+h/2;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w-lw;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A30346
void TiLib_Char_3::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w/2, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w/2;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w/2;
	line.y2 = y+h-lw;
	line.Draw();	// ---
	TiLib_Curve curve1(x+w/2, y+1*h/4+lw/4, w/2, h/4+lw/4, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve1.Draw();
	TiLib_Curve curve2(x+w/2, y+3*h/4-lw/4, w/2, h/4+lw/4, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve2.Draw();
/*	TiLib_Curve curve3(x+3*w/4,  y+1*h/4, 5, 5, 0*TILIB_PI, 2*TILIB_PI, 5, color);
	curve3.Draw();
	TiLib_Curve curve4(x+3*w/4,  y+3*h/4, 5, 5, 0*TILIB_PI, 2*TILIB_PI, 5, color);
	curve4.Draw();*/
}

//##ModelId=48C4CCBD00D2
 void TiLib_Char_3::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_3, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBD02C6
TiLib_Char_4::TiLib_Char_4()
{
}

//##ModelId=48DC54A40152
void TiLib_Char_4::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x+4*w/5-lw, y, x, y+h*3/4, lw, color);
	
	line.Draw();	//   /
	line.x1 = x;
	line.y1 = y+h*3/4;
	line.x2 = x+w;
	line.y2 = y+h*3/4;
	line.Draw();	// ---
	line.x1 = x+4*w/5-lw;
	line.y1 = y;
	line.x2 = x+4*w/5-lw;
	line.y2 = y+h;
	line.Draw();	//  |
}

//##ModelId=48DC54A402E8
void TiLib_Char_4::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBD02D6
 void TiLib_Char_4::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_4, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBE00E2
TiLib_Char_5::TiLib_Char_5()
{
}

//##ModelId=48DC54A500F4
void TiLib_Char_5::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h/2;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w-lw;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w-lw;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A5027B
void TiLib_Char_5::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ----
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+1*h/2;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w/2;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// --
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w/2;
	line.y2 = y+h-lw;
	line.Draw();	// --
	TiLib_Curve curve1(x+w/2, y+3*h/4-lw/4, w/2, h/4+lw/4, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve1.Draw();
}

//##ModelId=48C4CCBE00F1
 void TiLib_Char_5::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_5, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBE02E5
TiLib_Char_6::TiLib_Char_6()
{
}

//##ModelId=48DC54A60058
void TiLib_Char_6::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y+h/2;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w-lw;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A601EE
void TiLib_Char_6::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 1*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Line line(x, y+1*h/4, x, y+3*h/4, lw, color);
	line.Draw();
	TiLib_Curve curve2(x+w/2, y+2*h/3, w/2, h/3, 0*TILIB_PI, 2*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCBE02F5
 void TiLib_Char_6::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_6, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBF0101
TiLib_Char_7::TiLib_Char_7()
{
}

//##ModelId=48DC54A603B3
void TiLib_Char_7::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w/2-lw;
	line.y2 = y+h;
	line.Draw();	// /
}

//##ModelId=48DC54A70161
void TiLib_Char_7::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBF0111
 void TiLib_Char_7::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_7, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCBF02E5
TiLib_Char_8::TiLib_Char_8()
{
}

//##ModelId=48DC54A70317
void TiLib_Char_8::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w-lw, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A800C5
void TiLib_Char_8::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 2*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Curve curve2(x+w/2, y+3*h/4-lw/2, w/2, h/4+lw/2, 0*TILIB_PI, 2*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCBF02F5
 void TiLib_Char_8::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_8, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC000E2
TiLib_Char_9::TiLib_Char_9()
{
}

//##ModelId=48DC54A8027B
void TiLib_Char_9::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w-lw, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h/2;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A90039
void TiLib_Char_9::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 2*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Line line(x+w-lw, y+1*h/4, x+w-lw, y+3*h/4, lw, color);
	line.Draw();
	TiLib_Curve curve2(x+w/2, y+2*h/3, w/2, h/3, -1*TILIB_PI, 0*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCC00101
 void TiLib_Char_9::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_9, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC002C6
TiLib_Char_A::TiLib_Char_A()
{
}

//##ModelId=48DC54A901EE
void TiLib_Char_A::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x+(w-lw)/2, y, x, y+h/2, lw, color);
	
	line.Draw();	//    /
	line.x1 = x;
	line.y1 = y+h/2;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x+(w-lw)/2;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h/2;
	line.Draw();	//     \
	
	line.x1 = x+w-lw;
	line.y1 = y+h/2;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
//	line.x1 = x+w/6-lw;
	line.x1 = x;
	line.y1 = y+(h)*5/6-lw;
	line.x2 = x+w;
	line.y2 = y+(h)*5/6-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54A90375
void TiLib_Char_A::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC002D6
 void TiLib_Char_A::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_A, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC100F1
TiLib_Char_B::TiLib_Char_B()
{
}

//##ModelId=48DC54AA01BF
void TiLib_Char_B::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	U16 X[11], Y[11];
	X[0] = x;			Y[0] = y;
	X[1] = x+w*2/3-lw;	Y[1] = y;
	X[2] = x+w-lw;		Y[2] = y+h/6-lw/2;
	X[3] = x+w-lw;		Y[3] = y+h/3-lw/2;
	X[4] = x+w*2/3-lw;	Y[4] = y+h/2-lw/2;
	X[5] = x;			Y[5] = y+h/2-lw/2;
	TiLib_PolyLine polyline1(6, X, Y, lw, color);
	polyline1.Draw();
	X[0] = x+w*2/3-lw;	Y[0] = y+h/2-lw/2;
	X[1] = x+w-lw;		Y[1] = y+h*4/6-lw/2;
	X[2] = x+w-lw;		Y[2] = y+h*5/6-lw/2;
	X[3] = x+w*2/3-lw;	Y[3] = y+h;
	TiLib_PolyLine polyline2(4, X, Y, lw, color);
	polyline2.Draw();
	
	TiLib_Line line(x+w*2/3-lw, y+h-lw, x, y+h-lw, lw, color);
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	//  ---
}

//##ModelId=48DC54AA0355
void TiLib_Char_B::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w/2, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y+h/2-lw;
	line.x2 = x+w/2;
	line.y2 = y+h/2-lw;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w/2;
	line.y2 = y+h-lw;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve1.Draw();
	TiLib_Curve curve2(x+w/2, y+3*h/4-lw/2, w/2, h/4+lw/2, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCC10101
 void TiLib_Char_B::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_B, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC102F5
TiLib_Char_C::TiLib_Char_C()
{
}

//##ModelId=48DC54AB01A0
void TiLib_Char_C::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h-lw;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54AB0375
void TiLib_Char_C::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 1*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Line line(x, y+1*h/4, x, y+3*h/4, lw, color);
	line.Draw();	// |
	TiLib_Curve curve2(x+w/2, y+3*h/4, w/2, h/4, 1*TILIB_PI, 2*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCC10314
 void TiLib_Char_C::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_C, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC20120
TiLib_Char_D::TiLib_Char_D()
{
}

//##ModelId=48DC54AC01B0
void TiLib_Char_D::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	U16 X[11], Y[11];
	X[0] = x;				Y[0] = y;
	X[1] = x+w*1/2;			Y[1] = y;
	X[2] = x+w-lw;			Y[2] = y+(h-lw)*1/4;
	X[3] = x+w-lw;			Y[3] = y+(h-lw)*3/4;
	X[4] = x+(w-lw)*1/2;	Y[4] = y+(h-lw);
	X[5] = x;				Y[5] = y+(h-lw);
	X[6] = x;				Y[6] = y;
	TiLib_PolyLine polyline1(7, X, Y, lw, color);

	polyline1.Draw();
}

//##ModelId=48DC54AC0336
void TiLib_Char_D::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x, y+h, lw, color);
	line.Draw();
	TiLib_Curve curve1(x, y+1*h/2, w, h/2, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve1.Draw();
}

//##ModelId=48C4CCC20130
 void TiLib_Char_D::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_D, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC20333
TiLib_Char_Dash::TiLib_Char_Dash()
{
}

//##ModelId=48DC54AD0161
void TiLib_Char_Dash::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y+(h-lw)/2, x+w, y+(h-lw)/2, lw, color);
	
	line.Draw();	//   ---
}

//##ModelId=48DC54AD02F8
void TiLib_Char_Dash::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC20353
 void TiLib_Char_Dash::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_Dash, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC3019D
TiLib_Char_Dot::TiLib_Char_Dot()
{
}

//##ModelId=48DC54AE0133
void TiLib_Char_Dot::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	U16 hh = w/4;
	TiLib_Line line(x, y+h-hh, x+hh, y+h-hh, hh, color);
	
	line.Draw();	//   ---
}

//##ModelId=48DC54AE02C9
void TiLib_Char_Dot::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	U16 lower = (w/2 < h/4) ? w/2 : h/4;
	lower /= 2;
	TiLib_Curve curve1(x+w/2, y+3*h/4, lower, lower, 0*TILIB_PI, 2*TILIB_PI, lower, color);
	curve1.Draw();
}

//##ModelId=48C4CCC301BC
 void TiLib_Char_Dot::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_Dot, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC303DF
TiLib_Char_E::TiLib_Char_E()
{
}

//##ModelId=48DC54AF00F4
void TiLib_Char_E::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54AF028A
void TiLib_Char_E::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC40007
 void TiLib_Char_E::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_E, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}


//##ModelId=48C4CCC4021A
TiLib_Char_F::TiLib_Char_F()
{
}

//##ModelId=48DC54B000A6
void TiLib_Char_F::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w*3/4;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
}

//##ModelId=48DC54B0023C
void TiLib_Char_F::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC4022A
 void TiLib_Char_F::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_F, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC50017
TiLib_Char_I::TiLib_Char_I()
{
}

//##ModelId=48DC54B1000A
void TiLib_Char_I::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x+1*w/4, y, x+w*3/4, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x+(w-lw)/2;
	line.y1 = y;
	line.x2 = x+(w-lw)/2;
	line.y2 = y+h;
	line.Draw();	//  |
	line.x1 = x+1*w/4;
	line.y1 = y+(h-lw);
	line.x2 = x+w*3/4;
	line.y2 = y+(h-lw);
	line.Draw();	// ---
}

//##ModelId=48DC54B101A0
void TiLib_Char_I::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC50026
 void TiLib_Char_I::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_I, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC50249
TiLib_Char_L::TiLib_Char_L()
{
}

//##ModelId=48DC54B10356
void TiLib_Char_L::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x, y+h, lw, color);
	
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54B20104
void TiLib_Char_L::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC50259
 void TiLib_Char_L::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_L, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC60084
TiLib_Char_N::TiLib_Char_N()
{
}

//##ModelId=48DC54B202B9
void TiLib_Char_N::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x, y+h, lw, color);
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	// \
	
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
}

//##ModelId=48DC54B30077
void TiLib_Char_N::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	Draw_ByVector_OnlyLine(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC60094
 void TiLib_Char_N::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_N, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC60297
TiLib_Char_O::TiLib_Char_O()
{
}

//##ModelId=48DC54B3022D
void TiLib_Char_O::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw);
	line.x2 = x+w;
	line.y2 = y+(h-lw);
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+h;
	line.Draw();	//   |
}

//##ModelId=48DC54B303C3
void TiLib_Char_O::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve(x+w/2, y+h/2, w/2, h/2, 0*TILIB_PI, 2*TILIB_PI, lw, color);
	curve.Draw();
}

//##ModelId=48C4CCC602B7
 void TiLib_Char_O::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_O, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC700C3
TiLib_Char_P::TiLib_Char_P()
{
}

//##ModelId=48DC54B401DF
void TiLib_Char_P::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y;
	line.x2 = x+w-lw;
	line.y2 = y+(h-lw)/2;
	line.Draw();	//   |
}

//##ModelId=48DC54B40375
void TiLib_Char_P::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w/2, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y+h/2-lw;
	line.x2 = x+w/2;
	line.y2 = y+h/2-lw;
	line.Draw();	// ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+h;
	line.Draw();	// |
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, -1*TILIB_PI/2, 1*TILIB_PI/2, lw, color);
	curve1.Draw();
}

//##ModelId=48C4CCC700D2
 void TiLib_Char_P::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_P, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

//##ModelId=48C4CCC702A7
TiLib_Char_S::TiLib_Char_S()
{
}

//##ModelId=48DC54B501CF
void TiLib_Char_S::Draw_ByVector_OnlyLine(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Line line(x, y, x+w, y, lw, color);
	
	line.Draw();	//   ---
	line.x1 = x;
	line.y1 = y;
	line.x2 = x;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w;
	line.y2 = y+(h-lw)/2;
	line.Draw();	// ---
	line.x1 = x+w-lw;
	line.y1 = y+(h-lw)/2;
	line.x2 = x+w-lw;
	line.y2 = y+(h-lw);
	line.Draw();	// |
	line.x1 = x;
	line.y1 = y+h-lw;
	line.x2 = x+w;
	line.y2 = y+h-lw;
	line.Draw();	// ---
}

//##ModelId=48DC54B50356
void TiLib_Char_S::Draw_ByVector(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
	TiLib_Curve curve1(x+w/2, y+1*h/4, w/2, h/4, 0*TILIB_PI, 1*TILIB_PI, lw, color);
	curve1.Draw();
	TiLib_Line line1(x, y+1*h/4-1, x+w-lw, y+3*h/4, lw, color);
	line1.Draw();	// \  
	TiLib_Curve curve2(x+w/2, y+3*h/4, w/2, h/4, -1*TILIB_PI, 0*TILIB_PI, lw, color);
	curve2.Draw();
}

//##ModelId=48C4CCC702C6
 void TiLib_Char_S::Draw(U16 x, U16 y, U16 w, U16 h, U16 lw, U32 color)
{
//	TiLib_Char::Draw_Matrix_5_7(font_5_7_S, x, y, w, h, color);
	Draw_ByVector(x, y, w, h, lw, color);
}

#endif	// CONFIG_WITH_OSD
