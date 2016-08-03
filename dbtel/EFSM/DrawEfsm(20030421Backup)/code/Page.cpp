//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Page.h"  
#include "Main.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGPage *GPage;
//---------------------------------------------------------------------------
 
extern TMainForm *MainForm;
extern TMDIChild *currDoc;

GPoint Rotate(GPoint p, double radian) {
    GPoint p1;
    double r = sqrt((p.x)*(p.x) + (p.y)*(p.y));
    double a = p.Radian();
    p1.x = r*cos(a - radian);
    p1.y = r*sin(a - radian);
    return p1;
};

GPoint Shift(GPoint p, GPoint s) {
    GPoint p1;
    p1 = (p - s);
    return p1;
};

GPoint::GPoint() {
};

GPoint::GPoint(int xx, int yy) {
    x = xx;
    y = yy;
};
/*
GPoint GPoint::operator=(GPoint p) {
    x = p.x;
    y = p.y;
    return *this;
};*/

GPoint GPoint::operator-(GPoint p) {
    GPoint temp;
    temp.x = x - p.x;
    temp.y = y - p.y;
    return temp;
};

GPoint GPoint::operator+(GPoint p) {
    GPoint temp;
    temp.x = x + p.x;
    temp.y = y + p.y;
    return temp;
};

GPoint GPoint::operator/(int i) {
    GPoint temp;
    temp.x = x/2;
    temp.y = y/2;
    return temp;
};
         
bool GPoint::operator==(GPoint p) {
    return x == p.x && y == p.y;
};

double GPoint::Radian() {
    double a;
    if (x == 0) {
        if (y == 0) {
            a = 0;
        }
        else if (y > 0) {
            a = 1.5708;
        }
        else { // y < 0
            a = -1.5708;
        }
    }
    else if (y == 0) {
        if (x > 0) {
            a = 0;
        }
        else { // x < 0
            a = - 3.1416;
        }
    }
    else {
        a = atan(double(y)/double(x));
        if (x < 0) {
            if (a > 0) {
                a = a - 3.1416;
            }
            else {
                a = 3.1416 + a;
            }
        }
    }
    return a;
};
       
double GPoint::Distance(GPoint p) {
    GPoint q;
    q = p - *this;
    if (q == GPoint(0, 0)) {
        return 0;
    }
    else {
        return sqrt(double(q.x * q.x + q.y * q.y));
    }
};
     
GLine::GLine() {
};

GLine::GLine(GPoint aFrom, GPoint aTo) {
    from = aFrom;
    to = aTo;
};

void GLine::operator=(GLine aLine) {
    from = aLine.from;
    to = aLine.to;
};

double GLine::Radian() {
    GPoint p;
    p = from - to;
    return p.Radian();
};

double GLine::Distance(GPoint p) {
    GPoint vP, vFrom, vTo;
    double a = Radian();
    vP = Rotate(p, a);
    vFrom = Rotate(from, a);
    vTo = Rotate(to, a);
    if (vP.x > vFrom.x && vP.x < vTo.x || vP.x > vTo.x && vP.x < vFrom.x) {
        vP = Shift(vP, vFrom);
        if (vP.y < 0) {
            return -vP.y;
        }
        else {
            return vP.y;
        }
    }
    else {
        if (vP.Distance(vFrom) < vP.Distance(vTo)) {
            return vP.Distance(vFrom);
        }
        else {
            return vP.Distance(vTo);
        }
    }
};
             
GRect::GRect() {
};

GRect::GRect(int aLeft, int aTop, int aRight, int aBottom) {
    left = aLeft;
    top = aTop;
    right = aRight;
    bottom = aBottom;
};

GPoint GRect::GetFarthestPoint(GPoint p) {
    GPoint q(GPoint(left, top));
    double distance = p.Distance(q);

    double d = p.Distance(GPoint(right, top));
    if (d > distance) {
        q = GPoint(right, top);
        distance = d;
    }

    d = p.Distance(GPoint(right, bottom));
    if (d > distance) {
        q = GPoint(right, bottom);
        distance = d;
    }

    d = p.Distance(GPoint(left, bottom));
    if (d > distance) {
        q = GPoint(left, bottom);
        distance = d;
    }
    return q;
};

