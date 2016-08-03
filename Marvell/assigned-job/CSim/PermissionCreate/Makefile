# System register function CheckPermission_xxx() generate.

sources = main.cpp pc1.c pc2.c c_interface.cpp
objects = main.o pc1.o pc2.o c_interface.o

all: $(sources)
	gcc -o pc $(sources)

pc1.tab.h: pc1.y
	byacc -p xx -o pc1.tab.h -v pc1.y
	
pc1.c: pc1.lex pc1.tab.h
	flex -Pxx -opc1.c pc1.lex

pc2.tab.h: pc2.y
	byacc -o pc2.tab.h -v pc2.y

pc2.c: pc2.lex pc2.tab.h
	flex -opc2.c pc2.lex

.PHONY : clean
clean:
	rm -f pc1.c pc1.tab.h pc1.tab.h.output pc2.c pc2.tab.h pc2.tab.h.output y.output tmp.txt permission_codegen.cpp permission_codegen.h *~ *.o pc
