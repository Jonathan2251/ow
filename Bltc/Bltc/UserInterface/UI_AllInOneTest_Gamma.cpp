#if 1

#include "UI_AllInOneTest_Gamma.h"
#include "TiApi.h"
#include "TiLib.h"
#include "gitypes.h"
#include "crccalc.h"

#define DEBUG_PRINT
//#define DEBUG_PRINT	Tx

extern "C" void uarta_out(char c);

void DebugPrint(U8* s)
{
#if 0
	bool rs232enabled = lib.rs232.EnableOutput();
	for (int i = 0; i < strlen((S8*)s); i++) {
		if (s[i] != '@' && s[i] != '{' && s[i] != '}') {
			uarta_out((S8)s[i]);
		}
	}
	lib.rs232.DisableOutput();
#endif
}

UI_AllInOneTest1::UI_AllInOneTest1(UI_OutputMsg *outMsg) : UserInterface(outMsg)
{
}

void UI_AllInOneTest1::ClearRS232Buffer()
{
   U8 DummyData=0;  
   
   while(lib.rs232.RxByte(&DummyData)==SUCCESS)
   {
   }
}

int UI_AllInOneTest1::Tx(S8* pcFormat, ...)
{
	int result = 0;
	bool rs232enabled = lib.rs232.EnableOutput();
	va_list ap;
   
	va_start(ap, pcFormat);
//	bcmDbgPrintHelper( eDBG_OUT, eLVL_1, pcFormat, ap, false );
	result = lib.rs232.Print_WithArg(pcFormat, ap);
	va_end(ap);    /* clean things up before leaving */
	
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
	
	return result;
}

extern U8 debug_buff[8000];	// Gamma debug
extern int i_buff;

int UI_AllInOneTest1::Rx(S8* msg, U32 timeout_us) {
// Read character from RS232 to msg until EOF(0xff), EOL("\r\n"), or "}@"
// The msg is the form "@{..}@" without EOF or EOL
	U8 s[1000];
	int i = 0;
	TimeStruct IR_RxTimer;
	U8 c = RS232_NOKEY;
	bool findLeadCode = false;
	int result = RX_WITHOUT_ENDCODE;
	
	Init_timer(&IR_RxTimer,timeout_us*ADJUST_CPU_CLOCK_RATE);
/*	if (strlen((S8*)_rxBuff) > 1 && memcmp(_rxBuff, "@{", 2) == 0) {
		strcpy((S8*)s, (S8*)_rxBuff);
		i=strlen((S8*)s);
		findLeadCode = true;
		goto find_end_code;
	}*/
	while (1) {
		c = lib.rs232.GetKey();
		if (c != RS232_NOKEY) {
			s[i] = c;
			if (i >= 1) {
				if (memcmp(s+i-1, "@{", 2) == 0) {
					if (i > 1) {
						s[0] = '@';
						s[1] = '{';
						s[2] = '\0';
					}
					memcpy(_rxBuff, s, 2);
					_rxBuff[2] = '\0';
					i = 2;
					findLeadCode = true;
					Init_timer(&IR_RxTimer,timeout_us*ADJUST_CPU_CLOCK_RATE);
//				DEBUG_PRINT("findLeadCode = true\r\n");	// Gamma debug
					break;
				}
			}
			i++;
			if (i > 198) {
				msg[199] = '\0';
				DEBUG_PRINT("i > 198 in Find Lead Code\r\n");	// Gamma debug
				return RX_WITHOUT_ENDCODE;
			}
		}
		else if (Check_timer(&IR_RxTimer) == 1) {
//				DEBUG_PRINT("Timeout in Find Lead Code\r\n");	// Gamma debug
			return RX_TIMEOUT;
		}
	}
	
find_end_code:
	if (findLeadCode) {
		while (1) {
			c = lib.rs232.GetKey();
			if (c != RS232_NOKEY) {
				s[i] = c;
				_rxBuff[i] = c;
				if (memcmp(s+i-1, "}@", 2) == 0) {
					i++;
					result = RX_SUCESS;
					_rxBuff[0] = '\0';
					break;
				}
				else if (memcmp(s+i-1, "\r\n", 2) == 0) {
					i++;
					result = RX_WITHOUT_ENDCODE;
					_rxBuff[0] = '\0';
				DEBUG_PRINT("Find EOL\r\n");	// Gamma debug
					break;
				}
				else if (c == 0x0D) {
					i++;
					result = RX_WITHOUT_ENDCODE;
					_rxBuff[0] = '\0';
				DEBUG_PRINT("Find 0x0D\r\n");	// Gamma debug
					break;
				}
				else if (c == 0x0A) {
					i++;
					result = RX_WITHOUT_ENDCODE;
					_rxBuff[0] = '\0';
				DEBUG_PRINT("Find 0x0A\r\n");	// Gamma debug
					break;
				}
				else if (c == 0xFF) {
					i++;
					result = RX_WITHOUT_ENDCODE;
					_rxBuff[0] = '\0';
				DEBUG_PRINT("Find EOF\r\n");	// Gamma debug
					break;
				}
				i++;
				if (i > 198) {
					s[199] = '\0';
				DEBUG_PRINT("i > 198 in Find End Code\r\n");	// Gamma debug
					DebugPrint(s);	// Gamma debug
					result = RX_WITHOUT_ENDCODE;
					return result;
				}
			}
			else if (Check_timer(&IR_RxTimer) == 1) {
				result = RX_WITHOUT_ENDCODE;
				DEBUG_PRINT("Timeout in Find End Code\r\n");	// Gamma debug
				break;
			}
		}
		s[i] = '\0';
		strcpy(msg, (S8*)s);
	}
	
success_out:
	DebugPrint(s);
	
	return result;
}

