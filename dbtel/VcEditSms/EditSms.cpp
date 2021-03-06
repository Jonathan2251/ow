
//#define QSIMULATE

#include "EditSms.h"

#ifdef QSIMULATE
#include "test.h"
#else
#define u8	unsigned char
#define u16	unsigned short
#include "dr_lcd.h"
#include "hsd09ddl.h"
#endif

#include <stdio.h>
#include <string.h>

//#ifdef QSIMULATE
#define	KEY_0		'0'
#define	KEY_1		'1'
#define	KEY_2		'2'
#define	KEY_3		'3'
#define	KEY_4		'4'
#define	KEY_5		'5'
#define	KEY_6		'6'
#define	KEY_7		'7'
#define	KEY_8		'8'
#define	KEY_9		'9'
#define	KEY_DOWN	'd'
#define	KEY_EXIT	'e'
#define	KEY_F		'f'
#define	KEY_HASH	'h'
#define NO_KEY		'n'
#define	KEY_UP		'u'
#define KEY_INSERT	'i'
//#endif

QFont font;
QLcd lcd;
QSmsEdit editor;

#ifndef QSIMULATE
extern const u8 G_u8_dr_Words_index[];
extern const WordspatternFormat G_u8_dr_Wordspattern[];
extern "C" void p_dr_LCD_WRITE(u16 u16_index, u16 u16_cmd);
extern "C" void p_dr_DisplayBlock(u8 u8_Start_X , u8 u8_Start_Y , u8 u8_End_X, u8 u8_End_Y ,u8 u8_Data);
extern "C" void p_dr_displayChar(u8 u8_x_location,u8 u8_y_location,u8 u8_pattern_ptr,u8 u8_show_type);
extern "C" void p_os13_StartTimer(u16 u16_Time, u8 u8_Event);
extern "C" void p_os13_StopTimer(u8 u8_Event);
#endif

/*
static WordspatternFormat cc;

WordspatternFormat *QCharBmp_InverseColor(const char c) {
	static WordspatternFormat cc;
	u8 idx, i;

	idx = G_u8_dr_Words_index[c];
	cc.u8_width = G_u8_dr_Wordspattern[idx].u8_width;
	cc.u8_high = G_u8_dr_Wordspattern[idx].u8_high;
	for(i = 0; i < cc.u8_high; i++) {
		cc.u8_text[i] = ~G_u8_dr_Wordspattern[idx].u8_pattern[i];
	}
	return &cc;
}*/

// insert c before str
void Q_InsertChar(char *S, const char C) {
	char *p;

	for (p = &S[strlen(S)]; p >= S; p--) {
		*(p+1) = *p;
	}
	*S = C;
}

// QFont::member function

int QFont::CharWidth(/*QFont *font, */const u8 C) {
	u8 i;

	i = G_u8_dr_Words_index[C];
	return (int)G_u8_dr_Wordspattern[i].u8_width;
}

// There is 1 pixel between char. So if there is 3 char, it return (2 + total of width(char) ).
int QFont::StringWidth(/*QFont *font = 0, */const char *S) {
	int i;
	int idx;
	int size;
	int width;

	width = 0;
	size = strlen(S);
	for (i = 0; i < size; i++) {
		idx = (int)G_u8_dr_Words_index[S[i]];
		width += (int)G_u8_dr_Wordspattern[idx].u8_width + 1;
	}
	if (size == 0) {
		width--;
	}
	return width;
}


