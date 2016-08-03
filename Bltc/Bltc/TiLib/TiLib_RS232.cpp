#include "TiLib_RS232.h"
#include <stdarg.h>
#include "TiLib.h"

#include "bchp_common.h"
#include "bcmmips.h"
#include "bcmdbg.h"
 
#define PIN_MUX_CTRL_8_ADDR   BCM_PHYS_TO_K1(BCHP_PHYSICAL_OFFSET+BCHP_SUN_TOP_CTRL_REG_START+0x120)
#define PIN_MUX_CTRL_8_Reg   ( *(volatile U32 *) PIN_MUX_CTRL_8_ADDR )
 
#define RS232_ISENABLE  (PIN_MUX_CTRL_8_Reg  &=  0xC0)
#define RS232_ENABLE    (PIN_MUX_CTRL_8_Reg  |=  0xC0)
#define RS232_DISABLE   (PIN_MUX_CTRL_8_Reg  &= ~0x1C0)

// Baud rate setting for BCM7400
#define BAUD_19200_HI   0x01
#define BAUD_19200_LO   0x08
#define BAUD_38400_HI   0x00
#define BAUD_38400_LO   0x84
#define BAUD_115200_HI  0x00
#define BAUD_115200_LO  0x2C

#define PEN	0x08

extern TiLib lib;

// I rewrite these 2 function instead the TvMon original for disable RS232 print message during run before enter bltc.
// There still is a 'B' character print to RS232 during boot.
// It came from UserBspCppInit() of BspUser.c.

// extern "C" asm void uart_out_rename(char c);

#if 0
asm void uart_out_rename(char c)	// BLTC modify : Gamma
{
   .set noreorder
   li      t0, UART_ADR_BASE
        #if BCHP_CHIP==7400
        li      t2, THRE
uart_out1:      lw      t1, UART_SDW_LSR(t0)
        #else
        li      t2, TXDREGEMT
uart_out1:      lw      t1, UART_TXSTAT(t0)
        #endif
   and     t1, t1, t2
   bne     t1, t2, uart_out1
   nop
        #if BCHP_CHIP==7400
        sw      a0, UART_SDW_THR(t0)
        #else
        sw      a0, UART_TXDATA(t0)
        #endif
   jr      ra
   nop
   .set   reorder
}
#endif


void uart_out_rename(char c)	// BLTC modify : Gamma
{
   unsigned char stat;
   
   stat = (unsigned char) UART->sdw_lsr;
   while (!(stat & THRE))
      stat = (unsigned char) UART->sdw_lsr;
   UART->sdw_rbr_thr_dll = c;
}

extern "C" {

// I rewrite uarta_out() and BKNI_Vprintf for disable rs232 Tx (software solution for disable rs232 output),
// The original function name be renamed to uarta_out_rename() and BKNI_Vprintf_name().
void uarta_out(char c) {
	if (lib.rs232.IsOutputEnabled()) {
		uart_out_rename(c);
	}
}

int BKNI_Vprintf(const char *fmt, va_list ap)
{
	if (lib.rs232.IsOutputEnabled()) {
    	return vprintf(fmt, ap);
	}
	return 0;
}

// Write a printf() to hide printf() of stdio (in MSLC++_ISA1_FPU_LE_A.lib).
int printf(const char *pcFormat, ...)
{
    va_list    ap;
    int result;
    
    va_start( ap, pcFormat );
	result = tvMonIO.out.Vprintf(pcFormat, ap);
    // clean the stack
    va_end( ap );

    return result;
}

bool TiLib_rs232_IsOutputEnabled() {
	return lib.rs232.IsOutputEnabled();
}

bool TiLib_rs232_EnableOutput() {
	return lib.rs232.IsOutputEnabled();
}

bool TiLib_rs232_DisableOutput() {
	return lib.rs232.IsOutputEnabled();
}

}	// extern "C"

void pdebug(const char *pcFormat, ...) {
	if (pdebugEnable) {
		va_list    ap;

		// prepare the arguments
		va_start( ap, pcFormat );
		bcmDbgPrintHelper( eDBG_OUT, eLVL_1, (char*)pcFormat, ap, false );
		// clean the stack
		va_end( ap );
	}
}

