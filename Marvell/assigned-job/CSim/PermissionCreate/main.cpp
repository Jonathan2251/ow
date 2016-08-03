#include "c_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C" FILE *xxin;
extern "C" FILE *yyin;

extern "C" int xxparse();
extern "C" int yyparse();

void Init()
{
  token.idx = 0;
}

int main(int argc, char **argv)
{
	int error;

	if (argc != 2) {
		printf("example:\n");
		printf("	pc permission.def\n	permission.def : input script file for pc.\n");
		printf("	After run, it generate permission_codegen.cpp and permission_codegen.h\n");
		return 1;
	}

	xxin = fopen(argv[1], "r");
	if (xxin == NULL) {
		printf("cannot open file %s\n", argv[1]);
		exit(0);
	}
	outtmp = fopen("tmp.txt", "w");
	if (outtmp == NULL) {
		printf("cannot open file tmp.txt\n");
		return 1;
	}
    Init();
	strcpy(lexer_line.file, argv[1]);
	lexer_line.line = 1;
	error = xxparse();	// trigger pc1.y
	if (error == 1) {
		printf("Fail for parsing %s\n", argv[1]);
		return 1;
	}
	printf("Success for parsing %s\n", argv[1]);
	fclose(outtmp);

	outcpp = fopen("permission_codegen.cpp", "w");
	outh = fopen("permission_codegen.h", "w");
	yyin = fopen("tmp.txt", "r");
	if (yyin == NULL) {
		printf("cannot open file %s\n", argv[1]);
		goto err_exit;
	}
	strcpy(lexer_line.file, "tmp.txt");
	lexer_line.line = 1;
	error = yyparse();	// trigger pc2.y
	if (error == 1) {
		printf("Fail for parsing %s\n", "tmp.txt");
		goto err_exit;
	}
	printf("Success for parsing %s\n", "tmp.txt");
	printf("\nPlease check tmp.txt, permission_codegen.cpp and permission_codegen.h\n");
	
	fclose(outcpp);
	fclose(outh);
	fclose(yyin);
	
	return 0;
	
err_exit:
	fclose(outcpp);
	fclose(outh);
	fclose(yyin);

	return 1;
}