// QRect::member function
void QRect::SetGeometry(const int X, const int Y, const int Width, const int Height) {
	x = X;
	y = Y;
	width = Width;
	height = Height;
}
/*
// QLine::member function

// return pixels of width of
int QLine_Width(int line) {
	return 80;
}

int QLine_CoordinateY(int line) {
	if (line >= 0 && line <= 5) {
		return (line - 5)*15 + 1;
	}
	return -1;
}

void QLine_ShowCursor(int line, int x, int y) {
	int y1;

	if (y > 13) {
		return;
	}
	y1 = QLine_CoordinateY(line) + y;
	QCsr_Show(&lcd.cursor, x, y1);
}

void QLine_DrawCharBmp(int line, int x, int y, const WordspatternFormat *bmp) {
}


void QLine_EraseRegion(int line, int x1, int y1, int x2, int y2) {
	int y;

	y = Line_CoordinateY(line);
	p_dr_DisplayBlock(x1, y + y1, x2, y + y2, DR_Text_BG_Color);
}

void QLine_DrawChar(int line, int x, int y, const char c) {
}

// This function should be replace with Char_InverseColor() and Line_DrawBmp().
void QLine_DrawCharInverseColor(int line, int x, int y, const char c) {
	int y1;
	u8 i;

	y1 = Line_CoordinateY(line) + y;
	i = G_u8_dr_Words_index[c];
	cc.u8_width = G_u8_dr_Wordspattern[i].u8_width;
	cc.u8_high = G_u8_dr_Wordspattern[i].u8_high;
	p_dr_displayChar(x, y1, G_u8_dr_Wordspattern[i].u8_pattern, 1);
}

void QLine_DrawText(int line, int x, int y, const char *s) {
}

// aLine : 0..2
void QLine_Clear(int aLine) {
}
*/

// QLcd::member function

/*void QLcd_SetBackGroundColor(QLcd *lcd, int color) {
}
*/

void QLcd::DrawLine(const int X1, const int Y1, const int X2, const int Y2) {
	if (X1 == X2) { // vertical line
		int y1, y2;
		if (Y1 <= Y2) {
			y1 = Y1;
			y2 = Y2;
		}
		else {
			y1 = Y2;
			y2 = Y1;
		}
		for(int i = y1 ; i <= y2 ; i++) {
			p_dr_LCD_WRITE(SK43200A_XAddress, X1);//Set start X axis
			p_dr_LCD_WRITE(SK43200A_YAddress, i);//Set Y axis
			p_dr_LCD_WRITE(SK43200A_DispMemAcc, DR_Text_FG_Color);
		}
	}
	else if (Y1 == Y2) { // horizontal line
		int x1, x2;
		if (X1 <= X2) {
			x1 = X1;
			x2 = X2;
		}
		else {
			x1 = X2;
			x2 = X1;
		}
		p_dr_LCD_WRITE(SK43200A_XAddress, x1);//Set start X axis
		p_dr_LCD_WRITE(SK43200A_YAddress, Y1);//Set Y axis
		for(int i = x1 ; i <= x2 ; i++) {
			p_dr_LCD_WRITE(SK43200A_DispMemAcc, DR_Text_FG_Color);
		}
	}
}

// currently, system only implement drawing a horizontal/vertical line : from (x1, y1) to (x2, y2)
void QLcd::DrawRect(const int X, const int Y, const int Width, const int Height) {
	int y1, y2;

//	y1 = HEIGHT - Y;
//	y2 = HEIGHT - Y - Height;
	y1 = Y;
	y2 = Y + Height;
#ifdef QSIMULATE
//	printf("QLcd_DrawRect(%d, %d, %d, %d)", X, Y, Width, Height);
#else
	DrawLine(X, Y, X+Width-1, Y);
	DrawLine(X, Y+Height-1, X+Width-1, Y+Height-1);
	DrawLine(X, Y, X, Y+Height-1);
	DrawLine(X+Width-1, Y, X+Width-1, Y+Height-1);
/*	p_dr_DisplayBlock((u8)X, (u8)y1, (u8)X, (u8)y2, DR_Text_FG_Color); //draw vertical line
	p_dr_DisplayBlock((u8)(X+Width), (u8)y1, (u8)(X+Width), (u8)y2, DR_Text_FG_Color); //draw vertical line
	p_dr_DisplayBlock((u8)X, (u8)y1, (u8)(X+Width), (u8)y1, DR_Text_FG_Color); //draw horizontal line
	p_dr_DisplayBlock((u8)X, (u8)y2, (u8)(X+Width), (u8)y2, DR_Text_FG_Color); //draw horizontal line*/
#endif
}

