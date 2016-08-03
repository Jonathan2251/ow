/* lex for stationscr.txt :
example : refer to input file "stationscr.txt" 
*/

%{
#include <math.h>
#include <string.h>
#include "cs.tab.h"
#include "yyerror.h"

char result[10][2000];
char *p1, *p2;
%}

ID	[[:alpha:]_][[:alnum:]_]*
STR	\"[^\n\"]*\"
NUM	[0-9]+
%x comment 
%%
"/*"					BEGIN(comment);

"//"[^\n]*\n			{ECHO; lexer_line.line++;}	/* skip comment : from // to \n */
"C_VARIABLES"[ \t\n]*\{[^}]*\}	{ECHO;	p1 = strstr(yytext, "{"); p2 = strstr(p1, "}");
	memcpy(result[0], p1+1, p2-p1-1); result[0][p2-p1] = '\0'; yylval = result[0]; return C_VARS;	}
\n		{ ECHO; lexer_line.line++; }
[ \t]+				{ECHO; } /* skip space, tab, EndOfLine */
{STR}					{ECHO;	memcpy(result[0], yytext, yyleng); result[0][yyleng] = '\0'; yylval = result[0];	return STR;	}
"TESTITEM_DECLARATION"	{ECHO;	return TI_DECLARE;	}
"ACTIONITEM_DECLARATION"	{ECHO;	return AI_DECLARE;	}
"CONTROLITEM_DECLARATION"	{ECHO;	return CI_DECLARE;	}
"MODEL"					{ECHO;	memcpy(result[1], yytext, yyleng); result[1][yyleng] = '\0'; yylval = result[1];	return MODEL;}
"STATION"				{ECHO;	memcpy(result[1], yytext, yyleng); result[1][yyleng] = '\0'; yylval = result[1];	return STATION;}
"DESCRIPTION"			{ECHO;	return DESCRIPTION;	}
"ITEM"					{ECHO;	return ITEM;	}
"delay_100ms"			{ECHO;	return DELAY_100MS;	}
"wait_keyin"			{ECHO;	return WAIT_KEYIN;	}
{NUM}					{ECHO;	memcpy(result[2], yytext, yyleng); result[2][yyleng] = '\0'; yylval = result[2];	return NUM;	}
{ID}					{ECHO;	memcpy(result[3], yytext, yyleng); result[3][yyleng] = '\0'; yylval = result[3];	return ID;	}
[{}=:()|]				{ECHO;	return yytext[0]; }
.						{ECHO;	error_at_line (&lexer_line, "unexpected character `%s'", yytext);	}

<comment>\n			{ ECHO; lexer_line.line++; }
<comment>[^*\n]*		/* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*	/* eat up '*'s not followed by '/'s */
<comment>"*"+"/"		{ update_lineno (yytext, yyleng); BEGIN(INITIAL); }

%%

#ifndef yywrap
int yywrap()	{return 1;	}
#endif