#include "cputimer.h"
extern "C" {
extern char det_in_char(void);
extern void uart_out(char c);
}	// extern "C"
// Modify from bcmDbgGetLine() of bcmdbg.c
bool bcmDbgGetLine_WithTimeOut(char *pcBuf, long lBufSize, U32 timeout_us)
{
	char *s_ptr;
	volatile char input_char;
	long ii;
	TimeStruct IR_RxTimer;
	bool success;

	input_char = 0;
	//input_char = -1;
	s_ptr = pcBuf;
	ii = 0;
	Init_timer(&IR_RxTimer,timeout_us);

	while ((input_char != '\n') && (ii < lBufSize) && (Check_timer(&IR_RxTimer) != 1) )
	{

		while ((input_char = det_in_char()) < 0  && (Check_timer(&IR_RxTimer) != 1))
		{
			//	  	  HalServiceWatchDogManager();
		}

		if ((input_char == '\r') || (input_char == '\n') || ((U8)input_char == 0xFF))	// 0xFF = EOF
		{
			uart_out('\r');
			uart_out('\n');
			break;
		}
		else if (input_char == 0x08) // backspace
		{
			ii--;
			if( ii < 0 )
			{
				ii = 0;
			}
			else
			{           
				s_ptr--;
				uart_out(0x08); // deleting backspace
				uart_out(0x20); // deleting backspace
				uart_out(0x08); // deleting backspace
			}
		} 
		else 
		{
			ii++;            
			*s_ptr++ = input_char;
			uart_out(input_char); // putchar(input_char);
		}

	}
	*s_ptr = '\0';
	
	if (Check_timer(&IR_RxTimer) == 1) {
		success = false;
	}
	else {
		success = true;
	}
	return success;

} // bcmDbgGetLine_WithTimeOut

//##ModelId=46FC54BB01A3
TiLib_RS232::TiLib_RS232(U32 port)
{
	if (port == 0) {
		_addr = UARTA;
	}
	else if (port == 1) {
		_addr = UARTB;
	}
	_outputEnable = false;
	SetUartMode(BLT_Debug);
}

//##ModelId=48C494BF03E6
void TiLib_RS232::Init()
{
	_addr->sdw_iir_fcr |= 0x00000001;	// set FIFOE (FIFO Enable) bit
}

//##ModelId=46FC54BB01B2
bool TiLib_RS232::IsOutputEnabled()
{
	if (_outputEnable) {
//	if (RS232_ISENABLE) {
		return true;
	}
	else {
		return false;
	}
}

//##ModelId=46FC54BB01B3
bool TiLib_RS232::EnableOutput()
{
	bool oldStatus = IsOutputEnabled();
//	RS232_ENABLE;
 	_outputEnable = true;
	pdebugEnable = true;
//	_addr->sdw_lcr &= ~0x40;
	return oldStatus;
}

//##ModelId=46FC54BB01B4
bool TiLib_RS232::DisableOutput()
{
	bool oldStatus = IsOutputEnabled();
//	RS232_DISABLE;
	_outputEnable = false;
	pdebugEnable = false;
//	_addr->sdw_lcr |= 0x40;
	return oldStatus;
}

UartMode TiLib_RS232::GetUartMode()
{
    return	_mode;
}

