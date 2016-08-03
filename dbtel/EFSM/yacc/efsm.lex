/* lex for Extended Finite State Machine (efsm) */
%{
#include <math.h>
#include "y.tab.h"
#include "yyerror.h"
extern char *yylval;
char tmp[4][2000];
%}

%%
"/*"[^"*"]*"*"+("/"|[^/"*"][^"*"]*"*"+"/")	{ECHO;} /* skip comment */
"//"[^\n]*\n			{ECHO;}	/* skip comment : from // to \n */
[ \t\n]+				{ECHO;} /* skip space, tab, EndOfLine */
"state"					{ECHO;	return STATE;	}
"substate"				{ECHO;	return SUBSTATE;}
"thread"				{ECHO;	return THREAD;	}
"transition"			{ECHO;	return TRANSITION;	}
"from"					{ECHO;	return FROM;	}
"to"					{ECHO;	return TO;	}
":"|","					{ECHO;	return yytext[0];	}
"?"[^\?!]+"?"			{ECHO;	memcpy(tmp[0], &yytext[1], yyleng-2); tmp[0][yyleng-2] = '\0'; yylval = tmp[0];	return CONDITION;	}
"!"[^\?!]+"!"			{ECHO;	memcpy(tmp[1], &yytext[1], yyleng-2); tmp[1][yyleng-2] = '\0'; yylval = tmp[1];	return ACTION;	}
[_A-Za-z][_A-Za-z0-9]*	{ECHO;	memcpy(tmp[2], yytext, yyleng); tmp[2][yyleng] = '\0'; yylval = tmp[2];	return ID;	}
[0-9]+					{ECHO;	memcpy(tmp[3], yytext, yyleng); tmp[3][yyleng] = '\0'; yylval = tmp[3];	return PRIORITY;	}
.						{ECHO; return yytext[0];	}
%%

#ifndef yywrap
int yywrap()	{return 1;	}
#endif
