#ifndef TILIB_PERIPHERIAL_H_HEADER_INCLUDED_B904E3B3
#define TILIB_PERIPHERIAL_H_HEADER_INCLUDED_B904E3B3

#include "BltcBasicType.h"
#include "Observer.h"
#include "Subject.h"

//##ModelId=472C698202E5
class TiLib_Peripheral : public Observer
{
  public:
    //##ModelId=472C6983011F
    U32 USB_CheckVendorDeviceId(S8* str);

    //##ModelId=472C69830121
    U32 AC_Outlet_Init();

    //##ModelId=472C69830122
    U32 AC_Outlet_Status(bool on);

    //##ModelId=48114D2703C3
	void WatchDog_Disable();
	void WatchDog_Enable();
	void WatchDog_Serve();
    void Update(Subject *theChangedSubject);
    
  private:
    int _timeInterval;
};



#endif /* TILIB_PERIPHERIAL_H_HEADER_INCLUDED_B904E3B3 */
