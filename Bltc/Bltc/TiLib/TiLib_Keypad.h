// Author : Gamma Chen

#ifndef TILIB_KEYPAD_H_HEADER_INCLUDED_B904BC87
#define TILIB_KEYPAD_H_HEADER_INCLUDED_B904BC87

#include "BltcBasicType.h"
#include "fpcontrol.h"
#include "ModelConfig.h"

#define KEYTYPE					U8

#define KEYPAD_NOKEY			FP_NOKEY
#define KEYPAD_CUR_UP			FP_UPARROW_KEY
#define KEYPAD_CUR_DOWN			FP_DOWNARROW_KEY
#define KEYPAD_CUR_LEFT			FP_LEFTARROW_KEY
#define KEYPAD_CUR_RIGHT		FP_RIGHTARROW_KEY
#define KEYPAD_SELECT			FP_SELECT_KEY
#define KEYPAD_POWER			FP_POWER_KEY
#define KEYPAD_GUIDE			FP_GUIDE_KEY
#define KEYPAD_INFO				FP_INFO_KEY
#define	KEYPAD_MENU				FP_MENU_KEY
// KEYPAD_AB_SWITCH	: "option" key marked on P3 front panel
//##ModelId=472C7588023C
#define KEYPAD_AB_SWITCH		FP_FORMAT_KEY
#define KEYPAD_CH_UP			FP_CHANUP_KEY
#define KEYPAD_CH_DOWN			FP_CHANDOWN_KEY

#define	KEYPAD_SCROLL_UP		FP_CHANUP_KEY
#define	KEYPAD_SCROLL_DOWN		FP_CHANDOWN_KEY


#define KEYPAD_CUR_UP_U16			0xFFFD
#define KEYPAD_CUR_DOWN_U16			0xFEFF
#define KEYPAD_CUR_LEFT_U16			0xEFFF
#define KEYPAD_CUR_RIGHT_U16		0xFFEF
#define KEYPAD_SELECT_U16			0xDFFF
#define KEYPAD_POWER_U16			0xBFFF
#define KEYPAD_CH_UP_U16			0xFBFF
#define KEYPAD_CH_DOWN_U16			0xFFBF

struct KeyTable {
    //##ModelId=472C758803B3
	U16 key;
    //##ModelId=472C758803B4
	S8	*name;
};

//##ModelId=46FA38C400C9
class TiLib_Keypad
{
  public:
    //##ModelId=46FA38C40137
    void SetDebounce(U8 debounce);

    //##ModelId=46FA38C40139
    void Init();

    // void reset();
    //##ModelId=46FA38C4013A
    KEYTYPE GetKey();
    
    //##ModelId=4732FFC8026F
    U16 GetKey_U16();

    //##ModelId=46FA38C4013B
    KEYTYPE GetKeyWithCount(U32 count);

    //##ModelId=46FA38C40146
    bool LongPressed();

    //##ModelId=46FA38C40147
    S8* GetKeyName(KEYTYPE key);

  private:
    //##ModelId=46FA38C40126
    KEYTYPE Read();

    //##ModelId=46FA38C40127
    KEYTYPE ReadWithCount(U32 count);

    //##ModelId=46FA38C40107
    KEYTYPE _key;
    //##ModelId=46FA38C40117
    bool _longPressed;
};



//##ModelId=46FA38C400AB
struct TiLib_KeyTable
{
    //##ModelId=46FA38C400D9
    KEYTYPE key;
    //##ModelId=46FA38C400E8
    S8	* name;
};

#endif /* TILIB_KEYPAD_H_HEADER_INCLUDED_B904BC87 */
