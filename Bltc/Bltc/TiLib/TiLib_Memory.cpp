#include "TiLib.h"
#include "TvMonIO.h"

// For NVSRAM_xxx()
#include "addrutil.h"
#include "sdram_db.h"


#include "bchp_rdc.h"

// Follows define, copy from pfd.cpp
#define MTC_SUDB_FLASH		( GetFlashMemBase() | GetKSEG1Base() | 0x1060000 )	// Start of MTC SUDB in flash

#define SUDB_NVMEM_PRIM		( GetNvmemBase() | GetKSEG1Base() + 4 )
#define SUDB_NVMEM_SEC		( SUDB_NVMEM_PRIM + sizeof(StartDB_t) )


extern VOID DmaSramTo325xTransfer(VOID);
extern VOID Dma325xToSramTransfer(VOID);
extern VOID DmaSramToSramTransfer(U32 ulTestNumber);
extern VOID DmaMem0ToMem1or2Transfer(U32);

// For DDR test
U32 DDRCross_TestPoint[MAX_TEST_POINTS] =
{
//  First 32 Mega Start
	0x00000000,0x00000004,0x00000008,
	0x00000010,0x00000020,0x00000040,0x00000080,
	0x00000100,0x00000200,0x00000400,0x00000800,
	0x00001000,0x00002000,0x00004000,0x00008000,
	0x00010000,0x00020000,0x00040000,0x00080000,
	0x00100000,0x00200000,0x00400000,

	0x00800000,0x01000000,0x02000000,0x04000000,
	0x08000000,0x10000000,0x20000000,0x40000000,
	0x80000000
};

