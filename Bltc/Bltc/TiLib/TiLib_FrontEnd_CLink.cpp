
#include "BltcBasicType.h"
#include "TiLib.h"
#include "TiLib_FrontEnd.h"
#include "EntropicMenu.h"   // From TvMon
#include "EntropicPower.h"  // From TvMon


//##ModelId=472C556C02F0
U32 TiLib_FrontEnd::CLink::CLink_Device_Identify(U16 &pciVendorId, U16 &chipVersion)
{
    BOOL bInit = FALSE;

    /* detect the entropic chip and get the pci base address */
    bInit = EntropicChipInit();

    /* did we find the entropic chip? */
    if ( bInit ) {
		return SUCCESS;
    }
    else {
		return FAILURE;
    }
} 

//##ModelId=472C556C02FF
U32 TiLib_FrontEnd::CLink::CLink_CO_Channel_Tune(U16 CO_ChNum)
{
	EntFreqSet(CO_ChNum);
	EntropicContOutMode();
	
	return 0;
}

//##ModelId=472C556C0301
U32 TiLib_FrontEnd::CLink::CLink_CW_Channel_Tune(U16 CW_ChNum)
{
	EntFreqSet(CW_ChNum);
	EntropicCWmode();
	
	return 0;
}
