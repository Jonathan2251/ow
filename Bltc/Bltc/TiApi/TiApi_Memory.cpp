#include "TiApi_Memory.h"
#include "TiLib.h"

#include "vr5432.h"
#include "misc.h"
#include "userio.h"

// For NVSRAM_xxx()
#include "addrutil.h"
#include "cfi.h"
#include "flash.h"
#include "flashmanager.h"
#include "mainmenumgr.h"

extern VOID CopySudbStructures(FlashDriver *pFlashDriver);

extern "C" asm U32 SetCacheCoherency(U32 Coherency);

extern VOID OneStepPasswordUnlocking(FlashDriver *pFlashDriver);
extern BOOL UnProtectPFDSector(FlashDriver *pFlashDriver);
extern BOOL EraseSelectedPfdArea( FlashDriver *pFlashDriver, U16 *pBaseAddress,PFD_AREA Location );
extern BOOL GetPFDSectorProtectStatus(FlashDriver *pFlashDriver);
extern FLASH_ERROR UnlockFlashUsingPassword(FlashDriver *pFlashDriver);
extern FLASH_ERROR InitWritingFlashPassword(FlashDriver *pFlashDriver);
extern U16 GetSectorProtect(U32 Sector);
extern BOOL SetSpansionFreezeLockBit(FlashDriver *pFlashDriver);
extern BOOL SetUpNVRAM();

//##ModelId=46EB469303B4
U32 TiApi_Memory::DDR_RW_Test(U32* testPattern, U32 startAddr, U32 size, U32 Method)
{
	U32 ulErrCount = 0;
	U32 ulError = 1;

	if (Method == DDR_CROSS_TEST) {
		ulErrCount = lib.memory.DDR_Cross_Test(testPattern, startAddr, size);
		if (ulErrCount == 0) {
			ulError = 0;
		}
	}
	else if (Method == DDR_FULL_TEST) {
		ulError = lib.memory.DDR_Full_Test(testPattern, startAddr, size);
	}
	else if (Method == DDR_PINS_TEST) {
		ulError = lib.memory.DDR_PinsTest(testPattern, startAddr, size);
	}
	
	return ulError;
}

//##ModelId=46EB46940124
U32 TiApi_Memory::DMA_DDRToPCITransfer(DmaMemId pciDev)
{
	U32 lError = 1;

	switch (pciDev)
	{
	case DDR1:
		lError = lib.memory.DMA_DDR0_To_DDR1();
		break;
	case DDR2:
		lError = lib.memory.DMA_DDR0_To_DDR2();
		break;
	}

	return lError;
}


//Add by Dolphin
//##ModelId=472C54F300BC
U32 TiApi_Memory::DMA_DDRToDDRTransfer()
{
	U32 lError = 0;

	lError = lib.memory.DMA_DDRToDDR();

	return lError;
}

U32 SetCacheMode_R()
{
	U32 reg;
	U32 (*pX)(void) = SetCacheMode_R;
	U32 ExecAddr = (U32)pX;
	
	SetCacheCoherency(K0_WT);
	reg = Get_CP0_Register(CP0_CONFIG);
	if ((reg & 0x7) != K0_WT) {
		return 1;
	}
	SetCacheCoherency(K0_UN);
	reg = Get_CP0_Register(CP0_CONFIG);
	if ((reg & 0x7) != K0_UN) {
		return 1;
	}
	SetCacheCoherency(K0_ACC);
	reg = Get_CP0_Register(CP0_CONFIG);
	if ((reg & 0x7) != K0_ACC) {
		return 1;
	}		
	SetCacheCoherency(K0_WB);
	reg = Get_CP0_Register(CP0_CONFIG);
	if ((reg & 0x7) != K0_WB) {
		return 1;
	}
	return 0;
}

//##ModelId=4722F851032C
U32 TiApi_Memory::CACHE_ModeChangeTest()
{
	U32 lError = SetCacheMode_R();
	
	return lError;
}

