#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 4 "efsm.y"
#include <stdio.h>
#define YYSTYPE char*
#define MAX_STATE_LIST_SIZE	200
#define MAX_ID_LIST_SIZE	200
#define MAX_CASE_CODE_SIZE	5000
#define MAX_STATE_EVENT_NO	100
int passCounter;
char currStateId[33];
FILE *out;

typedef struct TypeIdList {
	char entry[MAX_ID_LIST_SIZE][33];
	int size;
}TIdList;

typedef struct TypeEfsm {
	struct TypeState {
		char id[33];
		int nextSiblingIdx;
		struct TypeCbt {
			char code[MAX_CASE_CODE_SIZE];
			void (*Add)(char *line, int i);
		}cbt;
		struct TypeRun {
			struct TypeAction {
				char code[MAX_CASE_CODE_SIZE];
				void (*Add)(char *line, int i);
			}action;
			struct TypeTransferState {
				struct TypeCondition {
					char code[MAX_CASE_CODE_SIZE];
				}condition[MAX_STATE_EVENT_NO];
				int size;
				void (*Add)(char *line, int i, int j);
			}transferState;
		}run;
	}state[MAX_STATE_LIST_SIZE];
	int size;
	void (*Print)();
	void (*AddState)(char *id);
	int (*FindState)(char *id);
	void (*SetNextSiblingIdx)(TIdList *idList);
	void (*InitialCbtRun)();
}TEfsm;

TIdList idList, idList1, idList2;
TEfsm efsm;

void IdL_Print(TIdList *idList)
{
	int i;

	for (i = 0; i < idList->size; i++) {
		printf("(id=%s)\n", idList->entry[i]);
	}
}

void IdL_AddId(TIdList *idList, char *id)
{
	if (idList->size < MAX_ID_LIST_SIZE) {
		strcpy(idList->entry[idList->size], id);
		idList->size++;
	}
	else {
		printf("over memory limit in IdL_AddId().\n");
		exit(1);
	}
}

void IdL_Clear(TIdList *idList)
{
	idList->size = 0;
}

void IdL_Copy(TIdList *dest, TIdList *source)
{
	int i;

	for (i = 0; i < source->size; i++) {
		strcpy(dest->entry[i], source->entry[i]);
	}
	dest->size = source->size;
}

void Efsm_Print()
{
	int i;

	for (i = 0; i < efsm.size; i++) {
		printf("(state=%s,nextSiblingIdx=%d)\n", efsm.state[i].id, efsm.state[i].nextSiblingIdx);
	}
}

void Efsm_AddState(char *id)
{
	if (efsm.size < MAX_STATE_LIST_SIZE) {
		strcpy(efsm.state[efsm.size].id, id);
		efsm.state[efsm.size].nextSiblingIdx = -1;
		efsm.size++;
	}
	else {
		printf("over memory limit in IdL_AddId().\n");
		exit(1);
	}
}

int Efsm_FindState(char *id)
{
	int i;
	for (i = 0; i < efsm.size; i++) {
		if (strcmp(efsm.state[i].id, id) == 0) {
			return i; 
		}
	}
	return -1;
}

void Efsm_SetNextSiblingIdx(TIdList *idList)
{
	int i, x, y;

	for (i = 0; i < idList->size - 1; i++) {
		x = efsm.FindState(idList->entry[i]);
		if (x == -1) {
			printf("error x in SetNextSiblingIdx();\n");
			exit(1);
		}
		y = efsm.FindState(idList->entry[i+1]);
		if (y == -1) {
			printf("error y in SetNextSiblingIdx();\n");
			exit(1);
		}
		efsm.state[x].nextSiblingIdx = y;
	}
}

void Efsm_Cbt_Add(char *line, int i)
{
	strcat(efsm.state[i].cbt.code, line);
}

void Efsm_Run_Action_Add(char *line, int i)
{
	strcat(efsm.state[i].run.action.code, line);
}

