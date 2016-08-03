#ifndef _STATION_H
#define _STATION_H

#include <stdio.h>

#define STATION_TYPE_WITH_SIE		0x01
#define STATION_TYPE_WITHOUT_SIE	0x00

#define KEY_TYPE	1
#define DELAY_TYPE	2

#define MAX_NR_STATIONS	17		// Number of stations 0..16
#define NR_ITEMS_IN_STATION	32	// Number of Items in one station
#define NR_TESTITEMS_IN_STATION	16	// Number of TestItems in one station
#define NR_DELAYLIST_IN_STATION		2*NR_TESTITEMS_IN_STATION	// Number of DelayList in one station
#define NR_KEYLIST_IN_STATION		2*NR_TESTITEMS_IN_STATION	// Number of waitKeyInList in one station

struct Arg {
	char type;
	union Data {
		char key[100];
		unsigned long delay;
	}data;
};

class Station {
private:
	int 	_id;
	char	_desc[1000];	// offset value in model._desc[], decsription for this station
	int		_itemsNr;
	int		_argNr;
	unsigned char	_item[NR_ITEMS_IN_STATION];
	Arg		_arg[NR_TESTITEMS_IN_STATION];
public:
	int Id();
	char* Description();
	int ItemsNr();
	int ArgNr();
	unsigned char Item(int idx);
	Arg GetArg(int x);
	void Reset();
	void SetId(int id);
	void SetDescription(char* desc);
	void AddItem(unsigned char item);
	void AddDelay(unsigned long x);
	void AddWaitKeyIn(char* x);
	void Print();
};

class StationArray {
private:
	Station _station[MAX_NR_STATIONS];
	bool 	_exist[MAX_NR_STATIONS];
	
public:
	void Reset();
	bool Exist(int i);
	Station* GetStation(int i);
	void AddStation(Station* station);
	void OutputErrCodeList(FILE* out);
	void OutputStationList(FILE* out);
	void OutputCreateStation(FILE* out, char *leadingChar);
	void Print();
};

#endif	// _STATION_H
