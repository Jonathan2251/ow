
sources = main.cpp preprocess.c cs.c macro.cpp item.cpp station.cpp model.cpp c_interface.cpp
objects = main.o preprocess.o cs.o macro.o item.o station.o model.o c_interface.o

all: $(sources)
	cl /o run/createstation.exe $(sources)
	cl /DDEBUG /o doc/createstation.exe $(sources)

preprocess.tab.h: preprocess.y
	byacc -p xx -o preprocess.tab.h -v preprocess.y
	
preprocess.c: preprocess.lex preprocess.tab.h
	flex -Pxx -opreprocess.c preprocess.lex

cs.tab.h: cs.y
	byacc -o cs.tab.h -v cs.y

cs.c: cs.lex cs.tab.h
	flex -ocs.c cs.lex

.PHONY : clean
clean:
	rm -f preprocess.tab.h.output preprocess.tab.h preprocess.c tmp.txt
	rm -f cs.c cs.tab.h cs.tab.h.output run/station_list.txt run/error_list.txt run/tmp.txt *.bak *.obj *.o cs.out run/CreateStation.cpp run/createstation.exe
	rm -f main.ncb main.opt main.plg
#	rm -R Debug
	rm -f doc/CreateStation.cpp doc/station_list.txt doc/error_list.txt doc/createstation.exe
