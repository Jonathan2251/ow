// Author : Dolphin

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"
#include "Bltc.h"

U32 TunerID_Check()
{
	U32 lError = 1;
	U8 tunerId;
	
	tunerId = lib.chipID.GetTunerID(0);
	if (tunerId == 0x00) {
		lError = 0;
	}
	if (lib.IsProductModel("QIP72XXP2")) {
		tunerId = lib.chipID.GetTunerID(1);
		if (tunerId == 0x00) {
			lError = 0;
		}
		else {
			lError = 1;
		}
	}
		
	return lError;
}

U32 Items::TI020()
{
	U32 lError = 1;
	U32 (*pf)() = &TunerID_Check;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