//##ModelId=46EB4693026B
U32 TiApi_Memory::FLASH_IDGet(U16 *flashID)
{
	U32 lError=0;
	FlashDriver	*pFlashDriver=NULL;

	if (InitializeFlashDriverArray(&pFlashDriver) == TRUE){
			
		PDEBUG("Flash driver init OK\r\n");
		*flashID=pFlashDriver->GetDeviceId();
	}
	else {
			
		PDEBUG("ERROR...Flash Driver Not Installed...\r\n");
		lError=1;
	}
	
	//PDEBUG("ID=%x \n\r",*flashID);	
	return lError;
		
}

U32 TiApi_Memory::FLASH_Check()
{
	char answer[100];
	U32 lError=0;
	FlashDriver	*pFlashDriver=NULL;
	
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	if (InitializeFlashDriverArray(&pFlashDriver) != TRUE) {
		PDEBUG("ERROR...Flash Driver Not Installed...\r\n");
		tvMonIO_clear();
		tvMonIO.out.enable = 0;
		lError = 1;
		return lError;
	}
	PDEBUG("Flash driver init OK\r\n");
	
	tvMonIO_out_getResult(tvMonIO.out._ostr, "Device Type = ", answer);
	if (strncmp(answer, "S29GL512N", strlen("S29GL512N")) != 0) {
		tvMonIO_clear();
		tvMonIO.out.enable = 0;
		lError = 1;
		return lError;
	}
	tvMonIO_out_getResult(tvMonIO.out._ostr, "Device Id = ", answer);
	if (strncmp(answer, "2301", strlen("2301")) != 0) {
		tvMonIO_clear();
		tvMonIO.out.enable = 0;
		lError = 1;
		return lError;
	}
	tvMonIO_out_getResult(tvMonIO.out._ostr, "EraseRegion0.NumberOfBlocks = ", answer);
	if (strncmp(answer, "512", strlen("512")) != 0) {
		tvMonIO_clear();
		tvMonIO.out.enable = 0;
		lError = 1;
		return lError;
	}
	tvMonIO_out_getResult(tvMonIO.out._ostr, "EraseRegion0.BlockSize      = ", answer);
	if (strncmp(answer, "131072", strlen("131072")) != 0) {
		tvMonIO_clear();
		tvMonIO.out.enable = 0;
		lError = 1;
		return lError;
	}
	PDEBUG("Full Device Id = %s\r\n", "7E2301");
	
	return lError;
}

//*****************************************************************************
//
// FLASH_Protect()
//
//		1.sector 0,1,..,7 protect
//		2.flash password protect
//		
//*****************************************************************************

//##ModelId=472A817C03E1
U32 TiApi_Memory::FLASH_Protect(void)
{
	U32 lError = 0;
	FlashDriver	*pFlashDriver = NULL;
	FLASH_ERROR FlashStatus = FLASH_OK;

	if (InitializeFlashDriverArray(&pFlashDriver) == TRUE) {
		PDEBUG("Flash driver init OK\r\n");			
	}
	else {
		PDEBUG("ERROR...Flash Driver Not Installed...\r\n");
		return lError = 1;
	}
	FlashStatus = UnlockFlashUsingPassword(pFlashDriver); //unlock flash with password
	
	if (FlashStatus == FLASH_PASSWORD_UNLOCKED) {
		PDEBUG("Flash unlock is OK\r\n");
	}		
	else {
		PDEBUG("Flash unlock is fail\r\n");		
	}
	// protect  sector 0,1,..,7 : boot code in 0..6 and pfd in 7
	FlashStatus = pFlashDriver->ProtectSectors(0,7);
	if (FlashStatus == FLASH_OK) {
			PDEBUG("Sectors 0 to 7 Status Protected\r\n");
	}
	else { 
			PDEBUG("Sectors 0 to 7 Status not all UNProtected\r\n");
			return lError=1;
	}
	
	// flash password protect
	FlashStatus = InitWritingFlashPassword(pFlashDriver);	// lock flash with password
	#if 0
	if (FlashStatus) {
		PDEBUG("Flash write password failed \r\n");
		lError = 1;
	}		  	
	else {
		PDEBUG("Flash write password successful\r\n");
	}
	#endif 		
			
	if (SetSpansionFreezeLockBit(pFlashDriver) == TRUE) {   // re-lock flash
		PDEBUG("Flash lock is OK\r\n");
	}
	else {
		PDEBUG("Flash unlock is fail\r\n");		
		lError = 1;
	}		
	
	return lError;
}

