// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 SATA_InternalDiskTest()
{
	U32 lError = 1;
	
	lError = api.peripheral.SATA_DiskTest(QIP7232_SATA_INTERNAL_PORT);
	
	return lError;
}

U32 Items::TI090()
{
	U32 lError = 1;
	U32 (*pf)() = &SATA_InternalDiskTest;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
