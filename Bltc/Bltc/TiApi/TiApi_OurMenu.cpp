#include "TiApi.h"
#include "TiLib.h"
#include "string.h"
#include "Timer_MsTick.h"
#include "TiApi_Memory.h"

#define MAX_STRING_INPUT	  100
#define OFF 0
#define ON	1

extern "C" bool pdebugEnable;

extern "C" {

	void ToUpperCase(S8 *from, S8 *to) {
	// Transfer small case in *from into big case *to. 
		S8 tmp[1000], *p, *q;
		
		for (p = from, q = tmp; *p != '\0'; p++, q++) {
			if (*p >= 'a' && *p <='z') {
				*q = 'A' + (*p - 'a');
			}
			else {
				*q = *p;
			}
		}
		*q = '\0';
		strcpy(to, tmp);
	}
	
}	// extern "C"

//##ModelId=481126F60192
void TiApi_OurMenu::DisplayMenu() {
	lib.rs232.Print("\r\n");
	lib.rs232.Print("1. DumpMemAddr            Dump Memory Address by user input address range\r\n");
	lib.rs232.Print("2. MCardPowerOnOff        MCard power on/off periodically by user input time period (For Tom, to capture the impulse at moment of power on)\r\n");
	lib.rs232.Print("3. Keypad                 Display input key (16 bits) value of keypad\r\n");
	lib.rs232.Print("4. ErasePfd               Erase pfd sectors\r\n");
	lib.rs232.Print("5. EnableTimerInt         Enable Timer Interrupt\r\n");
	lib.rs232.Print("6. DisableTimerInt        Disable Timer Interrupt\r\n");
	lib.rs232.Print("7. Led                    Led sub menu\r\n");
	lib.rs232.Print("8. Factory Clocks         Factory test to enable/disable clocks for testing\r\n");
	lib.rs232.Print("h or ?                    help\r\n");
	lib.rs232.Print("/                         exit menu\r\n");
	lib.rs232.Print("\r\n");
}

//##ModelId=481126F601A2
void TiApi_OurMenu::DisplayLedSubMenu() {
	lib.rs232.Print("\r\nLED Sub Menu:\r\n");
	lib.rs232.Print("1. ErrCodeRun             Display Long Error Code by Running Style (for long error code evaluation)\r\n");
	lib.rs232.Print("2. ErrCode4               Display Long Error Code by 4 Digits and repeat (for long error code evaluation\r\n");
	lib.rs232.Print("h or ?                    help\r\n");
	lib.rs232.Print("/                   	   exit LED Sub Menu\r\n");
	lib.rs232.Print("\r\n");
}

void TiApi_OurMenu::DisplayFactoryClocksSubMenu() {
	lib.rs232.Print("\r\n Factory Clocks Sub Menu:\r\n");
	lib.rs232.Print("1. Start Test\r\n");
	lib.rs232.Print("2. Stop Test\r\n");
	lib.rs232.Print("h or ?                    help\r\n");
	lib.rs232.Print("/                   	   exit  Factory Clocks Sub Menu\r\n");
	lib.rs232.Print("\r\n");
}

//##ModelId=481126F601A3
void TiApi_OurMenu::Enter() {
	S8  str[MAX_STRING_INPUT];
	bool oldStatus = lib.rs232.EnableOutput();
	
	DisplayMenu();
    while (1) {
		lib.rs232.GetLine(str, MAX_STRING_INPUT);
		ToUpperCase(str, str);
		if (strncmp(str, "1", strlen("1")) == 0) {
			DumpMemAddr();
		}
		else if (strncmp(str, "2", strlen("2")) == 0) {
			MCardPowerOnOff_Periodically();
		}
		else if (strncmp(str, "3", strlen("3")) == 0) {
			DisplayKeypadValue();
		}
		else if (strncmp(str, "4", strlen("4")) == 0) {
			FLASH_ErasePfdSector();
		}
		else if (strncmp(str, "5", strlen("5")) == 0) {
			EnableTimerInterrupt();
		}
		else if (strncmp(str, "6", strlen("6")) == 0) {
			DisableTimerInterrupt();
		}
		else if (strncmp(str, "7", strlen("7")) == 0) {
			EnterLedSubMenu();
		}
		else if (strncmp(str, "8", strlen("8")) == 0) {
			EnterFactoryClocksSubMenu();
		}
		else if (strncmp(str, "H", strlen("H")) == 0) {
			DisplayMenu();
		}
		else if (strncmp(str, "?", strlen("?")) == 0) {
			DisplayMenu();
		}
		else if (strncmp(str, "/", strlen("/")) == 0) {
			break;
		}
		else {
			lib.rs232.Print("Invalid command\r\n");
		}
		DisplayMenu();
	}
	
out:
	if (oldStatus == false) {
		lib.rs232.DisableOutput();
	}
}