void QLcd::EraseRect(const int X, const int Y, const int Width, const int Height) {
	int y1, y2;

//	y1 = HEIGHT - Y;
//	y2 = HEIGHT - Y - Height;
	y1 = Y;
	y2 = Y + Height-1;
#ifdef QSIMULATE
//	printf("QLcd_EraseRect(%d, %d, %d, %d)", X, Y, Width, Height);
#else
	p_dr_DisplayBlock((u8)X, (u8)y1, (u8)(X+Width-1), (u8)y2, DR_Text_BG_Color);
#endif
}

void QLcd::DrawChar(const int X, const int Y, const char C) {
	int y1;
	u8 i;

//	y1 = HEIGHT - Y - 15;
	y1 = Y + 14;
	i = (int)G_u8_dr_Words_index[C];
#ifdef QSIMULATE
	printf("%c", C);
#else
	p_dr_displayChar((u8)X, (u8)y1, i, (u8)2);
#endif
}

// This function should be replace with Char_InverseColor() and Line_DrawBmp().
void QLcd::DrawCharInverseColor(const int X, const int Y, const char C) {
	int y1;
	u8 i;

//	y1 = HEIGHT - Y - 15;
	y1 = Y + 14;
	i = (int)G_u8_dr_Words_index[C];
#ifdef QSIMULATE
	printf("~%c", C);
#else
	p_dr_displayChar((u8)X, (u8)y1, i, (u8)1);
#endif
}
/*
void QLcd_DrawText(QLcd *lcd, int x, int y, const char *s) {
	int width, x1;
	const char *p;

	x1 = x;
	for (p = s; *p != '\0'; p++) {
		width = QFont_CharWidth(*p);
		if (x1 + width > QLCD_WIDTH) {
			return;
		}
		QLcd_DrawChar(lcd, x1, y, *p);
		x1 = x1 + width + 1;
	}
}

// If over one line, truncate the over part
void QLcd_DrawTextNChar(QLcd *lcd, int x, int y, const char *s, int n) {
	int i;
	int width, x1;
	const char *p;

	x1 = x;
	for (i = 0, p = s; i < n && *p != '\0'; i++, p++) {
		width = QFont_CharWidth(*p);
		if (x1 + width > QLCD_WIDTH) {
			return;
		}
		QLcd_DrawChar(lcd, x1, y, *p);
		x1 = x1 + width + 1;
	}
}
*/

// QWindow::member function

bool QWindow::Visible() {
	return visible;
}

void QWindow::SetGeometry(const int X, const int Y, const int Width, const int Height) {
	out.SetGeometry(X, Y, Width, Height);
}

void QWindow::SetDisplayArea(const int X, const int Y, const int Width, const int Height) {
	in.SetGeometry(X, Y, Width, Height);
}

void QWindow::SetBordStyle(const int BordStyle) {
	bordStyle = BordStyle;
}

void QWindow::Clear() {
	lcd.EraseRect(in.x, in.y, in.width, in.height);
}

void QWindow::DrawChar(const int X, const int Y, const char C) {
	int x1, y1;

	x1 = in.x + X;
	y1 = in.y + Y;
	lcd.DrawChar(x1, y1, C);
}

// This function should be replace with Char_InverseColor() and Line_DrawBmp().
void QWindow::DrawCharInverseColor(const int X, const int Y, const char C) {
	int x1, y1;

	x1 = in.x + X;
	y1 = in.y + Y;
	lcd.DrawCharInverseColor(x1, y1, C);
}

void QWindow::DrawText(const int X, const int Y, const char *S) {
	int length;
	int width, x1, n;
	const char *p;

	length = strlen(S);
	width = 0;
	x1 = 0;
	for (p = S, n = 0; *p != '\0'; p++, n++) {
		width = font.CharWidth(*p);
		if (x1 + width > in.width) {
			return;
		}
		else {
			DrawChar(x1, Y, *p);
			x1 += width;
			// Erase this pixel between char
			lcd.EraseRect(in.x+x1, in.y+Y, in.width, QFONT_MAX_HEIGHT);
			x1++;
		}
	}
}