void UI_AllInOneTest1::Msg_Connect_MatchWithStr(S8 *request, S8* reply)
{
	S8 replyh[1000];
	U16 crc;
	int ms;
	S8 s[1000];
	bool correctReplyFromSIE = false;
	int rxResult;
	
	sprintf(replyh, "@{%s}@", reply);
	crc = CalcCRC16((U8*)request, strlen(request), 0, 0, 0, 0);
	Tx("@{%s%.4X}@\r\n", request, crc);
	ms = 0;
	while (!correctReplyFromSIE) {
		rxResult = Rx(s, 100000);
		if (rxResult == RX_SUCESS) {
			if (strcmp(s, replyh) == 0) {
				correctReplyFromSIE = true;
			}
			else {	// Type B error : "@{...}@" with incorrect byte in "..."
				Tx("@{%s%.4X}@\r\n", request, crc);
			}
		}
		else if (rxResult == RX_WITHOUT_ENDCODE) {	// Type A error
			Tx("@{E_ABORT}@\r\n");
		}
		ms += 100;
		if (ms >= 1000) {	// every 1 second, send request again
			Tx("@{%s%.4X}@\r\n", request, crc);
			ms = 0;
			lib.peripheral.WatchDog_Serve();
		}
	}
}

void UI_AllInOneTest1::Msg_Connect_MatchWithFunc(S8 *request, int (*matchFunc)(S8 *request, S8* reply))
{
	U16 crc;
	int ms;
	S8 s[1000];
	int match = 1;
	int rxResult;
	
	crc = CalcCRC16((U8*)request, strlen(request), 0, 0, 0, 0);
	Tx("@{%s%.4X}@\r\n", request, crc);
	ms = 0;
	while (match != 0) {
		rxResult = Rx(s, 100000);
		if (rxResult == RX_SUCESS) {
			match = matchFunc(request, s);
			if (match == 0) {
				break;
			}
			else {	// Type B error : "@{...}@" with incorrect byte in "..."
				Tx("@{%s%.4X}@\r\n", request, crc);
			}
		}
		else if (rxResult == RX_WITHOUT_ENDCODE) {	// Type A error
			Tx("@{E_ABORT}@\r\n");
		}
		ms += 100;
		if (ms >= 1000) {	// every 1 second, send request again
			Tx("@{%s%.4X}@\r\n", request, crc);
			ms = 0;
			lib.peripheral.WatchDog_Serve();
		}
	}
	
	return;
}

bool UI_AllInOneTest1::ConnectSIE()
{
	S8 msg[200];
	
	sprintf(msg, "H-%s", CONFIG_STB_MODEL_SIE);
	Msg_Connect_MatchWithStr(msg, msg);
	
	return true;
}


bool UI_AllInOneTest1::IsSingleItemTest(S8* msg)
{
	int len = strlen(msg);
	if (len == 13 && memcmp(msg, "@{TI", 4) == 0) {
		if (isdigit(msg[4]) && isdigit(msg[5]) && isdigit(msg[6]) ) {
			if (memcmp(msg+11, "}@", 2) == 0) {
				return true;
			}
		}
	}
	return false;
}

bool UI_AllInOneTest1::CheckCRC(S8* msg)
{
	int len = strlen(msg);
	U8	tmp[200];
	U16 crc;
	S8 sCrc[5];
	
// check CRC16
	memcpy(tmp, msg+2, len-8);	// CRC didn't include "@{"
	tmp[len-8] = '\0';
	crc = CalcCRC16((U8*)tmp,strlen((S8*)tmp),0,0,0,0);
	sprintf(sCrc, "%.4X", crc);
	if (memcmp(sCrc, msg+len-6, 4) == 0) {
		return true;
	}
	else {
		return false;
	}
}

int UI_AllInOneTest1::GetItemNr(S8* s)
{
	U8 tmp[4];
	int itemNr;
	
	memcpy(tmp, s, 3);
	tmp[3] = '\0';
	if (strcmp((S8*)tmp, "000") == 0) {
		itemNr = 0;
	}
	else {
		itemNr = atoi((S8*)tmp);
		if (itemNr == 0) {
			itemNr = -1;
		}
	}
	
	return itemNr;
}

int UI_AllInOneTest1::SingleItemTest_GetItemNr(S8* msg)
{
	return GetItemNr(msg+strlen("@{TI"));
}

