#include "station.h"
#include "item.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern ItemArray items;

// _key : When keypad input "UP" meaning station 1,
//									"POWER" & "LEFT" meaning station 6
const char* stationKey[MAX_NR_STATIONS] = { "", "UP", "LEFT", "DOWN", "RIGHT",
	"POWER+UP", "POWER+LEFT", "POWER+DOWN", "POWER+RIGHT",
	"MENU+UP", "MENU+LEFT", "MENU+DOWN", "MENU+RIGHT",
};

char stationsDesc[2000];
int stationsDescLen;

int Station::Id() {
	return _id;
}

char* Station::Description() {
	return _desc;
}

int Station::ItemsNr() {
	return _itemsNr;
}

int Station::ArgNr() {
	return _argNr;
}

unsigned char Station::Item(int idx) {
	return _item[idx];
}

Arg Station::GetArg(int x) {
	return _arg[x];
}

void Station::Reset() {
	int i;
	
	_id = 0;	
	_desc[0] = '\0';
	_itemsNr = 0;
	_argNr = 0;
/*	// NR_ITEMS_IN_STATION : Number of Items in one station
	for (i = 0; i < NR_ITEMS_IN_STATION; i++) {
		_item[i] = NO_ITEM;	//255 for unsigned char _item[]
	}*/
}

void Station::SetId(int id) {	
	_id = id;
}

void Station::SetDescription(char* desc) {
	strcpy(_desc, desc);
}

// add a ti to _ti[]
void Station::AddItem(unsigned char item) {
	_item[_itemsNr] = item;
	_itemsNr++;
}

void Station::AddDelay(unsigned long x) {
	AddItem(253);
	_arg[_argNr].type = DELAY_TYPE;
	_arg[_argNr].data.delay = x;
	_argNr++;
}

void Station::AddWaitKeyIn(char* x) {
	AddItem(254);
	_arg[_argNr].type = KEY_TYPE;
	strcpy(_arg[_argNr].data.key, x);
	_argNr++;
}

void Station::Print() {
	int i;
	char type[20];
	
	printf("_id = %d, _desc = %s, _itemsNr = %d\n", _id, _desc, _itemsNr);
	printf("_item = { ");
	for (i = 0; i < _itemsNr; i++) {
		printf("%d, ", _item[i]);
	}
	printf("}\n");
}

void StationArray::Reset() {
	int i;
	
	for (i = 0; i < MAX_NR_STATIONS; i++) {
		_exist[i] = false;
	}
}

bool StationArray::Exist(int i) {
	return _exist[i];
}

Station* StationArray::GetStation(int i) {
	return &_station[i];
}

void StationArray::AddStation(Station* station) {
	int id = station->Id();
	_station[id] = *station;
	_exist[id] = EXIST;
}

void StationArray::OutputErrCodeList(FILE* out) {
	int i, j;
	char *itemName;
	
	for (i = 1; i < MAX_NR_STATIONS; i++) {
		if (_exist[i]) {
			// output station information to out which is file "error_list.txt"
			fprintf(out, "\n- STATION %d -\n", i);
			if (strlen(_station[i].Description()) > 0) {
				fprintf(out, "DESCRIPTION : %s\n", _station[i].Description());
			}
			fprintf(out, "(0000)   No Error\n");
			int itemNr = 0;
			for (j = 0; j < _station[i].ItemsNr(); j++) {
				itemName = items[_station[i].Item(j)]->name;
				if (items[_station[i].Item(j)]->type == TEST_ITEM_TYPE) {
					fprintf(out, "(%.4x) %3d:%s\n", (1 << itemNr), _station[i].Item(j), itemName);
					itemNr++;
				}
			}
		}
	}
}

