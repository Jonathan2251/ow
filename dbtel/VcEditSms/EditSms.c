
#define QSIMULATE

#include "EditSms.h"

#ifdef QSIMULATE
#include "test.h"
#else
#include "dr_lcd.h"
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

QLcd lcd;
QSmsEdit editor;

#ifndef QSIMULATE
extern const u8 G_u8_dr_Words_index[];
extern const WordspatternFormat G_u8_dr_Wordspattern[];
extern void p_os13_StartTimer(u16 u16_Time, u8 u8_Event);
extern void p_os13_StopTimer(u8 u8_Event);
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
void Q_InsertChar(char *str, const char c) {
	char *p;

	for (p = &str[strlen(str)]; p >= str; p--) {
		*(p+1) = *p;
	}
	*str = c;
}

// QFont::member function

int QFont_CharWidth(/*QFont *font, */u8 c) {
	u8 i;

	i = G_u8_dr_Words_index[c];
	return (int)G_u8_dr_Wordspattern[i].u8_width;
}

// There is 1 pixel between char. So if there is 3 char, it return (2 + total of width(char) ).
int QFont_StringWidth(/*QFont *font = 0, */char *str) {
	int i;
	int idx;
	int size;
	int width;

	width = 0;
	size = strlen(str);
	for (i = 0; i < size; i++) {
		idx = (int)G_u8_dr_Words_index[str[i]];
		width += (int)G_u8_dr_Wordspattern[idx].u8_width + 1;
	}
	if (size == 0) {
		width--;
	}
	return width;
}