void Efsm_Run_TransferState_Add(char *line, int i, int priority)
{
	int j;

	if (priority == 0) {
		strcat(efsm.state[i].run.transferState.condition[0].code, line);
		if (efsm.state[i].run.transferState.size == 0) {
			efsm.state[i].run.transferState.size = 1;
		}
	}
	else if (priority > 0) {
		strcat(efsm.state[i].run.transferState.condition[priority].code, line);
		if (efsm.state[i].run.transferState.size <= priority) {
			efsm.state[i].run.transferState.size = priority + 1;
		}
	}
	else if (priority < 0) {
		printf("error in Efsm_Run_TransferState_Add();\n");
		exit(1);
	}
}

void Efsm_InitialCbtRun()
{
	int i;
	for (i = 0; i < efsm.size; i++) {
		efsm.state[i].cbt.code[0] = '\0';
		efsm.state[i].run.action.code[0] = '\0';
		efsm.state[i].run.transferState.size = 0;
		efsm.state[i].cbt.Add = Efsm_Cbt_Add;
		efsm.state[i].run.action.Add = Efsm_Run_Action_Add;
		efsm.state[i].run.transferState.Add = Efsm_Run_TransferState_Add;
	}
}

void CbtCodeGen(char *stateId, char *substateId)
{
	int i, j;
	char tmp[500];

	i = efsm.FindState(stateId);
	if (i != -1) {
		sprintf(tmp, "\t\t\ts->child = CreateBinTree(%s, s);\n", substateId);
		efsm.state[i].cbt.Add(tmp, i);
		if (efsm.state[i].nextSiblingIdx != -1) {
			sprintf(tmp, "\t\t\ts->sibling = CreateBinTree(%s, s->parent);\n", efsm.state[efsm.state[i].nextSiblingIdx].id);
			efsm.state[i].cbt.Add(tmp, i);
		}
	}
	else {
		printf("error in CbtCodeGen();\n");
		exit(1);
	}
}

void AddEndOfLine(char *out, char *in)
{
	char *p;

	for (p = in; *p != '\0'; p++) {
		if (*p == '\\' && *(p+1) == 'n') {
			sprintf(out + strlen(out), "\n");
			if (*(p+2) != '\0') {
				sprintf(out + strlen(out), "\t\t\t\t\t");
			}
			p++;
		}
		else {
			out[strlen(out) + 1] = '\0';
			out[strlen(out)] = *p;
		}
	}
}

void RunCodeGen(char *condition, char *action, TIdList *from, TIdList *to, int priority, char *stateId)
{
	int i, j;
	char *p;
	char tmp[2000];

	i = efsm.FindState(from->entry[from->size - 1]);
	if (i != -1) {
		/*action*/
		/*sprintf(tmp, "\t\t\t\tif (%s", condition);*/
		sprintf(tmp, "\t\t\t\tif (");
		AddEndOfLine(tmp, condition);
		for (j = 1; j < from->size; j++) {
			sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)", j, from->entry[from->size-1-j]);
		}
		sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)) {\n", j, stateId);
		/*sprintf(tmp + strlen(tmp), "\t\t\t\t\t%s\t\t\t\t}\n", action);*/
		sprintf(tmp + strlen(tmp), "\t\t\t\t\t");
		AddEndOfLine(tmp, action);
		sprintf(tmp + strlen(tmp), "\t\t\t\t}\n");
		efsm.state[i].run.action.Add(tmp, i);

		/* transferState*/
		/*sprintf(tmp, "\t\t\t\tif (%s", condition);*/
		sprintf(tmp, "\t\t\t\tif (");
		AddEndOfLine(tmp, condition);
		for (j = 1; j < from->size; j++) {
			sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)", j, from->entry[from->size-1-j]);
		}
		sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)) {\n", j, stateId);
		for (j = 0; j < from->size; j++) {
			sprintf(tmp + strlen(tmp), "\t\t\t\t\tcurr = curr->parent;\n");
		}
		sprintf(tmp + strlen(tmp), "\t\t\t\t\tdelete curr->child;\n");
		for (j = 0; j < to->size - 1; j++) {
			sprintf(tmp + strlen(tmp), "\t\t\t\t\tcurr->child = new BinTree<int>(%s, curr);\n", to->entry[j]);
			sprintf(tmp + strlen(tmp), "\t\t\t\t\tp = curr->child;\n");
		}
		if (to->size > 1) {
			sprintf(tmp + strlen(tmp), "\t\t\t\t\tp->child = CreateBinTree(%s, p);\n", to->entry[j]);
		}
		else {
			sprintf(tmp + strlen(tmp), "\t\t\t\t\tcurr->child = CreateBinTree(%s, curr);\n", to->entry[j]);
		}
		sprintf(tmp + strlen(tmp), "\t\t\t\t\tchangeState = true;\n");
		sprintf(tmp + strlen(tmp), "\t\t\t\t\tbreak;\n");
		sprintf(tmp + strlen(tmp), "\t\t\t\t}\n");
		efsm.state[i].run.transferState.Add(tmp, i, priority);
	}
	else {
		printf("error in RunCodeGen();\n");
		exit(1);
	}
}

