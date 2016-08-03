// Author:         Stony Tai

// ************************************************************************************************************
// 1. Using Modified M-Card (CCMTC)
// 2. Run DCCG
//	2.1 F8 : load Tool\EXTENDED_DATA_TIME_ZONE_VIRTUAL_USER_INTERFACE.txt
//	2.2 F6 : set Message Delay (msec): (0)
//	2.3 F2(Broad) : F1(Run)

/* Note:
1. The M Card which SN=NG3632NA0315 will be fail after Power-Off and On.
And will PASS after "Press Button Reset".
And will PASS after Power-Off and On, if Warm Up this M Card.
2. Other Card always PASS after Power-Off and On.
*/

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 MCard_OOBDownStreamTest125Mhz()
{
	U32 lError = 0;

	lError = api.mCard.OOBDownStreamTest(125000000);  //OOB 125 MHz 
	
	return lError;
}

U32 Items::TI075()
{
	U32 lError = 1;
	U32 (*pf)() = &MCard_OOBDownStreamTest125Mhz;
	
	int timeOut = 20;	// unit:1s, => 20 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}