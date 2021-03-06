//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GPage.h" 
#include "Main.h" 
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGPageForm *GPageForm;  
extern TMainForm *MainForm;
extern TMDIChild *currDoc;
/*
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

GPoint GPoint::operator=(GPoint p) {
    x = p.x;
    y = p.y;
    return *this;
};

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

void GControl::ActiveMove(GPoint p) {
};
  
void GControl::Move(GPoint p) {
};

void GControl::ConfirmMove(GPoint p) {
};

int GControl::Type() {
    return _type;
};

bool GControl::IsActive() {
    return _active;
};

void GControl::Active() {
    _active = true;
};

void GControl::Inactive() {
    _active = false;
};
        
bool State::PointInRect(GPoint p) {
    if (_rect.Left <= p.x && p.x <= _rect.Right && _rect.Top <= p.y && p.y <= _rect.Bottom) {
        return true;
    }
    else {
        return false;
    }
};

GPoint State::GetFarthestPoint(GPoint p) {
    GPoint q(GPoint(_rect.Left, _rect.Top));
    double distance = p.Distance(q);

    double d = p.Distance(GPoint(_rect.Right, _rect.Top));
    if (d > distance) {
        q = GPoint(_rect.Right, _rect.Top);
        distance = d;
    }

    d = p.Distance(GPoint(_rect.Right, _rect.Bottom));
    if (d > distance) {
        q = GPoint(_rect.Right, _rect.Bottom);
        distance = d;
    }

    d = p.Distance(GPoint(_rect.Left, _rect.Bottom));
    if (d > distance) {
        q = GPoint(_rect.Left, _rect.Bottom);
        distance = d;
    }
    return q;
};

GPoint State::GetShortestPoint(GPoint p) {
    GPoint q(_rect.Right, _rect.Top);
    double distance = p.Distance(q);

    double d = p.Distance(q);
    if (d < distance) {
        q = GPoint(_rect.Right, _rect.Top);
        distance = d;
    }

    d = p.Distance(GPoint(_rect.Right, _rect.Bottom));
    if (d < distance) {
        q = GPoint(_rect.Right, _rect.Bottom);
        distance = d;
    }

    d = p.Distance(GPoint(_rect.Left, _rect.Bottom));
    if (d < distance) {
        q = GPoint(_rect.Left, _rect.Bottom);
        distance = d;
    }
    return q;
};

State::State() {
    _type = TGPageForm::STATE;
};

State::State(int left, int top, int right, int bottom) {
    _rect.left = left;
    _rect.top = top;
    _rect.right = right;
    _rect.bottom = bottom;
    _type = TGPageForm::STATE;
};

void State::Initial(int left, int top, int right, int bottom, bool active) {
    _rect.left = left;
    _rect.top = top;
    _rect.right = right;
    _rect.bottom = bottom;
    _active = active;
    _type = TGPageForm::STATE;
};

void State::AdjustCursorIf(int x, int y) {
    int i = currDoc->ppc->ActivePageIndex;

    if (x >= _rect.left - 3 && x <= _rect.left + 3 && y >= _rect.top - 3  && y <= _rect.top + 3) {
        currDoc->page[i]->Cursor = TCursor(crCross);
    }
    else if (x >= _rect.right - 3 && x <= _rect.right + 3 && y >= _rect.top - 3  && y <= _rect.top + 3) {
        currDoc->page[i]->Cursor = TCursor(crCross);
    }
    else if (x >= _rect.right - 3 && x <= _rect.right + 3 && y >= _rect.bottom - 3  && y <= _rect.bottom + 3) {
        currDoc->page[i]->Cursor = TCursor(crCross);
    }
    else if (x >= _rect.left - 3 && x <= _rect.left + 3 && y >= _rect.bottom - 3  && y <= _rect.bottom + 3) {
        currDoc->page[i]->Cursor = TCursor(crCross);
    }
    else if (PointInRect(GPoint(x, y))) {  
        currDoc->page[i]->Cursor = TCursor(crSizeAll);
    }
    else {
        currDoc->page[i]->Cursor = TCursor(crDefault);
    }
};

void State::ActiveAdjustSize(GPoint p) {
    int i = currDoc->ActivePageIndex();
    switch (currDoc->page[i]->Cursor) {
        case TCursor(crCross):
            _pivot = GetFarthestPoint(p);
            currDoc->page[i]->movePt = p;
            break;
        case TCursor(crSizeAll):
            currDoc->page[i]->movePt = p;
            break;
        case TCursor(crDefault):
            _pivot = currDoc->page[i]->origin;
            break;
    }
    currDoc->page[i]->label->Caption = "ActiveAdjustSize";
};

void State::ActiveMove(GPoint p) {
    int i = currDoc->ActivePageIndex();
    currDoc->page[i]->movePt = p;
    _move = _rect;
};

void State::Move(GPoint p) {
    int i = currDoc->ActivePageIndex();
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psDashDot;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Rectangle(_move.Left, _move.Top, _move.Right, _move.Bottom); // clear the last draw
    GPoint shift = p - currDoc->page[i]->movePt;
    _move.Left += shift.x;
    _move.Right += shift.x;
    _move.Top += shift.y;
    _move.Bottom += shift.y;
    currDoc->page[i]->movePt = p;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Rectangle(_move.Left, _move.Top, _move.Right, _move.Bottom);  // draw
};
        
void State::ConfirmMove(GPoint p) {
    _rect = _move;
};

void State::AdjustSize(int x, int y) {
    int i = currDoc->ppc->ActivePageIndex;
    if (x > currDoc->page[i]->Width) {
        x = currDoc->page[i]->Width;
    }
    if (y > currDoc->page[i]->Height) {
        y = currDoc->page[i]->Height;
    }
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    if (_active) {
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psDashDot;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->Rectangle(_pivot.x, _pivot.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y); // clear the last draw
        currDoc->page[i]->movePt = GPoint(x, y);
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->Rectangle(_pivot.x, _pivot.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y); // draw
    }
    else {
        if (x > currDoc->page[i]->Width) {
            x = currDoc->page[i]->Width;
        }
        if (y > currDoc->page[i]->Height) {
            y = currDoc->page[i]->Height;
        }
        int x3 = (_rect.right - _rect.left)/6;
        int y3 = (_rect.bottom - _rect.top)/6;
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psSolid;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->RoundRect(currDoc->page[i]->origin.x, currDoc->page[i]->origin.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y, x3, y3); // clear the last draw
        currDoc->page[i]->movePt = GPoint(x, y);
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        currDoc->page[i]->Canvas->RoundRect(currDoc->page[i]->origin.x, currDoc->page[i]->origin.y, currDoc->page[i]->movePt.x, currDoc->page[i]->movePt.y, x3, y3); // draw
    }
};
         
void State::ChangeSize(int x, int y) {
    int temp;
    if (x < _pivot.x) {
        temp = _pivot.x;
        _pivot.x = x;
        x = temp;
    }
    if (y < _pivot.y) {
        temp = _pivot.y;
        _pivot.y = y;
        y = temp;
    }
    SetRect(_pivot.x, _pivot.y, x, y);
};

void State::Draw() {
    int i = currDoc->ppc->ActivePageIndex;  
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psSolid;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    int x3 = (_rect.right - _rect.left)/6;
    int y3 = (_rect.bottom - _rect.top)/6;
    currDoc->page[i]->Canvas->RoundRect(_rect.left, _rect.top, _rect.right, _rect.bottom, x3, y3);
    if (_active) {
        Active();
    }
};

void State::Active() {
//    GControl::Active();
    _active = true;
    int i = currDoc->ppc->ActivePageIndex;
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    currDoc->page[i]->Canvas->Pen->Color = clRed;
    currDoc->page[i]->Canvas->Pen->Style = psDashDot;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Rectangle(_rect.left, _rect.top, _rect.right, _rect.bottom);
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psSolid;
    currDoc->page[i]->Canvas->Pen->Mode = pmCopy;
    currDoc->page[i]->Canvas->Rectangle(_rect.left-3, _rect.top-3, _rect.left+3, _rect.top+3);
    currDoc->page[i]->Canvas->Rectangle(_rect.right-3, _rect.top-3, _rect.right+3, _rect.top+3);
    currDoc->page[i]->Canvas->Rectangle(_rect.right-3, _rect.bottom-3, _rect.right+3, _rect.bottom+3);
    currDoc->page[i]->Canvas->Rectangle(_rect.left-3, _rect.bottom-3, _rect.left+3, _rect.bottom+3);
};

void State::Inactive() {
//    GControl::Unactive();
    _active = false;
    int i = currDoc->ppc->ActivePageIndex;
    currDoc->page[i]->Refresh();
    currDoc->page[i]->Draw();
};

void State::operator=(State &state) {
    _rect.left = state.Left();
    _rect.top = state.Top();
    _rect.right = state.Right();
    _rect.bottom = state.Bottom();
};
 
int State::Left() {
    return _rect.left;
};

int State::Top() {
    return _rect.top;
};

int State::Right() {
    return _rect.right;
};

int State::Bottom() {
    return _rect.bottom;
};

void State::SetRect(int left, int top, int right, int bottom) {
    _rect.left = left;
    _rect.top = top;
    _rect.right = right;
    _rect.bottom = bottom;
};

bool State::InRange(int x, int y) {
    if (x >= _rect.left && x <= _rect.right && y >= _rect.top && y <= _rect.bottom) {
        return true;
    }
    else {
        return false;
    }
};

double State::Distance(int x, int y) {
    double distance, d;
    GPoint p(x, y), q;
    q = ConnectPoint(p);
    return q.Distance(p);
};

GPoint State::ConnectPoint(GPoint p) {
    GPoint m, q;
    m.x = (_rect.Left + _rect.Right) / 2;
    m.y = (_rect.Top + _rect.Bottom) / 2;
    GPoint leftTop = GPoint(_rect.Left, _rect.Top) - m;
    GPoint rightTop = GPoint(_rect.Right, _rect.Top) - m;
    GPoint rightBottom = GPoint(_rect.Right, _rect.Bottom) - m;
    GPoint leftBottom = GPoint(_rect.Left, _rect.Bottom) - m;
    double a = (p - m).Radian();
    double b;
    if (a >= rightTop.Radian() && a < rightBottom.Radian()) {
        b = double(p.y - m.y) / double(p.x - m.x);
        q.x = _rect.Right;
        q.y = m.y + b * (_rect.Right - m.x);
    }
    else if (rightBottom.Radian() <= a && a < leftBottom.Radian()) {
        b = double(p.x - m.x) / double(p.y - m.y);
        q.x = m.x + b * (_rect.Bottom - m.y);
        q.y = _rect.Bottom;
    }
    else if (a >= leftBottom.Radian() || a < leftTop.Radian()) {
        b = double(p.y - m.y) / double(p.x - m.x);
        q.x = _rect.Left;
        q.y = m.y + b * (_rect.Left - m.x);
    }
    else if (leftTop.Radian() <= a && a < rightTop.Radian()) {
        b = double(p.x - m.x) / double(p.y - m.y);
        q.x = m.x + b * (_rect.Top - m.y);
        q.y = _rect.Top;
    }

    return q;
};

int Transition::GetShortestPoint(GPoint p) {
    double distance = UNLIMIT;
    int select;
    for (int i = 0; i < _size; i++) {
        if (_points[i].Distance(p) < distance) {
            distance = _points[i].Distance(p);
            select = i;
        }
    }
    return select;
};
 
Transition::Transition() {
    _type = TGPageForm::TRANSITION;
};

void Transition::Initial(GPoint from, GPoint to, bool active) {
    _points[0] = from;
    _points[1] = (from + to) / 2;
    _points[2] = to;
    _size = 3;
    _active = active;
    _type = TGPageForm::TRANSITION;
    // gamma trace            
    int i = currDoc->ppc->ActivePageIndex;
    TVarRec tempvar[2] = {int(_points[1].x), int(_points[1].y)};
    currDoc->page[i]->label->Caption = Format("(%d,%d)", tempvar, 2);
};

void Transition::DrawArrow() {
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
    // gamma trace
    //TVarRec tempvar[7] = {float(a), int(point[0].x), int(point[0].y), int(point[1].x), int(point[1].y), int(point[2].x), int(point[2].y)};
    //currDoc->page[i]->label->Caption = Format("%f (%d,%d) (%d, %d) (%d, %d)", tempvar, 7);

    currDoc->page[i]->Canvas->Brush->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Color = clBlack;
    currDoc->page[i]->Canvas->Pen->Style = psSolid;
    currDoc->page[i]->Canvas->Pen->Mode = pmCopy;
    currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
    currDoc->page[i]->Canvas->Polygon(triangle, 2);
};

void Transition::AdjustCursorIf(int x, int y) { 
    int j = currDoc->ActivePageIndex();
    for (int i = 0; i < _size; i++) {
        if (x >= _points[i].x - 3 && x <= _points[i].x + 3 && y >= _points[i].y - 3  && y <= _points[i].y + 3) {
            currDoc->page[j]->Cursor = TCursor(crCross);
            return;
        }
    }
    currDoc->page[j]->Cursor = TCursor(crDefault);
};

void Transition::ActiveAdjustSize(GPoint p) {
    int i = currDoc->ActivePageIndex();
    switch (currDoc->page[i]->Cursor) {
        case TCursor(crDefault):
            _select = _size - 1;
            break;
        case crCross:
            _select = GetShortestPoint(p); 
            currDoc->page[i]->movePt = _points[_select];
            break;
    }
};

void Transition::AdjustSize(int x, int y) {
    int i = currDoc->ActivePageIndex();
    if (x > currDoc->page[i]->Width || y > currDoc->page[i]->Height) {
        return;
    }  
    GPoint tmp(_points[_select].x, _points[_select].y);
    if (_active) {
        currDoc->page[i]->Canvas->Pen->Color = clBlack;
        currDoc->page[i]->Canvas->Pen->Style = psDashDot;
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
        currDoc->page[i]->movePt = GPoint(x, y);
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
        currDoc->page[i]->movePt = GPoint(x, y);
        currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
        _points[_select] = currDoc->page[i]->movePt;
        currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
    }
};

void Transition::ChangeSize(int x, int y) {
    GPoint p(x, y), q;
    State *state;  
    int i = currDoc->ActivePageIndex();
    if (_select == 0) {
        if (_fromState->PointInRect(_points[0])) {
            state = currDoc->page[i]->GetShortestInRangeState(p.x, p.y);
            if (state != 0) {
                SetFromState(state);
            }
        }
    }
    else if (_select == _size - 1) {
        if (_toState->PointInRect(_points[_size - 1])) {
            q = _toState->ConnectPoint(_points[_size - 2]);
            state = currDoc->page[i]->GetShortestInRangeState(p.x, p.y);
            if (state != 0) {
                SetToState(state);
            }
        }
    }
    else {
        q = p;
        if (q.Distance(p) < 10) {
            _points[_select] = q;
            if (_select == 1) {
                _points[0] = _fromState->ConnectPoint(_points[1]);
            }      
            if (_select == _size - 2) {
                _points[_size - 1] = _toState->ConnectPoint(_points[_size - 2]);
            }
        }
    }
};

void Transition::Draw() {
    DrawArrow();
    if (_active) {
        Active();
    }
};

void Transition::Active() {
    int i = currDoc->ActivePageIndex();
    _active = true;
//    DrawArrow();
    currDoc->page[i]->Canvas->Brush->Color = clWhite;
    for (int j = 0; j < _size; j++) {
        currDoc->page[i]->Canvas->Rectangle(_points[j].x-3, _points[j].y-3, _points[j].x+3, _points[j].y+3);
    }
    currDoc->page[i]->Canvas->Pen->Color = clRed;
    currDoc->page[i]->Canvas->Pen->Style = psDashDot;
    currDoc->page[i]->Canvas->Pen->Mode = pmNotXor;
    currDoc->page[i]->Canvas->Polyline(_points, _size - 1);
};

void Transition::Inactive() {
    _active = false;
    int i = currDoc->ppc->ActivePageIndex;
    currDoc->page[i]->Refresh();
    currDoc->page[i]->Draw();
};

double Transition::Distance(int x, int y) {
    GPoint p(x, y);
    double distance = UNLIMIT, d;
    for (int i = 0; i < _size - 1; i++) {
        d = GLine(_points[i], _points[i+1]).Distance(p);
        if (d < distance) {
            distance = d;
        }
    }
    return distance;
};

State* Transition::FromState() {
    return _fromState;
};

State* Transition::ToState() {
    return _toState;
};

void Transition::SetFromState(State *from) {
    _points[0] = from->ConnectPoint(_points[1]);
    _fromState = from;
};

void Transition::SetToState(State *to) {
    _points[_size - 1] = to->ConnectPoint(_points[_size - 2]);
    _toState = to;
};

void TGPageForm::ChangeAllConnectTransitions(State *state) {
    for (int i = 0; i < _tranSize; i++) {
        if (_tran[i].FromState() == state) {
            _tran[i].SetFromState(state);
        }
        if (_tran[i].ToState() == state) {
            _tran[i].SetToState(state);
        }
    }
};

State* TGPageForm::GetShortestState(int x, int y) {
    State *shortest = 0;
    double distance = UNLIMIT, d;
    for (int i = 0; i < _stateSize; i++) {
        d = _state[i].Distance(x, y);
        if (d < distance) {
            distance = d;
            shortest = &_state[i];
        }
    }
    return shortest;
};
     
State* TGPageForm::GetShortestInRangeState(int x, int y) {
    State *shortest = 0;
    double distance = UNLIMIT, d;
    for (int i = 0; i < _stateSize; i++) {
        if (_state[i].InRange(x, y)) {
            d = _state[i].Distance(x, y);
            if (d < distance) {
                distance = d;
                shortest = &_state[i];
            }
        }
    }
    return shortest;
};

Transition* TGPageForm::GetShortestTransition(int x, int y) {
    Transition *shortest = 0;
    int distance = UNLIMIT, d;
    for (int i = 0; i < _tranSize; i++) {
        d = _tran[i].Distance(x, y);
        if (d < distance) {
            distance = d;
            shortest = &_tran[i];
        }
    }
    return shortest;
};
 
GControl* TGPageForm::SelectedControl() {
    return _selected;
};

void TGPageForm::AddState(int left, int top, int right, int bottom) {
    if (_stateSize < MAX_STATE) {
        int temp;
        if (right < left) {
            temp = left;
            left = right;
            right = temp;
        }
        if (bottom < top) {
            temp = top;
            top = bottom;
            bottom = temp;
        }
        if (!((right - left >= (3*2 + 1))&&(bottom - top >= (3*2 + 1)))) {
            Refresh();
            Draw();
            return;
        }
        _state[_stateSize].SetRect(left, top, right, bottom);
        _stateSize++;
        if (_selected) {
            _selected->Inactive();
        }
        _state[_stateSize - 1].Active();
        _selected = &_state[_stateSize - 1];

        label->Caption = "gamma:" + IntToStr(_stateSize);
        for (int j = 0; j < _stateSize; j++) {
            if (_state[j].IsActive()) {
                label->Caption = label->Caption + "1";
            }
            else {
                label->Caption = label->Caption + "0";
            }
        }
    }
    else {
        throw OverBound();
    }
};

void TGPageForm::AddTransition(GPoint from, GPoint to) {
    if (_tranSize < MAX_TRANSITION) {
        double d;
        State *fromState = GetShortestInRangeState(from.x, from.y);
        State *toState = GetShortestInRangeState(to.x, to.y);
        if (from.Distance(to) > 10 && fromState && toState) {
            _tran[_tranSize].Initial(from, to, false);
            _tran[_tranSize].SetFromState(fromState);
            _tran[_tranSize].SetToState(toState);
            _tranSize++;
            if (_selected) {
                _selected->Inactive();
            }
            _tran[_tranSize - 1].Active();
            _selected = &_tran[_tranSize - 1];
        }
        else {
            Refresh();
            Draw();
            return;
        }
    }
    else {
        throw OverBound();
    }
};

// Decide which control to be select by (x, y) which mouse down, and then put control pointer to _selected.
void TGPageForm::SelectControl(int x, int y) {
    State *state;
    Transition *tran;
    int d1, d2;

    state = GetShortestState(x, y);
    if (state != 0) {
        d1 = state->Distance(x, y);
    }
    else {
        d1 = UNLIMIT;
    }
    tran = GetShortestTransition(x, y);
    if (tran != 0) {
        d2 = tran->Distance(x, y);
    }
    else {
        d2 = UNLIMIT;
    }
    if (state != 0 || tran != 0) {
        if (_selected) {
            _selected->Inactive();
        }
        if (d1 < d2) {
            _selected = state; 
    // gamma trace
    int j = currDoc->ActivePageIndex();
    TVarRec tempvar[1] = {d1};
    currDoc->page[j]->label->Caption = Format("distance = %d", tempvar, 1);

        }
        else {
            _selected = tran;
    // gamma trace
    int j = currDoc->ActivePageIndex();
    TVarRec tempvar[1] = {d2};
    currDoc->page[j]->label->Caption = Format("distance = %d", tempvar, 1);

        }
        _selected->Active();
    }
};

void TGPageForm::Draw() {
    int i;
    for (i = 0; i < _stateSize; i++) {
        _state[i].Draw();
    }   
    for (i = 0; i < _tranSize; i++) {
        _tran[i].Draw();
    }
};
*/
//---------------------------------------------------------------------------
__fastcall TGPageForm::TGPageForm(TComponent* Owner)
 : TForm(Owner)//, _stateSize(0), _tranSize(0), _selected(0)
{
/*    Visible = true;
    Color = clWhite;
    BorderStyle = bsNone;
    Left = 200;
    Top = 0;
    Width = 200;
    Height = 800;
    Canvas->Pen->Mode = pmNotXor;
//  Canvas->CopyMode = cmMergePaint;
    label = new TLabel(this);
    label->Parent = this;

    PopupMenu = new TPopupMenu(this);
    TMenuItem *NewItem = new TMenuItem(PopupMenu);
    NewItem->Caption = "Format";
    PopupMenu->Items->Add(NewItem);
    NewItem = new TMenuItem(PopupMenu);
    NewItem->Caption = "Add Point";
    PopupMenu->Items->Add(NewItem);*/
}
//---------------------------------------------------------------------------
/*void __fastcall TGPageForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    switch (MainForm->drawButton) {
        case dtPoint:
            switch (Cursor) {
                case TCursor(crDefault):
                    SelectControl(X, Y);
                    break;
                case TCursor(crCross):
                    SelectedControl()->ActiveAdjustSize(GPoint(X, Y));
                    break;
                case TCursor(crSizeAll):
                    SelectedControl()->ActiveMove(GPoint(X, Y));
                    break;
            }
            break;
        case dtRectangle:
            Canvas->MoveTo(X, Y);
            origin = GPoint(X, Y);
            movePt = origin;
            tmpState.Initial(X, Y, X, Y, false);
            tmpState.ActiveAdjustSize(GPoint(X, Y));
            break;
        case dtLine:
            Canvas->MoveTo(X, Y);
            origin = GPoint(X, Y);
            movePt = origin;
            tmpTran.Initial(GPoint(X, Y), GPoint(X, Y), false);
            tmpTran.ActiveAdjustSize(GPoint(X, Y));
            break;
    }
    mouseDown = true;

    TVarRec tempvar[2] = {X, Y};
    currDoc->Memo1->Text = Format("Origin: (%d, %d)", tempvar, 2);

}
//---------------------------------------------------------------------------
void __fastcall TGPageForm::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if (mouseDown) {
        switch (MainForm->drawButton) {
            case dtPoint:
                if (SelectedControl() != 0) {
                    if (Cursor == TCursor(crCross)) {
                        SelectedControl()->AdjustSize(X, Y);
                        label->Caption = "AdjustSize()";
                    }
                    else if (Cursor == TCursor(crSizeAll)) {
                        SelectedControl()->Move(GPoint(X, Y));
                        label->Caption = "Move()";
                    }
                }
                break;
            case dtRectangle:
                tmpState.AdjustSize(X, Y);
                label->Caption = "_state.AdjustSize(X, Y)";
                break;
            case dtLine:
                tmpTran.AdjustSize(X, Y);
                break;
        }
    }
    else {
        switch (MainForm->drawButton) {
            case dtPoint:
                if (SelectedControl() != 0) {
                    SelectedControl()->AdjustCursorIf(X, Y);
                }
                break;
        }
    }

    TVarRec tempvar[2] = {X, Y};
    currDoc->Memo1->Text = Format("Current: (%d, %d)", tempvar, 2);
}
//---------------------------------------------------------------------------
void __fastcall TGPageForm::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (mouseDown) {
        if (X > Width) {
            X = Width;
        }
        if (Y > Height) {
            Y = Height;
        }
        switch (MainForm->drawButton) {
            case dtRectangle:
                AddState(origin.x, origin.y, X, Y);
                break;
            case dtLine:
                AddTransition(origin, GPoint(X, Y));
                break;
            case dtPoint:
        // test {
                if (SelectedControl() != 0) {
                    if (Cursor == TCursor(crCross)) {
                        SelectedControl()->ChangeSize(X, Y);
                        if (SelectedControl()->Type() == STATE) {
                            ChangeAllConnectTransitions((State*)SelectedControl());
                        }
                    }
                    else if (Cursor == TCursor(crSizeAll)) {
                        SelectedControl()->ConfirmMove(GPoint(X, Y));
                        if (SelectedControl()->Type() == STATE) {
                            ChangeAllConnectTransitions((State*)SelectedControl());
                        }
    TVarRec tempvar[2] = {X, Y};
    label->Caption = Format("Current: (%d, %d)", tempvar, 2);
                    }
                    Refresh(); // clear canvas
                    Draw();
                }
        // }
                break;
            default:
                break;
        }
    }
    mouseDown = false;
    if (Button == mbRight) {
    int j = currDoc->ActivePageIndex();
    int left = currDoc->Left + currDoc->ppc->Left + currDoc->pts[j]->Left + currDoc->page[j]->Left;
    int top = MainForm->ToolBar2->Height + currDoc->Top + currDoc->ppc->Top + currDoc->pts[j]->Top + currDoc->page[j]->Top;
        PopupMenu->Popup(left + X, top + Y);
    }
} */
//---------------------------------------------------------------------------
