#ifdef CONFIG_WITH_TUNER_TEST
#define CONFIG_WITH_TUNER_TEST

#include "TiApi_TunerTest.h"
#include "UI_AllInOneTest_Gamma.h"
#include "ModelConfig.h"
#include "Items.h"
#include "TiApi.h"
#include "TiLib.h"
#include "Timer_MsTick.h"

#include "crccalc.h"
#include "cputimer.h"

#define NUM_OF_TEST_CHANNELS	5

//#define CONFIG_TUNER_PERFORMANCE_LAB_SIMULATION

#ifdef CONFIG_TUNER_PERFORMANCE_LAB_SIMULATION
	const TiApi_TunerTest::TestChannel TEST_CHANNEL[NUM_OF_TEST_CHANNELS] = {	// for lab. simulation
		{6, QAM256},
		{8, QAM64},
		{6, QAM256},
		{6, QAM256},
		{8, QAM64}
	};
#else
	const TiApi_TunerTest::TestChannel TEST_CHANNEL[NUM_OF_TEST_CHANNELS] = {	// for QIP7kP2, Carrier with the channels (57MHz, 183MHz,453MHz, 549MHz, 855MHz)
		{2, QAM64},
		{8, QAM64},
		{70, QAM64},
		{86, QAM64},
		{137, QAM64}
	};
#endif

extern UI_AllInOneTest1 		ui_aio1;

U32 TiApi_TunerTest::GIT_Channel_Convert(U16 channel)
{	        
	U32 Range,RF;    
	          
	if(channel>=2   && channel<=4  ){ Range=2 , RF=57;  }
	if(channel==5   || channel==6  ){ Range=5 , RF=79;  }
	if(channel>=7   && channel<=13 ){ Range=7 , RF=177; }
	if(channel>=14  && channel<=22 ){ Range=14, RF=123; }
	if(channel>=23  && channel<=53 ){ Range=23, RF=219; }                       
	if(channel>=54  && channel<=61 ){ Range=54, RF=75;  }
	if(channel>=62  && channel<=161){ Range=62, RF=405; }        

	RF=(RF+(channel-Range)*6)*1000000;

	return RF;
}

U32 TiApi_TunerTest::TuneChannel(U8 tuner, U16 chNum)
{ 
   U32 RF_Frequency;

   if((chNum < 2) || (chNum > 161)) return FAILURE;

   RF_Frequency=GIT_Channel_Convert(chNum);
   api.frontEnd.QAM_SetFreq(tuner, RF_Frequency);
      
   return SUCCESS;
}

U32 TiApi_TunerTest::QAM_Lock(U8 inBand, U16 chNum, U8 demodType)
{
	U32 i;
	U32 ulError;

	ulError = TuneChannel(inBand+1, chNum);  
	if(ulError != SUCCESS)
	{
		lib.rs232.Print("\r\n< Error > Channel Tune failed.\r\n"); 
		return (1);
	}  

	if ((int)demodType == QAM64) {
		api.frontEnd.QAM_64Acq((int)(inBand+1));
	}
	else if ((int)demodType == QAM256) {
		api.frontEnd.QAM_256Acq((int)(inBand+1));
	}
	else {
		lib.rs232.Print("TiApi_FrontEnd::QAM_Lock(U8 inBand,U16 chNum,U8 demodType)\r\n");
		lib.rs232.Print("demodType = %d\r\n", demodType);
		return 1;
	}
	     
	return (0);
}

U32 TiApi_TunerTest::GetLockInfo(TestResult& testResult)
{
	U32 TestResult;
	int i;
	BERR_Code		eErr;
	etHalDemodIbStatus demodIbStatus;

	// QAM Lock check
	for (i = 0; i < 10; i++) {
		api.frontEnd.QAM_GetAllLockStatus((int)(testResult.inBand+1), &demodIbStatus);
		if (demodIbStatus.bIsQamLocked && demodIbStatus.bIsFecLocked) {
			break;
		}
		lib.DelayMS(100);
	}
	if (demodIbStatus.bIsQamLocked && demodIbStatus.bIsFecLocked) {
		testResult.qamLock = 1;
	}
	else {
		testResult.qamLock = 0;
	}
	
	testResult.ifAgc = (float) ((double)demodIbStatus.ulAcgLevel / 10.0);
	testResult.rfAgc = 0;
	testResult.snr = (float) ((double) demodIbStatus.ulSnrEstimate / 256.0);

	return (0);
}

