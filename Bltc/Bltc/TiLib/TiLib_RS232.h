#ifndef TILIB_RS232_H_HEADER_INCLUDED_B903D0E1
#define TILIB_RS232_H_HEADER_INCLUDED_B903D0E1

#include "BltcBasicType.h"
#include "bcmuart.h"

#ifdef CONFIG_DEBUG
	#define PDEBUG	pdebug
#else
	#define PDEBUG
#endif

#define CPU_CLOCK_ADJUST	4/3

#define RS232_NOKEY			0x00
#define RS232_ARROW_UP		0x41
#define RS232_ARROW_DOWN	0x42
#define RS232_ARROW_LEFT	0x43
#define RS232_ARROW_RIGHTT	0x44

//##ModelId=472C6E760358
enum UartMode {
	BLT_Debug,
	BLT_Btsc,
	BLT_SIE,
	BAUD_RATE_115200,
	BLT_MCARD_OOB_UPSTREM_TEST
};

extern void pdebug(const char *pcFormat, ...);
extern "C" bool pdebugEnable;
extern bool TiLib_rs232_IsOutputEnabled();
extern bool TiLib_rs232_EnableOutput();
extern bool TiLib_rs232_DisableOutput();

//##ModelId=46FC54BB004C
class TiLib_RS232
{
  public:
    bool _outputEnable;
    //##ModelId=46FC54BB01A3
    TiLib_RS232(U32 port);
    //##ModelId=48C494BF03E6
    void Init();
    //##ModelId=46FC54BB01B2
    bool IsOutputEnabled();
    //##ModelId=46FC54BB01B3
    bool EnableOutput();
    //##ModelId=46FC54BB01B4
    bool DisableOutput();
    UartMode GetUartMode();
    //##ModelId=46FC54BB01B6
    UartMode SetUartMode(UartMode mode);
    //##ModelId=4705AD4D013B
    U32 RxByte(U8* rxByte);
    //##ModelId=470DC6B00230
    int Print(S8* pcFormat, ...);
    //##ModelId=48114D4A0314
    int Print_WithArg(S8* pcFormat, va_list ap);
    //##ModelId=470E11380246
    void GetLine(S8* pcBuf, S32 lBufSize);
    //##ModelId=48114D4A0333
	bool GetLine_WithTimeOut(S8* pcBuf, S32 lBufSize, U32 timeout_us);
    //##ModelId=48114D4A0362
    U8	GetKey();
    //##ModelId=48114D4A0363
	void ClearData();

  private:
    //##ModelId=46FC54BB0089
    volatile UartChannel *_addr;
    //##ModelId=46FC54BB0193
    UartMode _mode;
    //##ModelId=4705A37E033E
//    bool _outputEnable;
};

#endif /* TILIB_RS232_H_HEADER_INCLUDED_B903D0E1 */
