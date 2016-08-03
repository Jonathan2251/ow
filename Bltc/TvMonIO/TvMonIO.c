//	author : Gamma Chen

#include "TvMonIO.h"
#include "bstd_defs.h"

#include <string.h>
#include <stdarg.h>

bool pdebugEnable = false;
extern void bcmDbgOutNoNewLine(char *pcFormat, ...);
extern asm uart_out(char c);

TvMonIO tvMonIO;


/*
tvMonIO.in.GetChar = GetBrcmChar;
tvMonIO.in.GetLine = bcmDbgGetLine;
tvMonIO.out.Printf 	= bcmDbgOutNoNewLine;
*/	
/*
void bcmDbgOutNoNewLine_R(const char *pcFormat, ...)
{
    va_list    ap;
    
    // prepare the arguments
    va_start( ap, pcFormat );
	bcmDbgPrintHelper( eDBG_OUT, eLVL_1, (char*)pcFormat, ap, false );
    // clean the stack
    va_end( ap );
}*/

int vprintf_R(const char *pcFormat, va_list ap)
{
	bcmDbgPrintHelper( eDBG_OUT, eLVL_1, (char *)pcFormat, ap, false );

    return 0;
}

char tvMonIO_in_getChar()
{
	unsigned char cBuffer[256];
	char* _istr = tvMonIO.in._istr;
	unsigned long _iidx = tvMonIO.in._iidx;
	
	if (tvMonIO.in._iidx > tvMonIO.in._isize) {
		return 0;
	}
	if ( (_istr[_iidx] & 0xff) == TVMONIO_OUTPUT_MSG_ENABLE) {
		tvMonIO.out.enable = 1;
		_iidx++;
	}
	else {
		tvMonIO.out.enable = 0;
	}
	if (tvMonIO.in._iidx > tvMonIO.in._isize) {
		return 0;
	}
	sscanf(&_istr[_iidx], "%s", cBuffer);
	_iidx = _iidx + strlen((char*)cBuffer) + 1;
	tvMonIO.in._iidx = _iidx;
	
	// Print TvMon input command to RS232
	if (pdebugEnable) {
		bcmDbgOutNoNewLine("\r\n%s\r\n)", cBuffer);
	}
	
	return (char)cBuffer[0];
}

char tvMonIO_in_getc()
{
	char c;
	char* _istr = tvMonIO.in._istr;
	unsigned long _iidx = tvMonIO.in._iidx;
	
	if (tvMonIO.in._iidx > tvMonIO.in._isize) {
		return 0;
	}
	if ( (_istr[_iidx] & 0xff) == TVMONIO_OUTPUT_MSG_ENABLE) {
		tvMonIO.out.enable = 1;
		_iidx++;
		return 0;
	}
	if ( (_istr[_iidx] & 0xff) == '\0') {
		tvMonIO.out.enable = 0;
		_iidx++;
		return 0;
	}
	if (tvMonIO.in._iidx > tvMonIO.in._isize) {
		return 0;
	}
	c = _istr[_iidx];
	_iidx++;
	tvMonIO.in._iidx = _iidx;
	
	// Print TvMon input command to RS232
	if (pdebugEnable) {
		bcmDbgOutNoNewLine("\r\n%s\r\n)", c);
	}
	return c;
}

void tvMonIO_in_getLine(char *pcBuf, long lBufSize)
{
	char cBuffer[256];
	char* _istr = tvMonIO.in._istr;
	unsigned long _iidx = tvMonIO.in._iidx;
	
	if (_istr[_iidx] == TVMONIO_OUTPUT_MSG_ENABLE) {
		tvMonIO.out.enable = 1;
		_iidx++;
	}
	else {
		tvMonIO.out.enable = 0;
	}
	strcpy(cBuffer, &_istr[_iidx]);
	_iidx = _iidx + strlen(cBuffer) + 1;
	tvMonIO.in._iidx = _iidx;
	strcpy(pcBuf, cBuffer);
	
	// Print TvMon input command to RS232
	if (pdebugEnable) {
		bcmDbgOutNoNewLine("\r\n%s\r\n)", cBuffer);
	}
}

void tvMonIO_in_clear()
{
	tvMonIO.in._isize = 0;
	tvMonIO.in._iidx = 0;
	tvMonIO.in._istr[0] = '\0';
}

void tvMonIO_in_addCmd(char* s)
{
	if (tvMonIO.in._isize + strlen(s) > 1000)
		return;
	strcpy(tvMonIO.in._istr+tvMonIO.in._isize, s);
	tvMonIO.in._isize = tvMonIO.in._isize + strlen(s) + 1;
}

void tvMonIO_in_addCmds(char* s[])
{
	int i;
	
	for (i = 0; *s[i] != '\0'; i++) {
		tvMonIO_in_addCmd(s[i]);
	}
	tvMonIO_in_addCmd(s[i]);
}

void tvMonIO_in_addCmdsN(char* s[], unsigned short size)
{
	int i;
	
	for (i = 0; i < size; i++) {
		tvMonIO_in_addCmd(s[i]);
	}
	tvMonIO_in_addCmd(s[i]);
}

