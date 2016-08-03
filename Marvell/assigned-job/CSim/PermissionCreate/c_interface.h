#ifndef _C_INTERFACE_H
#define _C_INTERFACE_H

#include <stdio.h>

#define YYSTYPE char*

/* A file position, mostly for error messages.  
   The FILE element may be compared using pointer equality.  */
struct fileloc {
  char file[256];
  int line;
};

#ifdef	__cplusplus
extern "C" {
#endif

#define LEX_BUFF_NUM_SIZE 20
#define LEX_BUFF_NAME_SIZE 1000

struct Token {
  char value[LEX_BUFF_NUM_SIZE][LEX_BUFF_NAME_SIZE];
  int idx;
};

extern struct Token token;
extern char* SaveToken(char* src, int leng);
extern void OutputFIELDNAME(char* text, int leng, FILE* o);
extern FILE *outtmp;
extern FILE *outcpp;
extern FILE *outh;
extern struct fileloc lexer_line;

extern char buff_op0[20];

#ifdef	__cplusplus
}
#endif

#endif	// _C_INTERFACE_H
