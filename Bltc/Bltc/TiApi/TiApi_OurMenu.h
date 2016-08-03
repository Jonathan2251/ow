#ifndef TIAPI_OURMENU_H_HEADER_INCLUDED
#define TIAPI_OURMENU_H_HEADER_INCLUDED

#include "BltcBasicType.h"

//##ModelId=480EE61F0203
class TiApi_OurMenu {
public:
    //##ModelId=481126F60192
	void DisplayMenu();
    //##ModelId=481126F601A2
	void DisplayLedSubMenu();
	
	void DisplayFactoryClocksSubMenu();
    //##ModelId=481126F601A3
	void Enter();
    //##ModelId=481126F601A4
	void EnterLedSubMenu();
	
	void EnterFactoryClocksSubMenu();
    //##ModelId=481126F601A5
	void DumpMemAddr();	// dump memory of address
    //##ModelId=481126F601A6
	void MCardPowerOnOff_Periodically();
    //##ModelId=481126F601B2
	void DisplayKeypadValue();
    //##ModelId=481126F601B3
	void FLASH_ErasePfdSector();
    //##ModelId=481126F601B4
	void EnableTimerInterrupt();
    //##ModelId=481126F601B5
	void DisableTimerInterrupt();
    //##ModelId=481126F601B6
	void RS232_EnableOutput();
    //##ModelId=481126F601C1
	void RS232_DisableOutput();
    //##ModelId=481126F601C2
    void DisplayRunningStr(char *str, U16* keyMatch, int nrKeyMatches);
    //##ModelId=481126F601E1
    void DisplayLongStr_By4_UntilKeyInMatch(char *str, U16* keyMatch, int nrKeyMatches);
    //##ModelId=481126F601F1
	void Led_ErrCodeRun();
    //##ModelId=481126F60200
	void Led_ErrCode4();
	
	void FactoryClocks_StartTest();
	void FactoryClocks_StopTest();
private:
    //##ModelId=481126F60201
	void MCard_PowerOnOffPeriodically(int ms);
};

#endif	// TIAPI_OURMENU_H_HEADER_INCLUDED
