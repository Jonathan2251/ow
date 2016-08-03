// Author : Stony Tai

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"

U32 MCard_IsLoadedCardVersion()
{
	U32 lError = 0;

	lError = lib.mCard.DoInit();
	if (lError) {
		PDEBUG("Init M Card fail\r\n");
		goto out;
	}
	lError = api.mCard.IsLoadedCardVersion();
	
out:
	return lError;
}

U32 Items::TI073()
{
	U32 lError = 1;
	U32 (*pf)() = &MCard_IsLoadedCardVersion;
	
	int timeOut = 20;	// unit:1s, => 20 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