//##ModelId=481126F601A4
void TiApi_OurMenu::EnterLedSubMenu() {
	S8  str[MAX_STRING_INPUT];
	
	DisplayLedSubMenu();
    while (1) {
		lib.rs232.GetLine(str, MAX_STRING_INPUT);
		ToUpperCase(str, str);
		if (strncmp(str, "1", strlen("1")) == 0) {
			Led_ErrCodeRun();
		}
		else if (strncmp(str, "2", strlen("2")) == 0) {
			Led_ErrCode4();
		}
		else if (strncmp(str, "H", strlen("H")) == 0) {
			DisplayLedSubMenu();
		}
		else if (strncmp(str, "?", strlen("?")) == 0) {
			DisplayLedSubMenu();
		}
		else if (strncmp(str, "/", strlen("/")) == 0) {
			break;
		}
		else {
			lib.rs232.Print("Invalid command\r\n");
		}
		DisplayLedSubMenu();
	}
}

void TiApi_OurMenu::EnterFactoryClocksSubMenu() {
	S8  str[MAX_STRING_INPUT];
	
	DisplayFactoryClocksSubMenu();
    while (1) {
		lib.rs232.GetLine(str, MAX_STRING_INPUT);
		ToUpperCase(str, str);
		if (strncmp(str, "1", strlen("1")) == 0) {
			FactoryClocks_StartTest();
		}
		else if (strncmp(str, "2", strlen("2")) == 0) {
			FactoryClocks_StopTest();
		}
		else if (strncmp(str, "H", strlen("H")) == 0) {
			DisplayFactoryClocksSubMenu();
		}
		else if (strncmp(str, "?", strlen("?")) == 0) {
			DisplayFactoryClocksSubMenu();
		}
		else if (strncmp(str, "/", strlen("/")) == 0) {
			break;
		}
		else {
			lib.rs232.Print("Invalid command\r\n");
		}
		DisplayFactoryClocksSubMenu();
	}
}

//##ModelId=481126F601A5
void TiApi_OurMenu::DumpMemAddr() {
	S8  StringBuffer[MAX_STRING_INPUT];
	S32 NumInput;
	U32 startAddr=0,endAddr=0;	
	
	bool oldStatus = lib.rs232.EnableOutput();
	lib.rs232.Print("Please input the (Start Address  End Address)\r\n");
	lib.rs232.Print("example : 0xb8000000 0xb8004000\r\n");
	lib.rs232.GetLine(StringBuffer,MAX_STRING_INPUT);
	
	NumInput= (S32) sscanf(StringBuffer, "%x %x" ,&startAddr ,&endAddr);
	
	if (NumInput<2) {
	
		lib.rs232.Print("Input parameters error \r\n");
		goto out;
	}	
	else if (NumInput==2) {
	
		//lib.rs232.Print("You are right start:%x end:%x\r\n",startAddr,endAddr);
		api.memory.FLASH_dump_content((U32*)startAddr,(U32*)endAddr);
		
	}
	else {
		
		lib.rs232.Print("Input parameters error \r\n");
	}	
out:
	if (oldStatus == false) {
		lib.rs232.DisableOutput();
	}
}

//##ModelId=481126F60201
void TiApi_OurMenu::MCard_PowerOnOffPeriodically(int ms)
{
	Timer_MsTick tick_ms;
	U8 ch;
	int mCardStatus = OFF;
	int count = ms;
	
	tick_ms.Reset();
	while (ch != 'q') {
		if (tick_ms.Tick()) {
			count--;
			if (count < 0) {
				if (mCardStatus == OFF) {
					api.mCard.PowerOn();
					mCardStatus = ON;
				}
				else {	// mCardStatus == ON
					api.mCard.PowerOff();
					mCardStatus = OFF;
				}
				count = ms;
			}
		}
		ch = lib.rs232.GetKey();
	}
}

//##ModelId=481126F601A6
void TiApi_OurMenu::MCardPowerOnOff_Periodically() {
	S8  StringBuffer[MAX_STRING_INPUT];
	S32 NumInput;
	int tick_100ms;
	
	bool oldStatus = lib.rs232.EnableOutput();
	lib.rs232.Print("Please input the time period (unit:100ms) for MCard Power On Off\r\n");
	lib.rs232.Print("example : \r\n");
	lib.rs232.Print("10\r\n");
	lib.rs232.Print("Meaning 10*100 ms => 1 seconds\r\n");
	lib.rs232.GetLine(StringBuffer, MAX_STRING_INPUT);
	
	NumInput= (S32) sscanf(StringBuffer, "%d", &tick_100ms);
	
	if (NumInput == 1) {
		lib.rs232.Print("Start...(press q to quit)\r\n");
		// Trigger at every 100ms. It must. If check MCard IsLoaded() too frequently, the insert MCard action seems will reset STB.
		MCard_PowerOnOffPeriodically(tick_100ms*100);
	}
	else {
		lib.rs232.Print("Input parameters error \r\n");
	}	

	if (oldStatus == false) {
		lib.rs232.DisableOutput();
	}
}

