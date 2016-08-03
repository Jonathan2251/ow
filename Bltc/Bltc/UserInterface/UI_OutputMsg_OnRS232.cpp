// Author : Gamma Chen

//##ModelId=48112C8A0196
#ifdef CONFIG_WITH_RS232

#include "UI_OutputMsg_OnRS232.h"

#include "Items.h"
#include "TiLib.h"

//##ModelId=48112C8A01E4
void UI_OutputMsg_OnRS232::ItemNr(U8 itemNr) const
{
	bool rs232enabled = lib.rs232.EnableOutput();
	if (itemNr < 100) {
		lib.rs232.Print("-%.2d-\r\n", itemNr);
	}
	else {
		lib.rs232.Print("-%.3d-\r\n", itemNr);
	}
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

//##ModelId=48112C8A0242
void UI_OutputMsg_OnRS232::RunningItem(U8 itemNr) const
{
	bool rs232enabled = lib.rs232.EnableOutput();
	if (itemNr < 100) {
		lib.rs232.Print("..%.2d\r\n", itemNr);
	}
	else {
		lib.rs232.Print("..%.3d\r\n", itemNr);
	}
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

//##ModelId=48112C8A02B0
void UI_OutputMsg_OnRS232::TestResult(U32 lError) const
{
	S8 result[10];
	
	if (lError == 0) {
		strcpy(result, "PASS");
	}
	else if (lError == 1) {
		strcpy(result, "FAIL");
	}
	else if (lError == 2) {
		strcpy(result, "No this test item");
	}
	else {
		sprintf(result, "%x", lError);
	}
	bool rs232enabled = lib.rs232.EnableOutput();
	lib.rs232.Print("Test result = %s\r\n", result);
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

//##ModelId=48112C8A031D
void UI_OutputMsg_OnRS232::UsrInputItemNr(U32 itemNr) const
{
	bool rs232enabled = lib.rs232.EnableOutput();
	lib.rs232.Print("%d", itemNr%10);
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

//##ModelId=48112C8A039A
void UI_OutputMsg_OnRS232::NewLine() const
{
	bool rs232enabled = lib.rs232.EnableOutput();
	lib.rs232.Print("\r\n");
	if (!rs232enabled) {	// restore RS232 status
		lib.rs232.DisableOutput();
	}
}

void  UI_OutputMsg_OnRS232::DisplayStr(char *str) const
{
	lib.rs232.Print(str);
}

#endif	// CONFIG_WITH_RS232

