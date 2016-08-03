#include "macro.h"

#include <stdlib.h>
#include <string.h>

int Macro::Find(char* name) {
	for (int i = 0; i < _size; i++) {
		if (strcmp(_entry[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void Macro::Reset() {
	_size = 0;
}

bool Macro::Add(char* name, char* body) {

	if (Find(name) >= 0) {
		printf("Macro::Add() %s exist\n", name);
		return false;
	}
	strcpy(_entry[_size].name, name);
	strcpy(_entry[_size++].body, body);
//	printf("name = %s, body = %s\n", _entry[_size-1].name, _entry[_size-1].body);
	
	return true;
}

bool Macro::Expand(char* name, FILE* out) {
	int i = Find(name);
	if (i >= 0) {
		fprintf(out, "%s", _entry[i].body);
		return true;
	}
	else {
		return false;
	}
}

Macro macro;