bool GRect::PointIn(GPoint p) {
    if (left <= p.x && p.x <= right && top <= p.y && p.y <= bottom) {
        return true;
    }
    else {
        return false;
    }
};

GPoint GRect::ConnectPoint(GPoint p) {
    GPoint m, q;
    m.x = (left + right) / 2;
    m.y = (top + bottom) / 2;
    GPoint leftTop = GPoint(left, top) - m;
    GPoint rightTop = GPoint(right, top) - m;
    GPoint rightBottom = GPoint(right, bottom) - m;
    GPoint leftBottom = GPoint(left, bottom) - m;
    double a = (p - m).Radian();
    double b;
    if (a >= rightTop.Radian() && a < rightBottom.Radian()) {
        b = double(p.y - m.y) / double(p.x - m.x);
        q.x = right;
        q.y = m.y + b * (right - m.x);
    }
    else if (rightBottom.Radian() <= a && a < leftBottom.Radian()) {
        b = double(p.x - m.x) / double(p.y - m.y);
        q.x = m.x + b * (bottom - m.y);
        q.y = bottom;
    }
    else if (a >= leftBottom.Radian() || a < leftTop.Radian()) {
        b = double(p.y - m.y) / double(p.x - m.x);
        q.x = left;
        q.y = m.y + b * (left - m.x);
    }
    else if (leftTop.Radian() <= a && a < rightTop.Radian()) {
        b = double(p.x - m.x) / double(p.y - m.y);
        q.x = m.x + b * (top - m.y);
        q.y = top;
    }

    return q;
};

double GRect::Distance(GPoint p) {
    double distance, d;
    GPoint q = ConnectPoint(p);
    return q.Distance(p);
};

State* Transition::FromState() {
    return _fromState;
};

State* Transition::ToState() {
    return _toState;
};

void Transition::SetFromState(State *from) {
    _fromState = from;
};

void Transition::SetToState(State *to) {
    _toState = to;
};

void GControl::ActiveMove(GPoint p) {
};
  
void GControl::Move(GPoint p) {
};

void GControl::ConfirmMove(GPoint p) {
};

int GControl::Type() {
    return _type;
};

bool GControl::IsSelected() {
    return _selected;
};

void GControl::Select() {
    _selected = true;
};

void GControl::Unselect() {
    _selected = false;
};

int GControl::Motion(GPoint pt, int mouseAction) {
	return NOTHING;
};

GTextBox::GTextBox(TGPage *parent) { 
	_parent = parent;
    _type = TGPage::GTEXTBOX;
};

GTextBox::GTextBox(TGPage *parent, GPoint pt, bool selected) {
    int i = currDoc->ActivePageIndex();
	_parent = parent;
    _selected = selected;  
//    _text = "NewStateName";
    _rect.left = pt.x;
    _rect.top = pt.y;
    _rect.right = pt.x + currDoc->page[i]->Canvas->TextWidth("NewState") + 6;
    _rect.bottom = pt.y + 50;
    _text = new TMemo(_parent);
    _text->Parent = _parent;
    _text->Top = _rect.top + 2;
    _text->Height = _rect.Bottom - _rect.top - 4;
    _text->Left = _rect.left + 2;
    _text->Width = _rect.Right - _rect.left - 4;
    _text->Lines->Add("NewState");
    _text->Alignment = taCenter;
    _text->BorderStyle = bsNone;
    _text->Enabled = false;
//    _text->Visible = true;
    _type = TGPage::GTEXTBOX;
};

void GTextBox::Initial(GRect *rect, bool selected) {
    _rect = *rect;
    _selected = selected;
    _type = TGPage::GTEXTBOX;
};
                  
GRect GTextBox::Rect() {
    return _rect;
};

void GTextBox::SetRect(GRect *rect) {
    _rect = *rect;
};