void QWindow::Show() {
	lcd.EraseRect(out.x, out.y, out.width, out.height);
	if (bordStyle == QWIN_BS_NONE) {
		lcd.DrawRect(out.x, out.y, out.width, out.height);
	}
	visible = true;
}

void QWindow::Hide() {
	lcd.EraseRect(out.x, out.y, out.width, out.height);
	visible = false;
}

// QSymbolPanel::member function

void QSymbolPanel::SetGeometry(const int X, const int Y, const int Width, const int Height) {
	bordStyle = QWIN_BS_NONE;
	out.SetGeometry(X, Y, Width, Height);
	in.SetGeometry(X, Y, Width, Height);
}

void QSymbolPanel::SetSelect(const int Select) {
	select = Select;
}

// Load Letter and Symbol using aKey information. eg. KEY_2 load ABC.
void QSymbolPanel::Load(const char Key, const bool Uppercase) {
	size = 4;
	select = 0;
	x = 1;
	switch (Key) {
	case '1':
		symbol[0] = '"';
		symbol[1] = '.';
		symbol[2] = ',';
		symbol[3] = '-';
		symbol[4] = '&';
		symbol[5] = ':';
		symbol[6] = '1';
		size = 7;
		break;
	case '2':
		if (Uppercase) {
			symbol[0] = 'A';
			symbol[1] = 'B';
			symbol[2] = 'C';
		}
		else {
			symbol[0] = 'a';
			symbol[1] = 'b';
			symbol[2] = 'c';
		}
		symbol[3] = '2';
		break;
	case '3':
		if (Uppercase) {
			symbol[0] = 'D';
			symbol[1] = 'E';
			symbol[2] = 'F';
		}
		else {
			symbol[0] = 'd';
			symbol[1] = 'e';
			symbol[2] = 'f';
		}
		symbol[3] = '3';
		break;
	case '4':
		if (Uppercase) {
			symbol[0] = 'G';
			symbol[1] = 'H';
			symbol[2] = 'I';
		}
		else {
			symbol[0] = 'g';
			symbol[1] = 'h';
			symbol[2] = 'i';
		}
		symbol[3] = '4';
		break;
	case '5':
		if (Uppercase) {
			symbol[0] = 'J';
			symbol[1] = 'K';
			symbol[2] = 'L';
		}
		else {
			symbol[0] = 'j';
			symbol[1] = 'k';
			symbol[2] = 'l';
		}
		symbol[3] = '5';
		break;
	case '6':
		if (Uppercase) {
			symbol[0] = 'M';
			symbol[1] = 'N';
			symbol[2] = 'O';
		}
		else {
			symbol[0] = 'm';
			symbol[1] = 'n';
			symbol[2] = 'o';
		}
		symbol[3] = '6';
		break;
	case '7':
		if (Uppercase) {
			symbol[0] = 'P';
			symbol[1] = 'Q';
			symbol[2] = 'R';
			symbol[3] = 'S';
		}
		else {
			symbol[0] = 'p';
			symbol[1] = 'q';
			symbol[2] = 'r';
			symbol[3] = 's';
		}
		symbol[4] = '7';
		size = 5;
		break;
	case '8':
		if (Uppercase) {
			symbol[0] = 'T';
			symbol[1] = 'U';
			symbol[2] = 'V';
		}
		else {
			symbol[0] = 't';
			symbol[1] = 'u';
			symbol[2] = 'v';
		}
		symbol[3] = '8';
		break;
	case '9':
		if (Uppercase) {
			symbol[0] = 'W';
			symbol[1] = 'X';
			symbol[2] = 'Y';
			symbol[3] = 'Z';
		}
		else {
			symbol[0] = 'w';
			symbol[1] = 'x';
			symbol[2] = 'y';
			symbol[3] = 'z';
		}
		symbol[4] = '9';
		size = 5;
		break;
	case '0':
		symbol[0] = ' ';
		symbol[1] = '0';
		size = 2;
		break;
    }
	symbol[size] = '\0';
}

