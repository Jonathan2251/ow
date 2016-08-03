#include "c_interface.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern "C" {	// All variables here using C linkage method.

FILE *outtmp;	// define (instance) out variable here, pc.y pc1.lex reference this.
FILE *outcpp;
FILE *outh;
fileloc lexer_line;

Token token;

char* SaveToken(char* src, int leng)
{
  if (token.idx >= LEX_BUFF_NUM_SIZE)
  {
    printf("error!!!,  token.idx = %d >= %d\n", token.idx, LEX_BUFF_NUM_SIZE);
  }
  memcpy(token.value[token.idx], src, leng);
  token.value[token.idx][leng] = '\0';
  token.idx++;
  if (token.idx == LEX_BUFF_NUM_SIZE)
  {
    token.idx = 0;
    return token.value[LEX_BUFF_NUM_SIZE-1];
  }
  else
  {
    return token.value[token.idx-1];
  }
}

void error_at_line (struct fileloc *pos, const char *msg, ...) {
  va_list ap;

  va_start (ap, msg);

  fprintf (stderr, "\nerror in file %s : line %d: ", pos->file, pos->line);
  vfprintf (stderr, msg, ap);
  fputc ('\n', stderr);

  va_end (ap);
}

void update_lineno (const char *l, size_t len)
{
  while (len-- > 0)
    if (*l++ == '\n')
      lexer_line.line++;
}

int yyerror(char *s)
{
	error_at_line (&lexer_line, s);
	return 1;
}

// For example:
//   input: text is CPTR_EL3.TCPAC, leng is strlen("CPTR_EL3.TCPAC")
//   output: write QueryRegField(CPTR, EL3, TCPAC) to out file
void CodeGen_QueryRegField(char* text, int leng, FILE* out)
{
  char* h;
  char* p;
  char regName[50];
  char exceptionLevel[20];
  char fieldName[50];

  fwrite("QueryRegField(\"\")", sizeof(char), strlen("QueryRegField("), out);
  h = text;
  p = strchr(h, '_');
  if (p != NULL)
  {
    memcpy(regName, h, p-h);
    memset(regName+(int)(p-h), '\0', 1);
    h = p+1;
  }
  p = strchr(h, '.');
  if (p != NULL)
  {
    memcpy(exceptionLevel, h, p-h);
    memset(exceptionLevel+(int)(p-h), '\0', 1);
    h = p+1;
  }
  memcpy(fieldName, h, leng+text-h);
  memset(fieldName+(int)(leng+text-h+1), '\0', 1);
  h = p+1;
  fwrite(regName, sizeof(char), strlen(regName), out);
  fwrite(", ", sizeof(char), strlen(", "), out);
  fwrite(exceptionLevel, sizeof(char), strlen(exceptionLevel), out);
  fwrite(", nsFIELD::", sizeof(char), strlen(", nsFIELD::"), out);
  fwrite(fieldName, sizeof(char), strlen(fieldName), out);
  fwrite(")", sizeof(char), strlen(")"), out);
}

char buff_op0[20];

void CodeGen_CheckPermission_FuncName(char* regName, char* exceptionLevel, FILE* ocpp, FILE* oh)
{
  fprintf(ocpp, "TypException CheckPermission_%s%s(TypSysRegRW /*RW*/, TypExceptionLevel level, TypContext& context)\n", regName, exceptionLevel);
  fprintf(ocpp, "{\n");

  fprintf(oh, "extern TypException CheckPermission_%s%s(TypSysRegRW RW, TypExceptionLevel level, TypContext& context);\n", regName, exceptionLevel);
}

void CodeGen_CheckPermission_ConditionTrap(char* condition, char* exceptionLevel, char* op0, FILE* o)
{
  char tmp[1000];

  memcpy(tmp, &condition[1], strlen(condition)-2);
  tmp[strlen(condition)-2] = '\0';

  fprintf(o, "  if (%s)\n", tmp);
  fprintf(o, "  {\n");
  fprintf(o, "    return Trap(%s, context, %s);\n", exceptionLevel, op0);
  fprintf(o, "  }\n");
}

void CodeGen_CheckPermission_FunctionEnd(FILE* o)
{
  fprintf(o, "\n");
  fprintf(o, "  return NO_TRAP;\n");
  fprintf(o, "}\n\n");
}

} // extern "C"