int MatchFreqLevel(S8 *request, S8* reply)
{
	if (strcmp("@{0dBmV}@", reply) == 0 || strcmp("@{+0dBmV}@", reply) == 0 || strcmp("@{-0dBmV}@", reply) == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void TiApi_TunerTest::Msg_ToTestNextFreq()
{
// The signalLevelSel sent to SIE is meanless, in fact, SIE adjust signal level without refer to this reply level.
	S8  *target;  
//	S8  *SL_Pattern_String[7]={"+15dBmV","+05dBmV","0dBmV","-05dBmV","-12dBmV","-15dBmV","-25dBmV"};      
	S8 rxStr[1000];
	int match;
	int (*func)(S8* request, S8* reply);

	lib.rs232.Print("\r\n");
#if 0	// Original Roger's Protocol
	target = SL_Pattern_String[signalLevelSel];
	if (signalLevelSel == 2) {
		ui_aio1.Msg_Connect_MatchWithFunc(target, MatchFreqLevel);
	}
	else {
		ui_aio1.Msg_Connect_MatchWithStr(target, target);
	}
	ui_aio1.Msg_Connect_MatchWithFunc("0dBmV", MatchFreqLevel);
#else
	ui_aio1.Msg_Connect_MatchWithStr("NEXT", "NEXT");
#endif
}

U32 TiApi_TunerTest::Msg_ReplyTestResult(TestResult testResult)
{
	char tmp[100];
	U16 crc;
	
	if (testResult.ifAgc >= 100) {
		testResult.ifAgc = 99.99;
	}
	if (testResult.rfAgc >= 100) {
		testResult.rfAgc = 99.99;
	}
	if (testResult.snr >= 100) {
		testResult.snr = 99.99;
	}
	testResult.snr = (float)((int)testResult.snr);
	sprintf(tmp, "%d/%.3d/%d/%2.2f/%2.2f/%2.2f", testResult.inBand, testResult.testChannel, testResult.qamLock, testResult.ifAgc, testResult.rfAgc, testResult.snr);
	crc = CalcCRC16((U8*)tmp, strlen(tmp), 0, 0, 0, 0);
	ui_aio1.Tx("@{%s/%.4X}@\r\n", tmp, crc);

	return 0;
}

U32 TiApi_TunerTest::PhaseNoiseTest(U8 tuner)
{   
	int  i;
	U32 lError=0;      
	TestResult testResult[6];
	S8 s[1000];
	bool correctReplyFromSIE = false;
	int rxResult;

	for(i = 0; i < NUM_OF_TEST_CHANNELS; i++) {
		testResult[i].testChannel	= TEST_CHANNEL[i].channelNum;
//		testResult[i].signalLevel	= 0;
		testResult[i].qamType	 	= TEST_CHANNEL[i].qamType;
		testResult[i].inBand		= tuner - 1;
		testResult[i].qamLock		= 0;
		testResult[i].snr			= 0;
//		testResult[i].xtalFreq		= 0;
		testResult[i].ifAgc			= 0;
		testResult[i].rfAgc			= 0;
	}            

	ui_aio1.Msg_Connect_MatchWithStr("TOB-PN", "TOB-PN");

	for(i = 0; i < NUM_OF_TEST_CHANNELS; i++) {
		QAM_Lock(testResult[i].inBand, testResult[i].testChannel, testResult[i].qamType);                   
		GetLockInfo(testResult[i]);

		Msg_ReplyTestResult(testResult[i]);
		while (1) { // Waiting IPC to notify next action.
			rxResult = ui_aio1.Rx(s, 100000);
			if (rxResult == RX_SUCESS && strcmp(s, "@{NEXT}@") == 0) {
				break;
			}
			lib.peripheral.WatchDog_Serve();
		}
	}   
	ui_aio1.Tx("@{END}@\r\n");

	return (0);
}

U32 TiApi_TunerTest::PerformanceTest(U8 tuner)
{
	S8  *SL_Pattern_String[7]={"+15dBmV","+05dBmV","0dBmV","-05dBmV","-12dBmV","-15dBmV","-25dBmV"};      
	U8  i;
	U32 lError=0; 
	TestResult testResult[NUM_OF_TEST_CHANNELS];
	S8 s[1000];
	int rxResult;
   
	for (i = 0; i < NUM_OF_TEST_CHANNELS; i++) {    
		testResult[i].testChannel	= TEST_CHANNEL[i].channelNum;
//		testResult[i].signalLevel	= 0;
		testResult[i].qamType	 	= TEST_CHANNEL[i].qamType;
		testResult[i].inBand		= tuner - 1;
		testResult[i].qamLock		= 0;
		testResult[i].snr			= 0;
		//      testResult[i].xtalFreq	= 0;
		testResult[i].ifAgc			= 0;
		testResult[i].rfAgc			= 0;
	}

	ui_aio1.Msg_Connect_MatchWithStr("TOB", "TOB");
	    
	for (i = 0; i < NUM_OF_TEST_CHANNELS; i++) {
	//		Msg_ToTestNextFreq();	// Change to wait "@{NEXT}@" as below as same as PhaseNoiseTest().
		  
		QAM_Lock(testResult[i].inBand, testResult[i].testChannel, testResult[i].qamType);
		GetLockInfo(testResult[i]);

		Msg_ReplyTestResult(testResult[i]);
		while (1) { // Waiting IPC to notify next action.
			rxResult = ui_aio1.Rx(s, 100000);
			if (rxResult == RX_SUCESS && strcmp(s, "@{NEXT}@") == 0) {
				break;
			}
			lib.peripheral.WatchDog_Serve();
		}
	}   

	ui_aio1.Tx("@{END}@\r\n");

	return (0);
}

#endif // #ifdef CONFIG_WITH_TUNER_TEST