char QSymbolPanel::SelectedSymbol() {
	if (select < size) {
		return symbol[select];
	}
	else {
		return '\0';
	}
}

void QSymbolPanel::Show() {
	char c;
//	WordspatternFormat *bmp;

	QWindow::Show();
	QWindow::DrawText(0, 0, symbol);
	c = SelectedSymbol();
	QWindow::DrawCharInverseColor(x, 0, c);
}

void QSymbolPanel::SelectNext() {
	char c;

	c = SelectedSymbol();
	QWindow::DrawChar(x, 0, c);
	select++;
	if (select < size) {
		x = x + font.CharWidth(c) + 1;
	}
	else {
		select = 0;
		x = 1;
	}
	QWindow::DrawCharInverseColor(x, 0, c);
}


// QMultiLineEdit::member function

// ------------------------------------------------------------------
// eg. QFont_CharWidth(*p) == 9;
//	edit.in.width = 99; text = "This is a dog.\nI am Gamma";
//	QMLE_SetRow(edit, text);
// the result:
//	row[0] = &("This is a ") = &text[0]
//	row[1] = &("dog.\n") = &text[10]
//	row[2] = &("Gamma\n") = &text[15]
//	row[3] = 0
// ------------------------------------------------------------------
void QMultiLineEdit::SetRow(char *S) {
	int i, width;
	bool eol;
	char *p;

	row[0] = S;
	for (i = 1; i < LINE_SIZE+1; i++) {
		row[i] = 0;
	}
	width = 0;
	for (i = 1, p = S; *p != '\0' && i < LINE_SIZE+1; p++) {
		if (*p != '\n') {
			width += font.CharWidth(*p);
			if (width > in.width || eol == true) {
				row[i] = p;
				i++;
				width = font.CharWidth(*p);
			}
			width++;
			eol = false;
		}
		else {
			eol = true;
		}
	}
}

void QMultiLineEdit::RowToString(const int i, char *S) {
	char *p;

	for (p = row[i]; p != row[i+1] && *p != '\n' && *p != '\0'; p++) {
		*S = *p;
		S++;
	}
	*S = '\0';
}

// ---------------------------------------------------------------------------
// y : 0 .. 2
// return the col of last char in the line y
// ---------------------------------------------------------------------------
int QMultiLineEdit::RowLength(const int Y) {
	int i;

	for (i = 0; row[Y][i] != '\n' && row[Y][i] != '\0'; i++) {
		if (&row[Y][i] == row[Y+1]) {
			return i - 1;
		}
	}
	return i;
}

char *QMultiLineEdit::Text() {
	return text;
}

int QMultiLineEdit::TextLength() {
	return (int)strlen(text);
}

int QMultiLineEdit::MaxLength() {
	return maxLength;
}

QCursor QMultiLineEdit::Cursor() {
	return cursor;
}

QPoint QMultiLineEdit::CursorPosition() {
	return cursorPos;
}

bool QMultiLineEdit::AutoWrap() {
	return autoWrap;
}

bool QMultiLineEdit::InsertMode() {
	return insertMode;
}

bool QMultiLineEdit::AtBeginning() {
	return &row[cursorPos.y][cursorPos.x] == text;
}

bool QMultiLineEdit::AtEnd() {
	return row[cursorPos.y][cursorPos.x] == '\0';
}

void QMultiLineEdit::SetText(const char *S) {
	strcpy(text, S);
	SetRow(text);
}

void QMultiLineEdit::SetMaxLength(const int MaxLength) {
	maxLength = MaxLength;
}

void QMultiLineEdit::SetCursor(const QCursor& Cursor) {
	cursor = Cursor;
}

