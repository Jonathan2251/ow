// **********************************************************
// Author : Dolphin
// **********************************************************
#include "TiLib_Chip.h"
#include "board_id.h"
#include "PCI.h"
#include "addrutil.h"
#include "TvMonIO.h"
#include "TiLib.h"
#include "userio.h"
#include "NxpTuner.h"
#include "Mot7400_TunerController.h"


extern U8 GetVersion();
extern VOID OHCITest(VOID);

//##ModelId=472C69830025
U32 TiLib_Chip::GetMainChip(U32 regAddr)
{
  U32 MainChip_Version;
  
  MainChip_Version=*((U32*)regAddr);

  return MainChip_Version;
}

//##ModelId=472C69830027
U32 TiLib_Chip::GetPCIDev(PeripheralChip chip)
{
    PciDeviceInfo pDINFO;
    U32 PCI_Device;
    
    switch (chip)
    { 
     case CT_DEMOD:
//           PCI_Device=IDSEL_BCM3250; 
           PCI_Device=IDSEL_BCM3517; 
           break;
           // PCI_Device=IDSEL_BCM3250;    
     case CT_1394CHIP:
           PCI_Device=IDSEL_1394;      
           break;
     
    }
    
	if (GetPciDeviceInfo(PCI_Device,&pDINFO))
	{
   	  printf("PCI Vendor ID 0x%08X\r\n",pDINFO.ulVendorId);

  	  return (pDINFO.ulVendorId);
  	}
  	else 
  	  return FAILURE;//return fail
	
}

//##ModelId=472C69830035
U32 TiLib_Chip::GetI2CDev(PeripheralChip chip)
{
    U8 PSoC_FW_Ver;
    
    switch (chip)
    { 
        case  CT_PSOC_FAN://Get PSoC FW version
           PSoC_FW_Ver=GetVersion(); 
           break;

  /*    case   CT_PSOC_FP:
            
           break;*/
     }
     return PSoC_FW_Ver;
}

U8 TiLib_Chip::GetTunerID(U8 tuner)
{
	unsigned long ulError=TUNER_OK;
	U8			ucRegBuffer[10];
	U8			ucNumRegs=1;
	U8			ucStartReg=0;
	
	if (tuner > 1) {
		return 0x00;
	}
	if (tuner == 1 && !lib.IsProductModel("QIP72XXP2")) {
		return 0x00;
	}
	ucRegBuffer[0] = 0x00;
	// Get tuner regs 0
	ulError = NxpGetTunerReg((unsigned int)tuner,ucStartReg,ucNumRegs,ucRegBuffer);
	if (ulError != TUNER_OK) {
		DB_PRINTF(( "Error obtaining NXP Tuner %d Register data\r\n", tuner+1 ));
		ucRegBuffer[0] = 0x00;
	}
	else {
		PDEBUG("Tuner %d ID = 0x%2x\r\n", tuner+1, ucRegBuffer[0]);
		if (ucRegBuffer[0] == 0xD6) {
			ucRegBuffer[0] = 0x00;
		}
	}
	
	return ucRegBuffer[0];
}

//##ModelId=472C69830037
U32 TiLib_Chip::GetBoardID()
{
//	U32 lError = 0;
	U32 BoardID = DetermineHardwareBoardId(TRUE);
 	
	return BoardID;
}

//Add by dolphin
//##ModelId=472C69830038
U32 TiLib_Chip::OHCI_Test()
{
  U32 lError = 0;
  S8 ans[10]="?";
  S8 pattern[10]="passed";  
  
  tvMonIO_clear();
  tvMonIO.out.enable = 1;
   
  OHCITest();

  tvMonIO.out.enable = 0;

  tvMonIO_out_getResult(tvMonIO.out._ostr,"Config Cycle ",ans);      

  (strcmp(ans,pattern)==0)? lError=0:lError=1;
   
  return lError; 
  
}
