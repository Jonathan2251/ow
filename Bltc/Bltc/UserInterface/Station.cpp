// Author : Gamma Chen

#ifdef CONFIG_UI_STATION_TEST

#include "Station.h"
#include "string.h"
#include "UserInterface.h"
#include "TiLib.h"

extern UserInterface* 		ui;

//##ModelId=46FA16CD0040
Station::Station()
{
	Init();
}

//##ModelId=46FA16CD0041
Station::Station(S8 id)
{
	Init();
	_id = id;
}

//##ModelId=46FA16CD004E
void Station::Init()
{
	_id = 0;
	_desc = 0;
	_argSize = 0;
	_error.code[0] = _error.code[1] = _error.code[2] = _error.code[3] = 0;
	_error.size = 0;
	_lastItemError = 0;
	_stopTest = false;
	for (int i = 0; i < NR_ITEMS; i++) {
		_item[i] = NO_ITEM;
	}
}

//##ModelId=46FA16CD004F
U8 Station::Id()
{
	return _id;
}

//##ModelId=480FFBE700FB
int Station::ItemsNr() {
	int nrItems = 0;
	
	for (int i = 0; i < NR_ITEMS; i++) {
		U8 item = _item[i];
		if (item == NO_ITEM) {
			break; 
		}
		nrItems++;
	}
	return nrItems;
}

//##ModelId=480FFBEC02A1
int Station::TestItemsNr() {
	int nrTestItems = 0;
	
	for (int i = 0; i < NR_ITEMS; i++) {
		U8 item = _item[i];
		if (item == NO_ITEM) {
			break; 
		}
		else if (gItem[item].type == TEST_ITEM_TYPE) {
			nrTestItems++;
		}
	}
	return nrTestItems;
}

//##ModelId=46FA16CD0050
S8* Station::Description()
{
	return _desc;
}

//##ModelId=46FA16CD0051
void Station::GetItemList(U8 *item)
{
	for (int i = 0; i < NR_ITEMS; i++) {
		item[i] = _item[i];
	}
}

//##ModelId=46FA16CD0053
U32 Station::GetArgList(U32 *arg)
{
	for (int i = 0; i < _argSize; i++) {
		arg[i] = _arg[i];
	}
	
	return _argSize;
}

//##ModelId=46FA16CD005F
void Station::SetId(U8 id)
{
	_id = id;
}

//##ModelId=46FA16CD0061
void Station::SetDesc(S8	* p)
{
	_desc = p;
}

//##ModelId=46FA16CD0063
void Station::SetItemList(U8 item[])
{
	int i;
	for (i = 0; i < NR_ITEMS && item[i] != NO_ITEM; i++) {
		_item[i] = item[i];
	}
	item[i] = NO_ITEM;
}

//##ModelId=46FA16CD006D
void Station::SetItemListFromStr(S8 *str)
{
	U8 itemList[NR_ITEMS];
	S8 seps[] = " ,\t\n";
	S8 *p = str;
	S8 *token;
	int i;
	
	for (i = 0; i < NR_ITEMS; i++) {
		itemList[i] = 255;
	}
	p += strlen("itemList");
	token = strtok(p, seps);
	for (i = 0; i < NR_ITEMS && token != NULL; i++) {
		itemList[i] = (U8)atoi(token);
		// Get next token:
		token = strtok( NULL, seps );
	}
	SetItemList(itemList);
}

//##ModelId=46FA16CD006F
void Station::SetArgList(U32 arg[NR_ARGLIST], U32 size)
{
	int i;
	for (i = 0; i < size; i++) {
		_arg[i] = arg[i];
	}
	_argSize = size;
}

//##ModelId=46FA16CD0072
void Station::AddArg(U32 arg)
{
	if (_argSize < NR_ARGLIST - 1) {
		_arg[_argSize] = arg;
		_argSize++;
	}
}

