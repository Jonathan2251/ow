#ifndef _C_INTERFACE_H
#define _C_INTERFACE_H

#include <stdio.h>

#define YYSTYPE char*

struct Define {
	char symbol[128];
	char replace[1000];
};

/* A file position, mostly for error messages.  
   The FILE element may be compared using pointer equality.  */
struct fileloc {
  char file[256];
  int line;
};

#define TEST_ITEM_TYPE		1
#define ACTION_ITEM_TYPE	2
#define CONTROL_ITEM_TYPE	3


#ifdef	__cplusplus
extern "C" {
#endif

extern FILE *out;
extern char cVars[10000];
extern struct fileloc lexer_line;

extern int macro_find(char* name, int size);
extern void macro_expand(char* name, FILE* out);
extern void items_add(char type, char* id, char* name);
extern void items_print();
extern void station_reset();
extern void items_dipslay();
extern void station_reset();
extern void station_setId(int id);
extern void station_setDescription(char* desc);
extern void station_addItem(char* item);
extern void station_addDelay(char* x);
extern void station_addWaitKeyIn(char* x);
extern void station_dipslay();
extern void stations_reset();
extern void stations_addStation();
extern void stations_display();
extern void model_setName(char *name);
extern void model_addStations();
extern void model_reset();
extern void models_addModel();
extern void models_display();

extern int CopyFromFile(char *inName, FILE *out);
extern void error_at_line (struct fileloc *pos, const char *msg, ...);
extern void update_lineno (const char *l, size_t len);
extern int yyerror(char *s);

#ifdef	__cplusplus
}
#endif

#endif	// _C_INTERFACE_H