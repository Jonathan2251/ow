yacc -o y.tab.h -v efsm.y
lex efsm.lex
cc lex.yy.c -lm