int GTextBox::Motion(GPoint pt, int mouseAction) {
    if (pt.x >= _rect.left - 3 && pt.x <= _rect.left + 3 && pt.y >= _rect.top - 3  && pt.y <= _rect.top + 3) {
        return ADJUST_SIZE;
    }
    else if (pt.x >= _rect.right - 3 && pt.x <= _rect.right + 3 && pt.y >= _rect.top - 3  && pt.y <= _rect.top + 3) {
        return ADJUST_SIZE;
    }
    else if (pt.x >= _rect.right - 3 && pt.x <= _rect.right + 3 && pt.y >= _rect.bottom - 3  && pt.y <= _rect.bottom + 3) {
        return ADJUST_SIZE;
    }
    else if (pt.x >= _rect.left - 3 && pt.x <= _rect.left + 3 && pt.y >= _rect.bottom - 3  && pt.y <= _rect.bottom + 3) {
        return ADJUST_SIZE;
    }
    else if (_rect.PointIn(pt)) {
        return MOVE;
    }
    else {
        return NOTHING;
    }
};

void GTextBox::ActiveChangeText() {
    _text->Enabled = true;
};

void GTextBox::UnactiveChangeText() {
    _text->Enabled = false;
};

void GTextBox::ActiveAdjustSize(GPoint pt) {
    int i = currDoc->ActivePageIndex();
	_pivot = _rect.GetFarthestPoint(pt);
    currDoc->page[i]->movePt = pt;
};

void GTextBox::AdjustSize(GPoint pt) {
    int i = currDoc->ppc->ActivePageIndex;
    if (pt.x > currDoc->page[i]->Width) {
        pt.x = currDoc->page[i]->Width;
    }
    if (pt.y > currDoc->page[i]->Height) {
        pt.y = currDoc->page[i]->Height;
    }
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    if (_selected) {
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psDashDot;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->Rectangle(_pivot.x, _pivot.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y); // clear the last draw
        currDoc->page[i]->movePt = pt;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->Rectangle(_pivot.x, _pivot.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y); // draw
    }
    else {
        if (pt.x > currDoc->page[i]->Width) {
            pt.x = currDoc->page[i]->Width;
        }
        if (pt.y > currDoc->page[i]->Height) {
            pt.y = currDoc->page[i]->Height;
        }
        int x3 = (_rect.right - _rect.left)/6;
        int y3 = (_rect.bottom - _rect.top)/6;
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psSolid;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->RoundRect(currDoc->page[i]->origin.x, currDoc->page[i]->origin.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y, x3, y3); // clear the last draw
        currDoc->page[i]->movePt = pt;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->RoundRect(currDoc->page[i]->origin.x, currDoc->page[i]->origin.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y, x3, y3); // draw
    }
};
         
void GTextBox::ChangeSize(GPoint pt) {
    int temp;
    if (pt.x < _pivot.x) {
        temp = _pivot.x;
        _pivot.x = pt.x;
        pt.x = temp;
    }
    if (pt.y < _pivot.y) {
        temp = _pivot.y;
        _pivot.y = pt.y;
        pt.y = temp;
    }
    SetRect(&GRect(_pivot.x, _pivot.y, pt.x, pt.y));    
    _text->Top = _rect.top + 2;
    _text->Height = _rect.Bottom - _rect.top - 4;
    _text->Left = _rect.left + 2;
    _text->Width = _rect.Right - _rect.left - 4;
};

void GTextBox::ActiveMove(GPoint pt) {
    int i = currDoc->ActivePageIndex();
    currDoc->page[i]->movePt = pt;
    _move = _rect;
};

void GTextBox::Move(GPoint pt) {
    int i = currDoc->ActivePageIndex();
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psDashDot;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Rectangle(_move.Left, _move.Top, _move.Right, _move.Bottom); // clear the last draw
    GPoint shift = pt - currDoc->page[i]->movePt;
    _move.Left += shift.x;
    _move.Right += shift.x;
    _move.Top += shift.y;
    _move.Bottom += shift.y;
    currDoc->page[i]->movePt = pt;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Rectangle(_move.Left, _move.Top, _move.Right, _move.Bottom);  // draw
};

