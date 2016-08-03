//##ModelId=48114CDC007D
#ifndef TILIB_CHIP_H_HEADER_INCLUDED_B9048941
#define TILIB_CHIP_H_HEADER_INCLUDED_B9048941

#include "BltcBasicType.h"

//##ModelId=48114CDC007D
enum PeripheralChip
{
  CT_MAIN_CHIP=0,  // CT_ : ChipType
  CT_DEMOD,
  CT_1394CHIP,
  CT_MPEG2_ENCODER,
  CT_ENTROPIC,
  CT_BOARD_ID,
  CT_PSOC_FAN,
  CT_PSOC_FP
};

//##ModelId=472C698202B6
class TiLib_Chip
{
  public:
    //##ModelId=472C69830025
    U32 GetMainChip(U32 regAddr);

    //##ModelId=472C69830027
    U32 GetPCIDev(PeripheralChip chip);

    //##ModelId=472C69830035
    U32 GetI2CDev(PeripheralChip chip);
    
    U8 GetTunerID(U8 tuner);

    //##ModelId=472C69830037
    U32 GetBoardID();

    //##ModelId=472C69830038
    U32 OHCI_Test();
};



#endif /* TILIB_CHIP_H_HEADER_INCLUDED_B9048941 */