#if 0
// For DDR test
U32 DDRCross_TestPoint[MAX_TEST_POINTS] =
{
//  First 32 Mega Start
	0x00000000,0x00000004,0x00000008,
	0x00000010,0x00000020,0x00000040,0x00000080,
	0x00000100,0x00000200,0x00000400,0x00000800,
	0x00001000,0x00002000,0x00004000,0x00008000,
	0x00010000,0x00020000,0x00040000,0x00080000,
	0x00100000,0x00200000,0x00400000,

	0x00800000,0x00800004,0x00800008,
	0x00800010,0x00800020,0x00800040,0x00800080,
	0x00800100,0x00800200,0x00800400,0x00800800,
	0x00801000,0x00802000,0x00804000,0x00808000,
	0x00810000,0x00820000,0x00840000,0x00880000,
	0x00900000,0x00a00000,0x00c00000,

	0x01000000,0x01000004,0x01000008,
	0x01000010,0x01000020,0x01000040,0x01000080,
	0x01000100,0x01000200,0x01000400,0x01000800,
	0x01001000,0x01002000,0x01004000,0x01008000,
	0x01010000,0x01020000,0x01040000,0x01080000,
	0x01100000,0x01200000,0x01400000,

	0x01800000,0x01800004,0x01800008,
	0x01800010,0x01800020,0x01800040,0x01800080,
	0x01800100,0x01800200,0x01800400,0x01800800,
	0x01801000,0x01802000,0x01804000,0x01808000,
	0x01810000,0x01820000,0x01840000,0x01880000,
	0x01900000,0x01a00000,0x01c00000,
// First 32 Mega End

	0x02000000,0x02000004,0x02000008,
	0x02000010,0x02000020,0x02000040,0x02000080,
	0x02000100,0x02000200,0x02000400,0x02000800,
	0x02001000,0x02002000,0x02004000,0x02008000,
	0x02010000,0x02020000,0x02040000,0x02080000,
	0x02100000,0x02200000,0x02400000,

	0x02800000,0x02800004,0x02800008,
	0x02800010,0x02800020,0x02800040,0x02800080,
	0x02800100,0x02800200,0x02800400,0x02800800,
	0x02801000,0x02802000,0x02804000,0x02808000,
	0x02810000,0x02820000,0x02840000,0x02880000,
	0x02900000,0x02a00000,0x02c00000,

	0x03000000,0x03000004,0x03000008,
	0x03000010,0x03000020,0x03000040,0x03000080,
	0x03000100,0x03000200,0x03000400,0x03000800,
	0x03001000,0x03002000,0x03004000,0x03008000,
	0x03010000,0x03020000,0x03040000,0x03080000,
	0x03100000,0x03200000,0x03400000,

	0x03800000,0x03800004,0x03800008,
	0x03800010,0x03800020,0x03800040,0x03800080,
	0x03800100,0x03800200,0x03800400,0x03800800,
	0x03801000,0x03802000,0x03804000,0x03808000,
	0x03810000,0x03820000,0x03840000,0x03880000,
	0x03900000,0x03a00000,0x03c00000,

	0x04000000,0x04000004,0x04000008,
	0x04000010,0x04000020,0x04000040,0x04000080,
	0x04000100,0x04000200,0x04000400,0x04000800,
	0x04001000,0x04002000,0x04004000,0x04008000,
	0x04010000,0x04020000,0x04040000,0x04080000,
	0x04100000,0x04200000,0x04400000,

	0x04800000,0x04800004,0x04800008,
	0x04800010,0x04800020,0x04800040,0x04800080,
	0x04800100,0x04800200,0x04800400,0x04800800,
	0x04801000,0x04802000,0x04804000,0x04808000,
	0x04810000,0x04820000,0x04840000,0x04880000,
	0x04900000,0x04a00000,0x04c00000,

	0x05000000,0x05000004,0x05000008,
	0x05000010,0x05000020,0x05000040,0x05000080,
	0x05000100,0x05000200,0x05000400,0x05000800,
	0x05001000,0x05002000,0x05004000,0x05008000,
	0x05010000,0x05020000,0x05040000,0x05080000,
	0x05100000,0x05200000,0x05400000,

	0x05800000,0x05800004,0x05800008,
	0x05800010,0x05800020,0x05800040,0x05800080,
	0x05800100,0x05800200,0x05800400,0x05800800,
	0x05801000,0x05802000,0x05804000,0x05808000,
	0x05810000,0x05820000,0x05840000,0x05880000,
	0x05900000,0x05a00000,0x05c00000,

	0x06000000,0x06000004,0x06000008,
	0x06000010,0x06000020,0x06000040,0x06000080,
	0x06000100,0x06000200,0x06000400,0x06000800,
	0x06001000,0x06002000,0x06004000,0x06008000,
	0x06010000,0x06020000,0x06040000,0x06080000,
	0x06100000,0x06200000,0x06400000,

	0x06800000,0x06800004,0x06800008,
	0x06800010,0x06800020,0x06800040,0x06800080,
	0x06800100,0x06800200,0x06800400,0x06800800,
	0x06801000,0x06802000,0x06804000,0x06808000,
	0x06810000,0x06820000,0x06840000,0x06880000,
	0x06900000,0x06a00000,0x06c00000,

	0x07000000,0x07000004,0x07000008,
	0x07000010,0x07000020,0x07000040,0x07000080,
	0x07000100,0x07000200,0x07000400,0x07000800,
	0x07001000,0x07002000,0x07004000,0x07008000,
	0x07010000,0x07020000,0x07040000,0x07080000,
	0x07100000,0x07200000,0x07400000,

	0x07800000,0x07800004,0x07800008,
	0x07800010,0x07800020,0x07800040,0x07800080,
	0x07800100,0x07800200,0x07800400,0x07800800,
	0x07801000,0x07802000,0x07804000,0x07808000,
	0x07810000,0x07820000,0x07840000,0x07880000,
	0x07900000,0x07a00000,0x07c00000,
};
#endif

VOID EnableHalDmaCode()
{

    U32  x;
    U32  ulTempData;
    volatile U32* ulTempReg;

	ulTempReg = (U32*) (BCHP_RDC_desc_0_config | 0xB0000000);	// First Reg of 32; 16bytes/4Longs to next reg
	
	for (x=0; x<32;	x++)
	{
		/* Read and disable the DMA Trigger (b26) */
		ulTempData = *ulTempReg;		// Get the current data
		ulTempData |= BCHP_RDC_desc_0_config_enable_MASK;		//  Disable the trigger
		*ulTempReg = ulTempData;		//   Save it
		ulTempReg+=0x04;				// Point to next Descriptor Register (04,14,24,etc...)
	}
}

//*****************************************************************************
//
// FLASH_boot_lock()
//
//		
//		boot sector 1,2,3 protect
//		
//*****************************************************************************
//##ModelId=472A819C03E1
U32 TiLib_Memory::FLASH_boot_protect(FlashDriver *pFlashDriver)
{
	U32 lError = 0;

	if (ProtectBootSector(pFlashDriver) == TRUE) {
		PDEBUG("BootSector Status Protected\r\n");
	}	  	
	else { 
		PDEBUG("BootSector Status UNProtected\r\n");
		lError = 1;
	}		
	
	return lError;
}
//*****************************************************************************
//
// Flash_password_lock()
//
//		
//		Flash passwork lock
//		
//*****************************************************************************
/*
//##ModelId=472A819D0018
U32 TiLib_Memory::FLASH_password_protect(FlashDriver *pFlashDriver)
{
	U32 lError = 0;

	return lError;
}
*/