bool UI_AllInOneTest1::IsMultiItemTest(S8* msg)
{
	bool pass;
	int len = strlen(msg);
	int i = 0;
	
	if ( (memcmp(msg, "@{", 2) != 0) || (memcmp(msg+len-2, "}@", 2) != 0) ) {
		return false;
	}
	
	i += strlen("@{");
	// Check 1th TIxxx
	pass = false;
	if ( (memcmp(msg+i, "TI", 2) == 0) && 
		 (isdigit(msg[i+2]) && isdigit(msg[i+3]) && isdigit(msg[i+4]) ) ) {
		pass = true;
	}
	if (!pass) {
		return false;
	}
	
	i += strlen("TIxxx");
	// Check 2nd..last +TIxxx
	while (i < (len-6) ) {	// strlen("<CRC16>}@") == 6
		pass = false;
		if ( (msg[i] == '+') &&
			 (isdigit(msg[i+1]) && isdigit(msg[i+2]) && isdigit(msg[i+3]) ) ) {
			pass = true;
		}
		if (!pass) {
			return false;
		}
		i += strlen("+xxx");
	}
	if (i != (len-6)) {
		return false;
	}
	
	return true;
}

int UI_AllInOneTest1::MultiItemTest_GetItemNr(S8* msg, int* itemNr)
{
// FAIL : return 0
// SUCCESS : return size of itemNr[]
	int i = 0;
	int len = strlen(msg);
	int size = 0;
	int number;
		
	i = strlen("@{TI");
	// Check 2nd..last +TIxxx
	while (i < (len-6) ) {
		number = GetItemNr(msg+i);
		if (number != -1) {
			itemNr[size] = number;
			size++;
		}
		else {
			size = 0;
			return size;
		}
		i += strlen("xxx+");
	}
	
	return size;
}

void UI_AllInOneTest1::RunTestItem()
{
	int rxResult;
	bool success;
	S8 msg[200];
	S8 tmp[200];
	int itemNr[100];
	int size;
	int i;
	U32	result;
	U32 lError = 0;
	U16 crc;
	
	while (1) {
		rxResult = Rx(msg, 100000);
		if (rxResult == RX_SUCESS) {
			if (IsSingleItemTest(msg)) {
				success = CheckCRC(msg);
				if (success) {
					itemNr[0] = SingleItemTest_GetItemNr(msg);
					if (itemNr[0] != -1) {
						Tx("@{TI%.3d}@\r\n", itemNr[0]);
						OutMsg()->RunningItem((U8)itemNr[0]);
						result = RunItem((U8)itemNr[0], 0);
						OutMsg()->TestResult(result);
						switch (result) {
						case TI_SUCCESS:
							Tx("@{T-PASS}@\r\n");
							Tx("@{T-PASS}@\r\n");
							Tx("@{T-PASS}@\r\n");
							break;
						case TI_FAILURE:
							Tx("@{T-FAIL}@\r\n");
							Tx("@{T-FAIL}@\r\n");
							Tx("@{T-FAIL}@\r\n");
							break;
						case TI_NO_THIS_ITEM:
							Tx("@{T-NULL}@\r\n");
							break;
						}
					}
				}
				else {
					Tx("@{E-CRC}@\r\n");
				}
			}
			else if (IsMultiItemTest(msg)) {
				success = CheckCRC(msg);
				if (success) {
					size = MultiItemTest_GetItemNr(msg, itemNr);
					if (size > 16) {
						Tx("@{E-OVERLOAD}@\r\n");
					}
					else if (size > 0) {
						sprintf(tmp, "%.3d", itemNr[0]);
						for (i = 1; i < size; i++) {
							sprintf(tmp+i*4-1, "+%.3d", itemNr[i]);
						}
						Tx("@{%s}@\r\n", tmp);
						lError = 0;
						for (i = 0; i < size; i++) {
							OutMsg()->RunningItem((U8)itemNr[i]);
							result = RunItem((U8)itemNr[i], 0);
							OutMsg()->TestResult(result);
							if (result == TI_FAILURE) {
								lError |= (1 << i);
							}
						}
						if (lError == 0) {
							Tx("@{T-PASS}@\r\n");
						}
						else { // lError > 0
							sprintf(tmp, "T-FAIL+%.4X", lError);	// caculate CRC
							crc = CalcCRC16((U8*)tmp,strlen(tmp),0,0,0,0);
							Tx("@{T-FAIL+%.4X%.4X}@\r\n", lError, crc);
						}
					}
				}
				else {
					Tx("@{E-CRC}@\r\n");
				}
			}
			else {
				Tx("@{E-CRC}@\r\n");
			}
		}
		else if (rxResult == RX_WITHOUT_ENDCODE) {
			Tx("@{E-ABORT}@\r\n");
		}
		lib.peripheral.WatchDog_Serve();
	}
}

void UI_AllInOneTest1::Run()
{
	bool success;
	
	UartMode oldMode = lib.rs232.SetUartMode(BLT_SIE);
	bool rs232enabled = lib.rs232.DisableOutput();
	ClearRS232Buffer();
	success = ConnectSIE();
	if (success) {
		RunTestItem();
	}
	if (rs232enabled) {	// restore RS232 status
		lib.rs232.EnableOutput();
	}
	lib.rs232.SetUartMode(oldMode);
}

#endif
