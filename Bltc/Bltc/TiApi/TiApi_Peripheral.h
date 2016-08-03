#ifndef TIAPI_PERIPHERIAL_H_HEADER_INCLUDED_B904DDD8
#define TIAPI_PERIPHERIAL_H_HEADER_INCLUDED_B904DDD8

#include "BltcBasicType.h"

#define QIP7232_USB_FRONTPANEL_PORT	1
#define QIP7232_USB_REARPANEL_PORT	2

#define QIP7232_SATA_INTERNAL_PORT	1
#define QIP7232_SATA_EXTERNAL_PORT	2

#define AC_OUTLET_ON	true
#define AC_OUTLET_OFF	false

//##ModelId=472C57B30355
class TiApi_Peripheral
{
  public:
    //##ModelId=472C57B402D8
     U32 USB_Test(U8 port);

    //##ModelId=472C57B402DA
     U32 ETHERNET_SingleInterLoopTest();

    //##ModelId=472C57B402DB
     U32 SATA_DiskTest(U8 port);

    //##ModelId=472C57B402E8
     U32 FPIR_Test(U8* key, U32 timeout);

    //##ModelId=472C57B402F7
     U32 FPKEYPAD_Test();

    //##ModelId=472C57B402F8
     U32 FPLED_Test();

    //##ModelId=472C57B402F9
     U32 ACOUTLET_Test(bool on);

	 U32 Jedi1394Interruptest();
};



#endif /* TIAPI_PERIPHERIAL_H_HEADER_INCLUDED_B904DDD8 */
