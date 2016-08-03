#if 1

#ifndef UI_ALL_IN_ONE_TEST_GAMMA_H_INCLUDED
#define UI_ALL_IN_ONE_TEST_GAMMA_H_INCLUDED

#include "UserInterface.h"

enum {RX_SUCESS, RX_TIMEOUT, RX_WITHOUT_ENDCODE};

//##ModelId=46CE7CCB013E
class UI_AllInOneTest1 : public UserInterface
{
  public: 
	UI_AllInOneTest1(UI_OutputMsg *outMsg);
    //##ModelId=46DCEEF60265
    void Run();

	void Msg_Connect_MatchWithStr(S8 *request, S8* reply);
	void Msg_Connect_MatchWithFunc(S8 *request, int (*matchFunc)(S8 *request, S8* reply));
  	int Tx(S8* pcFormat, ...);
  	int Rx(S8* msg, U32 timeout_us);
	bool CheckCRC(S8* msg);
	
  private:
	bool ConnectSIE();
	void RunTestItem();
	bool IsSingleItemTest(S8* msg);
	int GetItemNr(S8* s);
	int SingleItemTest_GetItemNr(S8* msg);
	bool IsMultiItemTest(S8* msg);
	int MultiItemTest_GetItemNr(S8* msg, int* itemNr);
  	void ClearRS232Buffer();
  	U8 _rxBuff[1000];
};

#endif // UI_ALL_IN_ONE_TEST_GAMMA_H_INCLUDED 
#endif