// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "Bltc.h"

U32 BoardID_Check()
{
	U32 lError = 1;
	U32 HWBoardID;
	
	lError=api.ChipID_Get(CT_BOARD_ID,&HWBoardID);
	PDEBUG("Board ID = %.8x\r\n", HWBoardID);
	
	if ((HWBoardID==HWID_QIP7100_P2 || HWBoardID==HWID_QIP72XX_P2)&&(!lError))
	     lError=0;
	else lError=1;
		
	return lError;
}

U32 Items::TI021()
{
	U32 lError = 1;
	U32 (*pf)() = &BoardID_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
