// Author : Gamma Chen

//##ModelId=48450C1602D9
#include "UI_OutputMsg_Composite.h"

UI_OutputMsg_Composite::UI_OutputMsg_Composite() : _size(0)
{
}

//##ModelId=48450C1602F9
bool UI_OutputMsg_Composite::Insert(UI_OutputMsg *outMsg)
{
	if (_size < 10) {
		_outMsg[_size] = outMsg;
		_size++;
		return true;
	}
	else {
		return false;
	}
}

bool UI_OutputMsg_Composite::Delete(UI_OutputMsg *outMsg)
{
	for (int i = 0; i < _size; i++) {
		if (_outMsg[i] == outMsg) {
			for (int j = i; j < _size - 1; j++) {
				_outMsg[j] = _outMsg[j+1];
			}
			_size--;
			return true;
		}
	}
	
	return false;
}

//##ModelId=48450C160366
void UI_OutputMsg_Composite::ItemNr(U8 itemNr) const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->ItemNr(itemNr);
	}
}

//##ModelId=48450C1603D3
void UI_OutputMsg_Composite::RunningItem(U8 itemNr) const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->RunningItem(itemNr);
	}
}

//##ModelId=48450C170078
void UI_OutputMsg_Composite::TestResult(U32 lError) const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->TestResult(lError);
	}
}

//##ModelId=48450C170105
void UI_OutputMsg_Composite::UsrInputItemNr(U32 itemNr) const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->UsrInputItemNr(itemNr);
	}
}

//##ModelId=48450C1701A1
void UI_OutputMsg_Composite::NewLine() const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->NewLine();
	}
}

void UI_OutputMsg_Composite::DisplayStr(char *str) const
{
	for (int i = 0; i < _size; i++) {
		_outMsg[i]->DisplayStr(str);
	}
}
