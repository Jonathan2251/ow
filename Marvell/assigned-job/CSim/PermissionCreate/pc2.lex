/* lex for tmp.txt :
example : refer to input file "tmp.txt" 
*/

%{
#include <math.h>
#include <string.h>
#include "pc2.tab.h"
#include "yyerror.h"
%}

DEFREG "define_reg"
DEFTRAP "define_trap"
DEFEND "define_end"
ID	[[:alpha:]_][[:alnum:]_]*
NUM	[0-9]+
STR	\"[^\n\"]*\"
%x comment 
%%
"/*"                    {ECHO; BEGIN(comment); }

"//"[^\n]*\n            {ECHO; lexer_line.line++;} /* skip comment : from // to \n */
\n                      {ECHO; lexer_line.line++; }
[ \t]+                  {ECHO; } /* skip space, tab, EndOfLine */
{DEFREG}                {ECHO; yylval = SaveToken(yytext, yyleng); return DEFREG;	}
{DEFTRAP}               {ECHO; yylval = SaveToken(yytext, yyleng); return DEFTRAP;	}
{DEFEND}                {ECHO; yylval = SaveToken(yytext, yyleng); return DEFEND; }
{NUM}                   {ECHO; yylval = SaveToken(yytext, yyleng); return NUM;	}
{ID}                    {ECHO; yylval = SaveToken(yytext, yyleng); return ID;	}
{STR}                   {ECHO; yylval = SaveToken(yytext, yyleng); return STR;	}
[(),]                   {ECHO; return yytext[0]; }
.                       {ECHO; error_at_line (&lexer_line, "unexpected character `%s'", yytext);	}

<comment>\n             {ECHO; lexer_line.line++; }
<comment>[^*\n]*        {ECHO; } /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   {ECHO; } /* eat up '*'s not followed by '/'s */
<comment>"*"+"/"        {ECHO; update_lineno (yytext, yyleng); BEGIN(INITIAL); }
%%

#ifndef yywrap
int yywrap()	{return 1;	}
#endif
