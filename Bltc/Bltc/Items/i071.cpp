// Author : Stony Tai

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 MCard_IsLoaded()
{
	bool mCardLoaded;
	U32 lError = 0;

	mCardLoaded = api.mCard.IsLoaded();
	if (mCardLoaded) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}

U32 Items::TI071()
{
	U32 lError = 1;
	U32 (*pf)() = &MCard_IsLoaded;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
