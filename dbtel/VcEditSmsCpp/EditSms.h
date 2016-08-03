#define u8	unsigned char
#define u16	unsigned short

const char Qt_NoBrush = 0x00;
const char Qt_SolidPattern = 0x01;
const char Qt_Gray = 0x08;

//enum {FALSE, TRUE};
enum {QWIN_BS_NONE, QWIN_BS_SINGLE};

#define QFONT_MAX_HEIGHT 14
#define QCSR_STANDARD 1

struct QPoint {
	int x;
	int y;
};

struct QRect {
	int x;
	int y;
	int width;
	int height;
	void SetGeometry(const int X, const int Y, const int Width, const int Height);
};
/*
typedef struct TypePen {
	int dummy; // prevent armcc compile error for no member
}QPen;*/

struct QBrush {
	char style;
};

class QFont {
public:
	enum {WHITE, GREY, BLACK};
//	int type;
//	int style;
//	int size;
	int CharWidth(/*QFont *font, */const u8 C);
	int StringWidth(/*QFont *font = 0, */const char *S);
};

// Lcd have 2 lines and each line can display 16 characters.
// Each character be displayed through a dedicate TLabel.
class QLcd {
private:
//	QPen pen;
	QBrush brush;
	char oldBrush;
	#define WIDTH 101
	#define HEIGHT 80
	int type;
public:
	const char Brush();
	void SetBrush(const char Brush);
	void RestoreBrush();
	void DrawLine(const int X1, const int Y1, const int X2, const int Y2, const u16 Color);
	void DrawRect(const int X, const int Y, const int Width, const int Height, const u16 Color);
	void EraseRect(int X, int Y, int Width, int Height);
	void DrawChar(const int X, const int Y, const char C);
	void DrawCharInverseColor(const int X, const int y, const char c);
};

struct QCursor {
	int type;
	QPoint p;
	int blink;
	bool on; // true / false : for blink
};

class QWindow {
protected:
	int bordStyle; // { BS_NONE, BS_SINGLE }
	QRect out; // the rectangle which the shape of window occupied
	QRect in; // the rectangle area which this window to show text, symbol or anything
	bool visible;
public:
	bool Visible();
	void SetGeometry(const int X, const int Y, const int Width, const int Height);
	void SetDisplayArea(const int X, const int Y, const int Width, const int Height);
	void SetBordStyle(const int BordStyle);
	void Clear();
	void DrawChar(const int X, const int Y, const char C);
	void DrawCharInverseColor(const int X, const int Y, const char C);
	void DrawText(const int X, const int Y, const char *S);
	void Show();
	void Hide();
};

class QSymbolPanel : public QWindow {
private:
	#define MAX_SIZE 7
	char symbol[MAX_SIZE+1];
	int size;   // size for symbol : value is depend on key pressed. eg. KEY_2 is 4 (ABC2), KEY_7 is 5 (PQRS7).
    int select; // idx for user select
	int x; // pixels offset of window that user selected char beginning
public:
	void SetGeometry(const int X, const int Y, const int Width, const int Height);
	void SetSelect(const int Select);
	void Load(const char key, bool uppercase);
	char SelectedSymbol();
	void Show();
	void SelectNext();
};

// Multiple Line Editor for input Alphabetic Text
class QMultiLineEdit : public QWindow {
private:
	#define MAX_TEXT_SIZE 161
	#define LINE_SIZE 3
	#define LINE_HEIGHT 15 // 15 pixel for each line
	char text[MAX_TEXT_SIZE];
	int maxLength; // max length for text
	QCursor cursor;
	QPoint cursorPos; // (x, y) : x is row of window, y is column
	char *row[4]; // point to each row begin
//	int pos; // pos for cursor at text, range is 0..strlen(text)
	bool autoWrap; // true / false
	bool insertMode; // true / false
	void SetRow(char *S);
	int RowLength(const int Y);
public:
	void RowToString(const int i, char *S);
	char *Text();
	int TextLength();
	int MaxLength();
	QCursor Cursor();
	QPoint CursorPosition();
	bool AutoWrap();
	bool InsertMode();
	bool AtBeginning();
	bool AtEnd();
	void SetText(const char *S);
	void SetMaxLength(const int MaxLength);
	void SetCursor(const QCursor& aCursor);
	bool SetCursorPosition(const int Row, const int Col);
	void SetAutoWrap(const bool AutoWrap);
	void SetInsertMode(const bool InsertMode);
	void ShowCursor();
	void HideCursor();
	void BlinkCursor();
	void ShowText();
	void Show();
	void ScrollUp();
	void ScrollDown();
	void CursorLeft();
	void CursorRight();
	void InsertChar(const char C);
	void UpdateChar(const char C);
};

class QSmsEdit {
private:
	#define CONTINUE 1
	#define EXIT 2
	#define TL_EXIT 120 // Time Limit for Exit QSmsEdit, = 120 * idleTime, 60 seconds
	#define TL_SELECT 4 // Time Limit for Select a Alphabetic, = 4 * idleTime, 2 seconds
	int idleTime; // eg. 1 is meaning 500ms, 2 is 1000ms
	char lastKeyPressed; // the key that user pressed last time.
	bool uppercase; // true / false
	QSymbolPanel panel;
	QMultiLineEdit edit;
public:
	void Begin(const char *S, const int MaxLength);
	int KeyIn(const char Key);
	int TimeOut500ms();
	void End();
};

