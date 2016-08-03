#ifndef TILIB_MEMORY_H_HEADER_INCLUDED_B9049DF3
#define TILIB_MEMORY_H_HEADER_INCLUDED_B9049DF3

#include "BltcBasicType.h"
#include "flash.h"

//##ModelId=48114D270104
enum FLASH_ErrCode {
	BL_FLASH_SUCCESS=0,
	BL_FLASH_WRITE_ERROR,
	BL_FLASH_WRITE_BUFFER_ERROR,
	BL_FLASH_ERASE_ERROR,
	BL_FLASH_UNLOCK_ERROR,
	BL_FLASH_LOCK_ERROR,
	BL_FLASH_ERROR_UNKNOWN
};

//##ModelId=472C7588022E
struct ImageCheck {
    //##ModelId=472C75880366
	U8	flashType;
    //##ModelId=472C75880374
	U8	imageType;
    //##ModelId=472C75880384
	U32	srcAddr;
    //##ModelId=472C75880394
	U32	destAddr;
    //##ModelId=472C75880395
	U32	size;
};

// For DDR Test :Add start
#define DDR_CROSS_TEST          0
#define DDR_FULL_TEST           1
#define DDR_PINS_TEST          	2
#define	SIZE_EIGHT_MEGA     0x00800000
#define MAX_ERROR_ALLOWS    10
#define MAX_BURN_TIMES	    3
#define MAX_TEST_PATTERN    3
//#define MAX_TEST_POINTS     88*4        // 128 Mega Btyes
#define MAX_TEST_POINTS     31
extern S8	_heap_end[];
extern void DisableHalDmaCode();
// Add end

/************************* Externs ***********************************/
extern BOOL ProtectBootSector( FlashDriver *pFlashDriver );
extern VOID EnableHalDmaCode();

//##ModelId=46E8F17103E2
class TiLib_Memory
{
  private:
    //##ModelId=4722F865038A
  	 U32 NVSRAM_CheckMtcSudbInAddr(U8* pSrc, U8* pDest, U16 uwBytes);
  	
  public:

    //##ModelId=472A819C03E1
    U32 FLASH_boot_protect(FlashDriver *pFlashDriver);
    
    //##ModelId=472A819D0018
//    U32 FLASH_password_protect(FlashDriver *pFlashDriver);
    
    U32 DDR_GetSize_MB();
    U32 DDR_PinsTest(U32* testPattern, U32 startAddr, U32 size);
    
    //##ModelId=46EE2706010C
    U32 DDR_Cross_Test(U32* testPattern, U32 startAddr, U32 size);

    //##ModelId=46EE271C0004
    U32 DDR_Full_Test(U32* testPattern, U32 startAddr, U32 size);

    //##ModelId=472C556D009E
    U32 DMA_DDRToDDR(); //add by dolphin
    
    U32 DMA_DDR0_To_DDR1();
    U32 DMA_DDR0_To_DDR2();
    
    //##ModelId=4722F866001F
	U32 NVSRAM_CheckMtcSudb();
};



#endif /* TILIB_MEMORY_H_HEADER_INCLUDED_B9049DF3 */
