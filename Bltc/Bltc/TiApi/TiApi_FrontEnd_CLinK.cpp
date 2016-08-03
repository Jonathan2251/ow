
//##ModelId=472C54F2039A
#include "TiApi_FrontEnd.h"
#include "TiLib.h"

U32 TiApi_FrontEnd::CLink::DeviceProperty_Identify(void)
{    
	U16 pciVendorId;
	U16 chipVersion;
	U32 lError = 0;

	if (lib.frontEnd.entropic.CLink_Device_Identify(pciVendorId, chipVersion) == SUCCESS) {
		lib.rs232.Print("Entropic PCI Vendor ID=%.4x\r\n", pciVendorId);
		lib.rs232.Print("Entropic Chip Version=%.4x\r\n", chipVersion);          
		lError = 0;
	}
	else {
        PDEBUG("Unable to detect Entropic chip\r\n");         
		lError = 1;
	}
	
	return lError;
} 

//##ModelId=472C54F2039C
U32 TiApi_FrontEnd::CLink::ContinuousOutput_ChannelSet(U16 CO_ChNum)
{   
	U32 lError = 0;

	if(DeviceProperty_Identify() != SUCCESS) {
		lError = 1;
	}
	else {
		lib.frontEnd.entropic.CLink_CO_Channel_Tune(CO_ChNum);
		lError = 0;
	}
	
	return lError;
} 

//##ModelId=472C54F203AB
U32 TiApi_FrontEnd::CLink::ContinuousWave_Test( U16 CW_ChNum)
{
	U32 lError = 0;

	if(DeviceProperty_Identify() != SUCCESS) {
		lError = 1;
	}
	else {
		lib.frontEnd.entropic.CLink_CW_Channel_Tune(CW_ChNum);
		lError = 0;
	}
	
	return lError;
} 
      
 