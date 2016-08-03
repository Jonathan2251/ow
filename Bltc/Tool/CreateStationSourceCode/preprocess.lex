/* lex for createstation.exe :
example : refer to input file "stationscr.txt" */

%{
#include "preprocess.tab.h"
#include "yyerror.h"
#include "c_interface.h"
#include <math.h>
#include <string.h>

typedef struct _FileInfo {
	char fileName[129];
	int line;
	YY_BUFFER_STATE bufState;
}FileInfo;

#define MAX_INCLUDE_DEPTH 10
FileInfo include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;
char *p, *p1;
char fileName[129];
int error;
static int i;

extern FILE *out;
extern struct fileloc lexer_line;
%}
%x incl define expand_macro
	/* exclude ">"(\x3E) */
INCLUDE_FN	[\x20-\x3B\x3C\x3D\x3F-\x7E]+
ID	[[:alpha:]_][[:alnum:]_]*
MACRONAME	[[:alpha:]_!@#$%^&*()-+=~][[:alnum:]_!@#$%^&*()-+=~]*
MACROBODY	[[:alnum:]_!@#$%^&*()-+=~/][^\n]*
%%
\n			{ ECHO;	fputc(xxtext[0], out); lexer_line.line++; }
"/*"[^*]*"*"+("/"|[^/*][^*]*"*"+"/")	{ECHO;  fwrite(xxtext, sizeof(char), xxleng, out); update_lineno (xxtext, xxleng);}  /* skip comment   */
"//"[^\n]*\n							{ECHO;  fwrite(xxtext, sizeof(char), xxleng, out); lexer_line.line++;}	/* skip comment : from // to \n */
"#include"		BEGIN(incl);
"#define"		BEGIN(define);
{MACRONAME}		{ i = macro_find(xxtext, xxleng);
		if (i >= 0) macro_expand(xxtext, out);
		else {
			ECHO;  fwrite(xxtext, sizeof(char), xxleng, out);
		}
	}
[ \t]+							{ECHO;  fwrite(xxtext, sizeof(char), xxleng, out); update_lineno (xxtext, xxleng);}
.								{ECHO;	fputc(xxtext[0], out); }
<<EOF>> {
		if ( --include_stack_ptr < 0 ) {
			yyterminate();
		}	
		else {
			fclose(xxin);
			xx_delete_buffer( YY_CURRENT_BUFFER );
			xx_switch_to_buffer( include_stack[include_stack_ptr].bufState );
		}
	}
	
<incl>{
[ \t]*\<[ \t]*{INCLUDE_FN}[ \t]*\>   { /* got the include file name */
		if ( include_stack_ptr >= MAX_INCLUDE_DEPTH ) {
			fprintf( stderr, "Includes nested too deeply" );
			exit( 1 );
		}
		include_stack[include_stack_ptr].line = lexer_line.line;
		include_stack[include_stack_ptr++].bufState = YY_CURRENT_BUFFER;
		
		p = strstr(xxtext, "<");
		//printf("xxtext = %s, p = %s\n", xxtext, p);
		if (p > 0) {
			p1 = strstr(p, ">");
			if (p1 > 0) {
				memcpy(fileName, p+1, p1-p-1);
				fileName[p1 - p - 1] = '\0';
				//printf("\nfileName = %s\n", fileName);
				//getchar();
			}
			else {
				printf("#include FILENAME error !!!\n");
				return 1;
			}
		}
		else {
			printf("#include FILENAME error !!!\n");
			return 1;
		}
       xxin = fopen( fileName, "r" );

       if ( ! xxin )
           printf("fopen(%s) error\n", fileName);

	   strcpy(include_stack[include_stack_ptr].fileName, fileName);
       xx_switch_to_buffer( xx_create_buffer( xxin, YY_BUF_SIZE ) );
	   strcpy(lexer_line.file, fileName);
	   lexer_line.line = 1;

       BEGIN(INITIAL);
	}
.	{
		printf("#include FILENAME error !!!\n");
		return 1;
	}
}	/* end of <incl> */

<define>{
\n			{ ECHO; lexer_line.line++; }
[ \t]*{MACRONAME}[ \t]*{MACROBODY}	{ macro_add(xxtext, xxleng); BEGIN(INITIAL);	}
.	{
		printf("#define statement error !!!\n");
		return 1;
	}
}	/* end of <define> */

%%

int xxerror(char *s)
{
	error_at_line (&lexer_line, s);
	return 1;
}

#ifndef xxwrap
int xxwrap()	{return 1;	}
#endif
