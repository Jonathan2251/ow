// author :	Gamma Chen

#ifndef _CREATE_STATION_H_
#define _CREATE_STATION_H_

#include "Item.h"
#include "Items.h"
#include "Station.h"
#include "TiLib_Keypad.h"
#include "ModelConfig.h"

#define MAX_NR_STATIONS		17

extern Item gItem[256];
extern Station station[MAX_NR_STATIONS];
extern void StationsInit();
extern bool IsProductModel(char *model);


#endif