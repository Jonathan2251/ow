/* BNF for Extended Finite State Machine (efsm) */

%{
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
		//action
		//sprintf(tmp, "\t\t\t\tif (%s", condition);
		sprintf(tmp, "\t\t\t\tif (");
		AddEndOfLine(tmp, condition);
		for (j = 1; j < from->size; j++) {
			sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)", j, from->entry[from->size-1-j]);
		}
		sprintf(tmp + strlen(tmp), " && curr->CheckParent(%d, %s)) {\n", j, stateId);
		//sprintf(tmp + strlen(tmp), "\t\t\t\t\t%s\t\t\t\t}\n", action);
		sprintf(tmp + strlen(tmp), "\t\t\t\t\t");
		AddEndOfLine(tmp, action);
		sprintf(tmp + strlen(tmp), "\t\t\t\t}\n");
		efsm.state[i].run.action.Add(tmp, i);

		// transferState
		//sprintf(tmp, "\t\t\t\tif (%s", condition);
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

%}

%start input

%token	STATE SUBSTATE THREAD TRANSITION FROM TO CONDITION ACTION ID PRIORITY

%% /* Grammar rules and actions follow */
input:
		  states
;

states:
		  state
		| states state
;

state:
		  STATE ID
	{
		if (passCounter == 0) {
			efsm.AddState($2);
		}
	}
		| STATE ID
	{
		if (passCounter == 0) {
			efsm.AddState($2);
		}
		else if (passCounter == 1) {
			strcpy(currStateId, $2);
		}
		IdL_Clear(&idList);
	}
		  state_body
;

state_body:
		  threads
	{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
		| substates
	{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
		  transitions
		| substates
	{
		if (passCounter == 1) {	
			CbtCodeGen(currStateId, idList.entry[0]);
		}
	}
;

threads:
		  thread
		| threads thread		  
;

thread:  
		  THREAD id_list
	{	if (passCounter == 0) {
			efsm.SetNextSiblingIdx(&idList);
		}
	} 
;

substates:
		  substate
	{
		if (passCounter == 1) {	
			(TIdList*)($$) = &idList;
		}
	}
		|
		  substates substate
	{
		if (passCounter == 1) {	
			(TIdList*)($$) = &idList;
		}
	}
;

substate:
		  SUBSTATE id_list
;

id_list:
		  ID	{ IdL_AddId(&idList, $1);	}
		| id_list ',' ID	{ IdL_AddId(&idList, $3);	}
;

transitions:
		  transition
		| transitions transition
;

transition:
		  TRANSITION PRIORITY CONDITION ACTION FROM
	{	IdL_Clear(&idList);	}
		nested_state_name 
	{	
		if (passCounter == 1) {	IdL_Copy(&idList1, &idList);	IdL_Clear(&idList);	}
	}
		TO nested_state_name 
	{
		if (passCounter == 1) {	IdL_Copy(&idList2, &idList);	IdL_Clear(&idList);
			RunCodeGen($3, $4, &idList1, &idList2, (int)atoi($2), currStateId);
		}
	}
;

nested_state_name:
		  ID	{ IdL_AddId(&idList, $1);	}
		| nested_state_name ':' ID	{ IdL_AddId(&idList, $3);	}
;

%%/* A lexical scanner generated by flex */

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

