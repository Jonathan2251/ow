#ifndef TILIB_MCARD_H_HEADER_INCLUDED_B90496E1
#define TILIB_MCARD_H_HEADER_INCLUDED_B90496E1

#include "BltcBasicType.h"

//##ModelId=46F36A77002F
class TiLib_MCard
{
  public:
    //##ModelId=46F2166203C7
    bool IsInit();

    //##ModelId=46F21736007F
    U32 DoInit();

    //##ModelId=48114D270317
    U32 OpenSession();

    //##ModelId=48114D270318
    U32 RouteOOBToCableCard();
    
    void Reset();
};



#endif /* TILIB_MCARD_H_HEADER_INCLUDED_B90496E1 */
