// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "OSD/TiLib_Glyph.h"
#include "TiLib.h"
#include <math.h>

//##ModelId=48C4BEF3019D
TiLib_Line::TiLib_Line()
{
}

//##ModelId=48C4BEF301CC
TiLib_Line::TiLib_Line(U16 X1, U16 Y1, U16 X2, U16 Y2, U16 LW, U32 COLOR)
{
	x1 = X1;
	y1 = Y1;
	x2 = X2;
	y2 = Y2;
	lw = LW;
	color = COLOR;
}

//##ModelId=48C4BEF40084
void TiLib_Line::DrawSlopeLine_DDA_Algorithm()
{
	int dx, dy, steps, k, j;
	double x_increment, y_increment, x, y, radian;
	U16 xlw;
	
	dx = x2 - x1;
	dy = y2 - y1;
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	}
	else {
		steps = abs(dy);
	}
	if (x1 < x2) {
	    radian = atan(double(y2-y1)/double(x2-x1));
	}
	else {
	    radian = atan(double(y1-y2)/double(x1-x2));
	}

    xlw = lw;
    
	x_increment = double(dx) / double(steps);
	y_increment = double(dy) / double(steps);
	x = x1;
	y = y1;
	lib.osd.SetPoint(x, y, color);
	for (k = 0; k < steps; k++) {
		x += x_increment;
		y += y_increment;
		for (j = 0; j < xlw; j++) {
			lib.osd.SetPoint((U16)x, (U16)y, color);
			x++;
		}
		x -= xlw;
	}
}

//##ModelId=48C4BEF40055
void TiLib_Line::Draw()
{
	int i = 0, j = 0;
	U16 min, max;
	U16 x, y;
	
	if (x1 == x2) { // vertical line
		if (y1 <= y2) {
			min = y1;
			max = y2;
		}
		else {
			min = y2;
			max = y1;
		}
		for (i = min; i <= max; i++) {
			x = x1;
			y = i;
			for (j = 0; j < lw; j++) {
				lib.osd.SetPoint(x, y, color);
				x++;
			}
		}
	}
	else if (y1 == y2) { // horizontal line
		if (x1 <= x2) {
			min = x1;
			max = x2;
		}
		else {
			min = x2;
			max = x1;
		}
		for (i = min; i <= max; i++) {
			x = i;
			y = y1;
			for (j = 0; j < lw; j++) {
				lib.osd.SetPoint(x, y, color);
				y++;
			}
		}
	}
	else {
		DrawSlopeLine_DDA_Algorithm();
	}
}

//##ModelId=48C4BEF6014F
TiLib_Rect::TiLib_Rect()
{
}

//##ModelId=48C4BEF6017E
TiLib_Rect::TiLib_Rect(U16 X, U16 Y, U16 W, U16 H, U32 COLOR)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	color = COLOR;
}

//##ModelId=48C4BEF60362
void TiLib_Rect::Draw()
{
#if 1
// The GFX_RECT_FILL() will conflict with Timer Interrupt Mechanism, TI003.
// Slower than call GFX_RECT_FILL() directly, but no conflict.
	int i = 0;
	TiLib_Line line(this->x, this->y, this->x+this->w, this->y, this->h, this->color);
	
	line.Draw();
#else
	lib.osd.DrawRect(this);
#endif
}

//##ModelId=48C4BEF401EB
TiLib_Point::TiLib_Point()
{
}

//##ModelId=48C4BEF4020A
TiLib_Point::TiLib_Point(U16 X, U16 Y, U32 COLOR)
{
}

//##ModelId=48C4BEF40304
void TiLib_Point::Draw()
{
}

//##ModelId=48C4BEF500D2
TiLib_PolyLine::TiLib_PolyLine()
{
}

//##ModelId=48C4BEF500F1
TiLib_PolyLine::TiLib_PolyLine(U16 POINT_NUM, U16 *X, U16* Y, U16 LW, U32 COLOR)
{
	pointNum = POINT_NUM;
	for (int i = 0; i < pointNum; i++) {
		x[i] = X[i];
		y[i] = Y[i];
	}
	lw = LW;
	color = COLOR;
}

