// Author : Gamma Chen

#ifndef UI_STATIONTEST_H_HEADER_INCLUDED_B904A538
#define UI_STATIONTEST_H_HEADER_INCLUDED_B904A538
#include "Station.h"
#include "UserInterface.h"

//##ModelId=46FA1A7302E8
class UI_StationTest : public UserInterface
{
  public:
    //##ModelId=48102AF102A9
	UI_StationTest(UI_OutputMsg *outMsg);
    //##ModelId=46FA2134003A
    void Run();
    //##ModelId=46FCDA4F010A
    U32 CreateStation(int stationId);
    //##ModelId=4705B0F7017B
    Station* CurrStation();
  private:
    //##ModelId=46FA1A8C0089
    Station* _station;
};



#endif /* UI_STATIONTEST_H_HEADER_INCLUDED_B904A538 */