bool QMultiLineEdit::SetCursorPosition(const int Row, const int Col) {
	int i, width;

	if (Row >= LINE_SIZE) {
		return false;
	}
	width = 0;
	for (i = 0; i < Col; i++) {
		width += font.CharWidth(row[Row][i]) + 1;
		if (width > in.width) {
			return false;
		}
	}
	cursorPos.y = Row;
	cursorPos.x = Col;
	cursor.p.y = (LINE_HEIGHT + 1) * Row;
	cursor.p.x = width;
	return true;
}

void QMultiLineEdit::SetAutoWrap(const bool AutoWrap) {
	autoWrap = AutoWrap;
}

void QMultiLineEdit::SetInsertMode(const bool InsertMode) {
	insertMode = InsertMode;
}

void QMultiLineEdit::ShowCursor() {
	if (cursor.type == QCSR_STANDARD) {
		lcd.DrawRect(in.x + cursor.p.x, in.y + cursor.p.y, in.x + cursor.p.x + 5, in.y + cursor.p.y);
		cursor.on = true;
	}
}

void QMultiLineEdit::HideCursor() {
	if (cursor.type == QCSR_STANDARD) {
		lcd.EraseRect(in.x + cursor.p.x, in.y + cursor.p.y, in.x + cursor.p.x + 5, in.y + cursor.p.y);
		cursor.on = false;
	}
}

void QMultiLineEdit::BlinkCursor() {
	if (cursor.on == true) {
		lcd.DrawRect(in.x + cursor.p.x, in.y + cursor.p.y, in.x + cursor.p.x + 5, in.y + cursor.p.y);
	}
	else {
		lcd.EraseRect(in.x + cursor.p.x, in.y + cursor.p.y, in.x + cursor.p.x + 5, in.y + cursor.p.y);
	}
	cursor.on = !cursor.on;
}

void QMultiLineEdit::ShowText() {
	int i;
	char str[60];

	for (i = 0; i < 3 && row[i] != 0; i++) {
		RowToString(i, str);
		QWindow::DrawText(0, (LINE_HEIGHT + 1)*i, str);
#ifdef QSIMULATE
		printf("\n");
#endif
	}
}

void QMultiLineEdit::Show() {
	QWindow::Show();
	ShowText();
}

void QMultiLineEdit::ScrollUp() {
	char *p;
	int i, width;

	for (i = LINE_SIZE; i > 0; i--) {
		row[i] = row[i-1];
	}
	if (row[0] != text) {
		// set row[0] = previous line begin char
		width = 0;
		for (p = row[0] - 1; p != text; p--) {
			if (p != row[0] - 1 && *p == '\n') {
				row[0] = p+1;
				break;
			}
			width += font.CharWidth(*p);
			if (width > in.width) {
				row[0] = p;
				break;
			}
			width++;
		}
	}
	cursorPos.y++;
	cursor.p.y += LINE_HEIGHT + 1;
	ShowText();
}

// gamma 20030414 trace to here
void QMultiLineEdit::ScrollDown() {
	char *p;
	int i, width;

	for (i = 0; i < LINE_SIZE; i++) {
		row[i] = row[i+1];
	}
	if (row[LINE_SIZE] != 0) {
		// set row[LINE_SIZE] = next line begin char
		width = 0;
		for (p = row[LINE_SIZE]; *p != '\0'; p++) {
			if (*p == '\n') {
				row[LINE_SIZE] = p+1;
				break;
			}
			width += font.CharWidth(*p);
			if (width > in.width) {
				row[LINE_SIZE] = p;
				break;
			}
			width++;
		}
		if (*p == '\0') {
			// if no next line => last line
			row[LINE_SIZE] = 0;
		}
	}
	cursorPos.y--;
	cursor.p.y -= LINE_HEIGHT - 1;
	ShowText();
}

