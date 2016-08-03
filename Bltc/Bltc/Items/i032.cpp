// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"
#include "addrutil.h"
#include "memcfg.h"

#include "sdram_db.h"

#define CONFIG_WITHOUT_TIMEOUT

extern U32 GetSdramStartTestAddr();

U32 DDR_ReadWrite_Test()
{
	U32 lError = 0;
	U32 lError1 = 0;
	U32 test_pattern[MAX_TEST_PATTERN] = {0x55555555,0xAAAAAAAA,0x24924924};
	U32 p_StartAddress = ((((U32)_heap_end|GetKSEG1Base())+7)& ~7); //8 byte alligned start

	U32 bytes = lib.memory.DDR_GetSize_MB();
	lib.rs232.Print("DDR size of bytes = %d\r\n", bytes);
	lib.rs232.Print("_heap_end = %x\r\n",(U32)_heap_end);
	U32 testBytes = bytes - ((U32)_heap_end - 0x80000000);
	lib.rs232.Print("testBytes = %d\r\n", testBytes);
	if (bytes != 0x20000000) {
		lError = 1;
		return lError;
	}
	if (bytes <= 0x10000000) {
	// DDR Size = 256MB or 128MB
		lError=api.memory.DDR_RW_Test(&test_pattern[0],p_StartAddress,testBytes,DDR_PINS_TEST);
	}
	else if (bytes <= 0x20000000) {
	// DDR Size = 512MB
		if (p_StartAddress < 0xB0000000) {
		// p_StartAddress < (0xA0000000+256MB)
			testBytes = 0x10000000 - ((U32)_heap_end - 0x80000000);
			lError=api.memory.DDR_RW_Test(&test_pattern[0],p_StartAddress,testBytes,DDR_PINS_TEST);
			// From 0xC0000000 to 0xCFFFFFFF is TLB map area, so ignore testing temporary.
			// The correct is 0x20000000 to 0x2FFFFFFF, refer to DCX3400_ProdSWImpacts_ENB-05350_x5.doc.
//			lError1=api.memory.DDR_RW_Test(&test_pattern[0],0x20000000,0x10000000,DDR_PINS_TEST);
		}
		// From 0xC0000000 to 0xCFFFFFFF is TLB map area, so ignore testing temporary.
//		else {
//			lError=api.memory.DDR_RW_Test(&test_pattern[0],p_StartAddress,testBytes,DDR_PINS_TEST);
//		}
	}
	if (lError != 0 || lError1 != 0) {
		lError = 1;
	}
	
	return lError;
}

U32 Items::TI032()
{
	U32 lError = 1;
	U32 (*pf)() = &DDR_ReadWrite_Test;
	
#ifdef CONFIG_WITHOUT_TIMEOUT
	lError = pf();
#else
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
#endif
	
	return lError;
}
