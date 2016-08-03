#include <stdio.h>

yyerror(s)
	char *s;
{
	printf("error = %s\n", s);
}
