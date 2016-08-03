// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 SATA_ExternalDiskTest()
{
	U32 lError = 0;
	
	lError = api.peripheral.SATA_DiskTest(QIP7232_SATA_EXTERNAL_PORT);
	
	return lError;
}

U32 Items::TI091()
{
	U32 lError = 0;
	U32 (*pf)() = &SATA_ExternalDiskTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
