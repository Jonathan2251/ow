#ifndef _ITEM_H
#define _ITEM_H

#include "c_interface.h"

#define EXIST		0x01
#define NOTEXIST	0x00


#define NR_ITEMS	256	// Number of all Items
#define NR_REAL_TESTITEMS	253	// Number of Real Test Items 0..252. 253, 254 are for control items.
#define ITEM_DELAY_100MS	253	// Control Item for Delay_ms(x*100);
#define ITEM_WAIT_KEYIN	254	// Control Item for waitKeyIn(KEY)
#define NO_ITEM			255

struct Item {
	char			type;
	unsigned char	id;
	char			name[500];
};

class ItemArray {
private:
// _item[] : Name string in "TI_DECLARATION" or "AI_DECLARATION" section
// for example :
// 		_item[0] = {0, "Front panel LED test"}
// 		_item[1] = {1, "Front panel keypad (manual) test (press power to exit)"}
// Please refer to "Tsmscr.txt"
	Item	_item[NR_ITEMS];
	bool 	_exist[NR_ITEMS];
public:
	Item* operator[](unsigned char iId);
	bool Exist(unsigned char iId);
	void Add(char type, unsigned char id, char* name);
	void Print();
	void Init();
};

#endif	// _ITEM_H
