// Author : Gamma Chen

#include "TiLib_Keypad.h"
#include "gitypes.h"
#include "fpcontrol.h"


#ifdef CONFIG_DISPLAY_STATION_INFO
KeyTable keyTable[12] = {
	{KEYPAD_NOKEY,		"KEY_NONE"},		// 0
	{KEYPAD_CUR_UP,		"KEY_CUR_UP"},
	{KEYPAD_CUR_DOWN,	"KEY_CUR_DOWN"},
	{KEYPAD_CUR_LEFT,	"KEY_CUR_LEFT"},
	{KEYPAD_CUR_RIGHT,	"KEY_CUR_RIGHT"},
	{KEYPAD_SELECT,		"KEY_SELECT"},		// 5
	{KEYPAD_POWER,		"KEY_POWER"},
	{KEYPAD_GUIDE,		"KEY_GUIDE"},
	{KEYPAD_INFO,		"KEY_INFO"},
	{KEYPAD_AB_SWITCH,	"KEY_AB_SWITCH"},
	{KEYPAD_CH_UP,		"KEY_CH_UP"},		// 10
	{KEYPAD_CH_DOWN,	"KEY_CH_DOWN"}		// 11
};
#endif // CONFIG_DISPLAY_STATION_INFO

//##ModelId=46FA38C40137
void TiLib_Keypad::SetDebounce(U8 debounce)
{
//	BKPD_SetDebounce(_hKpd, debounce);
}

//##ModelId=46FA38C40139
void TiLib_Keypad::Init()
{
//	_hKpd = bcmGetKpdHandle();
//	setDebounce(0);	// 0 : No Debounce
}

//##ModelId=46FA38C4013A
KEYTYPE TiLib_Keypad::GetKey()
{
	_key = GetFrontPanelKey();
	return _key;
}

#include "userio.h"

extern "C" BOOL gbFrontPanelInitialized;
extern "C" KeypadLedRegisters	*gKpdLedRegs;		// Pointer contains base of KEYPAD & LED UPG registers
// Follows modify from GetFrontPanelKey() of fpcontrol.c
/***************************************************************
*  Gets a FrontPanel Keypad Code (Non blocking)                *
*                                                              *
*  Enters : None                                               *
*  Exits  : Key code is returned                               *
****************************************************************/
//##ModelId=4732FFC8026F
U16 TiLib_Keypad::GetKey_U16()
{

FP_KEYCODE	cKeycode=FP_NOKEY;

U8 ucKeyPadRow_10, ucKeyPadRow_32;
U16	wRawKeycode = 0;

    if (gbFrontPanelInitialized == TRUE)
    {
#if DCX_PLATFORM  // Support old (7400 Interface) or new FP (PSoC) Keypad

	#if I2C_FRONTPANEL_INSTALLED

		// PSoC Keypad Interface...

		// Read the entire FP Keypad buffer
		//uwKeycode =  ~pxKeyPad->Read();
		wRawKeycode = pxKeyPad->Read();

		if (wRawKeycode != 0xFFFF)
		{
			//DB_PRINTF(("Keycode = 0x%x\r\n",wRawKeycode));
		}

	#else
		// 740x Keypad Interface 
	   	ucKeyPadRow_32 = (U8) gKpdLedRegs->ulLDK_Keyrow32 & 0xFF;
		ucKeyPadRow_10 = (U8) gKpdLedRegs->ulLDK_Keyrow10 & 0xFF;
		wRawKeycode = ucKeyPadRow_10 | (ucKeyPadRow_32<<8);
	#endif

#elif QIP7K_PLATFORM
		// Only 740x Keypad Interface supported
   		ucKeyPadRow_32 = gKpdLedRegs->ucLEDkeyrow32;
		ucKeyPadRow_10 = gKpdLedRegs->ucLEDkeyrow10;
		wRawKeycode = ucKeyPadRow_10 | (ucKeyPadRow_32<<8);

#elif QIP7KP2_PLATFORM
		// 740x Keypad Interface 
	   	ucKeyPadRow_32 = (U8) gKpdLedRegs->ulLDK_Keyrow32 & 0xFF;
		ucKeyPadRow_10 = (U8) gKpdLedRegs->ulLDK_Keyrow10 & 0xFF;
		wRawKeycode = ucKeyPadRow_10 | (ucKeyPadRow_32<<8);

#else
	#error : XXX_PLATFORM not define
#endif

//		cKeycode = TranslateKeycode(wRawKeycode); //LOCALSWAP(wRawKeycode));
	}
	else
	{
		DB_PRINTF(("\r\nFrontPanel Driver not Initialized\r\n\r\n"));
		cKeycode = FP_ERROR;
	}

	return wRawKeycode;
}

//##ModelId=46FA38C4013B
KEYTYPE TiLib_Keypad::GetKeyWithCount(U32 count)
{
}

//##ModelId=46FA38C40146
bool TiLib_Keypad::LongPressed()
{
}

#ifdef CONFIG_DISPLAY_STATION_INFO
//##ModelId=46FA38C40147
S8* TiLib_Keypad::GetKeyName(KEYTYPE key)
{
	static S8 sKey[200];
	S8	*p = sKey;
	int i;
	bool firstKeyFound = false;
	
	sKey[0] = '\0';
	for (i = 1; i < 12; i++) {	// from i = 1 (skip KEY_NONE
		if ((key & keyTable[i].key) != 0) {
			if (firstKeyFound) {
				strcpy(p, "|");
				p += strlen(p);
			}
			else {
				firstKeyFound = true;
			}
			strcpy(p, keyTable[i].name);
			p += strlen(p);
		}
	}
	
	return sKey;
}
#endif	// CONFIG_DISPLAY_STATION_INFO

//##ModelId=46FA38C40126
KEYTYPE TiLib_Keypad::Read()
{
}

//##ModelId=46FA38C40127
KEYTYPE TiLib_Keypad::ReadWithCount(U32 count)
{
}