void tvMonIO_in_print()
{
	int i;
	bcmDbgOutNoNewLine("TvMonIO.in :\r\n");
	bcmDbgOutNoNewLine("_idx = %d, _size = %d\r\n", tvMonIO.in._iidx, tvMonIO.in._isize);
	for (i = 0; i < tvMonIO.in._isize; i = i+strlen(&tvMonIO.in._istr[i])+1) {
		bcmDbgOutNoNewLine("%s\r\n", &tvMonIO.in._istr[i]);
	}
}

void tvMonIO_in_init()
{
	tvMonIO.in.GetChar = tvMonIO_in_getChar;
	tvMonIO.in.GetC = tvMonIO_in_getc;
	tvMonIO.in.GetLine = tvMonIO_in_getLine;
	tvMonIO_in_clear();
}

void tvMonIO_out_putChar(char c)
{
	tvMonIO.out._ostr[tvMonIO.out._osize++] = c;
}

void tvMonIO_out_clear()
{
	tvMonIO.out._osize = 0;
	tvMonIO.out._oidx = 0;
	tvMonIO.out._ostr[0] = '\0';
}

int tvMonIO_out_vprintf(const char *pcFormat, va_list ap)
{
	char s[1000];
	
	if (pdebugEnable) {
		vprintf(pcFormat, ap);
	}
	vsprintf(s, pcFormat, ap);
	if (strlen(tvMonIO.out._ostr) + strlen(s) > 4000) {
		tvMonIO_out_print();	// gamma trace
		tvMonIO_out_clear();
	}
    vsprintf(tvMonIO.out._ostr+tvMonIO.out._osize, pcFormat, ap);
    tvMonIO.out._osize = strlen(tvMonIO.out._ostr);
}

void tvMonIO_out_printf(const char *pcFormat, ...)
{
    va_list    ap;
    
    // prepare the arguments
    va_start( ap, pcFormat );
	if (pdebugEnable) {
		vprintf(pcFormat, ap);
	}
    if (tvMonIO.out.enable == 0) {
    	va_end( ap );
    	return;
    }
	tvMonIO_out_vprintf(pcFormat, ap);
    // clean the stack
    va_end( ap );
}

void tvMonIO_out_init()
{
	tvMonIO.out.PutChar = tvMonIO_out_putChar;
	tvMonIO.out.Vprintf = tvMonIO_out_vprintf;
	tvMonIO.out.Printf 	= tvMonIO_out_printf;
	tvMonIO_out_clear();
	tvMonIO.out.enable = 0;
}

void tvMonIO_out_print()
{
	int i;
	char	s[501];
	
	bcmDbgOutNoNewLine("TvMonIO.out :\r\n");
	bcmDbgOutNoNewLine("_idx = %d, _size = %d\r\n", tvMonIO.out._oidx, tvMonIO.out._osize);
	for (i = 0; i < tvMonIO.out._osize; i += 500) {
		memcpy(s, tvMonIO.out._ostr+i, 500);
		s[500] = '\0';
		bcmDbgOutNoNewLine("%s", s);
	}
}

void tvMonIO_out_getResult(char* pBegin, char* leadPattern, char* result)
{
	char* p = 0;
	char str[1000];
	
	if (pBegin == 0) {
		return;
	}
	p = strstr(pBegin, leadPattern);
	if (p == 0) {
		return;
	}
	p += strlen(leadPattern);
	sscanf(p, "%s", str);
	strcpy(result, str);
}

void tvMonIO_out_getResultN(char* pBegin, char* leadPattern, char* result, unsigned short resultLength)
{
	char* p = 0;
	
	if (pBegin == 0) {
		return;
	}
	p = strstr(pBegin, leadPattern);
	if (p == 0) {
		return;
	}
	p += strlen(leadPattern);
	if (resultLength < 999) {
		memcpy(result, p, resultLength);
		p[resultLength] = '\0';
	}
}

void tvMonIO_clear()
{
	tvMonIO_in_clear();
	tvMonIO_out_clear();
}

void tvMonIO_init()
{
	tvMonIO_in_init();
	tvMonIO_out_init();
}

void tvMonIO_toRS232()
{
//	tvMonIO.in.GetChar = GetBrcmChar;
	tvMonIO.in.GetC = bcmDbgGetCharNoEnter;
	tvMonIO.in.GetLine = bcmDbgGetLine;
	tvMonIO.out.Vprintf = vprintf_R;
	tvMonIO.out.Printf 	= bcmDbgOutNoNewLine;
	tvMonIO.out.PutChar = uart_out;
}

void tvMonIO_toSDRAM()
{
	tvMonIO.in.GetChar = tvMonIO_in_getChar;
	tvMonIO.in.GetC = tvMonIO_in_getc;
	tvMonIO.in.GetLine = tvMonIO_in_getLine;
	tvMonIO.out.Vprintf = tvMonIO_out_vprintf;
	tvMonIO.out.Printf 	= tvMonIO_out_printf;
	tvMonIO.out.PutChar = tvMonIO_out_putChar;
}

