#ifndef TIAPI_MEMORY_H_HEADER_INCLUDED_B9049FBC
#define TIAPI_MEMORY_H_HEADER_INCLUDED_B9049FBC

#include "TiLib_Memory.h"

enum DmaMemId {
	DDR_BCM3520 = 0, DDR0, DDR1, DDR2
};

//##ModelId=481125BE0017
enum PFD_AREA
	{ 	ENTIRE_AREA, FIRST8K_AREA, SECOND8K_AREA, TSI_AREA,
		FIVEC_AREA, HDCP_AREA, DDPC_AREA, FIRST_16KB, SECOND_16KB
	}; //+alan 

//##ModelId=46EDD6EB03AD
class TiApi_Memory
{
  public:
    //##ModelId=46EB4693026B
    U32 FLASH_IDGet(U16* flashID);
    
    U32 FLASH_Check();

    //##ModelId=472A817C03E1
    U32 FLASH_Protect(void);
    
    //##ModelId=481125BE021A
    bool FLASH_IsPFDAreaErased();
    
    //##ModelId=472A817D000A
    U32 FLASH_ErasePfdSector(U8 area);
    
    //##ModelId=472A817D0028
    U32 FLASH_dump_content(U32 *startAddr,U32 *endAddr);
    
    //##ModelId=481125BE021B
    U32 FLASH_TCMTC_SudbCrcCheck();
    
    //##ModelId=46EB469303B4
    U32 DDR_RW_Test(U32* testPattern, U32 startAddr, U32 size, U32 Method);

    //##ModelId=46EB46940087
    U32 DDR_MaxSizeDetect(S8* productName, U8* getSize);

    //##ModelId=46EB46940124
    U32 DMA_DDRToPCITransfer(DmaMemId pciDev);
    
    //##ModelId=472C54F300BC
    U32 DMA_DDRToDDRTransfer();
    
    //##ModelId=4722F851032C
    U32 CACHE_ModeChangeTest();

    //##ModelId=472A817D0057
    U32 NVSRAM_retention_write_test(U8 pattern);
    
    //##ModelId=472A817D0066
    U32 NVSRAM_retention_read_test(U8 pattern);
    
    //##ModelId=4722F851033B
	U32 NVSRAM_CopyMtcSudbToNvram();
    //##ModelId=4732832A0031
    U32 NVSRAM_SetupNvram();
};



#endif /* TIAPI_MEMORY_H_HEADER_INCLUDED_B9049FBC */
