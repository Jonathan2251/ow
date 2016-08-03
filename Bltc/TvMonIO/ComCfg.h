#ifndef COMCFG_H
#define COMCFG_H

#include "TvMonIO.h"

#define CONFIG_BUILD_BLTC	
#define CONFIG_BUILD_STATION

#define CONFIG_WITH_TVMON
// Follows can be defined only when CONFIG_WITH_TVMON defined
//#define CONFIG_TI10_USE_CMD
//#define CONFIG_TI22_USE_CMD
//#define CONFIG_TI27_USE_CMD
//#define CONFIG_TI29_USE_CMD
//#define CONFIG_TI31_USE_CMD
//#define CONFIG_TI42_USE_CMD
//#define CONFIG_TI43_USE_CMD
//#define CONFIG_TI46_USE_CMD
//#define CONFIG_TI46_USE_CMD
//#define CONFIG_TI48_USE_CMD
//#define CONFIG_TI56_USE_CMD
//#define CONFIG_TI57_USE_CMD
//#define CONFIG_TI60_USE_CMD
//#define CONFIG_MCARD_USE_CMD
//#define CONFIG_TI87_USE_CMD
//#define CONFIG_TI88_USE_CMD
//#define CONFIG_TI90_USE_CMD
//#define CONFIG_TI92_USE_CMD
//#define CONFIG_TI93_USE_CMD
//#define CONFIG_TI94_USE_CMD
//#define CONFIG_TI95_USE_CMD
//#define CONFIG_DIGITAL_AVTEST_PIP_USE_CMD
//#define CONFIG_TI103_USE_CMD
//#define CONFIG_TI104_USE_CMD
//#define CONFIG_TI105_USE_CMD
//#define CONFIG_TI106_USE_CMD
//#define CONFIG_TI107_USE_CMD
//#define CONFIG_TI108_USE_CMD
//#define CONFIG_TI109_USE_CMD
//#define CONFIG_TI110_USE_CMD
//#define CONFIG_TI111_USE_CMD
//#define CONFIG_TI112_USE_CMD
//#define CONFIG_TI113_USE_CMD
//#define CONFIG_TI114_USE_CMD
//#define CONFIG_AI141_USE_CMD

//#define CONFIG_IMPROVE_LINE_RATE

//#define CONFIG_WITH_TI75

#define CONFIG_DEBUG

#define CONFIG_PSOC_VERSION		0xE7	// for PPR2 of DCH3416P5

#define CONFIG_BCM7038_C5	// for EPR1


#define CONFIG_DISPLAY_STATION_INFO	// Display all items, station_list.txt error_list.txt on rs232
#define CONFIG_INPUT_STATION_FROM_RS232	// For debug. Input station test items from rs232

#endif