extern U32 GetDdrSize();

U32 TiLib_Memory::DDR_GetSize_MB()
{
	return GetDdrSize();
}

// Detect floating pin and cross pins error
// Author : Gamma Chen
U32 TiLib_Memory::DDR_PinsTest(U32* testPattern, U32 startAddr, U32 size)
{
// For instance, startAddr = 0b1010-0000-1010-1100-0000-0000-0000-0000 (0xA0AC0000),
//               size      = 0b0000-1111-0101-0100-0000-0000-0000-0000 (0x0F540000),
//               endAddr   = 0b1010-1111-1111-1111-1111-1111-1111-1111 (0xAFFFFFFF)
    U32 endAddr = (U32) (startAddr + size - 1);	
    U32 mask;
    U32 testAddr;
    U32 mostSignificantBits;
    U32 i, j, k, n;
    U32 ulErrorCount = 0;
    U32 ulError = 0;

    lib.rs232.Print("CPU Test Address Range (%.8x - %.8x)\r\n", startAddr, endAddr);
	for (i = 31; i > 0; i--) {
	// Find the first significant bit which startAddr different with endAddr;
		mask = (0xFFFFFFFF << i);
        if ( (startAddr & mask) != (endAddr & mask) )	{
            break;
        }
	}
	n = i;	// n == 27 in this instance
	lib.rs232.Print("Address pin n = %d, for mask\r\n", n);

	DisableHalDmaCode();	// Avoid conflict with OSD (possible trigger exception)

	mostSignificantBits = (endAddr & (0xFFFFFFFF << n));	// mostSignificantBits == 0b1010-10...0
	lib.rs232.Print("Most significant bits (mostSignificantBits) = %.8X, for mask\r\n", mostSignificantBits);
	
	for (i = 3; i <= n; i++) {
		testAddr = (mostSignificantBits | (0x01 << i) );
		*(volatile U32*)testAddr = 0xFFFFFFFF;
		*(volatile U32*)(testAddr+4) = 0xFFFFFFFF;
	}
	for (i = 3; i <= n; i++) {	// For this instance, n == 27
	// U32 address must start from 8 bytes alignment, => begin from i = 3
		for (j = 0; j < MAX_TEST_PATTERN; j++) {
		// In this instance, clear address 0xA8000008, 0xA8000010, ...
			for (k = 3; k <= n; k++) {
				testAddr = (mostSignificantBits | (0x01 << k) );
				*(volatile U32*)testAddr = 0xFFFFFFFF;
				*(volatile U32*)(testAddr+4) = 0xFFFFFFFF;
			}
			
			testAddr = (mostSignificantBits | (0x01 << i) );
			*(volatile U32*)testAddr = *(testPattern+j);
			*(volatile U32*)(testAddr+4) = *(testPattern+j);
			lib.rs232.Print("Write at address %.8X for pattern %.8X%.8X\r\n", testAddr, *(testPattern+j), *(testPattern+j));
			
			for (k = 3; k <= n; k++) {
				testAddr = (mostSignificantBits | (0x01 << k) );
				if (k == i) {
				// For address pin float/cross :
				//   For float pin case :
				//     A. Active High => 1=H
				//        ----------------
				//  pin#  3 2 1 0
				//        | | | |
				//        L H L L
				//        1. pin 2 is floating : when write 0b0100, *0b0100 won't be *(testPattern+j) 
				//        2. pin m (m!=2) is floating : *testAddr will be *(testPattern+j)
				//
				//     B. Active Low => 1=L
				//        ----------------
				//  pin#  3 2 1 0
				//        | | | |
				//        H L H H
				//        1. pin 2 is floating (so, pin 2 is always H) : when write 0b0100, *testAddr won't be *(testPattern+j) 
				//        2. pin m (m!=i) is floating : *testAddr will be *(testPattern+j)
				//   For cross pin case :
				//     A. Active High => 1=H
				//        ----------------
				//  pin#  3 2 1 0
				//        \ | | |
				//          H L L
				//        1. pins 2, 3 are cross : when write to 0b0100, it write to 0b1100 actually; 0b0100 won't be *(testPattern+j)
				//        2. pins 0, 1 are cross : when write to 0b0100, it write to 0b0100 actually; 0b0100 will be *(testPattern+j)
				// 
				//     B. Active Low => 1=L
				//        ----------------
				//  pin#  3 2 1 0
				//        \ | | |
				//          L H H
				//        1. pins 2, 3 are cross : when write to 0b0100, it write to 0b1100 actually; 0b0100 won't be *(testPattern+j)
				//        2. pins 0, 1 are cross : when write to 0b0100, it write to 0b0100 actually; 0b0100 will be *(testPattern+j)
				// 
					if (*(volatile U32*)testAddr != *(testPattern+j) ||
					    *(volatile U32*)(testAddr+4) != *(testPattern+j) ) {
						lib.rs232.Print("Error at testAddr %.8X for the expected test pattern %.8X%.8X, but actually is %.8X%.8X\r\n", testAddr, *(testPattern+j), *(testPattern+j), *(volatile F64*)testAddr, *(volatile F64*)testAddr);
						ulErrorCount++;
					}
				}
				else {
					if (*(volatile U32*)testAddr != 0xFFFFFFFF ||
					    *(volatile U32*)(testAddr+4) != 0xFFFFFFFF) {
						lib.rs232.Print("Error at testAddr %.8X for the expected test pattern %.8X%.8X, but actually is %.8X%.8X\r\n", testAddr, 0xFFFFFFFF, 0xFFFFFFFF, *(volatile F64*)testAddr, *(volatile F64*)(testAddr+4));
						ulErrorCount++;
					}
				}
			}
		}
	}
	
    EnableHalDmaCode();		// Without this, the OSD will dispear
    
    if (ulErrorCount > 0) {
    	ulError = 1;
    }
    else {
    	ulError = 0;
    }


    return ulError;
}