//##ModelId=481125BE021A
bool TiApi_Memory::FLASH_IsPFDAreaErased()
{
	U32 uwStartOffset = FLASH_BASE_ADDRESS+0x000E0000;
	U32 uwEndOffset = uwStartOffset + 3*SIZE_16KB;
	bool erased = true;
	U16 *p;

	for (p = (U16*)uwStartOffset; p < (U16*)uwEndOffset; p++)
	{
		if (*p != 0xFFFF) {
			erased = false;
			return erased;
		}
	}
	
	return erased;
}

//*****************************************************************************
//
//FLASH_ErasePfdSector()
//
//		1.check pfd sector unlock with password
//		2.get pfd status (Unprotect or none)
//		3.erase pfd sector
//		.
//		
//*****************************************************************************

//##ModelId=472A817D000A
U32 TiApi_Memory::FLASH_ErasePfdSector(U8 area)
{
	S8 *erase_pfd_confirm[] = {
		"Y",				// Are you sure you want to continue? Enter Y or N.
		"\0"};
	U32 lError = 0;
	FlashDriver	*pFlashDriver = NULL;
	U16	uwProtectStatus;
	FLASH_ERROR FlashStatus = FLASH_OK;

	if (InitializeFlashDriverArray(&pFlashDriver) == TRUE){
		PDEBUG("Flash driver init OK\r\n");
	}
	else {
		PDEBUG("ERROR...Flash Driver Not Installed...\r\n");
		return lError = 1;
	}	
	//OneStepPasswordUnlocking(pFlashDriver);
	//FlashStatus = UnlockFlashUsingPassword(pFlashDriver);
	
	//PDEBUG("FlashStatus:%x\r\n",FlashStatus);
	//return lError=1;
	
	FlashStatus = UnlockFlashUsingPassword(pFlashDriver);//unlock flash with password
	if (FlashStatus == FLASH_PASSWORD_UNLOCKED) {
		PDEBUG("Flash unlock is OK\r\n");
	}
	else {
		PDEBUG("Flash unlock is fail\r\n");
	}	
	lib.DelayMS(1000);
	
	//get pfd status
	if (GetPFDSectorProtectStatus(pFlashDriver) == TRUE) {
		PDEBUG("PFDSector Status Protected\r\n");
	
		// unprotect pfd sector
		if (UnProtectPFDSector(pFlashDriver) == TRUE) {
			PDEBUG("PFDSector Status Protected\r\n");
			//return lError=1;?? tvmon maybe have problem
		}		
		else {
			PDEBUG("PFDSector Status UNProtected\r\n");
		}
	}
	else {
		PDEBUG("PFDSector Status UNProtected\r\n");	
	}
	
	lib.DelayMS(1000);
	// erase pfd sector
	tvMonIO_clear();
	tvMonIO_in_addCmds(erase_pfd_confirm);
	if (EraseSelectedPfdArea(pFlashDriver,(U16*)FLASH_BASE_ADDRESS,(PFD_AREA)area) == TRUE) {
	 	PDEBUG("Entire PFD Data Area Erased\r\n");
	}
	else {
		PDEBUG("Entire PFD Data Area NOT Erased\r\n");
		return lError = 1;
	}		
			
	//FlashStatus = InitWritingFlashPassword(pFlashDriver); // lock flash with password
				
	lib.DelayMS(1000);		
	// protect  sector 0 to 7
	FlashStatus=pFlashDriver->ProtectSectors(0,7);
	if (FlashStatus == FLASH_OK)
		PDEBUG("Sectors 0 to 7 Status Protected\r\n");
		  	
	else { 
		PDEBUG("Sectors 0 to 7 Status not all Protected\r\n");
		return lError = 1;
	}
	
	if (SetSpansionFreezeLockBit(pFlashDriver) == TRUE)   // re-lock flash
		PDEBUG("Flash lock is OK\r\n");
	else {
		PDEBUG("Flash unlock is fail\r\n");		
		return lError = 1;
	}

	for (int i = 0; i < 8; i++) {
		uwProtectStatus = pFlashDriver->GetSectorProtect(i);
		if (uwProtectStatus != 1) {
			return lError = 1;
		}
	}

	return lError;
}

