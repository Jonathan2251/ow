#include "item.h"
#include <stdio.h>
#include <string.h>

Item* ItemArray::operator[](unsigned char iId) {
	return &_item[iId];
}

bool ItemArray::Exist(unsigned char iId) {
	return _exist[iId];
}

void ItemArray::Add(char type, unsigned char id, char* name) {
	_exist[id] = EXIST;
	_item[id].type = type;
	_item[id].id = id;
	strcpy(_item[id].name, name);
}

// Print to console window, for debug & trace.
void ItemArray::Print() {
	int i = 0;
	char sType[30];
	
	for (i = 0; i < NR_ITEMS; i++) {
		if (_exist[i] == EXIST) {
			if (_item[i].type == TEST_ITEM_TYPE) {
				strcpy(sType, "TEST_ITEM_TYPE");
			}
			else if (_item[i].type == ACTION_ITEM_TYPE) {
				strcpy(sType, "ACTION_ITEM_TYPE");
			}
			else if (_item[i].type == CONTROL_ITEM_TYPE) {
				strcpy(sType, "CONTROL_ITEM_TYPE");
			}
			printf("%d %s %s\n", i, sType, _item[i].name);
		}
	}
}

void ItemArray::Init() {
	int i = 0;
	
	for (i = 0; i < NR_ITEMS; i++) {
		_exist[i] = NOTEXIST;
		_item[i].type = 0;
		_item[i].id = 0;
		_item[i].name[0] = '\0';
	}
}

ItemArray items;
