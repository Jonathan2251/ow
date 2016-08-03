//	author : Gamma Chen

#ifndef _TVMON_IO_H
#define _TVMON_IO_H


//#include "gitypes.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
#include <OStypes.h>
#ifdef __cplusplus
}
#endif
*/
#include "stdarg.h"
#include "bcmdbg.h"

#define TVMONIO_OUTPUT_MSG_ENABLE '\t'

typedef struct _TvMonIO_In {
	char _istr[1000];
	unsigned long _iidx;
	unsigned long _isize;
	
	char (*GetChar)(void);
	char (*GetC)(void);
	void (*GetLine)(char *pcBuf, long lBufSize);
} TvMonIO_In;

typedef struct _TvMonIO_Out {
	char _ostr[4000];
	unsigned long _oidx;
	unsigned long _osize;
	unsigned int enable;
			
	int (*Vprintf)(const char *pcFormat, va_list ap);
	void (*Printf)(const char *pcFormat, ...);
	void (*PutChar)(char c);
} TvMonIO_Out;
	
typedef struct _TvMonIO {
	TvMonIO_In in;
	TvMonIO_Out out;
} TvMonIO;

extern TvMonIO tvMonIO;

#ifdef __cplusplus
extern "C" {
#endif

//void bcmDbgOutNoNewLine_R(const char *pcFormat, ...);
int vprintf_R(const char *pcFormat, va_list ap);
char GetBrcmChar();
void bcmDbgGetLine(char *pcBuf, long lBufSize);

char tvMonIO_in_getChar();
void tvMonIO_in_getLine(char *pcBuf, long lBufSize);
void tvMonIO_in_clear();
void tvMonIO_in_addCmd(char* s);
void tvMonIO_in_addCmds(char* s[]);
void tvMonIO_in_addCmdsN(char* s[], unsigned short size);
void tvMonIO_in_print();
void tvMonIO_in_init();

void tvMonIO_out_putChar(char c);
void tvMonIO_out_clear();
void tvMonIO_out_printf(const char *pcFormat, ...);
int tvMonIO_out_vprintf(const char *pcFormat, va_list ap);
void tvMonIO_out_init();
void tvMonIO_out_print();
void tvMonIO_out_getResult(char* pBegin, char* leadPattern, char* result);
void tvMonIO_out_getResultN(char* pBegin, char* leadPattern, char* result, unsigned short resultLength);

void tvMonIO_clear();
void tvMonIO_init();
void tvMonIO_toRS232();
void tvMonIO_toSDRAM();

#ifdef __cplusplus
}
#endif

#endif
