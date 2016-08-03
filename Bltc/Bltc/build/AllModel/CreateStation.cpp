#include "CreateStation.h"

#ifdef CONFIG_UI_STATION_TEST
Station station[MAX_NR_STATIONS];
#endif	// CONFIG_UI_STATION_TEST




#ifdef CONFIG_DISPLAY_STATION_INFO
Item gItem[256] = {
{0, TEST_ITEM_TYPE, "Display station, test items and version information on RS232, setting:(38400,8,O,1,n)"},
{1, TEST_ITEM_TYPE, "For debug, Switch to TvMon Command"},
{2, TEST_ITEM_TYPE, "For debug, Switch to our menu"},
{3, TEST_ITEM_TYPE, "For demo, loop forever (add time out solution)"},
{4, ACTION_ITEM_TYPE, "Reserved, for enable timer interrupt"},
{5, ACTION_ITEM_TYPE, "Reserved, for disable timer interrupt"},
{6, TEST_ITEM_TYPE, "For debug, display long error code on RS232"},
{7, TEST_ITEM_TYPE, "For debug, Decorator pattern demo"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{10, TEST_ITEM_TYPE, "For debug, erase PFD entire data area and protect sectors 0 to 7"},
{11, ACTION_ITEM_TYPE, "For debug, Enable pdebug and TvMon layer rs232 output"},
{12, ACTION_ITEM_TYPE, "For debug, Disable pdebug and TvMon layer rs232 output"},
{13, TEST_ITEM_TYPE, "For debug, test OSD"},
{14, TEST_ITEM_TYPE, "For debug, Cache mode change test (without Timeout)"},
{15, TEST_ITEM_TYPE, "For debug, station 4 test"},
{0, NO_ITEM_TYPE, ""},
{17, TEST_ITEM_TYPE, "OTP Lock regardless of initial state of device"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{20, TEST_ITEM_TYPE, "Tuner ID check (QIP7100P2:Tuner1, QIP72xxP2:Tuner1+Tuner2)"},
{21, TEST_ITEM_TYPE, "Board ID check"},
{22, TEST_ITEM_TYPE, "PSOC Version Check"},
{23, TEST_ITEM_TYPE, "Chip ID (BCM3517) access test (by check revision, QIP7100P2:Demod1, QIP72xxP2:Demod1+Demod2)"},
{24, TEST_ITEM_TYPE, "Chip ID BCM7400 access test (by read processor version)"},
{25, TEST_ITEM_TYPE, "Chip ID (SATA IO controller) access test (by read device ID)"},
{26, TEST_ITEM_TYPE, "Chip ID (JEDI 1394) access test (by read vendor ID)"},
{27, TEST_ITEM_TYPE, "Chip ID (Entropic EN2210) access test (by read device ID)"},
{28, TEST_ITEM_TYPE, "OTP Lock"},
{29, TEST_ITEM_TYPE, "1394 OHCI Test"},
{30, TEST_ITEM_TYPE, "OTP Lock Status Check"},
{31, TEST_ITEM_TYPE, "Cache mode change test"},
{32, TEST_ITEM_TYPE, "Verify SDRAM Size = 512MB, and System SDRAM address/data bus (word) test"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{35, TEST_ITEM_TYPE, "DMA from SDRAM to SDRAM"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{38, TEST_ITEM_TYPE, "Switch to MTC i.e. Copy the MTC SUDB to flash"},
{0, NO_ITEM_TYPE, ""},
{40, TEST_ITEM_TYPE, "TC/MTC Sudb CRC check"},
{41, TEST_ITEM_TYPE, "Flash Password Lock and protect sectors 0 to 7"},
{42, TEST_ITEM_TYPE, "Flash check, Device Type = S29GL512N, NumberOfBlocks = 512, BlockSize = 131072, Full Device Id = 7E2301"},
{43, TEST_ITEM_TYPE, "Verify PFD area is cleaned"},
{44, TEST_ITEM_TYPE, "DMA from DDR0 to DDR1"},
{45, TEST_ITEM_TYPE, "DMA from DDR0 to DDR2"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{51, ACTION_ITEM_TYPE, "Ask SIE to do VCXO test"},
{52, TEST_ITEM_TYPE, "Get VCXO test result from SIE"},
{53, TEST_ITEM_TYPE, "BCM3517 OOB receive test (75.25Mhz) (Just Lock)"},
{54, TEST_ITEM_TYPE, "BCM3517 OOB receive test (72.75Mhz) (Just Lock)"},
{55, TEST_ITEM_TYPE, "BCM3517 OOB receive test (125.0Mhz) (Just Lock)"},
{56, TEST_ITEM_TYPE, "Entropic CW Signal (Unmodulated) Test (1500MHz, 0 +/- ??dBm), MOCA test"},
{57, TEST_ITEM_TYPE, "Entropic Continuous Output Signal (Modulated) Test (1475 MHz to 1525, 0 +/- 3dBm), MOCA test"},
{58, TEST_ITEM_TYPE, "Entropic Continuous Output Signal (Modulated) Test (1125 MHz to 1175, 0 +/- 3dBm), MOCA test"},
{59, TEST_ITEM_TYPE, "Start Factory Clocks Test"},
{60, TEST_ITEM_TYPE, "Stop Factory Clocks Test (Back to default state)"},
{61, TEST_ITEM_TYPE, "Tuner 1(256QAM, freq=85000000, vPid=800, audio=801), Tuner 2(64QAM, freq=183000000, vPid=8c0) A/V test, with output interface Composite, Component, RF channel 3, s-video and Audio SPDIF"},
{62, TEST_ITEM_TYPE, "Tuner 2( 64QAM, freq=183000000, vPid=8c0, audio=801) A/V test (with M card), with output interface Composite, Component, RF channel 3, s-video and 1394 Tx Test"},
{63, TEST_ITEM_TYPE, "Tuner 1(256QAM, freq=85000000, vPid=800, audio=801) A/V test, with output interface Composite, Component, RF channel 3, s-video and Audio SPDIF"},
{64, TEST_ITEM_TYPE, "Tuner 1( 64QAM, freq=183000000, vPid=8c0, audio=801) A/V test (with M card), with output interface Composite, Component, RF channel 3, s-video, Audio SPDIF and 1394 Tx Test"},
{65, TEST_ITEM_TYPE, "Tuner 1(256QAM, freq=85000000, vPid=1d0, audio=1d1), Tuner 2(64QAM, freq=183000000, vPid=880) A/V test, with output interface Composite, Component, RF channel 3, s-video and Audio SPDIF"},
{66, TEST_ITEM_TYPE, "Tuner 1( 64QAM, freq=183000000, vPid=880, audio=881) A/V test (with M card), with output interface Composite, Component, RF channel 3, s-video, Audio SPDIF and 1394 Tx Test"},
{67, TEST_ITEM_TYPE, "Tuner 1(256QAM, freq=85000000, vPid=1d0, audio=1d1) A/V test, with output interface Composite, Component, RF channel 3 and Audio SPDIF"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{71, TEST_ITEM_TYPE, "M-Card Detect Test (Using original M-Card)"},
{0, NO_ITEM_TYPE, ""},
{73, TEST_ITEM_TYPE, "M-Card MSPI Test (Using original M-Card)"},
{74, TEST_ITEM_TYPE, "M-Card OOB Downstream Test (75.25MHz) (Using CCMTC M-Card)"},
{75, TEST_ITEM_TYPE, "M-Card OOB Downstream Test (125MHz) (Using CCMTC M-Card)"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{82, TEST_ITEM_TYPE, "Front panel LED test (for hardware team)"},
{83, TEST_ITEM_TYPE, "Front panel LED test (for product-line)"},
{0, NO_ITEM_TYPE, ""},
{85, TEST_ITEM_TYPE, "IR Receive Test (With timeout || Press Key 1 to exit)"},
{0, NO_ITEM_TYPE, ""},
{87, TEST_ITEM_TYPE, "Ethernet External loop back test"},
{88, TEST_ITEM_TYPE, "Rear panel USB port Test"},
{89, TEST_ITEM_TYPE, "Front panel USB port Test"},
{90, TEST_ITEM_TYPE, "SATA Hard Drive Test ( Internal ) (MTC do this)"},
{91, TEST_ITEM_TYPE, "SATA Hard Drive Test ( External ) (MTC do this)"},
{92, TEST_ITEM_TYPE, "AC_outlet ON"},
{93, TEST_ITEM_TYPE, "AC_outlet OFF"},
{94, TEST_ITEM_TYPE, "1394 interrupt line test"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{133, TEST_ITEM_TYPE, "Tuner 0 phase noise test"},
{134, TEST_ITEM_TYPE, "Tuner 1 phase noise test"},
{135, TEST_ITEM_TYPE, "Tuner 0 on-board performance test"},
{136, TEST_ITEM_TYPE, "Tuner 1 on-board performance test"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{201, ACTION_ITEM_TYPE, "Display error code on LED"},
{202, ACTION_ITEM_TYPE, "Display error code on LED and return Error Code to SIE"},
{203, ACTION_ITEM_TYPE, "Display PASS/FAIL on LED & OSD for last item test result"},
{204, ACTION_ITEM_TYPE, "Display error code on TV screen"},
{205, ACTION_ITEM_TYPE, "WaitKey(KEY_CH_UP|KEY_CH_DOWN)"},
{206, ACTION_ITEM_TYPE, "Automaticlly power on/off MCard during WaitKey(KEY_CH_UP|KEY_CH_DOWN)"},
{207, ACTION_ITEM_TYPE, "Display error code ( > 4 char) on LED"},
{208, ACTION_ITEM_TYPE, "Return Error Code ( > 4 char) to SIE"},
{209, ACTION_ITEM_TYPE, "WaitKey(KEY_SELECT)"},
{210, ACTION_ITEM_TYPE, "Automaticlly power on/off MCard during WaitKey(KEY_SELECT)"},
{211, ACTION_ITEM_TYPE, "Display error code on LED & OSD"},
{212, ACTION_ITEM_TYPE, "Endless loop if last item test fail"},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{0, NO_ITEM_TYPE, ""},
{253, CONTROL_ITEM_TYPE, "delay_100ms(x)"},
{0, NO_ITEM_TYPE, ""},
{255, TEST_ITEM_TYPE, "No Test Item"},
};

S8 stationsDesc[227];
int stationsDescLen;

#else
Item gItem[256] = {
{0, TEST_ITEM_TYPE},
{1, TEST_ITEM_TYPE},
{2, TEST_ITEM_TYPE},
{3, TEST_ITEM_TYPE},
{4, ACTION_ITEM_TYPE},
{5, ACTION_ITEM_TYPE},
{6, TEST_ITEM_TYPE},
{7, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{10, TEST_ITEM_TYPE},
{11, ACTION_ITEM_TYPE},
{12, ACTION_ITEM_TYPE},
{13, TEST_ITEM_TYPE},
{14, TEST_ITEM_TYPE},
{15, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{17, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{20, TEST_ITEM_TYPE},
{21, TEST_ITEM_TYPE},
{22, TEST_ITEM_TYPE},
{23, TEST_ITEM_TYPE},
{24, TEST_ITEM_TYPE},
{25, TEST_ITEM_TYPE},
{26, TEST_ITEM_TYPE},
{27, TEST_ITEM_TYPE},
{28, TEST_ITEM_TYPE},
{29, TEST_ITEM_TYPE},
{30, TEST_ITEM_TYPE},
{31, TEST_ITEM_TYPE},
{32, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{35, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{38, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{40, TEST_ITEM_TYPE},
{41, TEST_ITEM_TYPE},
{42, TEST_ITEM_TYPE},
{43, TEST_ITEM_TYPE},
{44, TEST_ITEM_TYPE},
{45, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{51, ACTION_ITEM_TYPE},
{52, TEST_ITEM_TYPE},
{53, TEST_ITEM_TYPE},
{54, TEST_ITEM_TYPE},
{55, TEST_ITEM_TYPE},
{56, TEST_ITEM_TYPE},
{57, TEST_ITEM_TYPE},
{58, TEST_ITEM_TYPE},
{59, TEST_ITEM_TYPE},
{60, TEST_ITEM_TYPE},
{61, TEST_ITEM_TYPE},
{62, TEST_ITEM_TYPE},
{63, TEST_ITEM_TYPE},
{64, TEST_ITEM_TYPE},
{65, TEST_ITEM_TYPE},
{66, TEST_ITEM_TYPE},
{67, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{71, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{73, TEST_ITEM_TYPE},
{74, TEST_ITEM_TYPE},
{75, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{82, TEST_ITEM_TYPE},
{83, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{85, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{87, TEST_ITEM_TYPE},
{88, TEST_ITEM_TYPE},
{89, TEST_ITEM_TYPE},
{90, TEST_ITEM_TYPE},
{91, TEST_ITEM_TYPE},
{92, TEST_ITEM_TYPE},
{93, TEST_ITEM_TYPE},
{94, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{133, TEST_ITEM_TYPE},
{134, TEST_ITEM_TYPE},
{135, TEST_ITEM_TYPE},
{136, TEST_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{201, ACTION_ITEM_TYPE},
{202, ACTION_ITEM_TYPE},
{203, ACTION_ITEM_TYPE},
{204, ACTION_ITEM_TYPE},
{205, ACTION_ITEM_TYPE},
{206, ACTION_ITEM_TYPE},
{207, ACTION_ITEM_TYPE},
{208, ACTION_ITEM_TYPE},
{209, ACTION_ITEM_TYPE},
{210, ACTION_ITEM_TYPE},
{211, ACTION_ITEM_TYPE},
{212, ACTION_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{253, CONTROL_ITEM_TYPE},
{0, NO_ITEM_TYPE},
{255, TEST_ITEM_TYPE},
};

#endif // CONFIG_DISPLAY_STATION_INFO

#ifdef CONFIG_UI_STATION_TEST
void StationsInit() {
	#ifdef CONFIG_DISPLAY_STATION_INFO
	S8* p = stationsDesc;
	#endif // CONFIG_DISPLAY_STATION_INFO

	if (IsProductModel("QIP72XXP2")) {
		#ifdef CONFIG_DISPLAY_STATION_INFO
		strcpy(stationsDesc, "1. Key : Cursor Up. 2. With SIE.");
		stationsDescLen = strlen(stationsDesc);
		p = stationsDesc + stationsDescLen + 1;
		strcpy(p, "1. Key : Cursor Left. 2.Using Cable Card loaded with Cable Card MTC(CCMTC).");
		p += strlen("1. Key : Cursor Left. 2.Using Cable Card loaded with Cable Card MTC(CCMTC).") + 1;
		strcpy(p, "1. Keys : Cursor Up + Cursor Left. 2. Using Original Cable Card.");
		p += strlen("1. Keys : Cursor Up + Cursor Left. 2. Using Original Cable Card.") + 1;
		strcpy(p, "1. Keys : Cursor Down. 2. Using Original Cable Card.");
		p += strlen("1. Keys : Cursor Down. 2. Using Original Cable Card.") + 1;
		p = stationsDesc;
		#endif // CONFIG_DISPLAY_STATION_INFO

		station[1].SetId(1);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[1].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList1[NR_ITEMS] = {59, 51, 20, 22, 23, 24, 26, 27, 29, 31, 32, 83, 52, 202, 211, 255};
		station[1].SetItemList(itemList1);

		station[2].SetId(2);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[2].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList2[NR_ITEMS] = {42, 94, 87, 88, 89, 35, 44, 45, 74, 54, 55, 211, 255};
		station[2].SetItemList(itemList2);

		station[3].SetId(3);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[3].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList3[NR_ITEMS] = {71, 73, 57, 209, 58, 211, 255};
		station[3].SetItemList(itemList3);

		station[4].SetId(4);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[4].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList4[NR_ITEMS] = {61, 41, 209, 64, 43, 211, 210, 28, 211, 212, 38, 203, 255};
		station[4].SetItemList(itemList4);

	}
	else if (IsProductModel("QIP7100P2")) {
		#ifdef CONFIG_DISPLAY_STATION_INFO
		strcpy(stationsDesc, "1. Key : Cursor Up. 2. With SIE.");
		stationsDescLen = strlen(stationsDesc);
		p = stationsDesc + stationsDescLen + 1;
		strcpy(p, "1. Key : Cursor Left. 2.Using Cable Card loaded with Cable Card MTC(CCMTC).");
		p += strlen("1. Key : Cursor Left. 2.Using Cable Card loaded with Cable Card MTC(CCMTC).") + 1;
		strcpy(p, "1. Keys : Cursor Up + Cursor Left. 2. Using Original Cable Card.");
		p += strlen("1. Keys : Cursor Up + Cursor Left. 2. Using Original Cable Card.") + 1;
		strcpy(p, "1. Keys : Cursor Down. 2. Using Original Cable Card.");
		p += strlen("1. Keys : Cursor Down. 2. Using Original Cable Card.") + 1;
		p = stationsDesc;
		#endif // CONFIG_DISPLAY_STATION_INFO

		station[1].SetId(1);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[1].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList1[NR_ITEMS] = {59, 51, 20, 23, 24, 26, 27, 29, 31, 32, 83, 52, 202, 211, 255};
		station[1].SetItemList(itemList1);

		station[2].SetId(2);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[2].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList2[NR_ITEMS] = {42, 94, 87, 88, 89, 35, 44, 45, 74, 54, 55, 211, 255};
		station[2].SetItemList(itemList2);

		station[3].SetId(3);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[3].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList3[NR_ITEMS] = {71, 73, 57, 209, 58, 211, 255};
		station[3].SetItemList(itemList3);

		station[4].SetId(4);
		#ifdef CONFIG_DISPLAY_STATION_INFO
		station[4].SetDesc(p);
		p += strlen(p) + 1;
		#endif // CONFIG_DISPLAY_STATION_INFO

		U8 itemList4[NR_ITEMS] = {63, 41, 209, 64, 43, 211, 210, 28, 211, 212, 38, 203, 255};
		station[4].SetItemList(itemList4);

	}
}
#endif	// CONFIG_UI_STATION_TEST
