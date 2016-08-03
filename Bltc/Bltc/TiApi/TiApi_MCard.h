#ifndef TIAPI_MCARD_H_HEADER_INCLUDED_B904D0C8
#define TIAPI_MCARD_H_HEADER_INCLUDED_B904D0C8

#include "BltcBasicType.h"
#include "Timer_MsTick.h"

//##ModelId=46F364000140
class TiApi_MCard
{
  public:
    //##ModelId=46EBA19F0221
//    int Init();

    //##ModelId=46EBAA2D017F
    U32 PowerOn();
    
    //##ModelId=481125BE01BC
    U32 PowerOff();

    //##ModelId=472A817C03A3
    U32 IsLoaded();
    
    //##ModelId=472C54F3005E
    U32 IsLoadedCardVersion();
    
    //##ModelId=481125BE01BD
    U32 OOBDownStreamTest(U32 rfFreqHz);
    
// Take Power On/Off MCard automatically by check MCard Attached/Detached for every 100ms.
	//##ModelId=481125BE01CC
    void AutoPowerOnOff();
    
  private:
	S32 _mCardStatus;
	Timer_MsTick _ms;
	S32 _count_ms;
};

#endif /* TIAPI_MCARD_H_HEADER_INCLUDED_B904D0C8 */