void GTextBox::ConfirmMove(GPoint pt) {
    _rect = _move;    
    _text->Top = _rect.top + 2;
    _text->Left = _rect.left + 2;
};

void GTextBox::Draw() {
    long width;   
	long pos;

//    _text->Refresh();
    int idx = currDoc->ppc->ActivePageIndex;
    currDoc->page[idx]->Canvas->Brush->Color = clWhite;
    currDoc->page[idx]->Canvas->Pen->Color = clBlack;
    currDoc->page[idx]->Canvas->Pen->Style = psSolid;
    currDoc->page[idx]->Canvas->Pen->Mode = pmCopy;
    currDoc->page[idx]->Canvas->Rectangle(_rect.left, _rect.top, _rect.right, _rect.bottom);
/*    width = currDoc->page[idx]->Canvas->TextWidth(_text->Lines->Text) + 6;
    if (width <= _rect.right - _rect.left - 6) {
    	pos = _rect.left + (_rect.right - _rect.left - width)/2;
//	    currDoc->page[idx]->Canvas->TextOut(pos, _rect.top+3, _text->Lines->Text);
    }
    else {
    	_rect.right = _rect.left + width;
//	    currDoc->page[idx]->Canvas->TextOut(_rect.left+3, _rect.top+3, _text->Lines->Text);
    }*/
    if (_selected) {
	    currDoc->page[idx]->Canvas->Pen->Color = clRed;
    	currDoc->page[idx]->Canvas->Pen->Style = psDashDot;
	    currDoc->page[idx]->Canvas->Pen->Mode = pmNotXor;
    	currDoc->page[idx]->Canvas->Rectangle(_rect.left, _rect.top, _rect.right, _rect.bottom);
		currDoc->page[idx]->Canvas->Pen->Color = clBlack;
	    currDoc->page[idx]->Canvas->Pen->Style = psSolid;
    	currDoc->page[idx]->Canvas->Pen->Mode = pmCopy;
	    currDoc->page[idx]->Canvas->Rectangle(_rect.left-3, _rect.top-3, _rect.left+3, _rect.top+3);
    	currDoc->page[idx]->Canvas->Rectangle(_rect.right-3, _rect.top-3, _rect.right+3, _rect.top+3);
	    currDoc->page[idx]->Canvas->Rectangle(_rect.right-3, _rect.bottom-3, _rect.right+3, _rect.bottom+3);
    	currDoc->page[idx]->Canvas->Rectangle(_rect.left-3, _rect.bottom-3, _rect.left+3, _rect.bottom+3);
    }
};

int GArrow::GetShortestPoint(GPoint pt) {
    double distance = UNLIMIT;
    int select;
    for (int i = 0; i < _size; i++) {
        if (_points[i].Distance(pt) < distance) {
            distance = _points[i].Distance(pt);
            select = i;
        }
    }
    return select;
};

GArrow::GArrow(TGPage *parent) {
	_parent = parent;
    _selected = false;
    _type = TGPage::GARROW;
};

void GArrow::Create(GPoint pt) {
	_size = 2;
    _points[0] = pt;
  	_points[1] = pt;
};

int GArrow::Motion(GPoint pt, int mouseAction) {
	if (mouseAction == MOUSE_DOWN) {
	    for (int i = 0; i < _size; i++) {
    	    if (pt.x >= _points[i].x - 3 && pt.x <= _points[i].x + 3 && pt.y >= _points[i].y - 3  && pt.y <= _points[i].y + 3) {
            	return ADJUST_SIZE;
	        }
    	    else if (Distance(pt) < 2) {
        	    return ADJUST_SIZE;
	        }
    	}
    }
    return NOTHING;
};

void GArrow::ActiveAdjustSize(int select) {
    int idx = currDoc->ActivePageIndex();

	_select = select;
	currDoc->page[idx]->movePt = _points[_select];
};