//##ModelId=46EE2706010C
U32 TiLib_Memory::DDR_Cross_Test(U32* testPattern, U32 startAddr, U32 size)
{
    U32 p_EndAddress = (U32) (startAddr | size);	
    U32 p_HeapEndAddress=(( ( (U32)_heap_end | startAddr ) + 7) & ~7); //8 byte alligned start
    U32 ulErrorCount = 0;
    U32 i,i_start,j,k,burn_times;

    // Find out the first suitable test points
    for(i = 0;i < MAX_TEST_POINTS;i++)
    {
        if((startAddr + DDRCross_TestPoint[i]) > p_HeapEndAddress)	
        {
            printf("\r\nTest_Point[%d]=%x",i,DDRCross_TestPoint[i]);
            break;
        }
    }

    // i_start will be the first test point
    i_start = i;
    printf("_heap_end memory %.8x\r\n",_heap_end);
    printf("p_HeapEndAddress memory %.8x\r\n",p_HeapEndAddress);
    printf("memory %.8x (%d KB) ~ %08x, start point at %.8x(%d)\r\n",
    startAddr,size/1024,p_EndAddress,startAddr + DDRCross_TestPoint[i],i);

	DisableHalDmaCode();	// Avoid conflict with OSD (possible trigger exception)

    // Clear all test points
    for(i = i_start;i < MAX_TEST_POINTS;i++) 
    {
		if (startAddr + DDRCross_TestPoint[i] > p_EndAddress) {
			break;
		}
		else {
			*(volatile U32*)(startAddr + DDRCross_TestPoint[i]) = 0xFFFFFFFF;
		}
    }
    // Start RAM Test 
    for(burn_times = 0;(ulErrorCount < MAX_ERROR_ALLOWS) && (burn_times < MAX_BURN_TIMES);burn_times++)
    {
	    for(i = i_start;(ulErrorCount < MAX_ERROR_ALLOWS) && (i < MAX_TEST_POINTS);i++)
          {
			if (startAddr + DDRCross_TestPoint[i] > p_EndAddress) {
				break;
			}
            for(j = 0;(ulErrorCount < MAX_ERROR_ALLOWS) && ( j < MAX_TEST_PATTERN);j++)
            {
                // Check if exceed the end address
                if((startAddr + DDRCross_TestPoint[i]) > p_EndAddress)
                {
                    //printf ("Exceed End\n");
                    break;
                }                    
                
                *(volatile U32*)(startAddr + DDRCross_TestPoint[i]) = *(testPattern+j);
                // Now we start corss test, only the test point we modified
                // will be the test pattern, other address should be all 0xFFFFFFFF
                for(k = i_start;(ulErrorCount < MAX_ERROR_ALLOWS) && (k < MAX_TEST_POINTS);k++)
                {
					if (startAddr + DDRCross_TestPoint[k] > p_EndAddress) {
						break;
					}
                    if(k == i)
                    {
                        if(*(volatile U32*)(startAddr + DDRCross_TestPoint[k]) != *(testPattern+j))
                        {
                            printf("Error list:\n");
                            printf("Burn times is %d\n",burn_times);
                            printf("test address(%x)\n",startAddr + DDRCross_TestPoint[k]);
                            printf("write data(%x)\n",testPattern+j);
                            printf("read data(%x)\n",*(volatile U32*)(startAddr + DDRCross_TestPoint[k]));
                            ulErrorCount++;
                        }
                    }
                    else if(*(volatile U32*)(startAddr + DDRCross_TestPoint[k]) != 0xFFFFFFFF)
                    {
                        printf("Error list:\n");
                        printf("Burn times is %d\n",burn_times);
                        printf("test address(%x)\n",startAddr + DDRCross_TestPoint[k]);
                        printf("not 0xFFFFFFFF and read data(%x)\n",*(volatile U32*)(startAddr + DDRCross_TestPoint[k]));
                        ulErrorCount++;
                    }
                }
            }	

            // Clear the test point 
            *(volatile U32*)(startAddr + DDRCross_TestPoint[i]) = 0xFFFFFFFF;
        }
    }

    if(ulErrorCount >= MAX_ERROR_ALLOWS)
    {
        printf("too many error address\n\r");
    }
    else
    {
        printf("ulErrorCount=%d\n\r",ulErrorCount);
    }
    EnableHalDmaCode();		// Without this, the OSD will dispear


    return ulErrorCount;
}

