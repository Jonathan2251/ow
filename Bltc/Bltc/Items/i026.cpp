// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib_RS232.h"

U32	IEEE1394_ChipID_Check()
{
	U32 lError = 1;
	U32 IEEE1394ID;
	
	lError=api.ChipID_Get(CT_1394CHIP,&IEEE1394ID);
	PDEBUG("JEDI 1394 vendor ID = %.8x\r\n", IEEE1394ID);
   
    if ((IEEE1394ID==IEEE1394Chip_VendorId)&&(!lError))
	     lError=0;
	else lError=1;
   	
	return lError;
}

U32 Items::TI026()
{
	U32 lError = 1;
	U32 (*pf)() = &IEEE1394_ChipID_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}