#include "c_interface.h"
#include "macro.h"
#include "item.h"
#include "station.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern Macro macro;
extern ItemArray items;
extern Station station;
extern StationArray stations;
extern Model model;
extern ModelArray models;

extern "C" {	// All variables here using C linkage method.
	
FILE *out;	// define (instance) out variable here, process.y reference this.
char cVars[10000];
fileloc lexer_line;

int macro_find(char* name, int size) {
	char tmp[129];
	int i;
	
	memcpy(tmp, name, size);
	tmp[size] = '\0';
	i = macro.Find(tmp);
	return i;
}

void macro_add(char* s, int size) {
	char tmp[129];
	char name[129];
	char body[500];
	char* p;
	
	memcpy(tmp, s, size);
	tmp[size] = '\0';
	sscanf(tmp, "%s", name);
	for (p = tmp ; *p == ' ' || *p == '\t'; p++);
	for (p += strlen(name); *p == ' ' || *p == '\t'; p++);
	strcpy(body, p);
//	printf("name = %s, body = %s\n", name, body);
	if (macro.Add(name, body)) {
		return;
	}
	else {
		exit(1);
	}
}

void macro_expand(char* name, FILE* out) {
	if (macro.Expand(name, out)) {
		return;
	}
	else {
		exit(1);
	}
}

void items_add(char type, char* id, char* name) {
	// remove \" from "....", ex. "Board ID check" => Board ID check
	name[strlen(name)-1] = '\0';
	unsigned char i = (unsigned char)atoi(id);
	items.Add(type, i, name+1);
}

void items_print() {
	items.Print();
}

void items_dipslay() {
	printf("\nAt this point, items =\n");
	items.Print();
	printf("\nPress anykey to continue =\n");
	getchar();
}

void station_reset() {	
	station.Reset();
}

void station_setId(int id) {	
	station.SetId(id);
}

void station_setDescription(char* desc) {
	char tmp[5000];
	
	strcpy(tmp, desc+1);
	tmp[strlen(tmp)-1] = '\0';
	station.SetDescription(tmp);
}

void station_addItem(char* item) {
	unsigned char i = (unsigned char)atoi(item);
	if (items.Exist(i)) {
		station.AddItem(i);
	}
	else {
		exit(1);
	};
}

void station_addDelay(char* x) {
	unsigned char i = (unsigned char)atoi(x);
	station.AddDelay(i);
}

void station_addWaitKeyIn(char* x) {
	station.AddWaitKeyIn(x);
}

void station_dipslay() {
	printf("\nAt this point, station =\n");
	station.Print();
	printf("\nPress anykey to continue =\n");
	getchar();
}

void stations_reset() {
	stations.Reset();
}

void stations_addStation() {
	stations.AddStation(&station);
}

void stations_display() {
	printf("\nAt this point, stations =\n");
	stations.Print();
	printf("\nPress anykey to continue =\n");
	getchar();
}

void model_setName(char *name) {
	model.SetName(name);
}

void model_addStations() {
	model.AddStations(&stations);
}

void model_reset() {
	model.Reset();
}

void models_addModel() {
	models.AddModel(&model);
	model.Reset();
}

void models_display() {
	printf("\nAt this point, models =\n");
	models.Print();
	printf("\nPress anykey to continue =\n");
	getchar();
}

int CopyFromFile(char *inName, FILE *out) {
	FILE *in;
	char inName1[1000];
	char buf[1000];
	
	strcpy(inName1, inName+1);
	inName1[strlen(inName1)-1] = '\0';
	in = fopen(inName1, "r");
	if (in == NULL) {
		printf("\ncannot open file %s\n", inName1);
		return 1;
	}
	while (!feof(in)) {
		int n = fread(buf, sizeof(char), 512, in);
		//printf("n = %d\n", n);
		fwrite(buf, sizeof(char), n, out);
	}
	fclose(in);
	return 0;
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

} // extern "C"
