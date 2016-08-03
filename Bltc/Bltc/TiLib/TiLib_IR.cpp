// Author : Gamma Chen

#include "TiLib_IR.h"
#include "BltcBasicType.h"
#include "TiLib.h"
#include "cputimer.h"

//#define CONFIG_BCM7400D0_BEFORE

#define IR_REMOTE_MODE              BKIR_KirDevice_eCirGI

extern "C" void BCM_Test_IR_Init(BKIR_ChannelHandle       *phIRChan,
                      BKIR_KirInterruptDevice  *pInterruptDevice,
                      BKIR_KirDevice           eDevice);
extern "C" void BCM_Test_IR_DeInit(BKIR_ChannelHandle hIRChan);

extern VOID IRRecvInit(VOID);

//##ModelId=4815267F0249
TiLib_IR::TiLib_IR()
{
//	Init();	// Init() must occurs at after TvMon Initialize.
}

#ifdef CONFIG_BCM7400D0_BEFORE
//##ModelId=4815267F0259
TiLib_IR::~TiLib_IR()
{
	/* Disable and Close the IR Receiver Device */
	BCM_Test_IR_DeInit(_hIRChan);
}

//##ModelId=4815267F0278
void TiLib_IR::Init()
{
	// Initialize the IR Receiver
	BCM_Test_IR_Init(&_hIRChan, &_interruptDevice, IR_REMOTE_MODE);
}

// GetKey() : Nobock function
//##ModelId=4815267F0297
U8 TiLib_IR::GetKey()
{
	unsigned char			data[5];
	unsigned int			readyFlag = 1;	// Hacker, it's important
//	bool			readyFlag = 0;	// Don't using this
	data[0] = IR_NOKEY;
	// Check for IR data
	BKIR_IsDataReady (_hIRChan, (bool*)&readyFlag);
	if (readyFlag)
	{
		BKIR_Read(_hIRChan, &_interruptDevice, data);
		data[4] = 0;
	}
	
	return data[0];
}

//##ModelId=4815267F02B6
void TiLib_IR::ClearData()
{
	unsigned char			data[5];
	unsigned int			readyFlag = 1;	// Hacker, it's important
//	bool			readyFlag = 0;	// Don't using this

	data[0] = IR_NOKEY;
	while (readyFlag) {
		// Check for IR data
		BKIR_IsDataReady (_hIRChan, (bool*)&readyFlag);
		if (readyFlag)
		{
			BKIR_Read(_hIRChan, &_interruptDevice, data);
			data[4] = 0;
//			BRCM_DBG_OUT(("IR Receiver Data:  0x%02X 0x%02X", data[0], data[1]));
//			lib.rs232.Print("ClearData IR Receiver:  0x%02X, 0x%02X\r\n", data[0], data[1]);
		}
	}
}

#else

TiLib_IR::~TiLib_IR()
{
}

void TiLib_IR::Init()
{
	// Clear Status
	IRRecvInit();
}

// GetKey() : Nobock function
U8 TiLib_IR::GetKey()
{
	U8	data[5];
	U8	status;
	
	data[0] = IR_NOKEY;
	
	// Read the IR Status Reg.
	status = IrControl(IR_READ_REG, IR_STATUS, 0);
	
	// Process Received Key
	if (status & 0x01) 
	{
		// Read the IR Key Data Regs.
		data[3] = IrControl(IR_READ_REG, IR_DATA3, 0);
		data[2] = IrControl(IR_READ_REG, IR_DATA2, 0);
		data[1] = IrControl(IR_READ_REG, IR_DATA1, 0);
		data[0] = IrControl(IR_READ_REG, IR_DATA0, 0);

		// Clear the IR Status Reg.
		IrControl(IR_WRITE_REG, IR_STATUS, 0);
	
//		PDEBUG("IR Receiver:  0x%02X, 0x%02X\r\n", data[0], data[1]);
	}
			
	return data[0];
}

void TiLib_IR::ClearData()
{
}

#endif