#line 293 "y.tab.h"
#define YYERRCODE 256
#define STATE 257
#define SUBSTATE 258
#define THREAD 259
#define TRANSITION 260
#define FROM 261
#define TO 262
#define CONDITION 263
#define ACTION 264
#define ID 265
#define PRIORITY 266
const short yylhs[] = {                                        -1,
    0,    1,    1,    2,    4,    2,    3,    8,    3,    3,
    5,    5,    9,    6,    6,   11,   10,   10,    7,    7,
   14,   15,   12,   13,   13,
};
const short yylen[] = {                                         2,
    1,    1,    2,    2,    0,    4,    1,    0,    3,    1,
    1,    2,    2,    1,    2,    2,    1,    3,    1,    2,
    0,    0,   10,    1,    3,
};
const short yydefred[] = {                                      0,
    0,    0,    0,    2,    0,    3,    0,    0,    0,    6,
    0,    0,   11,   14,   17,    0,    0,   12,    0,   15,
    0,    0,    0,   19,   18,    0,   20,    0,    0,   21,
    0,   24,    0,    0,    0,   25,    0,    0,
};
const short yydgoto[] = {                                       2,
    3,    4,   10,    7,   11,   12,   23,   19,   13,   16,
   14,   24,   33,   31,   35,
};
const short yysindex[] = {                                   -257,
 -263,    0, -257,    0,    0,    0, -248, -261, -261,    0,
 -253, -250,    0,    0,    0,  -30,  -30,    0, -245,    0,
 -249, -247, -245,    0,    0, -246,    0, -244, -243,    0,
 -242,    0,  -37, -241, -240,    0, -242,  -37,
};
const short yyrindex[] = {                                      0,
    0,    0,   25,    0,    9,    0,    0,    0,    0,    0,
    7,    3,    0,    0,    0,    1,   12,    0,    0,    0,
    0,    0,   13,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -236,    0,    0,    0,    0,    5,
};
const short yygindex[] = {                                      0,
    0,   24,    0,    0,    0,    0,    0,    0,   17,   20,
   18,    8,   -5,    0,    0,
};
#define YYTABLESIZE 271
const short yytable[] = {                                       1,
   16,    5,   10,   15,   23,    9,    7,    8,    4,    8,
    9,   13,    9,   21,   22,   25,   28,   30,   26,   29,
   34,   37,   32,   36,    1,   22,    6,   18,   17,   20,
   27,   38,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,   16,   10,
   16,   23,    8,    7,   23,    4,    5,    5,   13,    9,
   13,
};
const short yycheck[] = {                                     257,
    0,  265,    0,  265,    0,  259,    0,  258,    0,  258,
  259,    0,    0,   44,  260,  265,  263,  261,  266,  264,
   58,  262,  265,  265,    0,  262,    3,   11,    9,   12,
   23,   37,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,  257,
  260,  257,  260,  257,  260,  257,  258,  259,  257,  257,
  259,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 266
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,"':'",0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"STATE","SUBSTATE",
"THREAD","TRANSITION","FROM","TO","CONDITION","ACTION","ID","PRIORITY",
};
const char * const yyrule[] = {
"$accept : input",
"input : states",
"states : state",
"states : states state",
"state : STATE ID",
"$$1 :",
"state : STATE ID $$1 state_body",
"state_body : threads",
"$$2 :",
"state_body : substates $$2 transitions",
"state_body : substates",
"threads : thread",
"threads : threads thread",
"thread : THREAD id_list",
"substates : substate",
"substates : substates substate",
"substate : SUBSTATE id_list",
"id_list : ID",
"id_list : id_list ',' ID",
"transitions : transition",
"transitions : transitions transition",
"$$3 :",
"$$4 :",
"transition : TRANSITION PRIORITY CONDITION ACTION FROM $$3 nested_state_name $$4 TO nested_state_name",
"nested_state_name : ID",
"nested_state_name : nested_state_name ':' ID",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 400 "efsm.y"
/* A lexical scanner generated by flex */

void CopyHead(FILE *out)
{
	FILE *in;
	char *r;
	char buf[500];

	in = fopen("efsm_head.cpp", "rt");
	while (!feof(in)) {
		r = fgets(buf, 500, in);
		if (r != NULL) {
			fputs(buf, out);
		}
	}
	fclose(in);
}

void Efsm_Initial()
{
	efsm.size = 0;
	efsm.Print = Efsm_Print;
	efsm.AddState = Efsm_AddState;
	efsm.FindState = Efsm_FindState;
	efsm.SetNextSiblingIdx = Efsm_SetNextSiblingIdx;
	efsm.InitialCbtRun = Efsm_InitialCbtRun;
}

void Initial()
{
	Efsm_Initial();
}

void CopyFile(FILE *out, FILE *in)
{
	char *r;
	char buf[81];

	while (!feof(in)) {
		r = fgets(buf, 80, in);
		if (r != NULL) {
			fputs(buf, out);
		}
		else {
			fflush(out);
		}
	}
}

void GenerateCode(FILE *out)
{
	FILE *in;
	int i, j;

	in = fopen("pattern1.txt", "r");
	CopyFile(out, in);
	fclose(in);

	if (efsm.size > 0) {
		fprintf(out, "enum {%s = 1", efsm.state[0].id);
		for (i = 1; i < efsm.size; i++) {
			fprintf(out, ",\n %s", efsm.state[i].id);
		}
		fprintf(out, "\n};\n\n");

		fprintf(out, "string stateName[] = {\"\"");
		for (i = 0; i < efsm.size; i++) {
			fprintf(out, ",\n \"%s\"",efsm.state[i].id);
		}
		fprintf(out, "\n};\n\n");
	}
	fprintf(out, "#define STATES_SIZE %d\n", efsm.size);

	in = fopen("pattern2_1.txt", "r");
	CopyFile(out, in);
	fclose(in);

	fprintf(out, "\t_root = CreateBinTree(%s, 0);\n", efsm.state[efsm.size - 1].id);
	in = fopen("pattern2_2.txt", "r");
	CopyFile(out, in);
	fclose(in);

	for (i = 0; i < efsm.size; i++) {
		if (efsm.state[i].cbt.code[0] != '\0') {
			fprintf(out, "\t\tcase %s:\n", efsm.state[i].id);
			fprintf(out, "%s\t\t\tbreak;\n", efsm.state[i].cbt.code);
		}
	}

	in = fopen("pattern3.txt", "r");
	CopyFile(out, in);
	fclose(in);

	for (i = 0; i < efsm.size; i++) {
		if (efsm.state[i].run.action.code[0] != '\0') {
			fprintf(out, "\t\t\tcase %s:\n", efsm.state[i].id);
			fprintf(out, "%s", efsm.state[i].run.action.code);
			fprintf(out, "\t\t\t\tbreak;\n");
		}
	}

	in = fopen("pattern4.txt", "r");
	CopyFile(out, in);
	fclose(in);

	for (i = 0; i < efsm.size; i++) {
		if (efsm.state[i].run.transferState.size > 0) {
			fprintf(out, "\t\t\tcase %s:\n", efsm.state[i].id);
			for (j = 1; j < efsm.state[i].run.transferState.size; j++) {
				if (efsm.state[i].run.transferState.condition[j].code[0] != '\0') {
					fprintf(out, "%s", efsm.state[i].run.transferState.condition[j].code);
				}
			}
			if (efsm.state[i].run.transferState.condition[0].code[0] != '\0') {
				fprintf(out, "%s", efsm.state[i].run.transferState.condition[0].code);
			}
			fprintf(out, "\t\t\t\tbreak;\n");
		}
	}

	in = fopen("pattern5.txt", "r");
	CopyFile(out, in);
	fclose(in);
}

int main(int argc, char **argv)
{
	int error;
	int i;

	if (argc != 2) {
		printf("a.out file\n");
		printf("	file : input file for efsm.\n");
		return 1;
	}
	Initial();
	yyin = fopen("efsm.in1", "r");
	passCounter = 0;
	error = yyparse();
	if (error == 1) {
		printf("Fail for parsing 1\n");
		return 1;
	}
	efsm.InitialCbtRun();
	efsm.Print();
	printf("Success for parsing 1\n");
	fseek(yyin, 0, 0); // reset yyin to begin of file.
	passCounter++;
	out = fopen("test.h", "w");
	error = yyparse();
	fclose(yyin);
	if (error == 0) {
		printf("Success for parsing 2\n");
		GenerateCode(out);
	}
	else {
		printf("Fail for parsing 2\n");
	}
	fclose(out);
	return 0;
}

#line 640 "y.tab.h"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 4:
#line 297 "efsm.y"
{
		if (passCounter == 0) {
			efsm.AddState(yyvsp[0]);
		}
	}
break;
case 5:
#line 303 "efsm.y"
{
		if (passCounter == 0) {
			efsm.AddState(yyvsp[0]);
		}
		else if (passCounter == 1) {
			strcpy(currStateId, yyvsp[0]);
		}
		IdL_Clear(&idList);
	}
break;
case 7:
#line 317 "efsm.y"
{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
break;
case 8:
#line 323 "efsm.y"
{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
break;
case 10:
#line 330 "efsm.y"
{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
break;
case 13:
#line 344 "efsm.y"
{	if (passCounter == 0) {
			efsm.SetNextSiblingIdx(&idList);
		}
	}
break;
case 14:
#line 352 "efsm.y"
{
		if (passCounter == 1) {	
			(TIdList*)(yyval) = &idList;
		}
	}
break;
case 15:
#line 359 "efsm.y"
{
		if (passCounter == 1) {	
			(TIdList*)(yyval) = &idList;
		}
	}
break;
case 17:
#line 371 "efsm.y"
{ IdL_AddId(&idList, yyvsp[0]);	}
break;
case 18:
#line 372 "efsm.y"
{ IdL_AddId(&idList, yyvsp[0]);	}
break;
case 21:
#line 382 "efsm.y"
{	IdL_Clear(&idList);	}
break;
case 22:
#line 384 "efsm.y"
{	
		if (passCounter == 1) {	IdL_Copy(&idList1, &idList);	IdL_Clear(&idList);	}
	}
break;
case 23:
#line 388 "efsm.y"
{
		if (passCounter == 1) {	IdL_Copy(&idList2, &idList);	IdL_Clear(&idList);
			RunCodeGen(yyvsp[-7], yyvsp[-6], &idList1, &idList2, (int)atoi(yyvsp[-8]), currStateId);
		}
	}
break;
case 24:
#line 396 "efsm.y"
{ IdL_AddId(&idList, yyvsp[0]);	}
break;
case 25:
#line 397 "efsm.y"
{ IdL_AddId(&idList, yyvsp[0]);	}
break;
#line 936 "y.tab.h"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
