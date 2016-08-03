#ifdef CONFIG_WITH_TUNER_TEST
#define CONFIG_WITH_TUNER_TEST

#ifndef TIAPI_TUNER_TEST_H
#define TIAPI_TUNER_TEST_H

#include "BltcBasicType.h"
#include "TiLib_FrontEnd.h"

class TiApi_TunerTest {
private:
	struct TestChannel {
		U16 channelNum;
		U8	qamType;
	};
	
	typedef struct _Tuner_On_Board_Message
	{
	   U8    inBand;
	   U8    qamType;
	   U8    qamLock;
	   U8    signalLevel;
	   U16   testChannel;
//	   U32   xtalFreq;
	   float ifAgc;
	   float rfAgc;
	   float snr;
	}TestResult;

	U32 GIT_Channel_Convert(U16 channel);
	U32 TuneChannel(U8 tuner, U16 chNum);
    U32 QAM_Lock(U8 inBand, U16 chNum, U8 qamType);
    U32 GetLockInfo(TestResult& testResult);
    void Msg_ToTestNextFreq();
    U32 Msg_ReplyTestResult(TestResult testResult);
public:
    U32 PhaseNoiseTest(U8 tuner);
    U32 PerformanceTest(U8 tuner);
};

#endif /* TIAPI_TUNER_TEST_H */

#endif // #ifdef CONFIG_WITH_TUNER_TEST