//*****************************************************************************
//
// :FLASH_dump_content()
//
//		For debug, dump contents of Flash by user input address range
//		.
//		
//*****************************************************************************
//##ModelId=472A817D0028
U32 TiApi_Memory::FLASH_dump_content(U32 *pStartAddr,U32 *pEndAddr)
{
	U32 lError = 0;
	U32 ulCounter;
	U32 ulNumberOfWordsToDump;

	ulNumberOfWordsToDump = pEndAddr - pStartAddr +1;
	lib.rs232.Print("Start Address = 0x%x  :  End Address = 0x%x\r\n",pStartAddr,pEndAddr);
	
	for (ulCounter=0; ulCounter<ulNumberOfWordsToDump; ulCounter++)	{
		if ((ulCounter!=0)&&(ulCounter%4)) {
			lib.rs232.Print("%08x    ",*(pStartAddr+ulCounter));
		}
		else {
			lib.rs232.Print("\r\n0x%x  %08x    ",(pStartAddr+ulCounter),*(pStartAddr+ulCounter));
		} 
	}
	
	lib.rs232.Print("\r\n");
	
	return lError;
}


// Follows duplicate defined in pfd.cpp
//##ModelId=481125BE0008
enum Application
	{ BOOTLOADER, MTC, TC, BLTC, PRIMARY };