void QMultiLineEdit::CursorLeft() {
	HideCursor();
	if (cursorPos.x != 0) {
		// move to previous char in the same line
		cursorPos.x--;
		SetCursorPosition(cursorPos.y, cursorPos.x);
	}
	else if (cursorPos.y == 0) { // cursorPos.x == 0 && cursorPos.y == 0
		// still have previous line, move to last char in previous line
		cursorPos.y--;
		cursorPos.x = RowLength(cursorPos.y); // return the col of last char
		SetCursorPosition(cursorPos.y, cursorPos.x);
		ScrollUp();
	}
	else  { // cursorPos.x == 0 && cursorPos.y != 0
		// move to last char in previous line
		cursorPos.y--;
		cursorPos.x = RowLength(cursorPos.y);
		SetCursorPosition(cursorPos.y, cursorPos.x);
	}
	ShowCursor();
}

void QMultiLineEdit::CursorRight() {
	HideCursor();
	if (row[cursorPos.y][cursorPos.x] == '\n' ||
		&row[cursorPos.y][cursorPos.x + 1] == row[cursorPos.y+1]) {
		// move to begin char in next line
		cursorPos.x = 0;
		cursorPos.y++;
	}
	else {
		// move to next char in the same line
		cursorPos.x++;
	}
	SetCursorPosition(cursorPos.y, cursorPos.x);
	if (cursorPos.y == LINE_SIZE) {
		ScrollDown();
		ShowText();
	}
	ShowCursor();
}

// insert char before &row[cursorPos.y][cursorPos.x]
void QMultiLineEdit::InsertChar(const char C) {
	char *p;
	bool scrolldown;

	scrolldown = false;
	HideCursor();
	p = &row[cursorPos.y][cursorPos.x];
	Q_InsertChar(p, C);
	SetRow(row[0]);
	cursor.p.x += font.CharWidth(C) + 1;
	cursorPos.x++;
	if (cursor.p.x + font.CharWidth(row[cursorPos.y][cursorPos.x]) > in.width) {
		cursor.p.x = 0;
		cursor.p.y += LINE_HEIGHT + 1;
		cursorPos.x = 0;
		cursorPos.y++;
		if (cursorPos.y == LINE_SIZE) {
		// cursor in last line of window
			ScrollDown();
			scrolldown = true;
		}
	}
	if (scrolldown == false) {
		ShowText();
	}
	ShowCursor();
}

// update char at &row[cursorPos.y][cursorPos.x]
void QMultiLineEdit::UpdateChar(const char C) {
	bool scrolldown;
	int width;

	scrolldown = false;
	HideCursor();
	width = font.CharWidth(row[cursorPos.y][cursorPos.x+1]);
	row[cursorPos.y][cursorPos.x] = C;
	SetRow(row[0]);
	cursorPos.x++;
	if (cursorPos.x > RowLength(cursorPos.y) ) {
		cursorPos.x = 0;
		cursorPos.y++;
		SetCursorPosition(cursorPos.y, cursorPos.x);
		if (cursorPos.y == LINE_SIZE) {
		// cursor in last line of window
			ScrollDown();
			scrolldown = true;
		}
	}
	if (scrolldown == false) {
		ShowText();
	}
	ShowCursor();
}

void QSmsEdit::Begin(const char *S, const int MaxLength) {
	QCursor cursor;

	cursor.type = QCSR_STANDARD;
	cursor.p.x = 0;
	cursor.p.y = 0;
	cursor.blink = true;
	cursor.on = true;

	// set panel
	panel.SetGeometry(1, 0, 99, LINE_HEIGHT);
	panel.Hide();

	// set edit
	edit.SetBordStyle(QWIN_BS_SINGLE);
	edit.SetGeometry(0, 15, 101, LINE_HEIGHT*3+4);
	edit.SetDisplayArea(1, 16, 99, LINE_HEIGHT*3+2);
	edit.SetCursor(cursor);
	edit.SetCursorPosition(0, 0);
	edit.SetAutoWrap(true);
	edit.SetInsertMode(true);
	edit.SetText(S);
	edit.SetMaxLength(MaxLength);
	edit.Show();

	idleTime = 0;
    lastKeyPressed = NO_KEY;
    uppercase = true;

#ifndef QSIMULATE
	p_os13_StartTimer(50, MI_TIMER_2);
#endif

	edit.SetCursorPosition(2, 17);//gamma trace
}

