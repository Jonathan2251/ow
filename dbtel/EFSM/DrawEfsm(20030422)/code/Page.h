//---------------------------------------------------------------------------

#ifndef PageH
#define PageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------

#include <vector>

using namespace std;

enum GShape {sNone, sSelectRect, sRoundRect, sArrow, sSelectArrow};
enum mouseAct {MOUSE_DOWN, MOUSE_MOVE, MOUSE_UP, CLICK, DB_CLICK};
enum {NOTHING, ADJUST_SIZE, MOVE, CHANGE_TEXT};

struct GPoint : public TPoint {
    GPoint();
    GPoint(int xx, int yy);
    GPoint operator-(GPoint p);
//    GPoint operator=(GPoint p);
    GPoint operator+(GPoint p);
    GPoint operator/(int i);
    bool operator==(GPoint p);
    double Radian();
    double Distance(GPoint p);
};

struct GLine {
    GPoint from, to;
    GLine();
    GLine(GPoint aFrom, GPoint aTo);
    void operator=(GLine aLine);
    double Radian();
    double Distance(GPoint p);
};
                 
struct GRect : public TRect {
    GRect();
    GRect(int aLeft, int aTop, int aRight, int aBottom);
//    GRect operator=(GRect rect);
    GPoint GetFarthestPoint(GPoint p);
    bool PointIn(GPoint p); // return true if in TRect
    GPoint ConnectPoint(GPoint p); // find the shortest distance point on _rect edge.
    double Distance(GPoint p);
};
      
class State {
private:
    char id[500];
};

class Transition {
private:
    #define MAX_LINE 100
    #define MAX_LINE_SIZE 500
    State *_fromState, *_toState;
    char _condition[MAX_LINE][MAX_LINE_SIZE];
    char _action[MAX_LINE][MAX_LINE_SIZE];
public:
    Transition();
    void Initial(GPoint from, GPoint to, bool active);
    State* FromState();
    State* ToState();
    void SetFromState(State *from);
    void SetToState(State *to);
};

class GControl {
protected:
    int _type; // STATE or TRANSITION 
    bool _selected;
public:
    Type();
    bool IsSelected();

    // Change Cursor into Ajust Shape. Let user know the Control can be adjusted of size.
    virtual int Motion(GPoint pt, int mouseAction);
    virtual void ActiveAdjustSize(GPoint pt) = 0;
    virtual void ActiveMove(GPoint pt);  
    virtual void Move(GPoint pt);
    virtual void ConfirmMove(GPoint pt);
    virtual void AdjustSize(GPoint pt) = 0;   
    virtual void ChangeSize(GPoint pt) = 0;
    virtual void Draw() = 0;
    virtual void Select();
    virtual void Unselect();
};

class TGPage;

class GTextBox : public GControl {
private:
    TMemo *_text; // _text->Lines = state id
//	AnsiString _text;
    GRect _rect;
    GPoint _pivot;
    GRect _move; // for Move()
    State *_state;
    TGPage *_parent;
public:
    GTextBox(TGPage *parent);
    GTextBox(TGPage *parent, GPoint pt, bool selected);
    void Initial(GRect *rect, bool active);
    GRect Rect();
    void SetRect(GRect *rect);
    int Motion(GPoint pt, int mouseAction);
    void ActiveChangeText();     
    void UnactiveChangeText();
    void ActiveAdjustSize(GPoint pt);
    void AdjustSize(GPoint pt);
    void ChangeSize(GPoint pt);
    void ActiveMove(GPoint pt);
    void Move(GPoint pt);
    void ConfirmMove(GPoint pt);
    void Draw();
};
     
class GTextWin : public GControl {
private:
    TMemo *_text; // _text->Lines = [Condition]/!Action
    Transition *_tran; 
    TGPage *_parent;
public:
    GTextWin();
    GTextWin(TRect *rect);
    void Initial(TRect *rect, bool active);
    GRect Rect();
    void SetRect(GRect *rect);
    int Motion(GPoint pt, int mouseAction);
    void ActiveAdjustSize(GPoint pt);
    void AdjustSize(GPoint pt);
    void ChangeSize(GPoint pt);
    void ActiveMove(GPoint pt);
    void Move(GPoint pt);
    void ConfirmMove(GPoint pt);
    void Draw();
};

class GArrow : public GControl {
private:
    #define MAX_SIZE 10
    GTextBox *_from, *_to;
    GPoint _points[MAX_SIZE], _newP[MAX_SIZE];
	int _size, _newPSize;
    int _select; // which point index in _points[] is selected
    Transition *_tran;  
    TGPage *_parent;
    int GetShortestPoint(GPoint pt);
public:
	GArrow(TGPage *parent);
    void Create(GPoint pt);
    int Motion(GPoint pt, int mouseAction);
    void ActiveAdjustSize(int select);
    void ActiveAdjustSize(GPoint pt);
    void AdjustSize(GPoint pt);
    void ChangeSize(GPoint pt);
    void ActiveMove(GPoint pt);
    void Move(GPoint pt);
    void ConfirmMove(GPoint pt);
    void Draw();
    double Distance(GPoint pt);
    GTextBox *From();
    GTextBox *To();   
    void SetFrom(GTextBox *from);
    void SetTo(GTextBox *to);
    void ResetConnectPointFrom();
    void ResetConnectPointTo();
};

class TGPage : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *PopupMenu1;
	TMenuItem *OpenFormat1;
	TMenuItem *Addgamma1;
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
#define UNLIMIT         10000
    vector<State> _state;
    vector<Transition> _tran;

    vector<GTextBox> _tb; // GTextBox : with RoundRectangle shape to hold the text of state id
    vector<GTextWin> _tw; // GTextWin : with invisible shape to hold the text of condition and action of Transition
    vector<GArrow> _arrow; // GArrow : display shape arrow according information in Transition

    vector<GControl*> _selected;
    bool leftMouseDown;
    int motion;
    
   	ChangeTransitionActionDlg *dialog;

    class OverBound {};
    void ResetArrowsConnectPoint(GTextBox *tb); // Change All Arrows which connected tb
    GArrow* GetShortestArrow(GPoint pt);
    GTextBox* GetShortestTextBox(GPoint pt);
    void AdjustSelectedControlSize(GPoint pt);
    void MoveSelectedControl(GPoint pt);
    bool PointInSelectedArea(GPoint pt);
    void UnselectAll();
    void AddNewTextBox(GPoint pt);
    void AddTransition(GPoint from, GPoint to);
    friend class TMainForm;
public:		// User declarations 
    enum {GTEXTBOX, GTEXTWIN, GARROW };
    __fastcall TGPage(TComponent* Owner);  
    TPopupMenu *PopupMenu;
//    TLabel *label;
    GPoint origin, movePt;
    GArrow *tmpArrow;
    GControl *SelectedControl();        
    GTextBox* GetClosestInRangeTextBox(GPoint pt);

    // Decide which control to be select by (x, y) which mouse down, and then put control pointer to _selected.
    void SelectControl(GPoint pt);

    void Draw();
};
//---------------------------------------------------------------------------
extern PACKAGE TGPage *GPage;
//---------------------------------------------------------------------------
#endif