void GArrow::ActiveAdjustSize(GPoint pt) {
    int idx = currDoc->ActivePageIndex();
    
	for (int i = 0; i < _size; i++) {
		if (pt.x >= _points[i].x - 3 && pt.x <= _points[i].x + 3 && pt.y >= _points[i].y - 3  && pt.y <= _points[i].y + 3) {
			// adjust _points[_select]
			_select = i;
			currDoc->page[idx]->movePt = _points[_select];
			return;
		}
	}
	if (_size+1 >= MAX_SIZE) {
		return;
	}
	for (int i = 0; i < _size - 1; i++) {
		if (GLine(_points[i], _points[i+1]).Distance(pt) < 2) {
			// add a point between _points[i] and _points[i+1], and adjust the point
			for (int j = _size-1; j > i; j--) {
				_points[j+1] = _points[j];
			}
			_size++;
			_select = i+1;
			_points[_select] = pt;
			currDoc->page[idx]->movePt = _points[_select];
			return;
		}
	}

	// this case suppose not to occurs but if it occurs (may be the computer error),
	// then adjust the last point in arrow
	_select = _size - 1;
	currDoc->page[idx]->movePt = _points[_select];
};

void GArrow::AdjustSize(GPoint pt) {
    int i = currDoc->ActivePageIndex();
    if (pt.x > currDoc->page[i]->Width || pt.y > currDoc->page[i]->Height) {
		// do nothing if out of canvas
        return;
    }
    GPoint tmp(_points[_select].x, _points[_select].y);
    if (_selected) {
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psDashDot;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
        currDoc->page[i]->movePt = pt;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
        _points[_select] = tmp;
    }
    else {
        currDoc->page[i]->Canvas->Brush->Color = clWhite;
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psSolid;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
        currDoc->page[i]->movePt = pt;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
    }
};

void GArrow::ChangeSize(GPoint pt) {
    GTextBox *tb;
    int i = currDoc->ActivePageIndex();
    if (_select == 0) {
        if (_from->Rect().PointIn(_points[0])) {
            tb = currDoc->page[i]->GetClosestInRangeTextBox(pt);
            if (tb != 0) {
                SetFrom(tb);
                ResetConnectPointFrom();
            }
        }
    }
    else if (_select == _size - 1) {
        if (_to->Rect().PointIn(_points[_size - 1])) {
            tb = currDoc->page[i]->GetClosestInRangeTextBox(pt);
            if (tb != 0) {
                SetTo(tb);
                ResetConnectPointTo();
            }
        }
    }
    else {
        GLine line(_points[_select-1], _points[_select+1]);
        if (line.Distance(pt) > 1) {
            _points[_select] = pt;
            if (_select == 1) {
                _points[0] = _from->Rect().ConnectPoint(_points[1]);
            }
            if (_select == _size - 2) {
                _points[_size - 1] = _to->Rect().ConnectPoint(_points[_size - 2]);
            }
        }
        else {
            for (int i = _select; i < _size - 1; i++) {
                _points[i] = _points[i+1];
            }
            _size--;
        }
    }
};

void GArrow::ActiveMove(GPoint p) {
    int i = currDoc->ActivePageIndex();
    currDoc->page[i]->movePt = p;
	_newPSize = _size;
	for (i = 0; i < _size; i++) {
	    _newP[i] = _points[i];
	}
};

void GArrow::Move(GPoint pt) {
    int idx = currDoc->ppc->ActivePageIndex;
    
    currDoc->page[idx]->Canvas->Brush->Color = clWhite;
    currDoc->page[idx]->Canvas->Pen->Color = clBlack;
    currDoc->page[idx]->Canvas->Pen->Style = psDashDot;
    currDoc->page[idx]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[idx]->Canvas->Polyline(_points, _size - 1); // clear the last draw

    GPoint shift = pt - currDoc->page[idx]->movePt;
    for (int i = 0; i < _newPSize; i++) {
        _newP[i] = _newP[i] + shift;
    }

    currDoc->page[idx]->movePt = pt;
    currDoc->page[idx]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[idx]->Canvas->Polyline(_points, _size - 1); // draw
};
        
void GArrow::ConfirmMove(GPoint pt) {
    for (int i = 0; i < _newPSize; i++) {
        _points[i] = _newP[i];
    }
};

