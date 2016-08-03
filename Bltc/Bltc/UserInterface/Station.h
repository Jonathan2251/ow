// Author : Gamma Chen

//##ModelId=48102AEE0374
#ifndef STATION_H_HEADER_INCLUDED_B904CF9F
#define STATION_H_HEADER_INCLUDED_B904CF9F

#include "items.h"


#define NO_ITEM			255

#define NR_ITEMS		4*32
#define NR_TESTITEMS	4*16
#define NR_ARGLIST		2*NR_TESTITEMS

struct ErrorCode {
    //##ModelId=48102AEE0374
	int	size;
    //##ModelId=48102AEF00A5
	U32 code[4];
};

//##ModelId=46FA16CC0281
class Station
{
  public:
    //##ModelId=46FA16CD0040
    Station();

    //##ModelId=46FA16CD0041
    Station(S8 id);

    //##ModelId=46FA16CD004E
    void Init();

    //##ModelId=46FA16CD004F
    U8 Id();
	
    //##ModelId=46FA16CD0050
    S8* Description();

    //##ModelId=480FFBE700FB
	int ItemsNr();
    //##ModelId=480FFBEC02A1
	int TestItemsNr();
	
    //##ModelId=46FA16CD0051
    void GetItemList(U8 *item);

    //##ModelId=46FA16CD0053
    U32 GetArgList(U32 *arg);

    //##ModelId=46FA16CD005F
    void SetId(U8 id);

    //##ModelId=46FA16CD0061
    void SetDesc(S8	* p);

    //##ModelId=46FA16CD0063
    void SetItemList(U8 item[]);

    //##ModelId=46FA16CD006D
    void SetItemListFromStr(S8 *str);

    //##ModelId=46FA16CD006F
    void SetArgList(U32 arg[NR_ARGLIST], U32 size);

    //##ModelId=46FA16CD0072
    void AddArg(U32 arg);

    //##ModelId=46FA16CD007F
    void Test();

    //##ModelId=46FA16CD0080
    ErrorCode Error();

    //##ModelId=46FA16CD0081
    U32 LastItemError();

    //##ModelId=46FA16CD0082
    void StopTest();

    //##ModelId=46FA16CD0083
    void OutputStationList();

    //##ModelId=46FA16CD0084
    void OutputErrCodeList();

  private:
    //##ModelId=46FA16CC03C9
    S8 _id;
    //##ModelId=46FA16CC03CA
    S8	*_desc;
    //##ModelId=46FA16CC03D8
    U8 _item[NR_ITEMS];
    //##ModelId=46FA16CD0000
    U32 _arg[NR_ARGLIST];
    //##ModelId=46FA16CD0010
    U32 _argSize;
    //##ModelId=46FA16CD001F
    ErrorCode _error;
    //##ModelId=46FA16CD002F
    U32 _lastItemError;
    //##ModelId=46FA16CD003F
    bool _stopTest;
    
    //##ModelId=480FFC65038B
	void CaculateErrCode(S32 testItemIdx, S32* errCode);
	
    //##ModelId=480FFC7C0177
	void GetAlignmentSpaces(S8* spaces);
	
    //##ModelId=480FFC860187
	void OutputNoErrCodeEntry();
    //##ModelId=480FFC900010
	void OutputTestItemErrCodeEntry(S32* errCode, S32 itemNr);
    //##ModelId=480FFC9C02FE
	void OutputActionItemErrCodeEntry(S8* spaces, S32 itemNr);
    //##ModelId=480FFCA60139
	void OutputControlItemErrCodeEntry(S8* spaces, S32 itemNr, S32 argCnt);
	
};



#endif /* STATION_H_HEADER_INCLUDED_B904CF9F */
