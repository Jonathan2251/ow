
enum {BS_NONE, BS_SINGLE};
enum {FALSE, TRUE};
enum {WHITE, GREY, BLACK};

typedef struct TypePoint {
	int x;
	int y;
}QPoint;

typedef struct TypeRect {
	int x;
	int y;
	int width;
	int height;
}QRect;
/*
typedef struct TypePen {
	int dummy; // prevent armcc compile error for no member
}QPen;

typedef struct TypeBrush {
	int dummy;
}QBrush;
*/
typedef struct TypeFont {
	#define QFONT_MAX_HEIGHT 14
	int type;
	int style;
	int size;
}QFont;

// Lcd have 2 lines and each line can display 16 characters.
// Each character be displayed through a dedicate TLabel.
typedef struct TypeLcd {
//	QPen pen;
//	QBrush brush;
	#define QLCD_WIDTH 101
	#define QLCD_HEIGHT 80
	int type;
}QLcd;

typedef struct TypeCursor {
	#define QCSR_STANDARD 1
	int type;
	QPoint p;
	int blink;
	int on; // true / false : for blink
}QCursor;

typedef struct TypeWindow {
	int bordStyle; // { BS_NONE, BS_SINGLE }
	QRect out; // the rectangle which the shape of window occupied
	QRect in; // the rectangle area which this window to show text, symbol or anything
	int visible;
}QWindow;

typedef struct TypeSymbolPanel {
	#define QSP_MAX_SIZE 7
	char symbol[QSP_MAX_SIZE+1];
	int size;   // size for symbol : value is depend on key pressed. eg. KEY_2 is 4 (ABC2), KEY_7 is 5 (PQRS7).
    int select; // idx for user select
	int x; // pixels offset of window that user selected char beginning
	QWindow window;
}QSymbolPanel;

// Multiple Line Editor for input Alphabetic Text
typedef struct TypeMultiLineEdit {
	#define QMLE_MAX_TEXT_SIZE 161
	#define QMLE_LINE_SIZE 3
	#define QMLE_LINE_HEIGHT 15 // 15 pixel for each line
	char text[QMLE_MAX_TEXT_SIZE];
	int maxLength; // max length for text
	QCursor cursor;
	QPoint cursorPos; // (x, y) : x is row of window, y is column
	char *row[4]; // point to each row begin
//	int pos; // pos for cursor at text, range is 0..strlen(text)
	int autoWrap; // TRUE / FALSE
	int insertMode; // TRUE / FALSE
	QWindow window;
}QMultiLineEdit;

typedef struct TypeSmsEdit {
	#define QSE_CONTINUE 1
	#define QSE_EXIT 2
	#define QSE_TL_EXIT 120 // Time Limit for Exit QSmsEdit, = 120 * idleTime, 60 seconds
	#define QSE_TL_SELECT 4 // Time Limit for Select a Alphabetic, = 4 * idleTime, 2 seconds
	int idleTime; // eg. 1 is meaning 500ms, 2 is 1000ms
	char lastKeyPressed; // the key that user pressed last time.
	int uppercase; // TRUE / FALSE
	QSymbolPanel panel;
	QMultiLineEdit edit;
}QSmsEdit;