//##ModelId=481126F601B2
void TiApi_OurMenu::DisplayKeypadValue() {
	lib.rs232.Print("Press key (power) to exit.\r\n");
	U16 oldKey = 0xFFFF;
	while (1) {
		U16 key = lib.keypad.GetKey_U16();
		if (key == 0xFFFF) {
			oldKey = 0xFFFF;
		}
		else if (oldKey != key) {
			oldKey = key;
			lib.rs232.Print("%.4x %.4x\r\n", key, (U16)((~key) & (~KEYPAD_CH_DOWN_U16)));
		}
		else if ( (U16)((~key) & (~KEYPAD_POWER_U16)) != 0x0000 ) {
			break;
		}
	}
}

//##ModelId=481126F601B3
void TiApi_OurMenu::FLASH_ErasePfdSector() {
	lib.rs232.Print("Erase pfd area...\r\n");
	U32 lError = api.memory.FLASH_ErasePfdSector(ENTIRE_AREA);
	if (lError== 0) {
		lib.rs232.Print("SUCCESS\r\n");
	}
	else {
		lib.rs232.Print("FAIL\r\n");
	}
}

//##ModelId=481126F601B4
void TiApi_OurMenu::EnableTimerInterrupt() {
#ifdef CONFIG_ENABLE_TIMER_INTERRUPT
	lib.clock.EnableTimerInterrupt();
#endif
}

//##ModelId=481126F601B5
void TiApi_OurMenu::DisableTimerInterrupt() {
#ifdef CONFIG_ENABLE_TIMER_INTERRUPT
	lib.clock.DisableTimerInterrupt();
#endif
}

//##ModelId=481126F601B6
void TiApi_OurMenu::RS232_EnableOutput() {
	pdebugEnable = true;
	lib.rs232.EnableOutput();
}

//##ModelId=481126F601C1
void TiApi_OurMenu::RS232_DisableOutput() {
	pdebugEnable = false;
	lib.rs232.DisableOutput();
}


//##ModelId=481126F601C2
void TiApi_OurMenu::DisplayRunningStr(char *str, U16* keyMatch, int nrKeyMatches)
{
	Timer_MsTick tick_ms;
	int count = 300;
	int i = 0;
	int len = strlen(str);
	bool escape = false;
	
	tick_ms.Reset();
	while (!escape) {
		if (tick_ms.Tick()) {
			count--;
			if (count < 0) {
				// Trigger at every 0.5 seconds.
				lib.led->DisplayStr(str+i);
				i++;
				if (i >= len) {
					i = 0;
				}
				count = 300;
			}
		}
		api.mCard.AutoPowerOnOff();
		U16 key = lib.keypad.GetKey_U16();
		for (int j = 0; j < nrKeyMatches; j++) {
			if (key == keyMatch[j]) {
				escape = true;
				break;
			}
		}
	}
}

// Display 4 char at each time.
//##ModelId=481126F601E1
void TiApi_OurMenu::DisplayLongStr_By4_UntilKeyInMatch(char *str, U16* keyMatch, int nrKeyMatches)
{
	Timer_MsTick tick_ms;
	int count = 700;
	int i = 0;
	int round = strlen(str) / 4;
	bool escape = false;
	
	tick_ms.Reset();
	while (!escape) {
		if (tick_ms.Tick()) {
			count--;
			if (count < 0) {
				// Trigger at every 0.7 seconds.
				lib.led->DisplayStr(str+i*4);
				i++;
				if (i >= round) {
					i = 0;
				}
				count = 700;
			}
		}
		api.mCard.AutoPowerOnOff();
		U16 key = lib.keypad.GetKey_U16();
		for (int j = 0; j < nrKeyMatches; j++) {
			if (key == keyMatch[j]) {
				escape = true;
				break;
			}
		}
	}
}

//##ModelId=481126F601F1
void TiApi_OurMenu::Led_ErrCodeRun() {
	char sError[50];
	U16 keyMatch[4];
	
	lib.rs232.Print("Press key (channel down or channel up) to exit.\r\n");
	keyMatch[0] = KEYPAD_CH_UP_U16;
	keyMatch[1] = KEYPAD_CH_DOWN_U16;
	keyMatch[2] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_UP_U16);
	keyMatch[3] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_DOWN_U16);
	sprintf(sError, "    -001084024523-    ");
	DisplayRunningStr(sError, keyMatch, 4);
}

//##ModelId=481126F60200
void TiApi_OurMenu::Led_ErrCode4() {
	char sError[50];
	U16 keyMatch[4];
	
	lib.rs232.Print("Press key (channel down or channel up) to exit.\r\n");
	keyMatch[0] = KEYPAD_CH_UP_U16;
	keyMatch[1] = KEYPAD_CH_DOWN_U16;
	keyMatch[2] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_UP_U16);
	keyMatch[3] = (KEYPAD_CUR_RIGHT_U16 & KEYPAD_CH_DOWN_U16);
	sprintf(sError, "0010002084024523    ");
	api.menu.DisplayLongStr_By4_UntilKeyInMatch(sError, keyMatch, 4);
}

void TiApi_OurMenu::FactoryClocks_StartTest() {
	api.frontEnd.StartFactoryClkTest();
}

void TiApi_OurMenu::FactoryClocks_StopTest() {
	api.frontEnd.StopFactoryClkTest();
}