// QRect::member function
void QRect_SetGeometry(QRect *rect, int x, int y, int width, int height) {
	rect->x = x;
	rect->y = y;
	rect->width = width;
	rect->height = height;
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

// currently, system only implement drawing a horizontal/vertical line : from (x1, y1) to (x2, y2)
void QLcd_DrawRect(QLcd *lcd, int x, int y, int width, int height) {
	int y1, y2;

	y1 = QLCD_HEIGHT - y;
	y2 = QLCD_HEIGHT - y - height;
#ifdef QSIMULATE
//	printf("QLcd_DrawRect(%d, %d, %d, %d)", x, y, width, height);
#else
	p_dr_DisplayBlock((u8)x, (u8)y1, (u8)x, (u8)y2, DR_Text_FG_Color); //draw vertical line
	p_dr_DisplayBlock((u8)(x+width), (u8)y1, (u8)(x+width), (u8)y2, DR_Text_FG_Color); //draw vertical line
	p_dr_DisplayBlock((u8)x, (u8)y1, (u8)(x+width), (u8)y1, DR_Text_FG_Color); //draw horizontal line
	p_dr_DisplayBlock((u8)x, (u8)y2, (u8)(x+width), (u8)y2, DR_Text_FG_Color); //draw horizontal line
#endif
}

void QLcd_EraseRect(QLcd *lcd, int x, int y, int width, int height) {
	int y1, y2;

	y1 = QLCD_HEIGHT - y;
	y2 = QLCD_HEIGHT - y - height;
#ifdef QSIMULATE
//	printf("QLcd_EraseRect(%d, %d, %d, %d)", x, y, width, height);
#else
	p_dr_DisplayBlock((u8)x, (u8)y1, (u8)(x+width), (u8)y2, DR_Text_BG_Color);
#endif
}

void QLcd_DrawChar(QLcd *lcd, int x, int y, const char c) {
	int y1;
	u8 i;

	y1 = QLCD_HEIGHT - y - 15;
	i = (int)G_u8_dr_Words_index[c];
#ifdef QSIMULATE
	printf("%c", c);
#else
	p_dr_displayChar((u8)x, (u8)y1, i, (u8)2);
#endif
}

// This function should be replace with Char_InverseColor() and Line_DrawBmp().
void QLcd_DrawCharInverseColor(QLcd *lcd, int x, int y, const char c) {
	int y1;
	u8 i;

	y1 = QLCD_HEIGHT - y - 15;
	i = (int)G_u8_dr_Words_index[c];
#ifdef QSIMULATE
	printf("~%c", c);
#else
	p_dr_displayChar((u8)x, (u8)y1, i, (u8)1);
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

void QWin_Clear(QWindow *win) {
	QLcd_EraseRect(&lcd, win->in.x, win->in.y, win->in.width, win->in.height);
}

void QWin_DrawChar(QWindow *win, int x, int y, const char c) {
	int x1, y1;

	x1 = win->in.x + x;
	y1 = win->in.y + y;
	QLcd_DrawChar(&lcd, x1, y1, c);
}

// This function should be replace with Char_InverseColor() and Line_DrawBmp().
void QWin_DrawCharInverseColor(QWindow *win, int x, int y, const char c) {
	int x1, y1;

	x1 = win->in.x + x;
	y1 = win->in.y + y;
	QLcd_DrawCharInverseColor(&lcd, x1, y1, c);
}

void QWin_DrawText(QWindow *win, int x, int y, const char *s) {
	int length;
	int width, x1, n;
	const char *p;

	length = strlen(s);
	width = 0;
	x1 = 0;
	for (p = s, n = 0; *p != '\0'; p++, n++) {
		width = QFont_CharWidth(*p);
		if (x1 + width > win->in.width) {
			return;
		}
		else {
			QWin_DrawChar(win, x1, y, *p);
			x1 += width;
			// Erase this pixel between char
			QLcd_EraseRect(&lcd, win->in.x+x1, win->in.y+y, win->in.width, QFONT_MAX_HEIGHT);
			x1++;
		}
	}
}

void QWin_Show(QWindow *win) {
	QRect out;

	out = win->out;
	QLcd_EraseRect(&lcd, out.x, out.y, out.width, out.height);
	if (win->bordStyle == BS_SINGLE) {
		QLcd_DrawRect(&lcd, out.x, out.y, out.width, out.height);
	}
}


// QSymbolPanel::member function

void QSP_SetGeometry(QSymbolPanel *panel, int x, int y, int width, int height) {
	panel->window.bordStyle = BS_NONE;
	QRect_SetGeometry(&panel->window.out, x, y, width, height);
	QRect_SetGeometry(&panel->window.in, x, y, width, height);
}

// Load Letter and Symbol using aKey information. eg. KEY_2 load ABC.
void QSP_Load(QSymbolPanel *panel, const char key, int uppercase) {
	panel->size = 4;
	panel->select = 0;
	panel->x = 1;
	switch (key) {
	case '1':
		panel->symbol[0] = '"';
		panel->symbol[1] = '.';
		panel->symbol[2] = ',';
		panel->symbol[3] = '-';
		panel->symbol[4] = '&';
		panel->symbol[5] = ':';
		panel->symbol[6] = '1';
		panel->size = 7;
		break;
	case '2':
		if (uppercase) {
			panel->symbol[0] = 'A';
			panel->symbol[1] = 'B';
			panel->symbol[2] = 'C';
		}
		else {
			panel->symbol[0] = 'a';
			panel->symbol[1] = 'b';
			panel->symbol[2] = 'c';
		}
		panel->symbol[3] = '2';
		break;
	case '3':
		if (uppercase) {
			panel->symbol[0] = 'D';
			panel->symbol[1] = 'E';
			panel->symbol[2] = 'F';
		}
		else {
			panel->symbol[0] = 'd';
			panel->symbol[1] = 'e';
			panel->symbol[2] = 'f';
		}
		panel->symbol[3] = '3';
		break;
	case '4':
		if (uppercase) {
			panel->symbol[0] = 'G';
			panel->symbol[1] = 'H';
			panel->symbol[2] = 'I';
		}
		else {
			panel->symbol[0] = 'g';
			panel->symbol[1] = 'h';
			panel->symbol[2] = 'i';
		}
		panel->symbol[3] = '4';
		break;
	case '5':
		if (uppercase) {
			panel->symbol[0] = 'J';
			panel->symbol[1] = 'K';
			panel->symbol[2] = 'L';
		}
		else {
			panel->symbol[0] = 'j';
			panel->symbol[1] = 'k';
			panel->symbol[2] = 'l';
		}
		panel->symbol[3] = '5';
		break;
	case '6':
		if (uppercase) {
			panel->symbol[0] = 'M';
			panel->symbol[1] = 'N';
			panel->symbol[2] = 'O';
		}
		else {
			panel->symbol[0] = 'm';
			panel->symbol[1] = 'n';
			panel->symbol[2] = 'o';
		}
		panel->symbol[3] = '6';
		break;
	case '7':
		if (uppercase) {
			panel->symbol[0] = 'P';
			panel->symbol[1] = 'Q';
			panel->symbol[2] = 'R';
			panel->symbol[3] = 'S';
		}
		else {
			panel->symbol[0] = 'p';
			panel->symbol[1] = 'q';
			panel->symbol[2] = 'r';
			panel->symbol[3] = 's';
		}
		panel->symbol[4] = '7';
		panel->size = 5;
		break;
	case '8':
		if (uppercase) {
			panel->symbol[0] = 'T';
			panel->symbol[1] = 'U';
			panel->symbol[2] = 'V';
		}
		else {
			panel->symbol[0] = 't';
			panel->symbol[1] = 'u';
			panel->symbol[2] = 'v';
		}
		panel->symbol[3] = '8';
		break;
	case '9':
		if (uppercase) {
			panel->symbol[0] = 'W';
			panel->symbol[1] = 'X';
			panel->symbol[2] = 'Y';
			panel->symbol[3] = 'Z';
		}
		else {
			panel->symbol[0] = 'w';
			panel->symbol[1] = 'x';
			panel->symbol[2] = 'y';
			panel->symbol[3] = 'z';
		}
		panel->symbol[4] = '9';
		panel->size = 5;
		break;
	case '0':
		panel->symbol[0] = ' ';
		panel->symbol[1] = '0';
		panel->size = 2;
		break;
    }
	panel->symbol[panel->size] = '\0';
}

char QSP_SelectedSymbol(QSymbolPanel *panel) {
	if (panel->select < panel->size)
	{
		return panel->symbol[panel->select];
	}
	else
	{
		return '\0';
	}
}

void QSP_Show(QSymbolPanel *panel) {
	char c;
//	WordspatternFormat *bmp;

	QWin_Show(&panel->window);
	QWin_DrawText(&panel->window, 0, 0, panel->symbol);
	c = QSP_SelectedSymbol(panel);
	QWin_DrawCharInverseColor(&panel->window, panel->x, 0, c);
	panel->window.visible = TRUE;
}

void QSP_Hide(QSymbolPanel *panel) {
	QRect out;

	out = panel->window.out;
	QLcd_EraseRect(&lcd, out.x, out.y, out.width, out.height);
	panel->window.visible = FALSE;
}

void QSP_SelectNext(QSymbolPanel *panel) {
	char c;
	int width;

	c = QSP_SelectedSymbol(panel);
	QWin_DrawChar(&panel->window, panel->x, 0, c);
	panel->select++;
	if (panel->select < panel->size) {
		width = QFont_CharWidth(c);
		panel->x = panel->x + width + 1;
	}
	else {
		panel->select = 0;
		panel->x = 1;
	}
	QWin_DrawCharInverseColor(&panel->window, panel->x, 0, c);
}


// QMultiLineEdit::member function

// ------------------------------------------------------------------
// eg. QFont_CharWidth(*p) == 9;
//	edit.in.width = 99; text = "This is a dog.\nI am Gamma";
//	QMLE_SetRow(edit, text);
// the result:
//	edit->row[0] = &("This is a ") = &text[0]
//	edit->row[1] = &("dog.\n") = &text[10]
//	edit->row[2] = &("Gamma\n") = &text[15]
//	edit->row[3] = 0
// ------------------------------------------------------------------
void QMLE_SetRow(QMultiLineEdit *edit, char *s) {
	int i, width, eol;
	char *p;

	edit->row[0] = s;
	for (i = 1; i < (QMLE_LINE_SIZE+1); i++) {
		edit->row[i] = 0;
	}
	width = 0;
	for (i = 1, p = s; *p != '\0' && i < (QMLE_LINE_SIZE+1); p++) {
		if (*p != '\n') {
			width += QFont_CharWidth(*p);
			if (width > edit->window.in.width || eol == TRUE) {
				edit->row[i] = p;
				i++;
				width = QFont_CharWidth(*p);
			}
			width++;
			eol = FALSE;
		}
		else {
			eol = TRUE;
		}
	}
}

void QMLE_RowToString(QMultiLineEdit *edit, const int i, char *s) {
	char *p;

	for (p = edit->row[i]; p != edit->row[i+1] && *p != '\n' && *p != '\0'; p++) {
		*s = *p;
		s++;
	}
	*s = '\0';
}

// ---------------------------------------------------------------------------
// y : 0 .. 2
// return the col of last char in the line y
// ---------------------------------------------------------------------------
int QMLE_RowEnd(QMultiLineEdit *edit, int y) {
	int i;

	for (i = 0; edit->row[y][i] != '\n' && edit->row[y][i] != '\0'; i++) {
		if (&edit->row[y][i] == edit->row[y+1]) {
			return i - 1;
		}
	}
	return i;
}

void QMLE_SetText(QMultiLineEdit *edit, const char *text) {
	strcpy(edit->text, text);
	QMLE_SetRow(edit, edit->text);
}

int QMLE_SetCursorPosition(QMultiLineEdit *edit, int row, int col) {
	int i, width;

	if (row >= QMLE_LINE_SIZE) {
		return FALSE;
	}
	width = 0;
	for (i = 0; i < col; i++) {
		width += QFont_CharWidth(edit->row[row][i]) + 1;
		if (width > edit->window.in.width) {
			return FALSE;
		}
	}
	edit->cursorPos.y = row;
	edit->cursorPos.x = col;
	edit->cursor.p.y = (QMLE_LINE_HEIGHT + 1) * row;
	edit->cursor.p.x = width;
	return TRUE;
}

void QMLE_ShowCursor(QMultiLineEdit *edit) {
	if (edit->cursor.type == QCSR_STANDARD) {
		QLcd_DrawRect(&lcd, edit->window.in.x + edit->cursor.p.x, edit->window.in.y + edit->cursor.p.y, edit->window.in.x + edit->cursor.p.x + 5, edit->window.in.y + edit->cursor.p.y);
		edit->cursor.on = TRUE;
	}
}

void QMLE_HideCursor(QMultiLineEdit *edit) {
	if (edit->cursor.type == QCSR_STANDARD) {
		QLcd_EraseRect(&lcd, edit->window.in.x + edit->cursor.p.x, edit->window.in.y + edit->cursor.p.y, edit->window.in.x + edit->cursor.p.x + 5, edit->window.in.y + edit->cursor.p.y);
		edit->cursor.on = FALSE;
	}
}

void QMLE_DisplayCursor(QMultiLineEdit *edit) {
	if (edit->cursor.on == TRUE) {
		QLcd_DrawRect(&lcd, edit->window.in.x + edit->cursor.p.x, edit->window.in.y + edit->cursor.p.y, edit->window.in.x + edit->cursor.p.x + 5, edit->window.in.y + edit->cursor.p.y);
	}
	else {
		QLcd_EraseRect(&lcd, edit->window.in.x + edit->cursor.p.x, edit->window.in.y + edit->cursor.p.y, edit->window.in.x + edit->cursor.p.x + 5, edit->window.in.y + edit->cursor.p.y);
	}
	edit->cursor.on = ~edit->cursor.on;
}

void QMLE_ShowText(QMultiLineEdit *edit) {
	int i;
	char str[60];

	for (i = 0; i < 3 && edit->row[i] != 0; i++) {
		QMLE_RowToString(edit, i, str);
		QWin_DrawText(&edit->window, 0, (QMLE_LINE_HEIGHT + 1)*i, str);
#ifdef QSIMULATE
		printf("\n");
#endif
	}
}

void QMLE_Show(QMultiLineEdit *edit) {
	QWin_Show(&edit->window);
	QMLE_ShowText(edit);
}

void QMLE_ScrollUp(QMultiLineEdit *edit) {
	char *p;
	int i, width;

	for (i = QMLE_LINE_SIZE; i > 0; i--) {
		edit->row[i] = edit->row[i-1];
	}
	if (edit->row[0] != edit->text) {
		// set edit->row[0] = previous line begin char
		width = 0;
		for (p = edit->row[0] - 1; p != edit->text; p--) {
			if (p != edit->row[0] - 1 && *p == '\n') {
				edit->row[0] = p+1;
				break;
			}
			width += QFont_CharWidth(*p);
			if (width > edit->window.in.width) {
				edit->row[0] = p;
				break;
			}
			width++;
		}
	}
	edit->cursorPos.y++;
	edit->cursor.p.y += QMLE_LINE_HEIGHT + 1;
	QMLE_ShowText(edit);
}

// gamma 20030414 trace to here
void QMLE_ScrollDown(QMultiLineEdit *edit) {
	char *p;
	int i, width;

	for (i = 0; i < QMLE_LINE_SIZE; i++) {
		edit->row[i] = edit->row[i+1];
	}
	if (edit->row[QMLE_LINE_SIZE] != 0) {
		// set edit->row[QMLE_LINE_SIZE] = next line begin char
		width = 0;
		for (p = edit->row[QMLE_LINE_SIZE]; *p != '\0'; p++) {
			if (*p == '\n') {
				edit->row[QMLE_LINE_SIZE] = p+1;
				break;
			}
			width += QFont_CharWidth(*p);
			if (width > edit->window.in.width) {
				edit->row[QMLE_LINE_SIZE] = p;
				break;
			}
			width++;
		}
		if (*p == '\0') {
			// if no next line => last line
			edit->row[QMLE_LINE_SIZE] = 0;
		}
	}
	edit->cursorPos.y--;
	edit->cursor.p.y -= QMLE_LINE_HEIGHT - 1;
	QMLE_ShowText(edit);
}

void QMLE_MoveCursorToPrevChar(QMultiLineEdit *edit) {
	QMLE_HideCursor(edit);
	if (edit->cursorPos.x != 0) {
		// move to previous char in the same line
		edit->cursorPos.x--;
		QMLE_SetCursorPosition(edit, edit->cursorPos.y, edit->cursorPos.x);
	}
	else if (edit->cursorPos.y == 0) { // edit->cursorPos.x == 0 && edit->cursorPos.y == 0
		// still have previous line, move to last char in previous line
		edit->cursorPos.y--;
		edit->cursorPos.x = QMLE_RowEnd(edit, edit->cursorPos.y); // return the col of last char
		QMLE_SetCursorPosition(edit, edit->cursorPos.y, edit->cursorPos.x);
		QMLE_ScrollUp(edit);
	}
	else  { // edit->cursorPos.x == 0 && edit->cursorPos.y != 0
		// move to last char in previous line
		edit->cursorPos.y--;
		edit->cursorPos.x = QMLE_RowEnd(edit, edit->cursorPos.y);
		QMLE_SetCursorPosition(edit, edit->cursorPos.y, edit->cursorPos.x);
	}
	QMLE_ShowCursor(edit);
}

void QMLE_MoveCursorToNextChar(QMultiLineEdit *edit) {
	QMLE_HideCursor(edit);
	if (edit->row[edit->cursorPos.y][edit->cursorPos.x] == '\n' ||
		&edit->row[edit->cursorPos.y][edit->cursorPos.x + 1] == edit->row[edit->cursorPos.y+1]) {
		// move to begin char in next line
		edit->cursorPos.x = 0;
		edit->cursorPos.y++;
	}
	else {
		// move to next char in the same line
		edit->cursorPos.x++;
	}
	QMLE_SetCursorPosition(edit, edit->cursorPos.y, edit->cursorPos.x);
	if (edit->cursorPos.y == QMLE_LINE_SIZE) {
		QMLE_ScrollDown(edit);
		QMLE_ShowText(edit);
	}
	QMLE_ShowCursor(edit);

/*	if (p.x + QFont_CharWidth(edit->row[edit->cursorPos.y][edit->cursorPos.x]) + QFont_CharWidth(edit->row[edit->cursorPos.y][edit->cursorPos.x+1]) < edit->window.in.width) {
	// move to next char in the same line
		edit->cursor.p.x += QFont_CharWidth(edit->row[edit->cursorPos.y][edit->cursorPos.x]) + 1;
		edit->cursorPos.x++;
	}
	else {
	// move to begin char in the new line
		edit->cursor.p.x = 0;
		edit->cursor.p.y += QMLE_LINE_HEIGHT + 1;
		edit->cursorPos.x = 0;
		edit->cursorPos.y++;
		if (edit->cursorPos.y == QMLE_LINE_SIZE) {
		// cursor in last line of window
			QMLE_ScrollDown(edit);
		}
	}*/
}

// insert char before &edit->row[edit->cursorPos.y][edit->cursorPos.x]
void QMLE_InsertChar(QMultiLineEdit *edit, const char c) {
	char *p;
	int scrolldown;

	scrolldown = FALSE;
	QMLE_HideCursor(edit);
	p = &edit->row[edit->cursorPos.y][edit->cursorPos.x];
	Q_InsertChar(p, c);
	QMLE_SetRow(edit, edit->row[0]);
	edit->cursor.p.x += QFont_CharWidth(c) + 1;
	edit->cursorPos.x++;
	if (edit->cursor.p.x + QFont_CharWidth(edit->row[edit->cursorPos.y][edit->cursorPos.x]) > edit->window.in.width) {
		edit->cursor.p.x = 0;
		edit->cursor.p.y += QMLE_LINE_HEIGHT + 1;
		edit->cursorPos.x = 0;
		edit->cursorPos.y++;
		if (edit->cursorPos.y == QMLE_LINE_SIZE) {
		// cursor in last line of window
			QMLE_ScrollDown(edit);
			scrolldown = TRUE;
		}
	}
	if (scrolldown == FALSE) {
		QMLE_ShowText(edit);
	}
	QMLE_ShowCursor(edit);
}

// update char at &edit->row[edit->cursorPos.y][edit->cursorPos.x]
void QMLE_UpdateChar(QMultiLineEdit *edit, const char c) {
	int scrolldown;
	int width;

	scrolldown = FALSE;
	QMLE_HideCursor(edit);
	width = QFont_CharWidth(edit->row[edit->cursorPos.y][edit->cursorPos.x+1]);
	edit->row[edit->cursorPos.y][edit->cursorPos.x] = c;
	QMLE_SetRow(edit, edit->row[0]);
	edit->cursorPos.x++;
	if (edit->cursorPos.x > QMLE_RowEnd(edit, edit->cursorPos.y) ) {
		edit->cursorPos.x = 0;
		edit->cursorPos.y++;
		QMLE_SetCursorPosition(edit, edit->cursorPos.y, edit->cursorPos.x);
		if (edit->cursorPos.y == QMLE_LINE_SIZE) {
		// cursor in last line of window
			QMLE_ScrollDown(edit);
			scrolldown = TRUE;
		}
	}
	if (scrolldown == FALSE) {
		QMLE_ShowText(edit);
	}
	QMLE_ShowCursor(edit);
}

void QSE_Begin(QSmsEdit *smsEdit, const char *text, int maxLength) {
	QCursor cursor;
	QWindow win;
	QSymbolPanel panel;
	QMultiLineEdit editor;

//	aEditor->SetFont(&font);
//	Lcd_SetFont(&font);

	cursor.type = QCSR_STANDARD;
	cursor.p.x = 0;
	cursor.p.y = 0;
	cursor.blink = TRUE;
	cursor.on = TRUE;


	// set panel
	QSP_SetGeometry(&panel, 1, 0, 99, QMLE_LINE_HEIGHT);
	panel.window.visible = FALSE;

	// set variables of window
	win.bordStyle = BS_SINGLE;
	QRect_SetGeometry(&win.out, 0, 15, 101, QMLE_LINE_HEIGHT*3+4);
	QRect_SetGeometry(&win.in, 1, 16, 99, QMLE_LINE_HEIGHT*3+2);
	win.visible = TRUE;

	// initial editor
	editor.window = win;
	editor.maxLength = maxLength;
	editor.cursor = cursor;
	editor.cursorPos.x = 0;
	editor.cursorPos.y = 0;
	editor.autoWrap = TRUE;
	editor.insertMode = TRUE;

	smsEdit->idleTime = 0;
    smsEdit->lastKeyPressed = NO_KEY;
    smsEdit->uppercase = TRUE;
	smsEdit->panel = panel;
	smsEdit->edit = editor;

	QMLE_SetText(&smsEdit->edit, text);
	QMLE_Show(&smsEdit->edit);
#ifndef QSIMULATE
	p_os10_StartTimer(50, MI_TIMER_2);
#endif
}

int QSE_KeyIn(QSmsEdit *smsEdit, const char key) {
	QSymbolPanel *panel;
	QMultiLineEdit *edit;

	panel = &smsEdit->panel;
	edit = &smsEdit->edit;
	switch (key) {
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
            if ((int)strlen(edit->text) == edit->maxLength) {
//gamma mark                pt->buzzar->PlayFailTone();
                return QSE_CONTINUE;
            }
            if (key != smsEdit->lastKeyPressed) {
                QSP_Load(panel, key, smsEdit->uppercase);
				QSP_Show(panel);
            }
            else {
				QSP_SelectNext(panel);
            }
            smsEdit->idleTime = 0;
            break;
        case KEY_HASH:
            smsEdit->uppercase = !smsEdit->uppercase;
			QSP_Show(&smsEdit->panel);
            break;
        case KEY_UP:
			if (&edit->row[edit->cursorPos.y][edit->cursorPos.x] != edit->text) {
			// move cursorPos to previous char of text
				QMLE_MoveCursorToPrevChar(edit); // update edit->row[] if necessary.
			}
            break;
        case KEY_DOWN:
			if (edit->row[edit->cursorPos.y][edit->cursorPos.x] != '\0') {
			// move cursorPos to next char of text
				QMLE_MoveCursorToNextChar(edit); // update edit->row[] if necessary.
			}
            break;
		case KEY_INSERT:
			edit->insertMode = ~edit->insertMode;
			break;
        case KEY_F:
			return QSE_EXIT;
        case KEY_EXIT:
			return QSE_EXIT;
    }
    smsEdit->lastKeyPressed = key;
    return QSE_CONTINUE;
}

// at every 0.5 second (500 mini seconds) trigger this function
int QSE_TimeOut500ms(QSmsEdit *smsEdit) {
	QMultiLineEdit *edit;
	int len;

	edit = &smsEdit->edit;

	// blink cursor
	QMLE_DisplayCursor(edit);

	smsEdit->idleTime++;
	if (smsEdit->idleTime > QSE_TL_EXIT) {
    // if user didn't select a letter over 60 seconds.
		QSP_Hide(&smsEdit->panel);
		smsEdit->idleTime = 0;
		return QSE_EXIT;
	}
	if (smsEdit->idleTime > QSE_TL_SELECT) {
    // if user didn't press a key over 2 seconds.
		if (smsEdit->panel.window.visible == TRUE) {
			len = strlen(edit->text);
			if (len >= edit->maxLength) {
				return QSE_CONTINUE;
			}
			if (smsEdit->edit.insertMode == TRUE || 
				edit->row[edit->cursorPos.y][edit->cursorPos.x] == '\0' || 
				edit->row[edit->cursorPos.y][edit->cursorPos.x] == '\n') {
				// insert char to QMultiLineEdit::text before cursorPos
				QMLE_InsertChar(edit, QSP_SelectedSymbol(&smsEdit->panel) );
			}
			else {
				// update QMultiLineEdit::text at cursorPos
				QMLE_UpdateChar(edit, QSP_SelectedSymbol(&smsEdit->panel) );
			}
			QSP_Hide(&smsEdit->panel);
		}
		smsEdit->idleTime = 0;
		smsEdit->panel.select = 0;
		smsEdit->lastKeyPressed = NO_KEY;
	}
	return QSE_CONTINUE;
}

void QSE_End(QSmsEdit *smsEdit) {
	// restore last screen of lcd
#ifndef QSIMULATE
	p_os10_StopTimer(MI_TIMER_2);
#endif
}

#ifdef QSIMULATE
int main()
{
	char c[10];
	char symbol;

	QSE_Begin(&editor, "This is a dog. That is a cat.\nAll these two animals appear in family oftenly. Because they are easy to be kept. Gamma\n", 160);
//	QMLE_SetCursorPosition(&editor.edit, 2, 17);
	printf("cursorPos = (%d, %d)\n", editor.edit.cursorPos.x, editor.edit.cursorPos.y);
	while (1) {
		scanf("%s", c);
		if (strcmp(c, "q") == 0) {
			return 1;
		}
		else if (strcmp(c, "t") == 0) {
			QSE_TimeOut500ms(&editor);
			QSE_TimeOut500ms(&editor);
			QSE_TimeOut500ms(&editor);
			QSE_TimeOut500ms(&editor);
			QSE_TimeOut500ms(&editor);
		}
		else {
			QSE_KeyIn(&editor, c[0]);
		}
		symbol = QSP_SelectedSymbol(&editor.panel);
		printf("\nSelected symbol = %c\n", symbol);
	}
	QSE_End(&editor);
	return 1;
}
#endif