//##ModelId=46FA16CD007F
void Station::Test()
{
	U32 i, item;
	int testItemCount = 0;
	U32 argIdx = 0;
	
	_stopTest = false;
	_error.code[0] = _error.code[1] = _error.code[2] = _error.code[3] = 0;
	_error.size = ((TestItemsNr() - 1) / 16) + 1;
	//Manual test station 
	for (i = 0 ; i < NR_ITEMS; i++) {
		if (_stopTest) {
			break;
		}
		item = _item[i];
		if (item == NO_ITEM) {
			break; 
		}
		else if (gItem[item].type == TEST_ITEM_TYPE)  {
			ui->OutMsg()->RunningItem(item);
			_lastItemError = ui->RunItem(item, 0);
			if (_lastItemError) {
				if (testItemCount < 16) {
					_error.code[0] |= (1 << testItemCount);
				}
				else if (testItemCount >= 16 && testItemCount < 2*16) {
					_error.code[1] |= (1 << (testItemCount-16));
				}
				else if (testItemCount >= 2*16 && testItemCount < 3*16) {
					_error.code[2] |= (1 << (testItemCount-2*16));
				}
				else if (testItemCount >= 3*16 && testItemCount < 4*16) {
					_error.code[3] |= (1 << (testItemCount-3*16));
				}
			}
			testItemCount++;
		}
		else if (gItem[item].type == ACTION_ITEM_TYPE) {
			ui->RunItem(item, 0);
		}
		else if (gItem[item].type == CONTROL_ITEM_TYPE) {
			ui->RunItem(item, _arg[argIdx]);
			argIdx++;
		}
		lib.peripheral.WatchDog_Serve();
	}
}

//##ModelId=46FA16CD0080
ErrorCode Station::Error()
{
	return _error;
}

//##ModelId=46FA16CD0081
U32 Station::LastItemError()
{
	return _lastItemError;
}

//##ModelId=46FA16CD0082
void Station::StopTest()
{
	_stopTest = true;
}

#ifdef CONFIG_DISPLAY_STATION_INFO

//##ModelId=480FFC65038B
void Station::CaculateErrCode(S32 testItemIdx, S32* errCode) {
	if (testItemIdx < 16) {
		errCode[0] = (1 << testItemIdx);
		errCode[1] = 0;
		errCode[2] = 0;
		errCode[3] = 0;
	}
	else if (testItemIdx >= 16 && testItemIdx < 2*16) {
		errCode[0] = 0;
		errCode[1] = (1 << (testItemIdx-16));
		errCode[2] = 0;
		errCode[3] = 0;
	}
	else if (testItemIdx >= 2*16 && testItemIdx < 3*16) {
		errCode[0] = 0;
		errCode[1] = 0;
		errCode[2] = (1 << (testItemIdx-2*16));
		errCode[3] = 0;
	}
	else if (testItemIdx >= 3*16 && testItemIdx < 4*16) {
		errCode[0] = 0;
		errCode[1] = 0;
		errCode[2] = 0;
		errCode[3] = (1 << (testItemIdx-3*16));
	}
	else {
		lib.rs232.Print("Station::CaculateErrCode::Error!!!\r\n");
		exit(1);
	}
}

//##ModelId=480FFC7C0177
void Station::GetAlignmentSpaces(S8* spaces) {
	int nrTestItems = TestItemsNr();
	if (nrTestItems <= 16) {
		strcpy(spaces, "       ");
	}
	else if (nrTestItems > 16 && nrTestItems <= 2*16) {
		strcpy(spaces, "           ");
	}
	else if (nrTestItems > 2*16 && nrTestItems <= 3*16) {
		strcpy(spaces, "               ");
	}
	else if (nrTestItems > 3*16 && nrTestItems <= 4*16) {
		strcpy(spaces, "                   ");
	}
	else {
		lib.rs232.Print("Station::GetAlignmentSpaces::Error!!!\r\n");
		exit(1);
	}
}

//##ModelId=480FFC860187
void Station::OutputNoErrCodeEntry() {
	int nrTestItems = TestItemsNr();
	
	if (nrTestItems <= 16) {
		lib.rs232.Print("(0000)   No Error\r\n");
	}
	else if (nrTestItems > 16 && nrTestItems <= 2*16) {
		lib.rs232.Print("(00000000)   No Error\r\n");
	}
	else if (nrTestItems > 2*16 && nrTestItems <= 3*16) {
		lib.rs232.Print("(000000000000)   No Error\r\n");
	}
	else if (nrTestItems > 3*16 && nrTestItems <= 4*16) {
		lib.rs232.Print("(0000000000000000)   No Error\r\n");
	}
	else {
		lib.rs232.Print("Station::OutputNoErrCodeEntry::Error!!!\r\n");
		exit(1);
	}
}

