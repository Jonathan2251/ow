// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "OSD/TiLib_Text.h"

#include "TiLib_Char.h"

//##ModelId=48C4C1150398
TiLib_Text::TiLib_Text()
{
}

//##ModelId=48C4C11503C7
TiLib_Text::TiLib_Text(U16 X, U16 Y, U16 W, U16 H, U16 LW, U16 FONT_INTERVAL, U32 COLOR, S8* STR)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	lw = LW;
	gap = FONT_INTERVAL;
	color = COLOR;
	if (strlen(STR) < 100) {
		strcpy(str, STR);
	}
	else {
		strncpy(str, STR, 99);
		str[99] = '\0';
	}
}

//##ModelId=48C4C1160359
void TiLib_Text::ChangeStr(S8* STR)
{
	if (strlen(STR) < 100) {
		strcpy(str, STR);
	}
	else {
		strncpy(str, STR, 99);
		str[99] = '\0';
	}
}

//##ModelId=48C4C11603E6
U16 TiLib_Text::Width()
{
	return (w+gap)*strlen(str)-gap;
}

//##ModelId=48C4C117003D
U16 TiLib_Text::Height()
{
	return h;
}

//##ModelId=48C4C117007B
void TiLib_Text::Draw()
{
	int i = 0;
	
	for (S8* p = str, i = 0; *p != '\0'; p++, i++) {
		switch (*p) {
		case '0':
			font._0.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '1':
			font._1.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '2':
			font._2.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '3':
			font._3.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '4':
			font._4.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '5':
			font._5.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '6':
			font._6.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '7':
			font._7.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '8':
			font._8.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '9':
			font._9.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'A':
			font.A.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'B':
			font.B.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'C':
			font.C.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'D':
			font.D.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'E':
			font.E.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'F':
			font.F.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'I':
			font.I.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'L':
			font.L.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'N':
			font.N.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'O':
			font.O.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'P':
			font.P.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case 'S':
			font.S.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '-':
			font.dash.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		case '.':
			font.dot.Draw(x + i*(w+gap), y, w, h, lw, color);
			break;
		}
	}
}

#endif	// CONFIG_WITH_OSD
