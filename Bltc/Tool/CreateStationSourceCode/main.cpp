#include "macro.h"
#include "item.h"
#include "station.h"
#include "model.h"
#include "c_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern Macro macro;
extern ItemArray items;
extern Station station;
extern StationArray stations;
extern ModelArray models;

extern "C" FILE *xxin;
extern "C" FILE *yyin;
FILE *out1;
FILE *out2;
FILE *out3;

extern "C" int xxparse();
extern "C" int yyparse();

void init() {
	macro.Reset();
	items.Init();
	// After items.Init() the tis(class TestItemArray) object contents are
	//	_ti[0..255].id = 0,	_ti[0.255].name = "";
	//	_exist[0.255] = NOTEXIST
	
	station.Reset();
	// After station.Reset() the station(class Station) object contents are
	//	_id = 0;	
	//	_type = 0;
	//	_itemsNr = 0;
	
	stations.Reset();
	// After stations.Reset() the stations(class StationArray) object contents are
	//	_exist[0..16] = NOTEXIST;
	
	models.Reset();
}

/*
For exmpale, suppose the file "stationscr.txt" as follows
TESTITEM_DECLARATION
{
1       "Front panel LED test"
2       "Front panel keypad (manual) test (press power to exit)"
4       "CPU catch memory test"
5       "BCM3250 SDRAM Long Read/Write test"
6       "DMA from SDRAM to PCI (3250 memory)"
7       "DMA from PCI (3250 memory) to SDRAM"
}

STATION 01 WITHOUT_SIE
ITEM 01 02

STATION 02 WITHOUT_SIE
ITEM 05 06 07

And assume argv[1] == Tsmscr.txt, then after this be run it will create file "tsm_list.txt" which contents are,
--- TEST ERROR CODE LIST ---

- STATION 1 (UP) WITHOUT_SIE-
(0000)   No Error
(0001)   1:Front panel LED test
(0002)   2:Front panel keypad (manual) test (press power to exit)

- STATION 2 (LEFT) WITHOUT_SIE-
(0000)   No Error
(0001)   5:BCM3250 SDRAM Long Read/Write test
(0002)   6:DMA from SDRAM to PCI (3250 memory)
(0004)   7:DMA from PCI (3250 memory) to SDRAM

Explain:
	1. After 
		TESTITEM_DECLARATION
		{
		1       "Front panel LED test"
		2       "Front panel keypad (manual) test (press power to exit)"
		4       "CPU catch memory test"
		5       "BCM3250 SDRAM Long Read/Write test"
		6       "DMA from SDRAM to PCI (3250 memory)"
		7       "DMA from PCI (3250 memory) to SDRAM"
		}
	   be read the tis object (class TestItemArray) is
	//	_item[0].id = 0,	_item[0].name = '\0', _exist[0] = NOTEXIST
	//	_item[1].id = 1,	_item[1].name = "Front panel LED test", _exist[1] = EXIST
	//	_item[2].id = 2,	_item[1].name = "Front panel keypad (manual) test (press power to exit)", _exist[2] = EXIST
	//	_item[3].id = 0,	_item[3].name = '\0', _exist[3] = NOTEXIST
	//	_item[4].id = 4,	_item[4].name = "CPU catch memory test", _exist[4] = EXIST
	//	_item[5].id = 5,	_item[5].name = "BCM3250 SDRAM Long Read/Write test", _exist[5] = EXIST
	//	_item[6].id = 6,	_item[6].name = "DMA from SDRAM to PCI (3250 memory)", _exist[6] = EXIST
	//	_item[7].id = 7,	_item[7].name = "DMA from PCI (3250 memory) to SDRAM", _exist[7] = EXIST

	2. After 
		STATION 01 WITHOUT_SIE
		TI 01 02
	   be read 
	   
	   the station object content is
	//	_id = 1;	
	//	_type = WITHOUT_SIE;
	//	_itemsNr = 2;
	//	_item[0] = 01, _item[1] = 02, _item[2..15] = NO_ITEM;
	   the stations object content is
	//	_station[0] = 0s, _exist[0] = NOTEXIST
	//	_station[1] = station
	//	_station[2..16] = 0s, _exist[2..16] = NOTEXIST
	
	3. After 
		STATION 02 WITHOUT_SIE
		TI 05 06 07
	   be read 
	   
	   the station object content is
	//	_id = 2;	
	//	_type = WITHOUT_SIE;
	//	_itemsNr = 3;
	//	_item[0] = 05, _item[1] = 06, _item[2] = 07, _item[3..15] = 0xff;
	   the stations object content is
	//	_station[0] = 0s, _exist[0] = NOTEXIST
	//	_station[1] = 
	//		_id = 1, _type = WITHOUT_SIE, _tiNr = 2;
	//		_item[0] = 01, _item[1] = 02, _item[2..15] = NO_ITEM;
	//	_station[2] = station
	//	_station[3..16] = 0s, _exist[2..16] = NOTEXIST
*/

int main(int argc, char **argv)
{
	int error;

	if (argc != 2) {
		printf("example:\n");
		printf("	createstation stationscr.txt\n	stationscr.txt : input script file for createstation.\n");
		return 1;
	}
	init();
	xxin = fopen(argv[1], "r");
	if (xxin == NULL) {
		printf("cannot open file %s\n", argv[1]);
		exit(0);
	}
	out = fopen("tmp.txt", "w");
	if (out == NULL) {
		printf("cannot open file tmp.txt\n");
		return 1;
	}
	strcpy(lexer_line.file, argv[1]);
	lexer_line.line = 1;
	error = xxparse();	// trigger preprocess.y
	if (error == 1) {
		printf("Fail for parsing %s\n", argv[1]);
		return 1;
	}
	printf("Success for parsing %s\n", argv[1]);
	fclose(out);
	
	yyin = fopen("tmp.txt", "r");
	if (yyin == NULL) {
		printf("cannot open file tmp.txt\n");
		goto err_exit;
	}
	out1 = fopen("station_list.txt", "w");
	if (out1 == NULL) {
		printf("cannot open file station_list.txt\n");
		goto err_exit;
	}
	out2 = fopen("CreateStation.cpp", "w");
	if (out2 == NULL) {
		printf("cannot open file CreateStation.cpp\n");
		goto err_exit;
	}
	out3 = fopen("error_list.txt", "w");
	if (out1 == NULL) {
		printf("cannot open file error_list.txt\n");
		goto err_exit;
	}
	strcpy(lexer_line.file, "tmp.txt");
	lexer_line.line = 1;
	error = yyparse();	// trigger cs.y
	if (error == 1) {
		printf("Fail for parsing tmp.txt\n");
		goto err_exit;
	}
	printf("Success for parsing tmp.txt\n");
	
	models.OutputStationList(out1);
	models.OutputCreateStation(out2);
	models.OutputErrCodeList(out3);
	
	fclose(yyin);
	fclose(out1);
	fclose(out2);
	fclose(out3);
	
	return 0;
	
err_exit:
	fclose(yyin);
	fclose(out1);
	fclose(out2);
	fclose(out3);
	
	system("del station_list.txt error_list.txt CreateStation.cpp");
	return 1;
}
