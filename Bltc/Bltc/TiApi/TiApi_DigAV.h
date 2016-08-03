#ifndef TIAPI_DIGAV_H_HEADER_INCLUDED_B904C2DF
#define TIAPI_DIGAV_H_HEADER_INCLUDED_B904C2DF

#include "BltcBasicType.h"
#include "TiLib_DigAV.h"


enum {TUNER1=1, TUNER2};
enum {QAM64, QAM256};
enum {BYPASS_MCARD, ROUTE_TO_MCARD};


//##ModelId=46D4CD24032B
struct PcrPids {
    //##ModelId=46D4CD460195
	U32 pcr;
    //##ModelId=472C57B40038
	U32	vPids;
    //##ModelId=472C57B40086
	U32	aPids;
};

//##ModelId=46DBA763006C
struct ChanMap {
    //##ModelId=46DBA7730176
	U32	chan;
    //##ModelId=46DBA781007B
	U32	freq;
};

//##ModelId=4716E76F0058
struct TS_Msg
{
    //##ModelId=4716E90503A3
    int tuner;
    //##ModelId=4716E90503A4
    bool isFreq;
    union {
    	U32 freq;
    	U32 chan;
	};
    //##ModelId=4716E90503C4
    int qamType;
    //##ModelId=472C5E740159
    PcrPids pcrPids;
    //##ModelId=4716E90503D3
    int path;
};

//##ModelId=46EDDD18014D
class TiApi_DigAV
{
  public:

    //##ModelId=481125BE017E
	void EnableRemod();
    //##ModelId=481125BE017F
	void Audio_Start_Test_Tone(U32 stream);
    //##ModelId=481125BE018E
    U32 CreateTransportStream(TS_Msg ts);
    
    //##ModelId=46EB4693021D
    U32 PIP_CreateTransportStream(TS_Msg tuner1, TS_Msg tuner2);

    //##ModelId=46EB4693022C
    U32 ChangeChanMap(ChanMap chanMap);

    //##ModelId=46EB4693023C
    U32 OpenOutput(
        // 0x00000001 : COMPOSITE
        // 0x00000002 : COMPONENT
        // 0x00000004 : HDMI
        // 0x00000008 : OUT_1394
        // 0x00000010 : SVIDEO
        // 0x00000020 : RF
        // 0x00000040 : OUT_802_11
        // 0x00000080 : ETHERNET
        int flag);

    //##ModelId=46EB4693024C
    U32 OpenOutput_All();

	//##ModelId=4722F8510186
    U32 CloseOutput(
        // 0x00000001 : COMPOSITE
        // 0x00000002 : COMPONENT
        // 0x00000004 : HDMI
        // 0x00000008 : OUT_1394
        // 0x00000010 : SVIDEO
        // 0x00000020 : RF
        // 0x00000040 : OUT_802_11
        // 0x00000080 : ETHERNET
        int flag);
};


#endif /* TIAPI_DIGAV_H_HEADER_INCLUDED_B904C2DF */