void GArrow::Draw() {
    GPoint triangle[3];
    triangle[0] = _points[_size - 1];
    GPoint p;
    p = _points[_size - 1] - _points[_size - 2];
    double a = p.Radian();
    triangle[1].x = _points[_size - 1].x - 10 * cos(a + 0.4);
    triangle[1].y = _points[_size - 1].y - 10 * sin(a + 0.4);
    triangle[2].x = _points[_size - 1].x - 10 * cos(a - 0.4);
    triangle[2].y = _points[_size - 1].y - 10 * sin(a - 0.4);

    int i = currDoc->ppc->ActivePageIndex;
    currDoc->page[i]->Canvas->Brush->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psSolid;
    currDoc->page[i]->Canvas->Pen->Mode = pmCopy;
    currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
    currDoc->page[i]->Canvas->Polygon(triangle, 2);

    if (_selected) {
    	currDoc->page[i]->Canvas->Brush->Color = clWhite;
	    for (int j = 0; j < _size; j++) {
    	    currDoc->page[i]->Canvas->Rectangle(_points[j].x-3, _points[j].y-3, _points[j].x+3, _points[j].y+3);
	    }
    	currDoc->page[i]->Canvas->Pen->Color = clRed;
	    currDoc->page[i]->Canvas->Pen->Style = psDashDot;
    	currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
	    currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
    }
};

double GArrow::Distance(GPoint pt) {
    double distance = UNLIMIT, d;
    for (int i = 0; i < _size - 1; i++) {
        d = GLine(_points[i], _points[i+1]).Distance(pt);
        if (d < distance) {
            distance = d;
        }
    }
    return distance;
};

GTextBox *GArrow::From() {
    return _from;
};
 
GTextBox *GArrow::To() {
    return _to;
};

void GArrow::SetFrom(GTextBox *from) {
	_from = from;
};
           
void GArrow::SetTo(GTextBox *to) {
	_to = to;
};

void GArrow::ResetConnectPointFrom() {
    _points[0] = _from->Rect().ConnectPoint(_points[1]);
};
 
void GArrow::ResetConnectPointTo() {
    _points[_size - 1] = _to->Rect().ConnectPoint(_points[_size - 2]);
};

void TGPage::ResetArrowsConnectPoint(GTextBox *tb) {
    for (unsigned int i = 0; i < _arrow.size(); i++) {
        if (_arrow[i].From() == tb) {
            _arrow[i].ResetConnectPointFrom();
        }
		if (_arrow[i].To() == tb) {
            _arrow[i].ResetConnectPointTo();
        }
    }
};

void TGPage::AdjustSelectedControlSize(GPoint pt) {
	unsigned int i;
	for (i = 0; i < _selected.size(); i++) {
	    _selected[i]->ChangeSize(pt);
	}
	for (i = 0; i < _selected.size(); i++) {
    	if (_selected[i]->Type() == GTEXTBOX) {
    		ResetArrowsConnectPoint((GTextBox*)_selected[i]);
	    }
	}
};

void TGPage::MoveSelectedControl(GPoint pt) {
	unsigned int i;

	for (i = 0; i < _selected.size(); i++) {
		_selected[i]->ConfirmMove(pt);
	}
	for (i = 0; i < _selected.size(); i++) {
		if (_selected[i]->Type() == GTEXTBOX) {
			ResetArrowsConnectPoint((GTextBox*)_selected[i]);
		}
	}
};

GTextBox* TGPage::GetShortestTextBox(GPoint pt) {
    GTextBox *shortest = 0;
    double distance = UNLIMIT, d;
    for (unsigned int i = 0; i < _tb.size(); i++) {
        d = _tb[i].Rect().Distance(pt);
        if (d < distance) {
            distance = d;
            shortest = &_tb[i];
        }
    }
    return shortest;
};

GTextBox* TGPage::GetClosestInRangeTextBox(GPoint pt) {
    GTextBox *shortest = 0;
    double distance = UNLIMIT, d;
    for (unsigned int i = 0; i < _tb.size(); i++) {
        if (_tb[i].Rect().PointIn(pt)) {
            d = _tb[i].Rect().Distance(pt);
            if (d < distance) {
                distance = d;
                shortest = &_tb[i];
            }
        }
    }
    return shortest;
};