void StationArray::OutputStationList(FILE* out) {
	int i, j;
	char *itemName;
	
	for (i = 1; i < MAX_NR_STATIONS; i++) {
		if (_exist[i]) {
			// output station information to out which is file "station_list.txt"
			fprintf(out, "\n- STATION %d -\n", i);
			if (strlen(_station[i].Description()) > 0) {
				fprintf(out, "DESCRIPTION : %s\n", _station[i].Description());
			}
			fprintf(out, "(0000)   No Error\n");
			int itemNr = 0, argCnt = 0;
			for (j = 0; j < _station[i].ItemsNr(); j++) {
				itemName = items[_station[i].Item(j)]->name;
				if (items[_station[i].Item(j)]->type == TEST_ITEM_TYPE) {
					fprintf(out, "(%.4x) %3d:%s\n", (1 << itemNr), _station[i].Item(j), itemName);
					itemNr++;
				}
				else if (items[_station[i].Item(j)]->type == ACTION_ITEM_TYPE) {
					fprintf(out, "       %3d:%s\n", _station[i].Item(j), itemName);
				}
				else if (items[_station[i].Item(j)]->type == CONTROL_ITEM_TYPE) {
					Arg arg = _station[i].GetArg(argCnt);
					if (items[_station[i].Item(j)]->id == ITEM_WAIT_KEYIN) {
						if (arg.type == KEY_TYPE) {
							fprintf(out, "       %3d:wait_keyin(%s)\n", _station[i].Item(j), arg.data.key);
							argCnt++;
						}
						else {
							printf("Error in OutputStationList!!!\n");
							exit(1);
						}
					}
					else if (items[_station[i].Item(j)]->id == ITEM_DELAY_100MS) {
						if (arg.type == DELAY_TYPE) {
							fprintf(out, "       %3d:delay_100ms(%d)\n", _station[i].Item(j), arg.data.delay);
							argCnt++;
						}
						else {
							printf("Error in OutputStationList!!!\n");
							exit(1);
						}
					}
				}
			}
		}
	}
}

void StationArray::OutputCreateStation(FILE* out, char *leadingChar) {
	int i, j;
	char str[1000];
	char *p;
	Arg arg;
	
	for (i = 1; i < MAX_NR_STATIONS; i++) {
		if (_exist[i]) {
			fprintf(out, "%sstation[%d].SetId(%d);\n", leadingChar, i, i);
			
			fprintf(out, "%s#ifdef CONFIG_DISPLAY_STATION_INFO\n", leadingChar);
			fprintf(out, "%sstation[%d].SetDesc(p);\n", leadingChar, i);
			fprintf(out, "%sp += strlen(p) + 1;\n", leadingChar);
			fprintf(out, "%s#endif // CONFIG_DISPLAY_STATION_INFO\n\n", leadingChar);

			char tmp[500];
			char *p = tmp;
			for (j = 0; j < _station[i].ItemsNr(); j++) {
				sprintf(p, "%d, ", _station[i].Item(j));
				p += strlen(p);
			}
			sprintf(p, "%d", NO_ITEM);
			fprintf(out, "%sU8 itemList%d[NR_ITEMS] = {%s};\n", leadingChar, i, tmp);
			fprintf(out, "%sstation[%d].SetItemList(itemList%d);\n", leadingChar, i, i);

			if (_station[i].ArgNr() > 0) {
				str[0] = '\0';
				for (j = 0; j < _station[i].ArgNr() - 1; j++) {
					arg = _station[i].GetArg(j);
					if (arg.type == KEY_TYPE) {
						sprintf(str+strlen(str), "%s, ", arg.data.key);
					}
					else if (arg.type == DELAY_TYPE) {
						sprintf(str+strlen(str), "%d, ", arg.data.delay);
					}
				}
				arg = _station[i].GetArg(j);
				if (arg.type == KEY_TYPE) {
					sprintf(str+strlen(str), "%s", arg.data.key);
				}
				else if (arg.type == DELAY_TYPE) {
					sprintf(str+strlen(str), "%d", arg.data.delay);
				}
				fprintf(out, "%sU32 argList%d[NR_ARGLIST] = {%s};\n", leadingChar, i, str);
				fprintf(out, "%sstation[%d].SetArgList(argList%d, %d);\n", leadingChar, i, i, _station[i].ArgNr());
			}

			fprintf(out, "\n");
		}
	}
}

void StationArray::Print() {
	int i;
	char exist[10];
	
	for (i = 0; i < MAX_NR_STATIONS; i++) {
		if (_exist[i]) {
			_exist[i] ? strcpy(exist, "EXIST") : strcpy(exist, "NOTEXIST");
			printf("_exist[%d] = %s\n", i, exist); 
			printf("station[%d] =\n", i);
			_station[i].Print();
		}
	}
}

// station : keep test item list in station
// For example,
//	when like to keep station 1 test items information, the station will be
//	station.id = 1, station.tiSize = test items size for station 1,
//	station.ti[0] = 1st test item number in station 1,
//	station.ti[1] = 2nd test item number in station 1,...

Station station;

StationArray stations;