//##ModelId=48C4BEF502F5
void TiLib_PolyLine::Draw()
{
	TiLib_Line line;
	
	line.lw = lw;
	line.color = color;
	for (U16 i = 0; i < pointNum - 1; i++) {
		line.x1 = x[i];
		line.y1 = y[i];
		line.x2 = x[i+1];
		line.y2 = y[i+1];
		line.Draw();
	}
}

//##ModelId=48C4BEF10007
TiLib_Circle::TiLib_Circle()
{
}

//##ModelId=48C4BEF10036
TiLib_Circle::TiLib_Circle(U16 CX, U16 CY, U16 R, double FROM, double TO, U16 LW, U32 COLOR)
{
	cx = CX;
	cy = CY;
	r = R;
	lw = LW;
	color = COLOR;
	from = FROM;
	to = TO;
	SetAngle(FROM, TO);
}

//##ModelId=48C4BEF10381
void TiLib_Circle::SetAngle(double from, double to)
{
#if 1
	if (from >= 0 && from <= TILIB_PI/2) {
		bx = cx+cos(from)*r;
		by = cy-1*sin(from)*r;
	}
	else if (from > TILIB_PI/2 && from <= TILIB_PI) {
		bx = cx-cos(from-TILIB_PI/2)*r;
		by = cy-sin(from-TILIB_PI/2)*r;
	}
	else if (from > TILIB_PI && from <= 3*TILIB_PI/2) {
		bx = cx-1*cos(from-TILIB_PI)*r;
		by = cy+sin(from-TILIB_PI)*r;
	}
	else if (from > 3*TILIB_PI/2 && from <= 2*TILIB_PI) {
		bx = cx+1*cos(from-3*TILIB_PI/2)*r;
		by = cy+1*sin(from-3*TILIB_PI/2)*r;
	}
	
	if (to >= 0 && to <= TILIB_PI/2) {
		ex = cx+cos(to);
		ey = cy-1*sin(to);
	}
	else if (to > TILIB_PI/2 && to <= TILIB_PI) {
		ex = cx-cos(to-TILIB_PI/2)*r;
		ey = cy-sin(to-TILIB_PI/2)*r;
	}
	else if (to > TILIB_PI && to <= 3*TILIB_PI/2) {
		ex = cx-1*cos(to-TILIB_PI)*r;
		ey = cy+sin(to-TILIB_PI)*r;
	}
	else if (to > 3*TILIB_PI/2 && to <= 2*TILIB_PI) {
		ex = cx+1*cos(to-3*TILIB_PI/2)*r;
		ey = cy+1*sin(to-3*TILIB_PI/2)*r;
	}
#endif	
}

//##ModelId=48C4BEF200F1
void TiLib_Circle::SetPoint_IfInRange(int x, int y)
{
	lib.osd.SetPoint(x, y, color);
}

//    * (-x,+y) * (+x,+y) *
//      *       *       *
//        *     *     *
// (-y,+x)  *   *   *  (+y,+x)
//            * * *
//  ******************************
//            * * *
// (-y,-x)  *   *   *  (+y,-x)
//        *     *     *
//      *       *       *
//    * (-x,-y) * (+x,-y) *
//
//##ModelId=48C4BEF2022A
void TiLib_Circle::PlotCirclePoints(int x, int y)
{
	if (x < _xBound[1]) {	// Degree 45..90
		lib.osd.SetPoint(U16(cx+x), U16(cy+y), color);
	}
	if (x < _xBound[2]) {	// Degree 90..135
		lib.osd.SetPoint(U16(cx-x), U16(cy+y), color);
	}
	if (x < _xBound[6]) {	// Degree 270..315
		lib.osd.SetPoint(U16(cx+x), U16(cy-y), color);
	}
	if (x < _xBound[5]) {	// Degree 225..270
		lib.osd.SetPoint(U16(cx-x), U16(cy-y), color);
	}
	if (x < _xBound[0]) {	// Degree 0..45
		lib.osd.SetPoint(U16(cx+y), U16(cy+x), color);
	}
	if (x < _xBound[3]) {	// Degree 135..180
		lib.osd.SetPoint(U16(cx-y), U16(cy+x), color);
	}
	if (x < _xBound[7]) {	// Degree 315..360
		lib.osd.SetPoint(U16(cx+y), U16(cy-x), color);
	}
	if (x < _xBound[4]) {	// Degree 180..225
		lib.osd.SetPoint(U16(cx-y), U16(cy-x), color);
	}
}

