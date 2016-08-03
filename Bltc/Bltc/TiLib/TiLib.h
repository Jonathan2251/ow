#ifndef TILIB_H_HEADER_INCLUDED_B903ACC0
#define TILIB_H_HEADER_INCLUDED_B903ACC0
#include "TiLib_Chip.h"
#include "TiLib_DigAV.h"
#include "TiLib_FrontEnd.h"
#include "TiLib_IR.h"
#include "TiLib_Peripheral.h"
#include "TiLib_Keypad.h"
#include "TiLib_MCard.h"
#include "TiLib_Memory.h"
#include "TiLib_RS232.h"
#include "TiLib_Led.h"
#include "BltcBasicType.h"
#include "ModelConfig.h"
#include "TiLib_TvMonCmd.h"
#include "Timer_ISR.h"
#ifdef CONFIG_WITH_OSD
#include "TiLib_OSD_Driver.h"
#include "TiLib_Window.h"
#endif	// CONFIG_WITH_OSD

//##ModelId=46EE20B30388
class TiLib
{
  public:
    
    //##ModelId=470200FF00ED
    TiLib();
    
    //##ModelId=48114CDC0232
    void Init();
    
    //##ModelId=48C4925A007A
	AsmVoid EnableInterrupt();
    //##ModelId=48C4926F004C
	AsmVoid DisableInterrupt();
	
    //##ModelId=4702E17700C9
    void DelayMS(U32 ms);
    //##ModelId=4702F2FC017C
    U32 GetProductModel(S8* model);

    //##ModelId=4702F30903CE
    bool IsProductModel(S8* model);

    //##ModelId=48C4C7E300C2
    TiLib_Chip  chipID;
    //##ModelId=46FA271300AA
    TiLib_DigAV digAV;
    //##ModelId=46FA27130108
    TiLib_FrontEnd frontEnd;
    
    //##ModelId=46FA27130137
    TiLib_Peripheral peripheral;
    //##ModelId=46FA27130166
    TiLib_MCard mCard;
    //##ModelId=46FA27130194
    TiLib_Memory memory;
    //##ModelId=46FA3A3702F0
    TiLib_Keypad keypad;
    //##ModelId=48155E770027
    TiLib_IR ir;
    //##ModelId=46FC5584027D
    TiLib_RS232 rs232;
    //##ModelId=46FC9C90020A
    TiLib_Led* led;
    //##ModelId=47159C0C001E
    TiLib_TvMonCmd tvMonCmd;
    //##ModelId=48C4C84801CD
    Timer_ISR clock;
#ifdef CONFIG_WITH_OSD
    //##ModelId=48C4C5A60388
    TiLib_OSD_Driver osd;
    //##ModelId=48C4C5DA005C
    TiLib_Window window;
#endif	// CONFIG_WITH_OSD
};

extern TiLib lib;

#endif /* TILIB_H_HEADER_INCLUDED_B903ACC0 */