// Defined in pfd.cpp
extern BOOL VerifySudbImageAreas(Application App,BOOL bVerbose);	

	
//##ModelId=481125BE021B
U32 TiApi_Memory::FLASH_TCMTC_SudbCrcCheck()
{
	U32 lError = 0;
	FlashDriver	*pFlashDriver=NULL;
	BOOL MTC_OK, TC_OK;

	if (InitializeFlashDriverArray(&pFlashDriver) == TRUE){
		PDEBUG("Flash driver init OK\r\n");
	}
	else {
		PDEBUG("ERROR...Flash Driver Not Installed...\r\n");
		return lError = 1;
	}
	
	if (MTC_OK = VerifySudbImageAreas(MTC,TRUE)) {
		DB_PRINTF((" MTC Sudb/Image Verification : PASSED\r\n"));
	}
	else {
		DB_PRINTF((" MTC Sudb/Image Verification : FAILED\r\n"));
	}

	if (TC_OK = VerifySudbImageAreas(TC,TRUE)) {
		DB_PRINTF((" TC Sudb/Image Verification : PASSED\r\n"));
	}
	else {
		DB_PRINTF((" TC Sudb/Image Verification : FAILED\r\n"));
	}
				
	if (MTC_OK && TC_OK) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

//*****************************************************************************
//
// NVSRAM_retention_write_test()
//
//		NVSRAM data retention (write) test
//		.
//		
//*****************************************************************************

//##ModelId=472A817D0057
U32 TiApi_Memory::NVSRAM_retention_write_test(U8 pattern)
{
	U32 lError = 0;
	U32	p_StartAddress = 0;
	U32	p_EndAddress = 0;
	volatile U8 *p_TestAddress;
	U8 temp = 0;

	p_StartAddress = (GetKSEG1Base() | GetNvmemBase());
	p_EndAddress = (p_StartAddress + GetNvmemSize())- sizeof(U8);
	PDEBUG("NVSRAM data retention test (write) (pattern = 0x55)\n\r");
	
    p_TestAddress = (volatile U8 *)(p_StartAddress) + 0x2F000; //skip PFD data area;
    
    //write pattern to nvsram
	while((U32)	p_TestAddress <	p_EndAddress ) {
		*p_TestAddress = pattern;
		p_TestAddress++;
	}
    p_TestAddress = (volatile U8 *)(p_StartAddress) + 0x2F000; //skip PFD data area
    
    // read back from nvsram
    while((U32)	p_TestAddress <	p_EndAddress) {
    	temp = *p_TestAddress;
		if (temp != pattern) {
			PDEBUG("Address:%x data:%02x\n\r",p_TestAddress,temp);
			lError = 1;
		}
		p_TestAddress++;
	}
	
	return lError;
					
}
//*****************************************************************************
//
// NVSRAM_retention_read_test()
//
//		NVSRAM data retention (read) test
//		.
//		
//*****************************************************************************

//##ModelId=472A817D0066
U32 TiApi_Memory::NVSRAM_retention_read_test(U8 pattern)
{
	U32 lError = 0;
	U32	p_StartAddress = 0;
	U32	p_EndAddress = 0;
	volatile U8 *p_TestAddress;
	U8 temp = 0;

	p_StartAddress = (GetKSEG1Base() | GetNvmemBase());
	p_EndAddress = (p_StartAddress + GetNvmemSize())- sizeof(U8);
	PDEBUG("NVSRAM data retention test (read) (pattern = 0x55)\n\r");
	
    p_TestAddress = (volatile U8 *)(p_StartAddress) + 0x2F000; //skip PFD data area
    
    //read data from nvsram
	while((U32)	p_TestAddress <	p_EndAddress) {
    	temp = *p_TestAddress;
		if (temp != pattern) {
			PDEBUG("Address:%x data:%02x\n\r",p_TestAddress,temp);
			lError = 1;
		}
		p_TestAddress++;
	}
	
	return lError;
}

//##ModelId=4722F851033B
U32 TiApi_Memory::NVSRAM_CopyMtcSudbToNvram()
{
	char* p = 0;
	FlashDriver	*pFlashDriver = NULL;
	FLASH_ERROR FlashStatus = FLASH_ERROR_UNKNOWN;
	U32 lError = 0;
	
	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	if (InitializeFlashDriverArray(&pFlashDriver) == TRUE) {
	 // Copy the MTC SUDB from FLASH to NVMEM
		CopySudbStructures(pFlashDriver);
		tvMonIO.out.enable = 0;
		p = strstr(tvMonIO.out._ostr, " MTC Sudb/Image Verification : PASSED");
		if (p != 0) {
			lError = 0;
		}
		else {
			lError = 1;
		}
//		lError = lib.memory.NVSRAM_CheckMtcSudb();
	}
	else {
		lError = 1;
	}

return_result:
	/* Remove any Flash Drivers that were Installed */
	UnInitializeFlashDriverArray( pFlashDriver );
	
	return lError;
}


//##ModelId=4732832A0031
U32 TiApi_Memory::NVSRAM_SetupNvram()
{
	char *setup_nvsram[] = {
//		"pfd",  	// enter from TvMon menu to pfd menu
//		"g",		//
		"1",		//
//		"/", 		// exit pfd menu
//		"exit",		// exit TvMon menu
		"\0"};
	BOOL success;
	U32 lError;
	
	tvMonIO_clear();
	tvMonIO_in_addCmds(setup_nvsram);
	tvMonIO.out.enable = 1;
	success = SetUpNVRAM();
	tvMonIO.out.enable = 0;
	if (success == TRUE) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}
