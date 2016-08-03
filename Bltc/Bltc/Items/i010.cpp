// Author : Alan

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiApi_Memory.h"

U32 Items::TI010()
{
	U32 lError = TI_SUCCESS;

	lError = api.memory.FLASH_ErasePfdSector(ENTIRE_AREA);
	
	return lError;
}