//##ModelId=48C4BEF20381
void TiLib_Circle::BressCircle()
{
	int	p, x, y;
	
	x = 0;
	y = r;
	p = 3 - 2 * r;
	while (x < y) {
		PlotCirclePoints(x,y);
		if (p < 0) {
			p = p+4*x+6;
		}
		else {
			p = p+4*(x-y)+10;
			y--;
		}
		x++;
	}
	if (x = y) {
		PlotCirclePoints(x,y);
	}
}

//##ModelId=48C4BEF200C2
void TiLib_Circle::Draw()
{
	BressCircle();
#if 0
	int	end, p, x, y;
//	double s = from % (TILIB_PI/2);
	double s = TILIB_PI/4;
	
	end = r/2;
	x = 0;
	y = r;
	p = 3 - 2 * r;
	while (x < end) {
		lib.osd.SetPoint(cx+x, cy-y, color);
		if (p < 0) {
			p = p+4*x+6;
		}
		else {
			p = p+4*(x-y)+10;
			y--;
		}
		x++;
	}
	
/*	s = TILIB_PI/4;
	
	x = r*cos(s);
	y = r*sin(s);
	p = 2*(x-1)*(x-1)+y*y+(y-1)*(y-1)-2*r*r;
	while (x > 0) {
		lib.osd.SetPoint(cx+x, cy-y, color);
		if (p < 0) {
			p = p+4*x+6;
		}
		else {
			p = p+4*(x-y)+10;
			y++;
		}
		x--;
	}
	if (x = y) {
		lib.osd.SetPoint(cx+x, cy-y, color);
	}
*/	
	x = r*cos(TILIB_PI/4);
	y = r*sin(TILIB_PI/4);
	p = 2*(y+1)*(y+1)+x*x+(x-1)*(x-1)-2*r*r;
	while (y > 0) {
		lib.osd.SetPoint(cx+x, cy-y, color);
		if (p < 0) {
			p = p+4*y+6;
		}
		else {
			p = p+4*(y-x)+10;
			x--;
		}
		y--;
	}
	if (x = y) {
		lib.osd.SetPoint(cx+x, cy-y, color);
	}
#endif
}

//##ModelId=48C4BEF30016
void TiLib_Glyph::Draw()
{
}

//##ModelId=48DC5AE30185
TiLib_Curve::TiLib_Curve()
{
}

//##ModelId=48DC5AE301A5
TiLib_Curve::TiLib_Curve(U16 CX, U16 CY, U16 R1, U16 R2, double FROM, double TO, U16 LW, U32 COLOR)
{
	cx = CX;
	cy = CY;
	r1 = R1;
	r2 = R2;
	from = FROM;
	to = TO;
	lw = LW;
	color = COLOR;
}

//##ModelId=48DC5AE400E9
void TiLib_Curve::Draw()
{
	int i;
	int x, y;
	double angle;
	int bigger;
	double delta;
	
	bigger = (r1 > r2) ? r1 : r2;
	delta = 1.0 / (bigger*4.0);
	for (i = 0; i < lw; i++) {
		for (angle = from; angle <= to; angle += delta) {
			x = cx + (r1-i)*cos(angle);
			y = cy - (r2-i)*sin(angle);
			lib.osd.SetPoint(x, y, color);
		}
	}
}

#endif	// CONFIG_WITH_OSD

