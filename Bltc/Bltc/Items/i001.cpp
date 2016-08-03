// Author : Gamma Chen

#ifdef CONFIG_WITH_TVMON

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"


U32 Items::TI001()
{
	U32 lError = TI_SUCCESS;
	
	api.EnterToTvMonMenu();
	
	return lError;
}

#endif	// CONFIG_WITH_TVMON
