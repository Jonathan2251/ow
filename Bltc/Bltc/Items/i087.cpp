// Author : Gamma Chen

#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "Process.h"

#if 0	// not ready in ethernet test

// Follows, copy from ethernet.cpp
#define ETHERNET_7038	0

#define TEST_ARRAY_SIZE	256
enum eTestArrayPattern  { ZeroPattern=0, OnePattern, AlternatingPattern, RandomPattern };

struct LoopTestParameters
{
	U8					ucDeviceType;
	BOOL				bContinuousTestMode;
	eTestArrayPattern	eTestPattern;
	U8					TxTestArray[TEST_ARRAY_SIZE];
	BOOL				bSpeed;
	BOOL				bTimeoutMsgEnable;
};
typedef LoopTestParameters LTP;

extern VOID FillTestArray(U8 *pTestArray, eTestArrayPattern ePattern);
extern VOID PerformEthernetTest(LTP *pLtp);

U32 Ethernet_LoopTestSignleIteration()
{
    static U8 gcPinState=0;			// 0:Normal; 1:Swapped		 
	LTP		LoopTestParameters;
//	S8		cInput;
//	U8		TestArray[TEST_ARRAY_SIZE];
	BOOL	bLoop=TRUE;
	
	char answer[100] = "";
	char pattern[100] = "Got the packet";
	U32 lError = 0;

	tvMonIO_clear();
	tvMonIO.out.enable = 1;
	
	/* Init the Loop Test Paramters */
	LoopTestParameters.bSpeed = TRUE;					// 100 BaseT
	LoopTestParameters.bTimeoutMsgEnable = TRUE;		// Enable timeout messages
	LoopTestParameters.ucDeviceType = ETHERNET_7038;	// Bcm7038

	/* Init Test Array */
	LoopTestParameters.eTestPattern = ZeroPattern;
	FillTestArray(LoopTestParameters.TxTestArray,ZeroPattern);
				LoopTestParameters.bContinuousTestMode = FALSE;
				PerformEthernetTest(&LoopTestParameters);
				
	tvMonIO.out.enable = 1;
	
	tvMonIO_out_getResultN(tvMonIO.out._ostr, "Waiting for packet ...\r\n", answer, strlen(pattern) );
	if ( strcmp(answer, pattern) == 0 ) {
		lError = 0;
	}
	else {
		lError = 1;
	}
	
	return lError;
}
#endif

#if 1
U32 Ethernet_LoopTestSignleIteration()
{
	U32 lError = 0;
	
	lError = api.peripheral.ETHERNET_SingleInterLoopTest();
	
	return lError;
}
#endif

U32 Items::TI087()
{
	U32 lError = 0;
	U32 (*pf)() = &Ethernet_LoopTestSignleIteration;
	
	int timeOut = 5;	// unit:1s, => 5 seconds
	lError = PROCESS_CREATE_IF_DEF(pf, timeOut);
	
	return lError;
}
