// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"

U32 CACHE_ModeChange_Without_Timeout()
{
	U32 lError = 0;
	
	lError = api.memory.CACHE_ModeChangeTest();
	
	return lError;
}

U32 Items::TI014()
{
	U32 lError = 1;
	lError = CACHE_ModeChange_Without_Timeout();
	
	return lError;
}
