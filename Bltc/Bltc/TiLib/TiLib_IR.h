// Author : Gamma Chen

#ifndef TILIB_IR_H_HEADER_INCLUDED
#define TILIB_IR_H_HEADER_INCLUDED

#include "bkir.h"
#include "BltcBasicType.h"

#define KEYTYPE_IR					U8

#define IR_NOKEY					0xFF
#define IR_OK						0x11
#define IR_UP						0x34
#define IR_DOWN						0x35
#define IR_LEFT						0x36
#define IR_RIGHT					0x37
#define IR_0						0x00
#define IR_1						0x01
#define IR_2						0x02
#define IR_3						0x03
#define IR_4						0x04
#define IR_5						0x05
#define IR_6						0x06
#define IR_7						0x07
#define IR_8						0x08
#define IR_9						0x09

//##ModelId=481525E4035F
class TiLib_IR
{
  public:
    //##ModelId=4815267F0249
  	TiLib_IR();
    //##ModelId=4815267F0259
  	~TiLib_IR();
    //##ModelId=4815267F0278
    void Init();
    //##ModelId=4815267F0297
	U8 GetKey();
    //##ModelId=4815267F02B6
    void ClearData();
  private:
    //##ModelId=4815267F01EB
    BKIR_ChannelHandle _hIRChan;
    //##ModelId=4815267F021A
	BKIR_KirInterruptDevice	_interruptDevice;
};


#endif /* TILIB_IR_H_HEADER_INCLUDED */
