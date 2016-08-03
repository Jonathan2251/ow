/* lex for permission.def :
example : refer to input file "permission.def" 
*/

%{
#include <math.h>
#include <string.h>
#include "pc1.tab.h"
#include "yyerror.h"
#include "c_interface.h"
%}

DEFTRAP        "define_trap"
DEFEND         "define_end"
NS             "NS"
NOT_NS         "!NS"
FIELDNAME      [[:alpha:]_][[:alnum:]_]*[.][[:alpha:]_][[:alnum:]_]*
%x comment
%%
\n             {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp); lexer_line.line++; }
[ \t]+         {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp); } /* skip space, tab, EndOfLine */
{DEFTRAP}      {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp); yylval = SaveToken(yytext, yyleng); return DEFTRAP;	}
{DEFEND}       {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp); yylval = SaveToken(yytext, yyleng); return DEFEND; }
{NS}           {ECHO; fwrite("!IsSecureAccess()", sizeof(char), strlen("!IsSecureAccess()"), outtmp); yylval = SaveToken(yytext, yyleng); return NS;	}
{NOT_NS}       {ECHO; fwrite("IsSecureAccess()", sizeof(char), strlen("IsSecureAccess()"), outtmp); yylval = SaveToken(yytext, yyleng); return NS;	}
{FIELDNAME}    {ECHO; CodeGen_QueryRegField(xxtext, xxleng, outtmp); yylval = SaveToken(yytext, yyleng); return FIELDNAME;	}
[\"]           {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp); return yytext[0]; }
.              {ECHO; fwrite(xxtext, sizeof(char), xxleng, outtmp);}
%%

int xxerror(char *s)
{
	error_at_line (&lexer_line, s);
	return 1;
}

#ifndef xxwrap
int xxwrap()	{return 1;	}
#endif
