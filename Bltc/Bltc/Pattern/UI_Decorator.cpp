// Author : Gamma Chen

//##ModelId=480EA75F0114
#include "UI_Decorator.h"

//##ModelId=480EA75F0114
UI_Decorator::UI_Decorator(UserInterface *ui) : UserInterface(), _ui(ui)
{
	ChangeOutMsg(ui->OutMsg());
}

U8 UI_Decorator::SelectItem()
{
	return _ui->SelectItem();
}

//##ModelId=480EA75F017D
U8 UI_Decorator::ProcessKeyIn()
{
	return _ui->ProcessKeyIn();
}
