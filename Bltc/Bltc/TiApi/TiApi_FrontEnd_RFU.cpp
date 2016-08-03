#include "TiApi_FrontEnd.h"
#include "TiLib.h"

//##ModelId=472C68E002A1
int TiApi_FrontEnd::RFU::RFU_Init()
{
	return 0;
}

//##ModelId=472C5E740282
U32 TiApi_FrontEnd::RFU::Calibration(U32 freq, U32 powerLevelDbmv, float *foffset)
{
	return 0;
}

//##ModelId=472C5E740292
U32 TiApi_FrontEnd::RFU::TestPower(U32 freq, U32 powerLevelDbmv)
{
	return 0;
}

//##ModelId=472C5E7402A1
U32 TiApi_FrontEnd::RFU::ClosePower()
{
	return 0;
}

//##ModelId=472C5E7402A2
U32 TiApi_FrontEnd::RFU::SaveOffsetToNvsram(float offset)
{
	return 0;
}

//##ModelId=472C5E7402B0
U32 TiApi_FrontEnd::RFU::SaveOffsetToPFD(float offset)
{
	return 0;
}