GArrow* TGPage::GetShortestArrow(GPoint pt) {
    GArrow *shortest = 0;
    int distance = UNLIMIT, d;
    for (unsigned int i = 0; i < _arrow.size(); i++) {
        d = _arrow[i].Distance(pt);
        if (d < distance) {
            distance = d;
            shortest = &_arrow[i];
        }
    }
    return shortest;
};
                      
bool TGPage::PointInSelectedArea(GPoint pt) {
// dummy code for test
	return false;
};

void TGPage::UnselectAll() {
	vector<GControl*>::iterator p;
	for (p = _selected.begin(); p != _selected.end(); p++) {
    	(*p)->Unselect();
    }
    _selected.clear();
};

__fastcall TGPage::TGPage(TComponent *Owner)
 : TForm(Owner)
{
	tmpArrow = new GArrow(this);
    Visible = true;
    Color = clWhite;
    BorderStyle = bsNone;
    Left = 200;
    Top = 0;
    Width = 200;
    Height = 700;
    Canvas->Pen->Mode = pmNotXor;
//  Canvas->CopyMode = cmMergePaint;

    PopupMenu = new TPopupMenu(this);
    TMenuItem *NewItem = new TMenuItem(PopupMenu);
    NewItem->Caption = "Format";
    PopupMenu->Items->Add(NewItem);
    NewItem = new TMenuItem(PopupMenu);
    NewItem->Caption = "Add Point";
    PopupMenu->Items->Add(NewItem);
};

void TGPage::AddNewTextBox(GPoint pt) {
	UnselectAll();
    GTextBox tb(this, pt, true);
    _tb.push_back(tb); // add a new State to vector
    _selected.push_back(&(*_tb.rbegin()));
};

void TGPage::AddTransition(GPoint from, GPoint to) {
    double d;
    unsigned int i;

    GTextBox *fromTB = GetClosestInRangeTextBox(from);
    GTextBox *toTB = GetClosestInRangeTextBox(to);
    if (from.Distance(to) > 10 && fromTB && toTB) {
        tmpArrow->SetFrom(fromTB);
        tmpArrow->SetTo(toTB);
        tmpArrow->ResetConnectPointFrom();
        tmpArrow->ResetConnectPointTo();
        tmpArrow->Select();
        _arrow.push_back(*tmpArrow);
        for (i = 0; i < _selected.size(); i++) {
            _selected[i]->Unselect();
        }
        _selected.clear();
        _selected.push_back(&(*_arrow.rbegin()));
    }
};

// Decide which control to be select by (x, y) which mouse down, and then put control pointer to _selected.
void TGPage::SelectControl(GPoint pt) {
    GTextBox *tb;
    GArrow *arrow;
    int d1, d2;
    unsigned int i;

    tb = GetShortestTextBox(pt);
    if (tb != 0) {
        d1 = tb->Rect().Distance(pt);
    }
    else {
        d1 = UNLIMIT;
    }
    arrow = GetShortestArrow(pt);
    if (arrow != 0) {
        d2 = arrow->Distance(pt);
    }
    else {
        d2 = UNLIMIT;
    }
    if (tb != 0 || arrow != 0) {
        if (_selected.size() > 0) {
            for (i = 0; i < _selected.size(); i++) {
                _selected[i]->Unselect();
            }
            _selected.clear();
        }
        if (d1 < d2) {
            _selected.push_back(tb);
        }
        else {
            _selected.push_back(arrow);
        }
        _selected[0]->Select();
    }
};

void TGPage::Draw() {
    unsigned int i;
    for (i = 0; i < _tb.size(); i++) {
        _tb[i].Draw();
    }
    for (i = 0; i < _arrow.size(); i++) {
        _arrow[i].Draw();
    }
};