//##ModelId=480FFC900010
void Station::OutputTestItemErrCodeEntry(S32* errCode, S32 itemNr) {
	int nrTestItems = TestItemsNr();
	char *itemName = gItem[itemNr].name;
	
	if (nrTestItems <= 16) {
		lib.rs232.Print("(%.4x) %3d:%s\r\n", errCode[0], itemNr, itemName);
	}
	else if (nrTestItems > 16 && nrTestItems <= 2*16) {
		lib.rs232.Print("(%.4x%.4x) %3d:%s\r\n", errCode[1], errCode[0], itemNr, itemName);
	}
	else if (nrTestItems > 2*16 && nrTestItems <= 3*16) {
		lib.rs232.Print("(%.4x%.4x%.4x) %3d:%s\r\n", errCode[2], errCode[1], errCode[0], itemNr, itemName);
	}
	else if (nrTestItems > 3*16 && nrTestItems <= 4*16) {
		lib.rs232.Print("(%.4x%.4x%.4x%.4x) %3d:%s\r\n", errCode[3], errCode[2], errCode[1], errCode[0], itemNr, itemName);
	}
	else {
		lib.rs232.Print("Error!!!\ntoo many items in one station\r\n");
		exit(1);
	}
}

//##ModelId=480FFC9C02FE
void Station::OutputActionItemErrCodeEntry(S8* spaces, S32 itemNr) {
	S8 *itemName = gItem[itemNr].name;
	
	lib.rs232.Print("%s%3d:%s\r\n", spaces, itemNr, itemName);
}

//##ModelId=480FFCA60139
void Station::OutputControlItemErrCodeEntry(S8* spaces, S32 itemNr, S32 argCnt) {
	S8 *itemName = gItem[itemNr].name;

	if (gItem[itemNr].id == ITEM_WAIT_KEYIN) {
		lib.rs232.Print("%s%3d:wait_keyin(%s)\r\n", spaces, itemNr, _arg[argCnt]);
	}
	else if (gItem[itemNr].id == ITEM_DELAY_100MS) {
		lib.rs232.Print("%s%3d:delay_100ms(%d)\r\n", spaces, itemNr, _arg[argCnt]);
	}
}

void Station::OutputStationList() {
	S32 errCode[4];
	S8 spaces[100];
	
// output station information to out which is file "station_list.txt"
	if (strlen(Description()) > 0) {
		lib.rs232.Print("DESCRIPTION : %s\r\n", Description());
	}
	OutputNoErrCodeEntry();
	int testItemIdx = 0, argCnt = 0;
	for (int j = 0; j < ItemsNr(); j++) {
		GetAlignmentSpaces(spaces);
		if (gItem[_item[j]].type == TEST_ITEM_TYPE) {
			CaculateErrCode(testItemIdx, errCode);
			OutputTestItemErrCodeEntry(errCode, _item[j]);
			testItemIdx++;
		}
		else if (gItem[_item[j]].type == ACTION_ITEM_TYPE) {
			OutputActionItemErrCodeEntry(spaces, _item[j]);
		}
		else if (gItem[_item[j]].type == CONTROL_ITEM_TYPE) {
			OutputControlItemErrCodeEntry(spaces, _item[j], argCnt);
			argCnt++;
		}
	}
}

void Station::OutputErrCodeList() {
	S32 errCode[4];
	
	if (strlen(Description()) > 0) {
		lib.rs232.Print("DESCRIPTION : %s\r\n", Description());
	}
	OutputNoErrCodeEntry();
	int testItemIdx = 0;
	for (int j = 0; j < ItemsNr(); j++) {
		if (gItem[_item[j]].type == TEST_ITEM_TYPE) {
			CaculateErrCode(testItemIdx, errCode);
			OutputTestItemErrCodeEntry(errCode, _item[j]);
			testItemIdx++;
		}
	}
}

#endif	// CONFIG_DISPLAY_STATION_INFO


#endif	// CONFIG_UI_STATION_TEST
