// Author : Gamma Chen

#include "UserInterface.h"
#include "TiLib.h"

#include "string.h"

U8 gUI_state;
U32 gUI_usrInputNr;
ItemsIterator	gIterator;
UI_Timer gUI_timer;
UI_Timer gUI_timeout;
Timer_MsTick gUI_tick_ms;

//##ModelId=48102AF20103
UserInterface::UserInterface()
 : _iter(gIterator), _state(gUI_state), _usrInputNr(gUI_usrInputNr), _tick_ms(gUI_tick_ms), _timeout(gUI_timeout)
{
}

//##ModelId=48102AF20151
UserInterface::UserInterface(UI_OutputMsg *outMsg)
 : _iter(gIterator), _state(gUI_state), _usrInputNr(gUI_usrInputNr), _tick_ms(gUI_tick_ms), _timeout(gUI_timeout), _outMsg(outMsg)
{
}

//##ModelId=46CBAB58003C
U8 UserInterface::SelectItem()
{
	return SELECTING_ITEM;
}

//##ModelId=48102AF203A3
U8 UserInterface::ProcessKeyIn()
{
	return SELECTING_ITEM;
}

//##ModelId=46CE717302D3
U32 UserInterface::RunItem(U8 itemNr, U32 arg) const
{
	U32 lError = TI_NO_THIS_ITEM;
		
    switch (itemNr)
    {
    #ifdef CONFIG_DISPLAY_STATION_INFO
        case 0:
            lError = items.TI000();
            break;
	#endif // CONFIG_DISPLAY_STATION_INFO
	#ifdef CONFIG_WITH_TVMON
        case 1:
            lError = items.TI001();
            break;
	#endif // CONFIG_WITH_TVMON
        case 2:
            lError = items.TI002();
            break;
        case 3:
            lError = items.TI003();
            break;
        case 4:
            items.AI004();
            lError = 0;
            break;
        case 5:
            items.AI005();
            lError = 0;
            break;
        case 6:
            lError = items.TI006();
            break;
        case 7:
            lError = items.TI007();
            break;
        case 8:
            lError = items.TI008();
            break;
        case 9:
            lError = items.TI009();
            break;
        case 10:
            lError = items.TI010();
            break;
        case 11:
            items.AI011();
            lError = 0;
            break;
        case 12:
            items.AI012();
            lError = 0;
            break;
        case 13:
            lError = items.TI013();
            break;
        case 14:
            lError = items.TI014();
            break;
#ifdef CONFIG_UI_STATION_TEST
        case 15:
            lError = items.TI015();
            break;
#endif	// CONFIG_UI_STATION_TEST
        case 17:
            lError = items.TI017();
            break;
        case 19:
            lError = 1;
            break;
        case 20:
            lError = items.TI020();
            break;
        case 21:
            lError = items.TI021();
            break;
        case 22:
            lError = items.TI022();
            break;
        case 23:
            lError = items.TI023();
            break;
        case 24:
            lError = items.TI024();
            break;
        case 26:
            lError = items.TI026();
            break;
        case 27:
            lError = items.TI027();
            break;
        case 28:
            lError = items.TI028();
            break;
        case 29:
            lError = items.TI029();
            break;
        case 30:
            lError = items.TI030();
            break;
        case 31:
            lError = items.TI031();
            break;
        case 32:
            lError = items.TI032();
            break;
        case 35:
            lError = items.TI035();
            break;
        case 38:
            lError = items.TI038();
            break;
        case 40:
            lError = items.TI040();
            break;
        case 41:
            lError = items.TI041();
            break;
        case 42:
            lError = items.TI042();
            break;
        case 43:
            lError = items.TI043();
            break;
        case 44:
            lError = items.TI044();
            break;
        case 45:
            lError = items.TI045();
            break;
#ifdef CONFIG_UI_STATION_TEST
        case 51:
            items.AI051();
            lError = 0;
            break;
#endif	// CONFIG_UI_STATION_TEST
        case 52:
            lError = items.TI052();
            break;
        case 53:
            lError = items.TI053();
            break;
        case 54:
            lError = items.TI054();
            break;
        case 55:
            lError = items.TI055();
            break;
        case 56:
            lError = items.TI056();
            break;
        case 57:
            lError = items.TI057();
            break;
        case 58:
            lError = items.TI058();
            break;
        case 59:
            lError = items.TI059();
            break;
        case 60:
            lError = items.TI060();
            break;
        case 61:
            lError = items.TI061();
            break;
        case 62:
            lError = items.TI062();
            break;
        case 63:
            lError = items.TI063();
            break;
        case 64:
            lError = items.TI064();
            break;
        case 65:
            lError = items.TI065();
            break;
        case 66:
            lError = items.TI066();
            break;
        case 67:
            lError = items.TI067();
            break;
        case 71:
            lError = items.TI071();
            break;
        case 73:
            lError = items.TI073();
            break;
        case 74:
            lError = items.TI074();
            break;
        case 75:
            lError = items.TI075();
            break;
        case 82:
            lError = items.TI082();
            break;
        case 83:
            lError = items.TI083();
            break;
        case 85:
            lError = items.TI085();
            break;
        case 87:
            lError = items.TI087();
            break;
        case 88:
            lError = items.TI088();
            break;
        case 89:
            lError = items.TI089();
            break;
        case 90:
            lError = items.TI090();
            break;
        case 91:
            lError = items.TI091();
            break;
        case 92:
            lError = items.TI092();
            break;
        case 93:
            lError = items.TI093();
            break;
        case 94:
            lError = items.TI094();
            break;
	#ifdef CONFIG_WITH_TUNER_TEST
        case 133:
            lError = items.TI133();
            break;
        case 134:
            lError = items.TI134();
            break;                        
        case 135:
            lError = items.TI135();
            break;
        case 136:
            lError = items.TI136();
            break;
	#endif // #ifdef CONFIG_WITH_TUNER_TEST
#ifdef CONFIG_UI_STATION_TEST
        case 201:
            items.AI201();
            lError = 0;
            break;
        case 202:
            items.AI202();
            lError = 0;
            break;
#ifdef CONFIG_WITH_OSD
        case 203:
            items.AI203();
            lError = 0;
            break;
#endif	// CONFIG_WITH_OSD
#endif	// CONFIG_UI_STATION_TEST
#ifdef CONFIG_WITH_OSD
        case 204:
            items.AI204();
            lError = 0;
            break;
#endif	// CONFIG_WITH_OSD
        case 205:
            items.AI205();
            lError = 0;
            break;
        case 206:
            items.AI206();
            lError = 0;
            break;
#ifdef CONFIG_UI_STATION_TEST
        case 207:
            items.AI207();
            lError = 0;
            break;
        case 208:
            items.AI208();
            lError = 0;
            break;
#endif	// CONFIG_UI_STATION_TEST
        case 209:
            items.AI209();
            lError = 0;
            break;
        case 210:
            items.AI210();
            lError = 0;
            break;
#ifdef CONFIG_UI_STATION_TEST
#ifdef CONFIG_WITH_OSD
        case 211:
            items.AI211();
            lError = 0;
            break;
#endif	// CONFIG_WITH_OSD
        case 212:
            items.AI212();
            lError = 0;
            break;
#endif	// CONFIG_UI_STATION_TEST
        case 253:
            items.CI253(arg);
            break;
    }
    return lError;
}

#include "Items.h"

//##ModelId=46DCEEC30236
void UserInterface::Run()
{
	U8 itemNr;
	U32 lError;
	
	lib.rs232.SetUartMode(BLT_Debug);
	lib.rs232.DisableOutput();
	gUI_state = SCROLLING;
	gUI_usrInputNr = 0;
	_iter._itemNr = 0;
    while (1) {
	    OutMsg()->ItemNr(_iter.CurrentItem());
        itemNr = SelectItem();
        if (itemNr == EXIT_UI) {
            return;
        }
		OutMsg()->RunningItem(itemNr);
        lError = RunItem(itemNr, 0);
		OutMsg()->TestResult(lError);
		lib.DelayMS(1000);
    }
}

//##ModelId=48102AF301ED
UI_OutputMsg* UserInterface::OutMsg()
{
	return _outMsg;
}

//##ModelId=48102AF300C4
void UserInterface::ChangeOutMsg(UI_OutputMsg* outMsg)
{
	_outMsg = outMsg;
}