//---------------------------------------------------------------------
void __fastcall TGPage::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    unsigned int i;
    GPoint pt(X, Y);

    if (Button == mbLeft) {
    	if (_selected.size() == 1 && _selected[0]->Type() == TGPage::GTEXTBOX) {
        	GTextBox *selected = (GTextBox*)_selected[0];
    		if (!selected->Rect().PointIn(pt)) {
		    	selected->UnactiveChangeText();
            }
        }
	    switch (MainForm->drawButton) {
    	case dtSelect:
        	if (_selected.size() == 1) {
            	motion = _selected[0]->Motion(pt, MOUSE_DOWN);
	            if (motion == ADJUST_SIZE) {
    	            _selected[0]->ActiveAdjustSize(pt);
        	    }
            	else if (motion == MOVE) {
                	_selected[0]->ActiveMove(pt);
	            }
    	        else if (motion == NOTHING) {
        			SelectControl(pt);
            	    Refresh();
	            }
    	    }
        	else if (PointInSelectedArea(pt)) {
            	for (i = 0; i < _selected.size(); i++) {
                	_selected[i]->ActiveMove(pt);
	            }
    	        motion = MOVE;
        	}
	        else {
    	    	SelectControl(pt);
        	}
	        break;
    	case dtState:
    		AddNewTextBox(pt);
	        Refresh();
    	    break;
	    case dtTransition:
    	    Canvas->MoveTo(X, Y);
        	origin = pt;
	        movePt = origin;
    	    tmpArrow->Create(pt);
        	tmpArrow->ActiveAdjustSize(1);  // _select = 1, _points[1]
	        break;
    	}
	    leftMouseDown = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TGPage::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    GPoint pt(X, Y);

    if (leftMouseDown) {
        switch (MainForm->drawButton) {
            case dtSelect:
                if (motion == ADJUST_SIZE && _selected.size() == 1) {
                    _selected[0]->AdjustSize(pt);
                }
                else if (motion == MOVE) {
                    for (unsigned int i = 0; i < _selected.size(); i++) {
                        _selected[i]->Move(pt);
                    }
                }
                break;
            case dtTransition:
                tmpArrow->AdjustSize(pt);
                break;
        }
    }

    TVarRec tempvar[2] = {X, Y};
    currDoc->Memo1->Text = Format("Current: (%d, %d)", tempvar, 2);
}
//---------------------------------------------------------------------------

void __fastcall TGPage::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    GPoint pt(X, Y);

    if (Button == mbLeft) {
	    if (leftMouseDown && X <= Width && Y <= Height) {
    	    switch (MainForm->drawButton) {
        	case dtSelect:
            	if (_selected.size() == 1 && motion == ADJUST_SIZE) {
					AdjustSelectedControlSize(pt);
	    			Refresh(); // clear canvas and trigger TGPage::Draw()
    	        }
        	    else if (_selected.size() > 0 && motion == MOVE) {
					MoveSelectedControl(pt);
    				Refresh(); // clear canvas and trigger TGPage::Draw()
	            }
    	        break;
        	case dtTransition:
            	AddTransition(origin, pt);
	            Refresh();
    	        break;
        	}
	    }
    	leftMouseDown = false;
    }
    else if (Button == mbRight) {
/*        TPoint tp;
        tp = Mouse->CursorPos;
        PopupMenu->Popup(tp.x, tp.y);*/
		if (_selected.size() == 1) {
        	if (_selected[0]->Type() == TGPage::GTEXTBOX) {
        		GTextBox *selected = (GTextBox*)_selected[0];
	    		if (selected->Rect().PointIn(pt)) {
    	           	selected->ActiveChangeText();
    		        motion = CHANGE_TEXT;
                }
            }
            else if (_selected[0]->Type() == TGPage::GARROW) {
        		GArrow *selected = (GTextBox*)_selected[0];
	    		if (selected->Rect().PointIn(pt)) {
    		        motion = CHANGE_TEXT;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TGPage::FormPaint(TObject *Sender)
{
static int k;
k++;
	    MainForm->StatusBar->SimpleText = IntToStr(_tb.size());
	if (_tb.size() > 2 && _tb[2].IsSelected()) {
	    MainForm->StatusBar->SimpleText = "Selected == true" + IntToStr(_tb.size());
    }
    Draw();
}
//---------------------------------------------------------------------------


