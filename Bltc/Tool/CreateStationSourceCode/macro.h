#ifndef _MACRO_H
#define _MACRO_H

#include <stdio.h>

class Macro {
private:
	struct Entry {
		char	name[129];
		char	body[500];
	};
	Entry _entry[100];
	int _size;
public:
	void Reset();
	int Find(char* name);
	bool Add(char* name, char* body);
	bool Expand(char* name, FILE* out);
};

#endif // _MACRO_H