//##ModelId=46EE271C0004
U32 TiLib_Memory::DDR_Full_Test(U32* testPattern, U32 startAddr, U32 size)
{
	U32 lError = 0;
	
	return lError;
}

//##ModelId=472C556D009E
U32 TiLib_Memory::DMA_DDRToDDR()
{
	U32 lError = 0;
	S8 *ans="?";
	S8 *pattern="?";  

	tvMonIO_clear();
	tvMonIO.out.enable = 1;

	sprintf(pattern,"%s","completed");  

	DmaSramToSramTransfer(0);

	tvMonIO.out.enable = 0;

	tvMonIO_out_getResult(tvMonIO.out._ostr,"DMA Transfer ",ans);      

	(strcmp(ans,pattern)==0)? lError=0:lError=1;

	return lError;
}


U32 TiLib_Memory::DMA_DDR0_To_DDR1()
{
	U32 lError = 0;
	S8 *ans="?";
	S8 *pattern="?";  

	tvMonIO_clear();
	tvMonIO.out.enable = 1;

	sprintf(pattern,"%s","completed");  

	DmaMem0ToMem1or2Transfer(1);

	tvMonIO.out.enable = 0;

	tvMonIO_out_getResult(tvMonIO.out._ostr,"DMA Transfer ",ans);      

	(strcmp(ans,pattern)==0)? lError=0:lError=1;

	return lError;
}

U32 TiLib_Memory::DMA_DDR0_To_DDR2()
{
	U32 lError = 0;
	S8 *ans="?";
	S8 *pattern="?";  

	tvMonIO_clear();
	tvMonIO.out.enable = 1;

	sprintf(pattern,"%s","completed");  

	DmaMem0ToMem1or2Transfer(2);

	tvMonIO.out.enable = 0;

	tvMonIO_out_getResult(tvMonIO.out._ostr,"DMA Transfer ",ans);      

	(strcmp(ans,pattern)==0)? lError=0:lError=1;

	return lError;
}

//##ModelId=4722F865038A
 U32 TiLib_Memory::NVSRAM_CheckMtcSudbInAddr(U8* pSrc, U8* pDest, U16 uwBytes)
{
	U16 x;
	
	for (x=0; x<uwBytes; x++) {
		if (*(pDest+x) != *(pSrc+x)) {
			return 1;
		}
	}
	return 0;
}

//##ModelId=4722F866001F
U32 TiLib_Memory::NVSRAM_CheckMtcSudb() {
	U32 lError = 0;
	
	lError = NVSRAM_CheckMtcSudbInAddr( (U8*)MTC_SUDB_FLASH,(U8*)SUDB_NVMEM_PRIM,sizeof(StartDB_t) );
	if (lError) {
		return lError;
	}
	lError = NVSRAM_CheckMtcSudbInAddr( (U8*)MTC_SUDB_FLASH,(U8*)SUDB_NVMEM_SEC,sizeof(StartDB_t) );
	
	return lError;
}