int QSmsEdit::KeyIn(const char Key) {
	switch (Key) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		//case KEY_STAR:
		case '0':
            if (edit.TextLength() == edit.MaxLength()) {
//gamma mark                pt->buzzar->PlayFailTone();
                return CONTINUE;
            }
            if (Key != lastKeyPressed) {
                panel.Load(Key, uppercase);
				panel.Show();
            }
            else {
				panel.SelectNext();
            }
            idleTime = 0;
            break;
        case KEY_HASH:
            uppercase = !uppercase;
			panel.Show();
            break;
        case KEY_UP:
			if (!edit.AtBeginning()) {
			// move cursorPos to previous char of text
				edit.CursorLeft(); // update row[] if necessary.
			}
            break;
        case KEY_DOWN:
			if (!edit.AtEnd()) {
			// move cursorPos to next char of text
				edit.CursorRight(); // update row[] if necessary.
			}
            break;
		case KEY_INSERT:
			edit.SetInsertMode(!edit.InsertMode());
			break;
        case KEY_F:
			return EXIT;
        case KEY_EXIT:
			return EXIT;
    }
    lastKeyPressed = Key;
#ifndef QSIMULATE
	p_os13_StartTimer(50, MI_TIMER_2);
#endif
    return CONTINUE;
}

// at every 0.5 second (500 mini seconds) trigger this function
int QSmsEdit::TimeOut500ms() {
	int len;

	// blink cursor
	edit.BlinkCursor();

	idleTime++;
	if (idleTime > TL_EXIT) {
    // if user didn't select a letter over 60 seconds.
		panel.Hide();
		idleTime = 0;
		return EXIT;
	}
	if (idleTime > TL_SELECT) {
    // if user didn't press a key over 2 seconds.
		if (panel.Visible() == true) {
			if (edit.TextLength() >= edit.MaxLength()) {
				return CONTINUE;
			}
			char s[200];
			edit.RowToString(edit.CursorPosition().y, s);
			if (edit.InsertMode() == true ||
				s[edit.CursorPosition().x] == '\0' ||
				s[edit.CursorPosition().x] == '\n') {
				// insert char to edit.text before cursorPos
				edit.InsertChar(panel.SelectedSymbol());
			}
			else {
				// update edit.text at cursorPos
				edit.UpdateChar(panel.SelectedSymbol());
			}
			panel.Hide();
		}
		idleTime = 0;
		panel.SetSelect(0);
		lastKeyPressed = NO_KEY;
	}
	return CONTINUE;
}

void QSmsEdit::End() {
	// restore last screen of lcd
#ifndef QSIMULATE
	p_os13_StopTimer(MI_TIMER_2);
#endif
}

extern "C" void QSE_Begin(const char *Text, const int MaxLength) {
	editor.Begin(Text, MaxLength);
}

extern "C" int QSE_TimeOut500ms() {
	return editor.TimeOut500ms();
}

extern "C" int QSE_KeyIn(const char Key) {
	return editor.KeyIn(Key);
}

extern "C" void QSE_End() {
	editor.End();
}

#ifdef QSIMULATE
int main()
{
	char c[10];
	char symbol;

	QSE_Begin("This is a dog. That is a cat.\nAll these two animals appear in family oftenly. Because they are easy to be kept. Gamma\n", 160);
//	printf("cursorPos = (%d, %d)\n", editor.edit.cursorPos.x, editor.edit.cursorPos.y);
	while (1) {
		scanf("%s", c);
		if (strcmp(c, "q") == 0) {
			return 1;
		}
		else if (strcmp(c, "t") == 0) {
			QSE_TimeOut500ms();
			QSE_TimeOut500ms();
			QSE_TimeOut500ms();
			QSE_TimeOut500ms();
			QSE_TimeOut500ms();
		}
		else {
			QSE_KeyIn(c[0]);
		}
	}
	QSE_End();
	return 1;
}
#endif
