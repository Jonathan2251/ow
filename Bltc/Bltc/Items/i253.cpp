/*****************************************************************************
Module:			Control Item 253
Funtion:		delay_100ms(x)	// ex. delay_100ms(20) => delay 2 seconds
Parameter:		None
Return:			None
Author:			Gamma Chen
*****************************************************************************/

#include "ModelConfig.h"
#include "TiLib.h"
#include "Items.h"

void Items::CI253(U32 arg)
{
	if (arg != 0) {
		lib.DelayMS(arg*100);
	}
	
	return;
}
