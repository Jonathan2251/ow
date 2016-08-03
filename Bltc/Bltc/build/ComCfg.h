#ifndef COMCFG_H
#define COMCFG_H

#define CONFIG_BUILD_BLTC	
#define CONFIG_BUILD_STATION

#define CONFIG_UI_STATION_TEST
#define CONFIG_UI_ALL_IN_ONE_TEST

//#define CONFIG_BCM7400D0_BEFORE
#define CONFIG_ENABLE_TIMER_INTERRUPT

#define CONFIG_WITH_TVMON
#define CONFIG_WITH_RS232
#define CONFIG_WITH_KEYPAD
#define CONFIG_WITH_IR
#define CONFIG_WITH_LED
#define CONFIG_WITH_OSD
#define CONFIG_MUTI_INPUT
#define CONFIG_WITH_TUNER_TEST

//#define CONFIG_DELAY_TRACE	// For CPU and IO overlay analsis. Around 8 seconds for station 1 to 4.

#define CONFIG_FLASH_ID	0x2301
#define BCM3517_REV 	"B0"
#define BCM7400_REV_C0 0x74000020 //for prototype
#define BCM7400_REV_D0 0x74000030 //for EPR1 later
#define BCM7400_REV_D1 0x74000031 //for EPR1 20071226 later
#define BCM7400_REV_D2 0x74000032 //for PPR2 20080324
#define BCM7400_REV_E0 0x74000040 //for Proto 3 of QIP7100P2 20090406
/*C0 = 0x7400 0020
  D0 = 0x7400 0030

  BCM7400 "K" part:
  - JTAG Locked
  - PCI Client Mastering Unlocked 
  
  BCM7400 "TBD" part:
  - JTAG Unlocked
  - PCI Client Mastering Unlocked
*///Ref. by DCX_ProdSWImpacts_ENB-5350_x3_WIP_101707.doc

//#define PSoC_FW_Version 0x07
//#define PSoC_FW_Version 0x09	// For PPR1
#define PSoC_FW_Version 0x0A	// For PPR2 20080324

#define IEEE1394Chip_VendorId 0x8101104C


// Follows can be defined only when CONFIG_WITH_TVMON defined

//#define CONFIG_IMPROVE_LINE_RATE

//#define CONFIG_WITH_TI75

#define CONFIG_DEBUG

#define CONFIG_PSOC_VERSION		0xE7	// for PPR2 of DCH3416P5

#define CONFIG_BCM7038_C5	// for EPR1


#define CONFIG_DISPLAY_STATION_INFO	// Display all items, station_list.txt error_list.txt on rs232
#define CONFIG_INPUT_STATION_FROM_RS232	// For debug. Input station test items from rs232

#endif