//##ModelId=46FC54BB01B6
UartMode TiLib_RS232::SetUartMode(UartMode mode)
{
	// Follows came from BSP_Qiph.c
	// 8 bits, 1 stop, no parity, enable tx/rx 
/*	li		t1, DLS_8BITS
	sw		t1, UART_SDW_LCR(t0)
     // set baud rate 
	li		t0, UARTA_BASE
	li		t3, DLAB
	sw		t3, UART_SDW_LCR(t0)
	sw		zero, UART_SDW_DLH(t0)
	li		t3, 44	// baud rate = 81MHz / (16 *44) = 115200 approx 
	sw		t3, UART_SDW_DLL(t0)
*/	
	UartMode oldMode = _mode;
	
	switch(mode)
	{
		case BLT_Debug:	// (38400, 8, O, 1, N)
		    // Set Baud Rate register.
		    _addr->sdw_lcr |= DLAB;	// set bit DLAB
		    _addr->sdw_dlh_ier = BAUD_38400_HI;
		    _addr->sdw_rbr_thr_dll = BAUD_38400_LO;
		    _addr->sdw_lcr &= ~DLAB;	// clear DLAB bit
		    // Set Control register.
		    _addr->sdw_lcr |= 0x00000010;	// set EPS (Even Parity Select) bit
		    _addr->sdw_lcr |= 0x00000008;	// set PEN (Parity Enable) bit                         
//		    _addr->control = BITM8 | PAREN | TXEN | RXEN | PODD;
			break;
		case BLT_SIE:	// (19200, 8, N, 1, N)
		    // Set Baud Rate register.
		    _addr->sdw_lcr |= DLAB;	// set DLAB bit
		    _addr->sdw_dlh_ier = BAUD_19200_HI;
		    _addr->sdw_rbr_thr_dll = BAUD_19200_LO;
		    _addr->sdw_lcr &= ~DLAB;	// clear DLAB bit
		    // Set Control register.
		    _addr->sdw_lcr &= ~0x00000010;	// clear EPS (Even Parity Select) bit
		    _addr->sdw_lcr &= ~0x00000008;	// clear PEN (Parity Enable) bit
		    break;
		case BAUD_RATE_115200:
		case BLT_MCARD_OOB_UPSTREM_TEST:	// (115200, 8, N, 1, N)
		    // Set Baud Rate register.
		    _addr->sdw_lcr |= DLAB;	// set DLAB bit
		    _addr->sdw_dlh_ier = BAUD_115200_HI;
		    _addr->sdw_rbr_thr_dll = BAUD_115200_LO;
		    _addr->sdw_lcr &= ~DLAB;	// clear DLAB bit
		    // Set Control register.
//		    _addr->control = BITM8 | PAREN | TXEN | RXEN;
			break;
	}
    
    _mode = mode;
    return	oldMode;	
}

//##ModelId=4705AD4D013B
U32 TiLib_RS232::RxByte(U8* rxByte)
{
   if (_addr->sdw_lsr & DR)
   {
      *rxByte = _addr->sdw_rbr_thr_dll;
      return SUCCESS;
   }
   return FAILURE;
}

//##ModelId=470DC6B00230
int TiLib_RS232::Print(S8* pcFormat, ...)
{
	va_list ap;
   
	va_start(ap, pcFormat);
	bcmDbgPrintHelper( eDBG_OUT, eLVL_1, pcFormat, ap, false );
	va_end(ap);    /* clean things up before leaving */
	
	return 0;
}

//##ModelId=48114D4A0314
int TiLib_RS232::Print_WithArg(S8* pcFormat, va_list ap)
{
	bcmDbgPrintHelper( eDBG_OUT, eLVL_1, pcFormat, ap, false );
	
	return 0;
}

//##ModelId=470E11380246
void TiLib_RS232::GetLine(S8* pcBuf, S32 lBufSize)
{
	bcmDbgGetLine(pcBuf, lBufSize);
}

//##ModelId=48114D4A0333
bool TiLib_RS232::GetLine_WithTimeOut(S8* pcBuf, S32 lBufSize, U32 timeout_us)
{
	return bcmDbgGetLine_WithTimeOut(pcBuf, lBufSize, timeout_us*CPU_CLOCK_ADJUST);
}

/*
U8 debug_buff[8000];	// Gamma debug
int i_buff = 0;*/

//##ModelId=48114D4A0362
U8 TiLib_RS232::GetKey()
{
	U8 key = RS232_NOKEY;
	
	if (RxByte(&key) == SUCCESS) {
		return key;
	}
	else {
		return RS232_NOKEY;
	}
}

//##ModelId=48114D4A0363
void TiLib_RS232::ClearData()
{
	U8 c = 0x01;
	while (c != RS232_NOKEY) {
		c = RS232_NOKEY;
		lib.rs232.RxByte(&c);
	}
}


