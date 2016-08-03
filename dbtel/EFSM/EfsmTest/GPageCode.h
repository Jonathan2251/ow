//---------------------------------------------------------------------------

#ifndef PageH
#define PageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
enum GShape {sNone, sSelectRect, sRoundRect, sArrow, sSelectArrow};

struct GPoint : public TPoint {
    GPoint();
    GPoint(int xx, int yy);
    GPoint operator-(GPoint p);
    GPoint operator=(GPoint p);
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

class GControl {
protected:
    int _type; // STATE or TRANSITION 
    bool _active;
public:
    Type();
    bool IsActive();

    // Change Cursor into Ajust Shape. Let user know the Control can be adjusted of size.
    virtual void AdjustCursorIf(int x, int y) = 0; 
    virtual void ActiveAdjustSize(GPoint p) = 0;
    virtual void ActiveMove(GPoint p);  
    virtual void Move(GPoint p);
    virtual void ConfirmMove(GPoint p);
    virtual void AdjustSize(int x, int y) = 0;   
    virtual void ChangeSize(int x, int y) = 0;
    virtual void Draw() = 0;
    virtual void Active();
    virtual void Inactive();
};

class State : public GControl {
private:
    TRect _rect;
    GPoint _pivot;
    TRect _move; // for Move()
    GPoint GetFarthestPoint(GPoint p); 
    GPoint GetShortestPoint(GPoint p);
public:
    State();
    State(int left, int top, int right, int bottom);
    void Initial(int left, int top, int right, int bottom, bool active);
    bool PointInRect(GPoint p);
    void AdjustCursorIf(int x, int y);
    void ActiveAdjustSize(GPoint p);
    void ActiveMove(GPoint p);
    void Move(GPoint p);
    void ConfirmMove(GPoint p);
    void AdjustSize(int x, int y);
    void ChangeSize(int x, int y);
    void Draw();
    void Active();
    void Inactive();
    void operator=(State &state);
    int Left();
    int Top();
    int Right();
    int Bottom();
    void SetRect(int left, int top, int right, int bottom);
    bool InRange(int x, int y);
    double Distance(int x, int y);
    GPoint ConnectPoint(GPoint p); // find the shortest distance point on _rect edge.
};

class Transition : public GControl {
private:
    #define MAX_SIZE 5
    GPoint _points[5];
    int _size;
    State *_fromState, *_toState;
    int _select; // which point index is selected
    int GetShortestPoint(GPoint p);
public:
    Transition();
    void Initial(GPoint from, GPoint to, bool active);
    void DrawArrow();
    void AdjustCursorIf(int x, int y);
    void ActiveAdjustSize(GPoint p);
    void AdjustSize(int x, int y);
    void ChangeSize(int x, int y);
    void Draw();
    void Active();
    void Inactive();
    double Distance(int x, int y);
    State* FromState();   
    State* ToState();
    void SetFromState(State *from);
    void SetToState(State *to);
};

class TGPage : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormHide(TObject *Sender);
private:	// User declarations
#define MAX_STATE       300
#define MAX_TRANSITION  500
#define MAX_CONTROL     MAX_STATE + MAX_TRANSITION
#define UNLIMIT         10000
//    TForm *form;
    State _state[MAX_STATE];
    int _stateSize;
    Transition _tran[MAX_TRANSITION];
    int _tranSize;
    GControl *_selected;
    bool mouseDown;
    class OverBound {};
    void ChangeAllConnectTransitions(State *state);
    Transition* GetShortestTransition(int x, int y);
    State* GetShortestState(int x, int y);
    friend class TMainForm;
public:		// User declarations 
    enum {STATE, TRANSITION};
    __fastcall TGPage(TComponent* Owner);  
    TPopupMenu *PopupMenu;
    TLabel *label;
    GPoint origin, movePt;
    State tmpState;
    Transition tmpTran;  
    State* GetShortestInRangeState(int x, int y);
    GControl *SelectedControl();
    void AddState(int left, int top, int right, int bottom);
    void AddTransition(GPoint from, GPoint to);

    // Decide which control to be select by (x, y) which mouse down, and then put control pointer to _selected.
    void SelectControl(int x, int y);
    //
    //void SelectControl(int);

    // Move *_selected to (x, y)
    void MoveSelectedControl(int x, int y);
    void Draw();
};
//---------------------------------------------------------------------------
extern PACKAGE TGPage *GPage;
//---------------------------------------------------------------------------
#endif
