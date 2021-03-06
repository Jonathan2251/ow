#include <string>
#include <fstream>
#include <mmsystem.hpp> // for SDK function sndPlaySound(), waveOutGetVolume(), waveOutSetVolume().

#include "Group.h"

using namespace std;
   
#define ICON_WIDTH 15  // gamma
#define ICON_HEIGHT 13  // gamma

AnsiString keyName[] = {"KEY_UP", "KEY_DOWN", "KEY_LEFT", "KEY_RIGHT", "KEY_REDIAL", "KEY_ON_OFF",
    "KEY_HANDSFREE", "KEY_TOP_MESSAGE", "KEY_FT_SERVICES", "KEY_SOS", "KEY_MENU",
    "KEY_1", "KEY_2", "KEY_3", "KEY_4", "KEY_5", "KEY_6", "KEY_7", "KEY_8", "KEY_9",
    "KEY_STAR", "KEY_0", "KEY_HASH", "KEY_SOS_SMS", "KEY_SMS", "KEY_ON_OFF_LONG_PRESS"
};

AnsiString stateName[] = {"POWER_OFF", "CONNECT_BASE", "MAIN_MENU",
    "BROWSING_MENU_TREE", "SET_AUTO_ANSWER_ON_OFF", "SET_ANY_KEY_ANSWER_ON_OFF",
    "SET_DIRECT_CALL_ON_OFF", "SEARCH_BASE", "INPUT_PIN", "REGISTER_BASE", "HS_VOL_SETTING",
    "EXT_RING_VOL_SETTING", "INT_RING_VOL_SETTING", "REGISTRAION_BASE",
    "EDIT_SMS", "OFF_HOOK", "DIAL_NUMBER"
};

PT *pt;
extern FT *ft;
extern int timeOut;

char KeyToAscii(int key) {
    char c;
    switch (key) {
        case KEY_1:
            c = '1';
            break;
        case KEY_2:
            c = '2';
            break;
        case KEY_3:
            c = '3';
            break;
        case KEY_4:
            c = '4';
            break;
        case KEY_5:
            c = '5';
            break;
        case KEY_6:
            c = '6';
            break;
        case KEY_7:
            c = '7';
            break;
        case KEY_8:
            c = '8';
            break;
        case KEY_9:
            c = '9';
            break;
        case KEY_0:
            c = '0';
            break;
    }
    return c;
};
   
string KeyToAscii(string &str) {
    string s;
    for (unsigned int i = 0; i < str.size(); i++) {
        switch (str[i]) {
        case char(KEY_1):
            s.push_back('1');
            break;
        case char(KEY_2):
            s.push_back('2');
            break;
        case char(KEY_3):
            s.push_back('3');
            break;
        case char(KEY_4):
            s.push_back('4');
            break;
        case char(KEY_5):
            s.push_back('5');
            break;
        case char(KEY_6):
            s.push_back('6');
            break;
        case char(KEY_7):
            s.push_back('7');
            break;
        case char(KEY_8):
            s.push_back('8');
            break;
        case char(KEY_9):
            s.push_back('9');
            break;
        case char(KEY_0):
            s.push_back('0');
            break;
        }
    }
    return s;
};

string AsciiToKey(string &str) {
    string s;
    for (unsigned int i = 0; i < str.size(); i++) {
        switch (str[i]) {
        case '1':
            s.push_back(char(KEY_1));
            break;
        case '2':
            s.push_back(KEY_2);
            break;
        case '3':
            s.push_back(KEY_3);
            break;
        case '4':
            s.push_back(KEY_4);
            break;
        case '5':
            s.push_back(KEY_5);
            break;
        case '6':
            s.push_back(KEY_6);
            break;
        case '7':
            s.push_back(KEY_7);
            break;
        case '8':
            s.push_back(KEY_8);
            break;
        case '9':
            s.push_back(KEY_9);
            break;
        case '0':
            s.push_back(KEY_0);
            break;
        }
    }
    return s;
};

/*
the pt_config.txt format is:
HSVolume 5 //comment
First string is identify for Config Variables, second data is the Confg Variable Value,
between on them is spaces, "//" and after "//" until end_of_line is comment.
*/
PTConfig::PTConfig() {
// set default value
    for (int i = 0; i < 4; i++) {
        HaveRegisteredBase[i] = false;
    }
    AlarmTime.Format = 1;
    AlarmTime.Hour = 0;   
    AlarmTime.Minute = 0;
    AlarmTime.Second = 0;
    
    ifstream ifs(ParamStr(1).c_str());
    while (!ifs.eof()) {
        string s;
        ifs >> s;
        if (s == "AnswerPhone:AutoAnswer") {
            ifs >> s;
            if (s == "On") {
                AnswerPhone_AutoAnswer = ON;
            }
            else {
                AnswerPhone_AutoAnswer = OFF;
            }
        }
        else if (s == "AnswerPhone:AnyKeyAnswer") {
            ifs >> s;
            if (s == "On") {
                AnswerPhone_AnyKeyAnswer = ON;
            }
            else {
                AnswerPhone_AnyKeyAnswer = OFF;
            }
        }
        else if (s == "Click") {
            ifs >> s;
            if (s == "On") {
                Click = ON;
            }
            else {
                Click = OFF;
            }
        }
        else if (s == "ChargingTone") {
            ifs >> s;
            if (s == "On") {
                ChargingTone = ON;
            }
            else {
                ChargingTone = OFF;
            }
        }
        else if (s == "ConfirmTone") {
            ifs >> s;
            if (s == "On") {
                ConfirmTone = ON;
            }
            else {
                ConfirmTone = OFF;
            }
        }
        else if (s == "HaveRegistered") {
            ifs >> s;
            if (s == "Base1") {
                HaveRegisteredBase[0] = true;
            }
            else if (s == "Base2") {
                HaveRegisteredBase[1] = true;
            }
            else if (s == "Base3") {
                HaveRegisteredBase[2] = true;
            }
            else if (s == "Base4") {
                HaveRegisteredBase[3] = true;
            }
        }
        else if (s == "FindBase1") {
            ifs >> s;
            if (s == "true") {
                FindBase[0] = true;
            }
            else {
                FindBase[0] = false;
            }
        }
        else if (s == "FindBase2") {
            ifs >> s;
            if (s == "true") {
                FindBase[1] = true;
            }
            else {
                FindBase[1] = false;
            }
        }
        else if (s == "FindBase3") {
            ifs >> s;
            if (s == "true") {
                FindBase[2] = true;
            }
            else {
                FindBase[2] = false;
            }
        }
        else if (s == "FindBase4") {
            ifs >> s;
            if (s == "true") {
                FindBase[3] = true;
            }
            else {
                FindBase[3] = false;
            }
        }
        else if (s == "RegisterBase1") {
            ifs >> s;
            if (s == "Success") {
                SuccessRegisterBase[0] = true;
            }
            else {
                SuccessRegisterBase[0] = false;
            }
        }
        else if (s == "RegisterBase2") {
            ifs >> s;
            if (s == "Success") {
                SuccessRegisterBase[1] = true;
            }
            else {
                SuccessRegisterBase[1] = false;
            }
        }
        else if (s == "RegisterBase3") {
            ifs >> s;
            if (s == "Success") {
                SuccessRegisterBase[2] = true;
            }
            else {
                SuccessRegisterBase[2] = false;
            }
        }                    
        else if (s == "RegisterBase4") {
            ifs >> s;
            if (s == "Success") {
                SuccessRegisterBase[3] = true;
            }
            else {
                SuccessRegisterBase[3] = false;
            }
        }
        else if (s == "SelectBase") {
            ifs >> SelectBase;  // 0..4, 0:best base, 1:base1, ...
            if (SelectBase < 0 || SelectBase > 4) {
                SelectBase = 0;
            }
        }
        else if (s == "PriorityCallAlert") {
            ifs >> PriorityCallAlert;
        }
        else if (s == "IdleDisplayOn") {
            ifs >> IdleDisplayOn;
        }  
        else if (s == "IdleDisplay") {
            ifs >> IdleDisplay;
        }
        else if (s == "IdleDisplay:ProductName") {
            char temp[1000];
            ifs.get(temp, 20, '\n');
            IdleDisplay_Product = temp;
        }
        else if (s == "IdleDisplay:BaseName") {
            ifs >> IdleDisplay_Base;
        }
        else if (s == "SelectLanguage") {
            ifs >> SelectLanguage;
        }
        else if (s == "ExternalRingMelody") {
            ifs >> ExtRingMelody;
            if (ExtRingMelody < 0 || ExtRingMelody > 9) {
                ExtRingMelody = 0;
            }
        }
        else if (s == "ExternalRingVolume") {
            ifs >> ExtRingVol;
            if (ExtRingVol < 0 || ExtRingVol > 9) {
                ExtRingVol = 0;
            }
        }
        else if (s == "InternalRingMelody") {
            ifs >> IntRingMelody;
            if (IntRingMelody < 0 || IntRingMelody > 9) {
                IntRingMelody = 0;
            }
        }
        else if (s == "InternalRingVolume") {
            ifs >> IntRingVol;
            if (IntRingVol < 0 || IntRingVol > 9) {
                IntRingVol = 0;
            }
        }
        else if (s == "AlarmOnOff") {
            ifs >> AlarmOnOff;
        }
        else if (s == "AlarmTime::Hour") {
            ifs >> AlarmTime.Hour;
        }  
        else if (s == "AlarmTime::Minute") {
            ifs >> AlarmTime.Minute;
        }
        else if (s == "AlarmTime::Second") {
            ifs >> AlarmTime.Second;
        }
        else if (s == "AppointOnOff") {
            ifs >> AppointOnOff;
        } 
        else if (s == "MemoNoteOnOff") {
            ifs >> MemoNoteOnOff;
        }
        else if (s == "RoomMonitorOnOff") {
            ifs >> RoomMonitorOnOff;
        }
        else if (s == "Battery") {
            ifs >> batteryStatus;
        }
        else if (s == "[Phone_Book]") {
            ifs >> s;
            if (s == "{") {
                PhoneBookEntry pbe;
                for (int i = 0; i < 40;) {
                    ifs >> s;
                    if (s == "name") {
                        ifs >> pbe.name;
                    }
                    else if (s == "office_tel") {
                        ifs >> pbe.tel.office;
                        if (pbe.tel.office == "null") {
                            pbe.tel.office = "";
                        }
                    }
                    else if (s == "mobile_tel") {
                        ifs >> pbe.tel.mobile;
                        if (pbe.tel.mobile == "null") {
                            pbe.tel.mobile = "";
                        }
                    }
                    else if (s == "fax_tel") {
                        ifs >> pbe.tel.fax;
                        if (pbe.tel.fax == "null") {
                            pbe.tel.fax = "";
                        }
                        pb.Add(pbe);
                        i++;
                    }
                    else if (s == "}") {
                        break;
                    }
                }
            }
        }
        else if (s == "[Ring_Vip]") {
            ifs >> s;
            if (s == "{") {
                for (int i = 0; i < 5; i++) {
                    ifs >> s;
                    if (s == "Melody") {
                        ifs >> Vip[i].Melody;
                    } 
                    ifs >> s;
                    if (s == "Volume") {
                        ifs >> Vip[i].Vol;
                    }
                }
            }
        }
    }
    return;
};

void PTConfig::SaveConfig() {
    ofstream ofs(ParamStr(1).c_str());
    if (AnswerPhone_AutoAnswer == ON) {
        ofs << "AnswerPhone:AutoAnswer On" << endl;
    }
    else {
        ofs << "AnswerPhone:AutoAnswer Off" << endl;
    }
    if (AnswerPhone_AnyKeyAnswer == ON) {
        ofs << "AnswerPhone:AnyKeyAnswer On" << endl;
    }
    else {
        ofs << "AnswerPhone:AnyKeyAnswer Off" << endl;
    }
    if (Click == ON) {
        ofs << "Click On" << endl;
    }
    else {
        ofs << "Click Off" << endl;
    }
    if (ChargingTone == ON) {
        ofs << "ChargingTone On" << endl;
    }
    else {
        ofs << "ChargingTone Off" << endl;
    }   
    if (ConfirmTone == ON) {
        ofs << "ConfirmTone On" << endl;
    }
    else {
        ofs << "ConfirmTone Off" << endl;
    }

    if (HaveRegisteredBase[0]) {
        ofs << "HaveRegistered Base1" << endl;
    }
    if (HaveRegisteredBase[1]) {
        ofs << "HaveRegistered Base2" << endl;
    }
    if (HaveRegisteredBase[2]) {
        ofs << "HaveRegistered Base3" << endl;
    }
    if (HaveRegisteredBase[3]) {
        ofs << "HaveRegistered Base4" << endl;
    }

    if (FindBase[0]) {
        ofs << "FindBase1 true" << endl;
    }
    if (FindBase[1]) {
        ofs << "FindBase2 true" << endl;
    }
    if (FindBase[2]) {
        ofs << "FindBase3 true" << endl;
    }
    if (FindBase[3]) {
        ofs << "FindBase4 true" << endl;
    }

    if (SuccessRegisterBase[0]) {
        ofs << "RegisterBase1 Success" << endl;
    }
    else {
        ofs << "RegisterBase1 Fail" << endl;
    }
    if (SuccessRegisterBase[1]) {
        ofs << "RegisterBase2 Success" << endl;
    }
    else {
        ofs << "RegisterBase2 Fail" << endl;
    }
    if (SuccessRegisterBase[2]) {
        ofs << "RegisterBase3 Success" << endl;
    }
    else {
        ofs << "RegisterBase3 Fail" << endl;
    }
    if (SuccessRegisterBase[3]) {
        ofs << "RegisterBase4 Success" << endl;
    }
    else {
        ofs << "RegisterBase4 Fail" << endl;
    }
    ofs << "SelectBase " << SelectBase << endl;  // 0..4
    ofs << "PriorityCallAlert " << PriorityCallAlert << endl;
    ofs << "IdleDisplayOn " << IdleDisplayOn << endl;  
    ofs << "IdleDisplay " << IdleDisplay << " // 0:Base Name, 1:Date & Time" << endl;
    ofs << "IdleDisplay:ProductName " << IdleDisplay_Product << endl;
    ofs << "IdleDisplay:BaseName " << IdleDisplay_Base << endl;
    ofs << "SelectLanguage " << SelectLanguage << endl;

    ofs << "ExternalRingMelody " << ExtRingMelody << endl;
    ofs << "ExternalRingVolume " << ExtRingVol << endl;
    ofs << "InternalRingMelody " << IntRingMelody << endl;
    ofs << "InternalRingVolume " << IntRingVol << endl;
    ofs << "AlarmOnOff " << AlarmOnOff << endl;
    ofs << "AlarmTime::Hour " << AlarmTime.Hour << endl; 
    ofs << "AlarmTime::Minute " << AlarmTime.Minute << endl;  
    ofs << "AlarmTime::Second " << AlarmTime.Second << endl;
    ofs << "AppointOnOff " << AppointOnOff << " // 0:On, 1:Off" << endl;
    ofs << "MemoNoteOnOff " << MemoNoteOnOff << " // 0:On, 1:Off" << endl;
    ofs << "RoomMonitorOnOff " << RoomMonitorOnOff << " // 0:On, 1:Off" << endl;
    ofs << "Battery " << batteryStatus << endl;

    ofs << "[Phone_Book] {" << endl;
    for (int i = 0; i < pb.Size(); i++) {
        ofs << "name " << pb.Item(i).name << "\t";
        if (pb.Item(i).tel.office.empty()) {
            ofs << "office_tel " << "null\t\t";
        }
        else {
            ofs << "office_tel " << pb.Item(i).tel.office << "\t";
        }
        if (pb.Item(i).tel.mobile.empty()) {
            ofs << "mobile_tel " << "null\t\t";
        }
        else {
            ofs << "mobile_tel " << pb.Item(i).tel.mobile << "\t";
        }
        if (pb.Item(i).tel.fax.empty()) {
            ofs << "fax_tel " << "null" << endl;
        }
        else {
            ofs << "fax_tel " << pb.Item(i).tel.fax << endl;
        }
    }
    ofs << "}" << endl;

    ofs << "[Ring_Vip] {" << endl;
    for (int i = 0; i < 5; i++) {
        ofs << "Melody " << Vip[i].Melody << "\t";
        ofs << "Volume " << Vip[i].Vol << endl;
    }
    ofs << "}" << endl;
    return;
};

MenuTree::MenuTree() {
};

int MenuTree::GetCurrNodeIdx() {
    int i;
    TTreeNode *p = currNode->Parent->getFirstChild();
    for (i = 0; p != currNode; i++) {
        p = p->getNextSibling();
    }
    return i;
};

void MenuTree::EnterPhoneBookMenu() {
    Form1->TreeView1->LoadFromFile("tree\\phone_book_tree.txt");
    root = Form1->TreeView1->Items->GetFirstNode();
    currNode = root;
    currNode = currNode->getFirstChild();
    Form1->TreeView1->Visible = true;
    currNode->Expanded = true;
    currNode->Selected = true;
};

void MenuTree::ExitPhoneBookMenu() {
    Form1->TreeView1->SaveToFile("tree\\phone_book_tree.txt");
};

void MenuTree::EnterFunctionMenu() {
    Form1->TreeView1->LoadFromFile("tree\\function_tree.txt");
    root = Form1->TreeView1->Items->GetFirstNode();
    currNode = root;
    currNode = currNode->getFirstChild();
    Form1->TreeView1->Visible = true;
    currNode->Expanded = true;
    currNode->Selected = true;
};

void MenuTree::ExitFunctionMenu() {
    Form1->TreeView1->SaveToFile("tree\\function_tree.txt");
};

void MenuTree::EnterOptionMenu() {
    returnNodeId = currNode->ItemId;
    Form1->TreeView1->LoadFromFile("tree\\option_tree.txt");
    root = Form1->TreeView1->Items->GetFirstNode();
    currNode = root;
    currNode = currNode->getFirstChild();
    Form1->TreeView1->Visible = true;
    currNode->Expanded = true;
    currNode->Selected = true;
};

void MenuTree::ExitOptionMenu() {
    Form1->TreeView1->LoadFromFile("tree\\phone_book_tree.txt");
    root = Form1->TreeView1->Items->GetFirstNode();
    currNode = Form1->TreeView1->Items->GetNode(returnNodeId);
    currNode->Expanded = true;
    currNode->Selected = true;
};

// Each character displayed by unique dedicated TLabel.
CChar::CChar(int x, int y, int w, int h) {
    ch = new TLabel(Form1);
    ch->Parent = Form1;
    ch->Font->Size = 8;
    ch->Alignment = taCenter;
    ch->Left = x;
    ch->Top = y;
    ch->Width = w;
    ch->Height = h;
    ch->Transparent = true;
    ch->Visible = true;
    blink = false;
};

void CChar::operator=(char c) {
    ch->Caption = c;
};
  
bool CChar::operator==(char c) {
    return ch->Caption == c;
};

void CChar::StopBlink() { //Stop character blinking.
    ch->Visible = true;
    blink = false;
};

Line::Line(int x, int y) : length(0), blink(false) {
    for (int i = 0; i < 16; i++) {
        pos[i] = new CChar(x+i*10, y, 10, 14); // gamma:need to change when porting a new handset image.
    }
};

// Each line on lcd can display 16 characters.
void Line::operator=(char *s) {
    int k = strlen(s);
    if (k > 16) {
        k = 16;
    }
    int i;
    for (i = 0; i < k; i++) {
        *pos[i] = s[i];
    }
    length = i;
    for ( ; i < 16; i++) {
        *pos[i] = ' ';
    }
};

void Line::operator=(string s) {
    *this = &s[0];
};

void Line::operator=(AnsiString s) {
    *this = s.c_str();
};

void Line::operator=(GDate &date) {
    char s[11];
    AnsiString temp;
    if (date.Format == 0) {
        strcpy(s, "0000/00/00");
        temp = IntToStr(date.Year);
        if (temp.Length() <= 4) {
            memcpy(s + 4 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Month);
        if (temp.Length() <= 2) {
            memcpy(s + 7 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Day);
        if (temp.Length() <= 2) {
            memcpy(s + 10 - temp.Length(), temp.c_str(), temp.Length());
        }
    }
    else if (date.Format == 1) {
        strcpy(s, "00/00/0000");
        temp = IntToStr(date.Month);
        if (temp.Length() <= 2) {
            memcpy(s + 2 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Day);
        if (temp.Length() <= 2) {
            memcpy(s + 5 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Year);
        if (temp.Length() <= 4) {
            memcpy(s + 10 - temp.Length(), temp.c_str(), temp.Length());
        }
    }
    else if (date.Format == 2) {
        strcpy(s, "00/00/0000");
        temp = IntToStr(date.Day);
        if (temp.Length() <= 2) {
            memcpy(s + 2 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Month);
        if (temp.Length() <= 2) {
            memcpy(s + 5 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(date.Year);
        if (temp.Length() <= 4) {
            memcpy(s + 10 - temp.Length(), temp.c_str(), temp.Length());
        }
    }
    *this = s;
};

void Line::operator=(GTime time) {
    char s[8];
    AnsiString temp;
    if (time.Format == 0) {
        if (time.Hour < 12) {
            strcpy(s, "AM00:00");
        }
        else {
            strcpy(s, "PM00:00");
            time.Hour -= 12;
        }
        temp = IntToStr(time.Hour);
        if (temp.Length() <= 2) {
            memcpy(s + 4 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(time.Minute);
        if (temp.Length() <= 2) {
            memcpy(s + 7 - temp.Length(), temp.c_str(), temp.Length());
        }
    }
    else if (time.Format == 1) {
        strcpy(s, "00:00");
        temp = IntToStr(time.Hour);
        if (temp.Length() <= 2) {
            memcpy(s + 2 - temp.Length(), temp.c_str(), temp.Length());
        }
        temp = IntToStr(time.Minute);
        if (temp.Length() <= 2) {
            memcpy(s + 5 - temp.Length(), temp.c_str(), temp.Length());
        }
    }
    *this = s;
};

int Line::Length() {
    return length;
};

void Line::StopBlink() { //Stop all characters on Line blinking.
    blink = false;
    for (int i = 0; i < 15; i++) {
        pos[i]->StopBlink();
    }
};

AnsiString Line::ToAnsiString() {
    static AnsiString s;
    s = "";
    int len = Length();
    for (int i = 0; i < len; i++) {
        if (pos[i]->ch->Caption == "") {
            s = s + " ";
        }
        else {
            s = s + pos[i]->ch->Caption;
        }
    }
    return s;
};

string Line::Tostring() {
    static string s;
    AnsiString temp;
    temp = ToAnsiString();
    s = temp.c_str();
    return s;
};

GDate Line::ToDate(int dateFormat) {
    GDate date;
    AnsiString str = ToAnsiString();
    date.Format = dateFormat;
    if (date.Format == 0) {
        //SubString(idx, count) : idx begin from 1, not 0
        date.Year = str.SubString(1, 4).ToInt();
        date.Month = str.SubString(6, 2).ToInt();
        date.Day = str.SubString(9, 2).ToInt();
    }
    else if (date.Format == 1) {
        date.Month = str.SubString(1, 2).ToInt();
        date.Day = str.SubString(4, 2).ToInt();
        date.Year = str.SubString(7, 4).ToInt();
    }
    else if (date.Format == 2) {
        date.Day = str.SubString(1, 2).ToInt();
        date.Month = str.SubString(4, 2).ToInt();
        date.Year = str.SubString(7, 4).ToInt();
    }
    return date;
};
   
GTime Line::ToTime(int timeFormat) {
    GTime time;
    AnsiString str = ToAnsiString();
    time.Format = timeFormat;
    if (time.Format == 0) {
        //SubString(idx, count) : idx begin from 1, not 0
        time.Hour = str.SubString(3, 2).ToInt();
        if (str.SubString(1, 2) == "PM") {
            time.Hour += 12;
        }
        time.Minute = str.SubString(6, 2).ToInt();
    }
    else if (time.Format == 1) {
        time.Hour = str.SubString(1, 2).ToInt();
        time.Minute = str.SubString(4, 2).ToInt();
    }
    return time;
};

AnsiString Line::operator+(char *s) {
    for (int i = length; i < 16; i++) {
        *pos[i] = ' ';
    }
    return ToAnsiString();
};

void Line::operator+=(char *s) {
    int k = strlen(s)+length;
    if (k > 16) {
        k = 16;
    }
    int i;
    for (i = length; i < k; i++) {
        *pos[i] = s[i-length];
    }
    length = i;
};

void Line::Visible() {
    for (int i = 0; i < length; i++) {
        pos[i]->ch->Visible = true;
    }
};
      
void Line::Invisible() {
    for (int i = 0; i < length; i++) {
        pos[i]->ch->Visible = false;
    }
};

Icon::Icon() : blink(false) {
    image = new TImage(Form1);
    image->Parent = Form1;
};

void Icon::StopBlink() {
    blink = false;
    image->Visible = true;
};

SymbolPanel::SymbolPanel(Line *line[2]) {
    for (int i = 0; i < 7; i++) {
        ch[i] = new TStaticText(Form1);
        ch[i]->Parent = Form1;
        ch[i]->Font->Size = line[0]->pos[i]->ch->Font->Size;
        ch[i]->Alignment = taCenter;
        ch[i]->Left = line[0]->pos[2*i]->ch->Left;
        ch[i]->Top = line[0]->pos[i]->ch->Top;
        ch[i]->Width = 2*line[0]->pos[i]->ch->Width;
        ch[i]->Height = line[0]->pos[i]->ch->Height;
        ch[i]->Color = TColor(0x008BBB89);
        ch[i]->Visible = false;
    }
    size = 7; // necessary
};

bool SymbolPanel::Visible() {
    return ch[0]->Visible;
};

// Load Letter and Symbol using aKey information
void SymbolPanel::Load(int aKey, bool upperCase) {
    size = 4;
    switch (aKey) {
        case KEY_1:
            ch[0]->Caption = '"';
            ch[1]->Caption = '.';
            ch[2]->Caption = ',';
            ch[3]->Caption = '-';
            ch[4]->Caption = '&';
            ch[5]->Caption = ':';
            ch[6]->Caption = '1';
            size = 7;
            idx = 0;
            break;
        case KEY_2:
            if (upperCase) {
                ch[0]->Caption = 'A';
                ch[1]->Caption = 'B';
                ch[2]->Caption = 'C';
            }
            else {
                ch[0]->Caption = 'a';
                ch[1]->Caption = 'b';
                ch[2]->Caption = 'c';
            }
            ch[3]->Caption = '2';
            idx = 0;
            break;
        case KEY_3:
            if (upperCase) {
                ch[0]->Caption = 'D';
                ch[1]->Caption = 'E';
                ch[2]->Caption = 'F';
            }
            else {
                ch[0]->Caption = 'd';
                ch[1]->Caption = 'e';
                ch[2]->Caption = 'f';
            }   
            ch[3]->Caption = '3';
            idx = 0;
            break;
        case KEY_4:
            if (upperCase) {
                ch[0]->Caption = 'G';
                ch[1]->Caption = 'H';
                ch[2]->Caption = 'I';
            }
            else {
                ch[0]->Caption = 'g';
                ch[1]->Caption = 'h';
                ch[2]->Caption = 'i';
            }    
            ch[3]->Caption = '4';
            idx = 0;
            break;
        case KEY_5:
            if (upperCase) {
                ch[0]->Caption = 'J';
                ch[1]->Caption = 'K';
                ch[2]->Caption = 'L';
            }
            else {
                ch[0]->Caption = 'j';
                ch[1]->Caption = 'k';
                ch[2]->Caption = 'l';
            }  
            ch[3]->Caption = '5';
            idx = 0;
            break;
        case KEY_6:
            if (upperCase) {
                ch[0]->Caption = 'M';
                ch[1]->Caption = 'N';
                ch[2]->Caption = 'O';
            }
            else {
                ch[0]->Caption = 'm';
                ch[1]->Caption = 'n';
                ch[2]->Caption = 'o';
            }  
            ch[3]->Caption = '6';
            idx = 0;
            break;
        case KEY_7:
            if (upperCase) {
                ch[0]->Caption = 'P';
                ch[1]->Caption = 'Q';
                ch[2]->Caption = 'R';
                ch[3]->Caption = 'S';
            }
            else {
                ch[0]->Caption = 'p';
                ch[1]->Caption = 'q';
                ch[2]->Caption = 'r';
                ch[3]->Caption = 's';
            }
            ch[4]->Caption = '7';
            size = 5;
            idx = 0;
            break;
        case KEY_8:
            if (upperCase) {
                ch[0]->Caption = 'T';
                ch[1]->Caption = 'U';
                ch[2]->Caption = 'V';
            }
            else {
                ch[0]->Caption = 't';
                ch[1]->Caption = 'u';
                ch[2]->Caption = 'v';
            }   
            ch[3]->Caption = '8';
            idx = 0;
            break;
        case KEY_9:
            if (upperCase) {
                ch[0]->Caption = 'W';
                ch[1]->Caption = 'X';
                ch[2]->Caption = 'Y';
                ch[3]->Caption = 'Z';
            }
            else {
                ch[0]->Caption = 'w';
                ch[1]->Caption = 'x';
                ch[2]->Caption = 'y';
                ch[3]->Caption = 'z';
            }
            ch[4]->Caption = '9';
            size = 5;
            idx = 0;
            break;
        case KEY_0:
            ch[0]->Caption = ' ';
            ch[1]->Caption = '0';
            size = 2;
            idx = 0;
            break;
    }
};

void SymbolPanel::Show() {
    for (int i = 0; i < size; i++) {
        if (i == idx) {
            ch[i]->BorderStyle = sbsSingle;
        }
        else {
            ch[i]->BorderStyle = sbsNone;
        }
        ch[i]->Visible = true;
    }
};

void SymbolPanel::Hidden() {
    for (int i = 0; i < size; i++) {
        ch[i]->Visible = false;
    }
};

char SymbolPanel::ActiveSymbol() {
    return ch[idx]->Caption[1];
};

Lcd::Lcd(PTConfig *aCfg) {
    line[0] = new Line(56, 154);// gamma:need to change when porting a new handset image.
    line[1] = new Line(56, 172);// gamma:need to change when porting a new handset image.
    panel = new SymbolPanel(line);

    for (int i = 0; i < BELL; i++) {
        icon[i] = new Icon;
        icon[i]->image->Left = line[1]->pos[0]->ch->Left + ICON_WIDTH*i;
        icon[i]->image->Top = line[1]->pos[0]->ch->Top + 22;
        icon[i]->image->Width = ICON_WIDTH;
        icon[i]->image->Height = ICON_HEIGHT;
        icon[i]->image->Transparent = true;
        icon[i]->image->Stretch = true;
        icon[i]->image->Visible = false;
    }
    icon[FUNCTION_SETTING]->image->Picture->LoadFromFile("bmp\\icon_function_setting.bmp");
    icon[TAM]->image->Picture->LoadFromFile("bmp\\icon_TAM.bmp");
    icon[LINE]->image->Picture->LoadFromFile("bmp\\icon_line.bmp");
    icon[HOOK]->image->Picture->LoadFromFile("bmp\\icon_hook.bmp");
    icon[INTERCOM]->image->Picture->LoadFromFile("bmp\\icon_intercom.bmp");
    icon[MUTE]->image->Picture->LoadFromFile("bmp\\icon_mute.bmp");
    if (aCfg->batteryStatus == "half") {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_half.bmp");
    }
    else if (aCfg->batteryStatus == "low") {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_low.bmp");
    }
    else {
        icon[BATTERY]->image->Picture->LoadFromFile("bmp\\icon_battery_full.bmp");
    }
    icon[ANTENNA]->image->Picture->LoadFromFile("bmp\\icon_antenna.bmp");
    icon[SPEAKER]->image->Picture->LoadFromFile("bmp\\icon_speaker.bmp");
    icon[KMESSAGE]->image->Picture->LoadFromFile("bmp\\icon_message.bmp");

    icon[BELL] = new Icon;
    icon[BELL]->image->Left = line[1]->pos[0]->ch->Left + ICON_WIDTH;
    icon[BELL]->image->Top = line[1]->pos[0]->ch->Top;
    icon[BELL]->image->Width = ICON_WIDTH;
    icon[BELL]->image->Height = line[1]->pos[0]->ch->Height;
    icon[BELL]->image->Transparent = true;
    icon[BELL]->image->Stretch = true;
    icon[BELL]->image->Visible = false;
    icon[BELL]->image->Picture->LoadFromFile("bmp\\icon_bell.bmp");

    icon[SELECT1] = new Icon;
    icon[SELECT1]->image->Left = line[0]->pos[14]->ch->Left+3;
    icon[SELECT1]->image->Top = line[0]->pos[14]->ch->Top+1;
    icon[SELECT1]->image->Width = line[0]->pos[15]->ch->Height-2;
    icon[SELECT1]->image->Height = line[0]->pos[15]->ch->Height-2;
    icon[SELECT1]->image->Transparent = true;
    icon[SELECT1]->image->Stretch = true;
    icon[SELECT1]->image->Visible = false;
    icon[SELECT1]->image->Picture->LoadFromFile("bmp\\icon_select.bmp");

    icon[SELECT2] = new Icon;
    icon[SELECT2]->image->Left = line[1]->pos[14]->ch->Left+3;
    icon[SELECT2]->image->Top = line[1]->pos[15]->ch->Top+1;
    icon[SELECT2]->image->Width = line[1]->pos[15]->ch->Height-2;
    icon[SELECT2]->image->Height = line[1]->pos[15]->ch->Height-2;
    icon[SELECT2]->image->Transparent = true;
    icon[SELECT2]->image->Stretch = true;
    icon[SELECT2]->image->Visible = false;
    icon[SELECT2]->image->Picture->LoadFromFile("bmp\\icon_select.bmp");
};

void Lcd::Clear() {
    *line[0] = "";
    *line[1] = "";
    for (int i = 0; i < ICON_SIZE; i++) {
        icon[i]->image->Visible = false;
    }
};

void Lcd::DisplaySearching() {
    *line[0] = "";
    *line[1] = "   Searching...";
};

void Lcd::DisplayMainMenu() {
    TDateTime DateTime = Time();  // store the current date and time
    AnsiString str = TimeToStr(DateTime); // convert the time to a string
    *line[0] = pt->cfg->IdleDisplay_Product;
    char temp[17];
    memset(temp, ' ', 16);
    temp[16] = '\0';
    memcpy(temp+5, str.SubString(5, 6).c_str(), 6);
    int k = pt->cfg->IdleDisplay_Base.size();
    strcpy(temp+16-k, &pt->cfg->IdleDisplay_Base[0]);
    *line[1] = temp;
    icon[BELL]->image->Visible = true;  
    pt->lcd->icon[FUNCTION_SETTING]->image->Visible = false;
    return;
};
      
void Lcd::DisplayTime() {
    TDateTime DateTime = Time();  // store the current date and time
    AnsiString str = TimeToStr(DateTime); // convert the time to a string
    *line[0] = "";
    char temp[17];
    memset(temp, ' ', 16);
    temp[16] = '\0';
    memcpy(temp+5, str.SubString(5, 6).c_str(), 6);
    int k = pt->cfg->IdleDisplay_Base.size();
    strcpy(temp+16-k, &pt->cfg->IdleDisplay_Base[0]);
    *line[1] = temp;
    icon[BELL]->image->Visible = true;
    return;
};

void Lcd::DisplayCurrentMenu(MenuTree *aMenuTree) {
    *line[0] = aMenuTree->currNode->Text.c_str();
    if (aMenuTree->currNode->getNextSibling() != 0) {
        *line[1] = aMenuTree->currNode->getNextSibling()->Text.c_str();
    }
    else {
        *line[1] = "----------------";
    }
    icon[BELL]->image->Visible = false;
    return;
};

void Lcd::Trigger() {
    static bool blinkOn;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 16; j++) {
            if (line[i]->blink || line[i]->pos[j]->blink) {
                line[i]->pos[j]->ch->Visible = blinkOn;
            }
        }
    }
    for (int i = 0; i < ICON_SIZE; i++) {
        if (icon[i]->blink) {
            icon[i]->image->Visible = blinkOn;
        }
    }
    blinkOn = !blinkOn;
};

void Lcd::StopBlink() {
    for (int i = 0; i < 2; i++) {
        line[i]->StopBlink();
    }
};

void Key::ShowKeyPressed(TImage *image2) {
    image2->Left = Left;
    image2->Top = Top;
    image2->Width = Width;
    image2->Height = Height;
    image2->Picture->LoadFromFile(bitMapFile);
    image2->Visible = true;
};

// gamma:need to change when porting a new handset image.
KeyPad::KeyPad() { // set rectangle for each keypad to display
/* The value about position and size of keys are measured in the original bitmap image.
   So, we need to adust according the size of Image1.*/
    AnsiString dir = "bmp\\";

    key[KEY_UP].Left = 90;
    key[KEY_UP].Top = 234;
    key[KEY_UP].Width = 90;
    key[KEY_UP].Height = 64;
    key[KEY_UP].color = clRed;
    key[KEY_UP].bitMapFile = dir + "key_up.bmp";

    key[KEY_DOWN].Left = 92;
    key[KEY_DOWN].Top = 232;
    key[KEY_DOWN].Width = 88;
    key[KEY_DOWN].Height = 63;
    key[KEY_DOWN].color = TColor(clRed - 1);
    key[KEY_DOWN].bitMapFile = dir + "key_down.bmp";

    key[KEY_LEFT].Left = 91;
    key[KEY_LEFT].Top = 232;
    key[KEY_LEFT].Width = 90;
    key[KEY_LEFT].Height = 64;
    key[KEY_LEFT].color = TColor(clRed - 2);
    key[KEY_LEFT].bitMapFile = dir + "key_left.bmp";

    key[KEY_RIGHT].Left = 89;
    key[KEY_RIGHT].Top = 231;
    key[KEY_RIGHT].Width = 89;
    key[KEY_RIGHT].Height = 66;
    key[KEY_RIGHT].color = TColor(clRed - 3);
    key[KEY_RIGHT].bitMapFile = dir + "key_right.bmp";

    key[KEY_REDIAL].Left = 40;
    key[KEY_REDIAL].Top = 233;
    key[KEY_REDIAL].Width = 53;
    key[KEY_REDIAL].Height = 30;
    key[KEY_REDIAL].color = TColor(clRed - 4);
    key[KEY_REDIAL].bitMapFile = dir + "key_redial.bmp";

    key[KEY_ON_OFF].Left = 38;
    key[KEY_ON_OFF].Top = 264;
    key[KEY_ON_OFF].Width = 59;
    key[KEY_ON_OFF].Height = 35;
    key[KEY_ON_OFF].color = TColor(clRed - 5);
    key[KEY_ON_OFF].bitMapFile = dir + "key_on_off.bmp";
         
    key[KEY_TOP_MESSAGE].Left = 44;
    key[KEY_TOP_MESSAGE].Top = 289;
    key[KEY_TOP_MESSAGE].Width = 68;
    key[KEY_TOP_MESSAGE].Height = 34;
    key[KEY_TOP_MESSAGE].color = TColor(clRed - 6);
    key[KEY_TOP_MESSAGE].bitMapFile = dir + "key_top_message.bmp";
                       
    key[KEY_SOS_SMS].Left = 100;
    key[KEY_SOS_SMS].Top = 295;
    key[KEY_SOS_SMS].Width = 68;
    key[KEY_SOS_SMS].Height = 30;
    key[KEY_SOS_SMS].color = TColor(clRed - 7);
    key[KEY_SOS_SMS].bitMapFile = dir + "key_sos_sms.bmp";

    key[KEY_SMS].Left = 158;
    key[KEY_SMS].Top = 291;
    key[KEY_SMS].Width = 68;
    key[KEY_SMS].Height = 30;
    key[KEY_SMS].color = TColor(clRed - 8);
    key[KEY_SMS].bitMapFile = dir + "key_sms.bmp";

    key[KEY_HANDSFREE].Left = 177;
    key[KEY_HANDSFREE].Top = 266;
    key[KEY_HANDSFREE].Width = 51;
    key[KEY_HANDSFREE].Height = 29;
    key[KEY_HANDSFREE].color = TColor(clRed - 9);
    key[KEY_HANDSFREE].bitMapFile = dir + "key_handsfree.bmp";

    key[KEY_MENU].Left = 174;
    key[KEY_MENU].Top = 232;
    key[KEY_MENU].Width = 53;
    key[KEY_MENU].Height = 29;
    key[KEY_MENU].color = TColor(clRed - 10);
    key[KEY_MENU].bitMapFile = dir + "key_menu.bmp";

    key[KEY_1].Left = 39;
    key[KEY_1].Top = 326;
    key[KEY_1].Width = 55;
    key[KEY_1].Height = 28;
    key[KEY_1].color = TColor(clRed - 11);
    key[KEY_1].bitMapFile = dir + "key_1.bmp";
    strcpy(key[KEY_1].symbol, "1");

    key[KEY_2].Left = 102;
    key[KEY_2].Top = 330;
    key[KEY_2].Width = 64;
    key[KEY_2].Height = 27;
    key[KEY_2].color = TColor(clRed - 12);
    key[KEY_2].bitMapFile = dir + "key_2.bmp";
    strcpy(key[KEY_2].symbol, "2");

    key[KEY_3].Left = 172;
    key[KEY_3].Top = 327;
    key[KEY_3].Width = 57;
    key[KEY_3].Height = 27;
    key[KEY_3].color = TColor(clRed - 13);
    key[KEY_3].bitMapFile = dir + "key_3.bmp";
    strcpy(key[KEY_3].symbol, "3");

    key[KEY_4].Left = 41;
    key[KEY_4].Top = 358;
    key[KEY_4].Width = 55;
    key[KEY_4].Height = 27;
    key[KEY_4].color = TColor(clRed - 14);
    key[KEY_4].bitMapFile = dir + "key_4.bmp";
    strcpy(key[KEY_4].symbol, "4");

    key[KEY_5].Left = 102;
    key[KEY_5].Top = 361;
    key[KEY_5].Width = 61;
    key[KEY_5].Height = 26;
    key[KEY_5].color = TColor(clRed - 15);
    key[KEY_5].bitMapFile = dir + "key_5.bmp";
    strcpy(key[KEY_5].symbol, "5");

    key[KEY_6].Left = 170;
    key[KEY_6].Top = 359;
    key[KEY_6].Width = 57;
    key[KEY_6].Height = 26;
    key[KEY_6].color = TColor(clRed - 16);
    key[KEY_6].bitMapFile = dir + "key_6.bmp";
    strcpy(key[KEY_6].symbol, "6");

    key[KEY_7].Left = 44;
    key[KEY_7].Top = 389;
    key[KEY_7].Width = 54;
    key[KEY_7].Height = 26;
    key[KEY_7].color = TColor(clRed - 17);
    key[KEY_7].bitMapFile = dir + "key_7.bmp";
    strcpy(key[KEY_7].symbol, "7");

    key[KEY_8].Left = 104;
    key[KEY_8].Top = 391;
    key[KEY_8].Width = 59;
    key[KEY_8].Height = 25;
    key[KEY_8].color = TColor(clRed - 18);
    key[KEY_8].bitMapFile = dir + "key_8.bmp";
    strcpy(key[KEY_8].symbol, "8");

    key[KEY_9].Left = 170;
    key[KEY_9].Top = 389;
    key[KEY_9].Width = 54;
    key[KEY_9].Height = 26;
    key[KEY_9].color = TColor(clRed - 19);
    key[KEY_9].bitMapFile = dir + "key_9.bmp";
    strcpy(key[KEY_9].symbol, "9");

    key[KEY_STAR].Left = 51;
    key[KEY_STAR].Top = 420;
    key[KEY_STAR].Width = 49;
    key[KEY_STAR].Height = 26;
    key[KEY_STAR].color = TColor(clRed - 20);
    key[KEY_STAR].bitMapFile = dir + "key_star.bmp";
    strcpy(key[KEY_STAR].symbol, "*");

    key[KEY_0].Left = 106;
    key[KEY_0].Top = 422;
    key[KEY_0].Width = 56;
    key[KEY_0].Height = 24;
    key[KEY_0].color = TColor(clRed - 21);
    key[KEY_0].bitMapFile = dir + "key_0.bmp";
    strcpy(key[KEY_0].symbol, "0");

    key[KEY_HASH].Left = 169;
    key[KEY_HASH].Top = 420;
    key[KEY_HASH].Width = 50;
    key[KEY_HASH].Height = 26;
    key[KEY_HASH].color = TColor(clRed - 22);
    key[KEY_HASH].bitMapFile = dir + "key_hash.bmp";
    strcpy(key[KEY_HASH].symbol, "#");

    key[KEY_FT_SERVICES].Left = 68;
    key[KEY_FT_SERVICES].Top = 458;
    key[KEY_FT_SERVICES].Width = 34;
    key[KEY_FT_SERVICES].Height = 27;
    key[KEY_FT_SERVICES].color = TColor(clRed - 23);
    key[KEY_FT_SERVICES].bitMapFile = dir + "key_ft_services.bmp";

    key[KEY_SOS].Left = 166;
    key[KEY_SOS].Top = 458;
    key[KEY_SOS].Width = 35;
    key[KEY_SOS].Height = 28;
    key[KEY_SOS].color = TColor(clRed - 24);
    key[KEY_SOS].bitMapFile = dir + "key_sos.bmp";


    for (int i = KEY_UP; i < KEY_SIZE; i++) { // bitmap size is (270, 530)
        key[i].Left = key[i].Left * Form1->Image1->Width / 270;
        key[i].Top = key[i].Top * Form1->Image1->Height / 530;
        key[i].Width = key[i].Width * Form1->Image1->Width / 270;
        key[i].Height = key[i].Height * Form1->Image1->Height / 530;
    }
    return;
};

/*
Return the key_id of the key be pressed. Return -1 if (x, y) is not on any key.
Image3 painting each key button with different color.
So, we decide Mouse Down (x, y) is to press a specified key by color of
pixel(x, y) in Image3 is the same with Key Color.
*/
int KeyPad::WhichKeyPressed(int x, int y) {
    int i;

    int x1 = x*270/Form1->Image3->Width; // adjust (x, y) by the ratio of original image to (Image3->Width, Image3->Height)
    int y1 = y*530/Form1->Image3->Height;

    for (i = KEY_UP; i <= KEY_SMS; i++) {
        if (Form1->Image3->Picture->Bitmap->Canvas->Pixels[x1][y1] == key[i].color) {
            return i;
        }
    }
    return -1;
}

Buzzar::Buzzar() : dir("Sounds\\") {
    waveOutSetVolume(0, 0x20002000);// initial audio volume

 // Suppose voice files is in the same directory.
    voiceFile[KEY_UP] = dir + "keytone1.wav"; 
    voiceFile[KEY_DOWN] = dir + "keytone1.wav";
    voiceFile[KEY_LEFT] = dir + "keytone1.wav";
    voiceFile[KEY_RIGHT] = dir + "keytone1.wav";
    voiceFile[KEY_REDIAL] = dir + "keytone1.wav";
    voiceFile[KEY_ON_OFF] = dir + "keytone1.wav";
    voiceFile[KEY_HANDSFREE] = dir + "keytone1.wav";
    voiceFile[KEY_TOP_MESSAGE] = dir + "keytone1.wav";
    voiceFile[KEY_FT_SERVICES] = dir + "keytone1.wav";
    voiceFile[KEY_SOS] = dir + "keytone1.wav";  
    voiceFile[KEY_MENU] = dir + "keytone1.wav";

    voiceFile[KEY_1] = dir + "keytone1.wav";
    voiceFile[KEY_2] = dir + "keytone2.wav";
    voiceFile[KEY_3] = dir + "keytone3.wav";
    voiceFile[KEY_4] = dir + "keytone4.wav";
    voiceFile[KEY_5] = dir + "keytone5.wav";
    voiceFile[KEY_6] = dir + "keytone6.wav";
    voiceFile[KEY_7] = dir + "keytone7.wav";
    voiceFile[KEY_8] = dir + "keytone8.wav";
    voiceFile[KEY_9] = dir + "keytone9.wav";
    voiceFile[KEY_STAR] = dir + "keytone_star.wav";
    voiceFile[KEY_0] = dir + "keytone0.wav";
    voiceFile[KEY_HASH] = dir + "keytone#.wav";

    // voice for Ring Melody
    voiceFile[KEY_SMS+1] = dir + "ringtone0.wav";
    voiceFile[KEY_SMS+2] = dir + "ringtone1.wav";
    voiceFile[KEY_SMS+3] = dir + "ringtone2.wav";
    voiceFile[KEY_SMS+4] = dir + "ringtone3.wav";
    voiceFile[KEY_SMS+5] = dir + "ringtone4.wav";
    voiceFile[KEY_SMS+6] = dir + "ringtone5.wav";
    voiceFile[KEY_SMS+7] = dir + "ringtone6.wav";
    voiceFile[KEY_SMS+8] = dir + "ringtone7.wav";
    voiceFile[KEY_SMS+9] = dir + "ringtone8.wav";
    voiceFile[KEY_SMS+10] = dir + "ringtone9.wav";
};

Buzzar::~Buzzar() {
    sndPlaySound( NULL, NULL); // stop that audio playing voice
};

// Play the voice of key be pressed.
// Each key has it's voice.
void Buzzar::PlayKeyPressedVoice(int aKey) {
    if (aKey >= KEY_SIZE || voiceFile[aKey] == "") { // no voice file
        return;
    }
    sndPlaySound(voiceFile[aKey].c_str(), SND_ASYNC|SND_NODEFAULT);
};

void Buzzar::PlayPowerOnMelody() {
    waveOutSetVolume(0, 0x80008000);
    string s;
    s = dir + "power_on.wav";
    sndPlaySound(s.c_str(), SND_ASYNC|SND_NODEFAULT);
};

void Buzzar::PlayPowerOffMelody() {
    waveOutSetVolume(0, 0x80008000);
    string s;
    s = dir + "power_on.wav";
    sndPlaySound(s.c_str(), SND_ASYNC|SND_NODEFAULT);
};

void Buzzar::PlayConfirmTone() {
    waveOutSetVolume(0, 0x80008000);
    string s;
    s = dir + "confirm_tone.wav";
    sndPlaySound(s.c_str(), SND_ASYNC|SND_NODEFAULT);
};
 
void Buzzar::PlayFailTone() {
    waveOutSetVolume(0, 0x80008000);
    string s;
    s = dir + "fail_tone.wav";
    sndPlaySound(s.c_str(), SND_ASYNC|SND_NODEFAULT);
};

void Buzzar::PlayDialTone() {
    waveOutSetVolume(0, 0x80008000);
    string s;
    s = dir + "entertalk.wav";
    sndPlaySound(s.c_str(), SND_ASYNC|SND_LOOP|SND_NODEFAULT);
};

// melody : 0..9
// volIdx : 0..9
void Buzzar::PlayRing(int melody, int volIdx) {
    DWORD vol;
    vol = (0xFFFF / 9 * volIdx);
    vol = vol*0x10000+vol;
    waveOutSetVolume(0, vol);
    sndPlaySound(voiceFile[KEY_SMS + 1 + melody].c_str(), SND_ASYNC|SND_NODEFAULT);
};

void Buzzar::PlayExtIncomingCallRing(int ringType) {
    string s;
    switch (ringType) {
        case RING_1:
            s = dir + "ringtone1.wav";
            break;
    }
    sndPlaySound(s.c_str(), SND_ASYNC|SND_LOOP|SND_NODEFAULT);
};

void Buzzar::SetVol(DWORD vol) {
    waveOutSetVolume(0, vol);
};

void Buzzar::Silence() {
    sndPlaySound( NULL, NULL); // stop that audio playing voice
};

void ShowSmsWriteBufOnLcd(SMS *sms, Lcd *lcd) {
    lcd->StopBlink();
    int k = sms->writeBuf.size();
    if (k < 16) {
        *lcd->line[0] = sms->writeBuf + "_";
        *lcd->line[1] = ""; 
        lcd->line[0]->pos[k]->blink = true;
    }
    else if (k < 32) { // 16 <= k <= 31
        *lcd->line[0] = sms->writeBuf.substr(0, 16);
        *lcd->line[1] = sms->writeBuf.substr(16, k - 16) + "_";
        lcd->line[1]->pos[k-16]->blink = true;
    }
    else { // k >= 32
        *lcd->line[0] = sms->writeBuf.substr(k - 31, 16);
        *lcd->line[1] = sms->writeBuf.substr(k - 15, 15) + "_";
        lcd->line[1]->pos[15]->blink = true;
    }
};

void PTMMI::SingleSelectDlg::Display() {
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    if (_select == _curr) {
        pt->lcd->icon[SELECT1]->image->Visible = true;
    }
    else {
        pt->lcd->icon[SELECT1]->image->Visible = false;
    }
    if (_select == _curr + 1) {
        pt->lcd->icon[SELECT2]->image->Visible = true;
    }
    else {
        pt->lcd->icon[SELECT2]->image->Visible = false;
    }
};

void PTMMI::SingleSelectDlg::Initial(int select, int size, int curr) {
    _select = select;
    _size = size;
    _curr = curr;
    Display();
};

void PTMMI::SingleSelectDlg::Input(int key) {
    switch (pt->ptxx_dr_key) {
    case KEY_UP:
        if (pt->menuTree->currNode->getPrevSibling() != 0) { // First Child
            pt->menuTree->currNode = pt->menuTree->currNode->getPrevSibling();
            _curr--;
        }
        else {
            pt->menuTree->currNode = pt->menuTree->currNode->Parent->GetLastChild();
            _curr = _size - 1;
        }
        Display();
        break;
    case KEY_DOWN:
        if (pt->menuTree->currNode->getNextSibling() != 0) {
            pt->menuTree->currNode = pt->menuTree->currNode->getNextSibling();
            _curr++;
        }
        else { // Last Child
            pt->menuTree->currNode = pt->menuTree->currNode->Parent->getFirstChild();
            _curr = 0;
        }
        Display();
        break;
    }
};

int PTMMI::SingleSelectDlg::Current() {
    return _curr;
};

void PTMMI::MultiSelectDlg::Display() {
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    pt->lcd->icon[SELECT1]->image->Visible = _select[_curr];
    if (_curr + 1 < _size) {
        pt->lcd->icon[SELECT2]->image->Visible = _select[_curr + 1];
    }
    else {
        pt->lcd->icon[SELECT2]->image->Visible = false;
    }
};

void PTMMI::MultiSelectDlg::Initial(bool *select, int size, int curr) {
    for (int i = 0; i < size; i++) {
        _select[i] = select[i];
    }
    _size = size;
    _curr = curr;
    Display();
};

void PTMMI::MultiSelectDlg::Input(int key) {
    switch (pt->ptxx_dr_key) {
    case KEY_UP:
        if (pt->menuTree->currNode->getPrevSibling() != 0) { // First Child
            pt->menuTree->currNode = pt->menuTree->currNode->getPrevSibling();
            _curr--;
        }
        else {
            pt->menuTree->currNode = pt->menuTree->currNode->Parent->GetLastChild();
            _curr = _size - 1;
        }
        Display();
        break;
    case KEY_DOWN:
        if (pt->menuTree->currNode->getNextSibling() != 0) {
            pt->menuTree->currNode = pt->menuTree->currNode->getNextSibling();
            _curr++;
        }
        else { // Last Child
            pt->menuTree->currNode = pt->menuTree->currNode->Parent->getFirstChild();
            _curr = 0;
        }
        Display();
        break;
    }
};

int PTMMI::MultiSelectDlg::Current() {
    return _curr;
};

void PTMMI::InputTextDlg::Initial(string title, string text, unsigned int maxSize) {
    _title = title;
    _text = text;
    _maxSize = maxSize;
    _begin = 0;
    _pos = 0;
    _selTime = 0;
    _timeOut = 0;
    _lastKey = NO_KEY;
    _uppercase = true;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = title;
    *pt->lcd->line[1] = _text;
    pt->lcd->line[1]->pos[0]->blink = true;
    if (_text.size() == 0) {
        *pt->lcd->line[1] += "_";
    }
};

int PTMMI::InputTextDlg::Input() {
    switch (pt->act_event) {
    case DR_KEYPAD_IND:
        switch (pt->ptxx_dr_key) {
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
        //case KEY_STAR:
        case KEY_0: 
            if (_text.size() == _maxSize) {
                pt->buzzar->PlayFailTone();
                return CONTINUE;
            }
            if (_lastKey != pt->ptxx_dr_key) {
                pt->lcd->line[0]->Invisible();
                pt->lcd->panel->Load(pt->ptxx_dr_key, _uppercase);
            }
            else {
                pt->lcd->panel->idx = (pt->lcd->panel->idx+1)%pt->lcd->panel->size;
            }
            pt->lcd->panel->Show();
            _selTime = 0;
            break;
        case KEY_HASH:
            _uppercase = !_uppercase;
            break;
        case KEY_UP:
            if (_begin + _pos == _text.size()) {
                *pt->lcd->line[1] = &_text[_begin];
            }
            if (_pos > 0) {
                _pos--;
            }
            pt->lcd->line[1]->StopBlink();
            pt->lcd->line[1]->pos[_pos]->blink = true;
            break;
        case KEY_DOWN:
            if (_pos < 15 && _begin + _pos < _text.size() && _text.size() < _maxSize) {
                _pos++;
            }
            if (_begin + _pos == _text.size() && pt->lcd->line[1]->pos[_pos]->ch->Caption != "_") {
                *pt->lcd->line[1] += "_";
            }
            pt->lcd->line[1]->StopBlink();
            pt->lcd->line[1]->pos[_pos]->blink = true;
            break;
        case KEY_F:
            pt->lcd->panel->Hidden();
            pt->lcd->line[0]->Visible();
            pt->lcd->line[1]->StopBlink();
            break;
        case KEY_EXIT:
            pt->lcd->panel->Hidden();
            pt->lcd->line[0]->Visible();
            pt->lcd->line[1]->StopBlink();
            break;
        }
        _lastKey = pt->ptxx_dr_key;
    case TIME_OUT_500MS:
        _selTime++;
        _timeOut++;
    // if user didn't select a letter over 60 seconds.
        if (_timeOut > 120) {
            pt->lcd->panel->Hidden();
            pt->lcd->line[1]->StopBlink();
            return EXIT_DLG;
        }
        if (_selTime > 4) {
            if (pt->lcd->panel->Visible()) {
                if (_begin + _pos < _text.size()) {
                    _text[_begin + _pos] = pt->lcd->panel->ActiveSymbol();
                }
                else { // _begin + _pos == _text.size()
                    _text += pt->lcd->panel->ActiveSymbol();
                }
                if (_pos == 15) {
                    _begin++;
                }
                else { // _pos < 15
                    _pos++;
                }
                *pt->lcd->line[1] = &_text[_begin];
                if (_begin + _pos == _text.size()) {
                    *pt->lcd->line[1] += "_";
                }
                pt->lcd->line[1]->StopBlink();
                pt->lcd->line[1]->pos[_pos]->blink = true;
                pt->lcd->panel->Hidden();
                pt->lcd->line[0]->Visible();
            }
            _selTime = _timeOut = 0;
            pt->lcd->panel->idx = 0;
            _lastKey = NO_KEY;
        }
        break;
    }
    return CONTINUE;
};

string PTMMI::InputTextDlg::Text() {
    return _text;
};

void PTMMI::InputTextDlg::BackToInput() {
    _selTime = 0;
    _timeOut = 0;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = _title;
    *pt->lcd->line[1] = _text;
    pt->lcd->line[1]->pos[_pos]->blink = true;
    if (_pos == _text.size()) {
        *pt->lcd->line[1] += "_";
    }
};

void PTMMI::InputDigitsDlg::Initial(string title, string digits, unsigned int maxSize) {
    _digits = digits;
    _maxSize = maxSize;
    _begin = 0;
    _pos = 0;  
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    *pt->lcd->line[0] = title;
    *pt->lcd->line[1] = _digits;
    pt->lcd->line[1]->pos[0]->blink = true;
    if (_digits.size() == 0) {
        *pt->lcd->line[1] += "_";
    }
};

void PTMMI::InputDigitsDlg::Input() {
    char c;
    switch (pt->act_event) {
    case DR_KEYPAD_IND:
        switch (pt->ptxx_dr_key) {
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
        case KEY_0:
            if (_digits.size() == _maxSize) {
                pt->buzzar->PlayFailTone();
                return;
            }
            c = KeyToAscii(pt->ptxx_dr_key);
            if (_begin + _pos < _digits.size()) {
                _digits[_begin + _pos] = c;
            }
            else { // _begin + _pos == _tel.size()
                _digits += c;
            }
            if (_pos == 15) {
                _begin++;
            }
            else { // _pos < 15
                _pos++;
            }
            break;
        case KEY_UP:
            if (_pos > 0) {
                _pos--;
            }
            break;
        case KEY_DOWN:
            if (_pos < 15 && _begin + _pos < _digits.size() && _digits.size() < _maxSize) {
                _pos++;
            } 
            if (_begin + _pos == _digits.size() && pt->lcd->line[1]->pos[_pos]->ch->Caption != "_") {
                *pt->lcd->line[1] += "_";
            }
            pt->lcd->line[1]->StopBlink();
            pt->lcd->line[1]->pos[_pos]->blink = true;
            break;
        case KEY_F:
            pt->lcd->line[1]->StopBlink();
            break;
        case KEY_EXIT:
            pt->lcd->line[1]->StopBlink();
            break;
        }
        *pt->lcd->line[1] = &_digits[_begin];
        if (_begin + _pos == _digits.size() && pt->lcd->line[1]->Length() < 16) {
            *pt->lcd->line[1] += "_";
        }
        pt->lcd->line[1]->StopBlink();
        pt->lcd->line[1]->pos[_pos]->blink = true;
        break;
    }
};

string PTMMI::InputDigitsDlg::Digits() {
    return _digits;
};

void PTMMI::SelectPhoneBookEntryDlg::Initial(PhoneBook *pb, int curr) {
    _pb = pb;
    _curr = curr;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    *pt->lcd->line[0] = _pb->Item(_curr).name;
    if (_curr + 1 < _pb->Size()) {
        *pt->lcd->line[1] = _pb->Item(_curr + 1).name;
    }
    else {
        *pt->lcd->line[1] = "-----------------";
    }
};

void PTMMI::SelectPhoneBookEntryDlg::Input() {
    switch (pt->act_event) {
    case DR_KEYPAD_IND:
        switch (pt->ptxx_dr_key) {
        case KEY_UP:
            if (_curr > 0) {
                _curr--;
            }
            else {
                _curr = _pb->Size() - 1;
            }
            break;
        case KEY_DOWN:
            _curr = (_curr + 1) % _pb->Size();
            break;
        }
        if (pt->ptxx_dr_key == KEY_UP || pt->ptxx_dr_key == KEY_DOWN) {
            *pt->lcd->line[0] = _pb->Item(_curr).name;
            if (_curr + 1 < _pb->Size()) {
                *pt->lcd->line[1] = _pb->Item(_curr + 1).name;
            }
            else {
                *pt->lcd->line[1] = "----------------";
            }
        }
        break;
    }
};

int PTMMI::SelectPhoneBookEntryDlg::CurrentIdx() {
    return _curr;
};
  
PhoneBookEntry PTMMI::SelectPhoneBookEntryDlg::CurrentItem() {
    return _pb->Item(_curr);
};

void PTMMI::InputPinDlg::Initial(string title, string pin) {
    _pin = pin;   
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = title;
    *pt->lcd->line[1] = _pin;
    pt->lcd->line[1]->pos[0]->blink = true;
    if (_pin.size() == 0) {
        *pt->lcd->line[1] += "_";
    }
};

void PTMMI::InputPinDlg::Input() {
    switch (pt->ptxx_dr_key) {
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_6:
    case KEY_7:
    case KEY_8:
    case KEY_9:
    case KEY_0:
        if (_pin.size() < 7) {
            _pin.push_back(KeyToAscii(pt->ptxx_dr_key));
            pt->lcd->line[1]->pos[_pin.size() - 1]->StopBlink();
            pt->lcd->line[1]->pos[_pin.size() - 1]->ch->Caption = "*";
            *pt->lcd->line[1] += "_";
            pt->lcd->line[1]->pos[_pin.size()]->blink = true;
        }
        else if (_pin.size() == 7) {
            _pin.push_back(KeyToAscii(pt->ptxx_dr_key));
            pt->lcd->line[1]->pos[_pin.size() - 1]->ch->Caption = "*";
        }
        else { // _pin.size() == 8
            pt->buzzar->PlayFailTone();
        }
        break;
    }
};
             
string PTMMI::InputPinDlg::Pin() {
    return _pin;
};

void PTMMI::InputDateDlg::Initial(string title, GDate date) {
    _date = date;
    _pos = 0;   
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = title;
    *pt->lcd->line[1] = _date;
    pt->lcd->line[1]->pos[0]->blink = true;
};

void PTMMI::InputDateDlg::Input() {
    switch (pt->ptxx_dr_key) {
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_6:
    case KEY_7:
    case KEY_8:
    case KEY_9:
    case KEY_0:
        *pt->lcd->line[1]->pos[_pos] = KeyToAscii(pt->ptxx_dr_key);
        pt->lcd->line[1]->pos[_pos]->StopBlink();
        _pos = (_pos + 1) % 10;
        if (*pt->lcd->line[1]->pos[_pos] == '/') {
            _pos++;
        }
        pt->lcd->line[1]->pos[_pos]->blink = true;
        break;
    case KEY_F:
        _date = pt->lcd->line[1]->ToDate(_date.Format);
        break;
    }
};

GDate PTMMI::InputDateDlg::Date() {
    return _date;
};

void PTMMI::InputTimeDlg::Initial(string s1, string s2, GTime time) {
    _time = time;
    if (_time.Format == 0) {
        _pos = 2;
    }
    else {
        _pos = 0;
    }
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = s1;
    if (!s2.empty()) {
        *pt->lcd->line[1] = s2;
    }
    else {
        *pt->lcd->line[1] = _time;
    }
    pt->lcd->line[1]->pos[_pos]->blink = true;
};

void PTMMI::InputTimeDlg::Input() {
    switch (pt->ptxx_dr_key) {
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_6:
    case KEY_7:
    case KEY_8:
    case KEY_9:
    case KEY_0:
        *pt->lcd->line[1]->pos[_pos] = KeyToAscii(pt->ptxx_dr_key);
        pt->lcd->line[1]->pos[_pos]->StopBlink();
        if (_time.Format == 0) {
            _pos++;
            if (_pos > 6) {
                _pos = 2;
            }
            else if (*pt->lcd->line[1]->pos[_pos] == ':') {
                _pos++;
            }
        }
        else {
            _pos++;
            if (_pos > 4) {
                _pos = 0;
            }
            else if (*pt->lcd->line[1]->pos[_pos] == ':') {
                _pos++;
            }
        }
        pt->lcd->line[1]->pos[_pos]->blink = true;
        break;
    case KEY_F:
        _time = pt->lcd->line[1]->ToTime(_time.Format);
        break;
    }
};

GTime PTMMI::InputTimeDlg::Time() {
    return _time;
};

void PTMMI::AdjustScaleDlg::Initial(string title, int scale) {
    _scale = scale;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = title;
    *pt->lcd->line[1] = "   0123456789";
    pt->lcd->line[1]->pos[_scale + 3]->blink = true;
};

void PTMMI::AdjustScaleDlg::Input() {
    switch (pt->ptxx_dr_key) {
        case KEY_UP:
        // increase Vol
            pt->lcd->line[1]->pos[_scale+3]->ch->Visible = true;
            pt->lcd->line[1]->pos[_scale+3]->blink = false;
            if (_scale > 0) _scale--;
            pt->lcd->line[1]->pos[_scale+3]->blink = true;
            break;
        case KEY_DOWN:
            pt->lcd->line[1]->pos[_scale+3]->ch->Visible = true;
            pt->lcd->line[1]->pos[_scale+3]->blink = false;
            if (_scale < 9) _scale++;
            pt->lcd->line[1]->pos[_scale+3]->blink = true;
            break;
    }
};

int PTMMI::AdjustScaleDlg::Scale() {
    return _scale;
};

void PTMMI::RingSettingDlg::Initial(int *melody, int melodyBegin, int *vol) {
    _melody = melody;
    _melodyBegin = melodyBegin;
    _vol = vol;
    _adjustScaleDlg.Initial("Melody", *_melody);
    pt->buzzar->PlayRing(melodyBegin + *_melody, *_vol);
    _state = MELODY_SETTING;
};

void PTMMI::RingSettingDlg::Input() {
    switch (pt->act_event) {
    case DR_KEYPAD_IND:
        _adjustScaleDlg.Input();
        switch (pt->ptxx_dr_key) {
        case KEY_UP:
            if (_state == MELODY_SETTING) {
                pt->buzzar->PlayRing(_melodyBegin + _adjustScaleDlg.Scale(), *_vol);
            }
            else if (_state == VOL_SETTING) {
                pt->buzzar->PlayRing(*_melody, _adjustScaleDlg.Scale());
            }
            break;
        case KEY_DOWN:
            if (_state == MELODY_SETTING) {
                pt->buzzar->PlayRing(_melodyBegin + _adjustScaleDlg.Scale(), *_vol);
            }
            else if (_state == VOL_SETTING) {
                pt->buzzar->PlayRing(_melodyBegin + *_melody, _adjustScaleDlg.Scale());
            }
            break;
        case KEY_F:
            if (_state == MELODY_SETTING) {
                *_melody = _adjustScaleDlg.Scale();
                pt->lcd->line[1]->StopBlink();
                *pt->lcd->line[0] = "";
                *pt->lcd->line[1] = "Setting OK";
                pt->buzzar->PlayConfirmTone();
                _timeOut = 0;
                _state = FINISH_MELODY_SETTING;
            }
            else if (_state == VOL_SETTING) {
                *_vol = _adjustScaleDlg.Scale(); 
                pt->lcd->line[1]->StopBlink();
                *pt->lcd->line[0] = "";
                *pt->lcd->line[1] = "Setting OK";
                pt->buzzar->PlayConfirmTone();
                _state = FINISH_VOL_SETTING;
            }
            break;
        case KEY_EXIT:
            pt->buzzar->Silence();
            _state = EXIT_DLG;
            break;
        }
        break;
    case TIME_OUT_500MS:
        _timeOut++;
        if (_timeOut > 3) {
            if (_state == FINISH_MELODY_SETTING) {
                _adjustScaleDlg.Initial("Volume", *_vol);
                pt->buzzar->PlayRing(_melodyBegin + *_melody, *_vol);
                _timeOut = 0;
                _state = VOL_SETTING;
            }
            else if (_state == FINISH_VOL_SETTING) {
                _state = EXIT_DLG;
            }
        }
        break;
    }
};

int PTMMI::RingSettingDlg::State() {
    return _state;
};

void PTMMI::TransmitPhoneBookDlg::RemovePBFromTree() {
    TTreeNode *p, *q;
    p = pt->menuTree->currNode->getFirstChild();
    if (p == 0) {
        return;
    }
    q = p->getNextSibling();
    while (q != 0) {
        Form1->TreeView1->Items->Delete(p);
        p = q;
        q = q->getNextSibling();
    }
    Form1->TreeView1->Items->Delete(p);
};

void PTMMI::TransmitPhoneBookDlg::Initial(PhoneBook *toPB) {
    _toPB = toPB;
    _state = SELECT_PHONE_BOOK_ENTRY;
    for (int i = 0; i < pt->cfg->pb.Size(); i++) {
        _select[i] = false;
        Form1->TreeView1->Items->AddChild(pt->menuTree->currNode, pt->cfg->pb.Item(i).name.c_str());
    }
    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
    _msDlg.Initial(_select, pt->cfg->pb.Size(), 0);
};

void PTMMI::TransmitPhoneBookDlg::Input() {
    switch (pt->act_event) {
    case DR_KEYPAD_IND:
        _msDlg.Input(pt->ptxx_dr_key);
        switch (pt->ptxx_dr_key) {
        case KEY_RIGHT:
            _select[_msDlg.Current()] = !_select[_msDlg.Current()];
            _msDlg.Initial(_select, pt->cfg->pb.Size(), _msDlg.Current());
            break;
        case KEY_EXIT:
            if (_state == SELECT_PHONE_BOOK_ENTRY) {
                pt->menuTree->currNode = pt->menuTree->currNode->Parent;
                pt->menuTree->currNode->Selected = true;
                RemovePBFromTree();
                pt->lcd->icon[SELECT1]->image->Visible = false;
                pt->lcd->icon[SELECT2]->image->Visible = false;
                pt->lcd->DisplayCurrentMenu(pt->menuTree);
                _state = EXIT_DLG;
            }
            else if (_state == SHOW_TOTAL_CHOOSE) {
                _msDlg.Initial(_select, pt->cfg->pb.Size(), _msDlg.Current());
                _state = SELECT_PHONE_BOOK_ENTRY;
            }
            break;
        case KEY_F:
            if (_state == SELECT_PHONE_BOOK_ENTRY) {
                int sum = 0;
                for (int i = 0; i < pt->cfg->pb.Size(); i++) {
                    if (_select[i]) {
                        sum++;
                    }
                }
                pt->lcd->icon[SELECT1]->image->Visible = false;
                pt->lcd->icon[SELECT2]->image->Visible = false;
                *pt->lcd->line[0] = "Total";
                *pt->lcd->line[1] = "Choose " + IntToStr(sum);
                _state = SHOW_TOTAL_CHOOSE;
            }
            else if (_state == SHOW_TOTAL_CHOOSE) {
                if (_toPB == &ft->cfg.pb) {
                // add pt->cfg->pb to _toPB
                    for (int i = 0; i < pt->cfg->pb.Size(); i++) {
                        if (_toPB->Find(pt->cfg->pb.Item(i).name) == -1) {
                            _toPB->Add(pt->cfg->pb.Item(i));
                        }
                    }
                }
                *pt->lcd->line[0] = "Waiting";
                *pt->lcd->line[1] = "Transmit...";
                _timeOut = 0;
                _state = WAIT_TRANSMIT;
            }
            break;
        }
        break;
    case TIME_OUT_500MS:
        _timeOut++;
        if (_timeOut > 3 && _state == WAIT_TRANSMIT) {
            _timeOut = 0;
            *pt->lcd->line[0] = "";
            *pt->lcd->line[1] = "Transmit OK!";
            pt->buzzar->PlayConfirmTone();
            _state = SHOW_TRANSMIT_OK;
        }
        else if (_timeOut > 3 && _state == SHOW_TRANSMIT_OK) {
            pt->menuTree->currNode = pt->menuTree->currNode->Parent; 
            pt->menuTree->currNode->Selected = true;
            RemovePBFromTree();
            pt->lcd->icon[SELECT1]->image->Visible = false;
            pt->lcd->icon[SELECT2]->image->Visible = false;
            pt->lcd->DisplayCurrentMenu(pt->menuTree); 
            _timeOut = 0;
            _state = EXIT_DLG;
        }
        break;
    }
};

int PTMMI::TransmitPhoneBookDlg::State() {
    return _state;
};

PTMMI::PTMMI() : state(POWER_OFF), showIncomingCall(false) {
};

int PTMMI::State() {
    return state;
};

bool PTMMI::NodeIs(TTreeNode *p, char *str) {
    string s[20];
    int size;
    int i = 0;
    int j;
    int k = 0;
    char temp[100];
    size = strlen(str);
    for (j = 0; j < size; j++) {
        if (str[j] == ':') {
            memcpy(temp, str + k, j - k);
            temp[j - k] = '\0';
            s[i] = temp;
            k = j + 1;
            i++;
        }
    }      
    memcpy(temp, str + k, j - k);
    temp[j - k] = '\0';
    s[i] = temp;
    size = i + 1;

    if (p->Level < size) {
        return false;
    }
    for (i = size - 1; i > -1; i--) {
        if (p->Text != s[i].c_str()) {
            return false;
        }
        p = p->Parent;
    }
    return true;
};

void PTMMI::BackToBrowsingFunctionMenu() {
    pt->menuTree->currNode->Expanded = false;
    pt->menuTree->currNode = pt->menuTree->currNode->Parent;
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
};

bool PTMMI::CheckPin(string &pin1, string &pin2){
    if (pin1 == pin2) {
        return true;
    }
    else {
        return false;
    }
};

void PTMMI::EnterMainMenu() {
    pt->menuTree->currNode = pt->menuTree->root;
    Form1->TreeView1->Visible = false;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->lcd->DisplayMainMenu();
    while (!trace.empty())  trace.pop();
    state = MAIN_MENU;
};

void PTMMI::EnterPhoneBook() {
    pt->menuTree->EnterPhoneBookMenu();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    pt->lcd->icon[FUNCTION_SETTING]->image->Visible = true;
    state = BROWSING_PHONE_BOOK_TREE;
};

void PTMMI::ExitPhoneBook() {
    pt->menuTree->ExitPhoneBookMenu();
};

void PTMMI::EnterFunctionMenu() {
    pt->menuTree->EnterFunctionMenu();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    pt->lcd->icon[FUNCTION_SETTING]->image->Visible = true;
    state = BROWSING_FUNCTION_MENU_TREE;
};
      
void PTMMI::ExitFunctionMenu() {
    pt->menuTree->ExitFunctionMenu();
};

void PTMMI::EnterSelectPhoneBookEntry(int k, PhoneBook *pb) {
    if (k >= pb->Size()) {
        return;
    }
    States st;
    st.state = state;
    st.substate = substate;
    trace.push(st);
    selectPhoneBookEntryDlg.Initial(pb, k);
    state = SELECT_PHONE_BOOK_ENTRY;
    substate = PICK_UP;
};

void PTMMI::ExitSelectPhoneBookEntry() {
    pt->lcd->line[1]->StopBlink();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    state = trace.top().state;
    substate = trace.top().substate;
    trace.pop();
};

void PTMMI::EnterOption() {
    States st;
    st.state = state;
    st.substate = substate;
    trace.push(st);
    pt->menuTree->EnterOptionMenu();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    state = ENTER_OPTION;
    substate = BROWSING_MENU;
};

void PTMMI::ExitOption() {
    pt->menuTree->ExitOptionMenu();
    state = trace.top().state;
    substate = trace.top().substate;
    trace.pop();               
    if (pb->Size() > 0) {
        int k = selectPhoneBookEntryDlg.CurrentIdx();
        if (k >= pb->Size()) {
            k = 0;
        }
        selectPhoneBookEntryDlg.Initial(pb, k);
    }
    else {
        ExitSelectPhoneBookEntry();
    }
};

void PTMMI::ShowMessageForOneSec(string s1, string s2) {
    States st;
    st.state = state;
    st.substate = substate;
    trace.push(st);
    pt->lcd->StopBlink();
    *pt->lcd->line[0] = s1;
    *pt->lcd->line[1] = s2;
    pt->lcd->icon[SELECT1]->image->Visible = false;
    pt->lcd->icon[SELECT2]->image->Visible = false;
    pt->buzzar->PlayConfirmTone();
    timeOut = 0;
    state = SHOW_MESSAGE;
};

void PTMMI::ExitShowMessageForOneSec() {
    timeOut = 0;
    state = trace.top().state;
    substate = trace.top().substate;
    trace.pop();
};

void PTMMI::EnterChangeName(string s1, string s2, int size) {
    States st;
    st.state = state;
    st.substate = substate;
    trace.push(st);
    inputTextDlg.Initial(s1, s2, size);
    pt->buzzar->PlayConfirmTone();
    timeOut = 0;
    state = CHANGE_NAME;
};

void PTMMI::ExitChangeName() {
    pt->lcd->StopBlink();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    timeOut = 0;
    state = trace.top().state;
    substate = trace.top().substate;
    trace.pop();
};

void PTMMI::EnterRingSetting(int *melody, int beginMelody, int *vol) {
    States st;
    st.state = state;
    st.substate = substate;
    trace.push(st);
    ringSettingDlg.Initial(melody, beginMelody, vol);
    state = RING_SETTING;
};

void PTMMI::ExitRingSetting() {
    pt->lcd->StopBlink();
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
    state = trace.top().state;
    substate = trace.top().substate;
    trace.pop();
};

// Up the menuTree and display current node of menu
void PTMMI::Up() {
    pt->menuTree->currNode->Expanded = false;
    if (pt->menuTree->currNode->getPrevSibling() != 0) { // First Child
        pt->menuTree->currNode = pt->menuTree->currNode->getPrevSibling();
    }
    else {
        pt->menuTree->currNode = pt->menuTree->currNode->Parent->GetLastChild();
    }
    pt->menuTree->currNode->Expanded = true;
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
};

// Down the menuTree and display current node of menu
void PTMMI::Down() {
    pt->menuTree->currNode->Expanded = false;
    if (pt->menuTree->currNode->getNextSibling() != 0) {
        pt->menuTree->currNode = pt->menuTree->currNode->getNextSibling();
    }
    else { // Last Child
        pt->menuTree->currNode = pt->menuTree->currNode->Parent->getFirstChild();
    }
    pt->menuTree->currNode->Expanded = true;
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
};

// Left the menuTree and display current node of menu
void PTMMI::Left() {
    pt->menuTree->currNode->Expanded = false;
    pt->menuTree->currNode = pt->menuTree->currNode->Parent;
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
};
          
// Right the menuTree and display current node of menu
void PTMMI::Right() {
    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
    pt->menuTree->currNode->Expanded = true;
    pt->menuTree->currNode->Selected = true;
    pt->lcd->DisplayCurrentMenu(pt->menuTree);
};

void PTMMI::Run() {
    static int *select;
    static bool confirmInputPin;
    static string tempPin;
    static bool haveRegisterHS[5];
    static int size;
    static string temp;
    static string name;
    int result;
    static ViewCallListIterator iter(&ft->cfg.cl);
    static CID cid;
    static int page;
    static int k;
    string s;
    static PhoneBookEntry pbe; // used by Add Entry  
    GTime time;
    if (state != POWER_OFF) {
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_ON_OFF_LONG_PRESS:
                pt->menuTree->currNode = pt->menuTree->root;
                Form1->TreeView1->Visible = false;
                pt->lcd->Clear();
                pt->buzzar->PlayPowerOffMelody();
                ft->sender->Active = false;
                timeOut = 0;
                state = POWER_OFF;
                return;
            }
            break;
        }
    }
    switch (state) {
    case POWER_OFF:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_ON_OFF_LONG_PRESS:
                pt->lcd->icon[BATTERY]->image->Visible = true;
                pt->lcd->icon[ANTENNA]->blink = true;
                pt->lcd->DisplaySearching();
                pt->buzzar->PlayPowerOnMelody();
                timeOut = 0;
                state = CONNECT_BASE;
                break;
            }
            break;
        }
        break;
    case CONNECT_BASE:
        switch (pt->act_event) {
        case TIME_OUT_500MS:
            if (timeOut > 3) {
                if (pt->cfg->FindBase[pt->cfg->SelectBase - 1]) {
                    pt->lcd->icon[ANTENNA]->StopBlink();
                    ft->sender->Active = true;
                    offHook = false;
                    EnterMainMenu();
                }
            }
            break;
        }
        break;
    case MAIN_MENU:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_PHONE_BOOK:
                EnterPhoneBook(); //state = BROWSING_PHONE_BOOK_TREE;
                break;
            case KEY_F:
                EnterFunctionMenu(); //state = BROWSING_FUNCTION_MENU_TREE;
                break;
            case KEY_SMS:
                *pt->lcd->line[0] = "_";
                *pt->lcd->line[1] = "";
                pt->lcd->line[0]->pos[0]->blink = true;
                pt->sms->writeBuf.clear();
                state = EDIT_SMS;
                break;
            case KEY_ON_OFF:
                *pt->lcd->line[0] = "";
                *pt->lcd->line[1] = "";
                pt->lcd->icon[BELL]->image->Visible = false;
                pt->buzzar->PlayDialTone();
                offHook = true;
                state = DIAL_NUMBER;
                break;
            }
            break;
        }
        break;
    case BROWSING_PHONE_BOOK_TREE:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_UP:
                Up();
                break;
            case KEY_DOWN:
                Down();
                break;
            case KEY_EXIT:
                if (pt->menuTree->currNode->Parent != pt->menuTree->root) {
                    Left();
                }
                else {
                    ExitPhoneBook();
                    EnterMainMenu();
                }
                break;
            case KEY_F:
                if (pt->menuTree->currNode->Parent->Text == "Search") {
                    if (pt->menuTree->currNode->Text == "HS Book") {
                        pb = &pt->cfg->pb;
                    }
                    else { // pt->menuTree->currNode->Text == "BS Book"
                        pb = &ft->cfg.pb;
                    }
                    inputTextDlg.Initial("Name:", "", 20);
                    state = SEARCH_PHONE_BOOK;
                }
                else if (pt->menuTree->currNode->Parent->Text == "Add Entry") {
                    if (pt->menuTree->currNode->Text == "HS Book") {
                        pb = &pt->cfg->pb;
                    }
                    else { // pt->menuTree->currNode->Text == "BS Book"
                        pb = &ft->cfg.pb;
                    }
                    inputTextDlg.Initial("Name:", "", 20);
                    state = ADD_PHONE_BOOK_ENTRY;
                    substate = INPUT_NAME;
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "Delete:One by one")) {
                    if (pt->menuTree->currNode->Text == "HS Book") {
                        pb = &pt->cfg->pb;
                    }
                    else { // pt->menuTree->currNode->Text == "BS Book"
                        pb = &ft->cfg.pb;
                    }
                    EnterSelectPhoneBookEntry(0, pb);
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "Delete:Delete all")) {
                    if (pt->menuTree->currNode->Text == "HS Book") {
                        pb = &pt->cfg->pb;
                    }
                    else { // pt->menuTree->currNode->Text == "BS Book"
                        pb = &ft->cfg.pb;
                    }
                    *pt->lcd->line[0] = "Are you sure?";
                    *pt->lcd->line[1] = "";
                    state = DELETE_ALL_PHONE_BOOK;
                    substate = CONFIRM;
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "Assign Tone:Rename Group")) {
                    EnterChangeName("Name:", "", 20);
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "Assign Tone:Group")) {
                    k = pt->menuTree->GetCurrNodeIdx();
                    EnterRingSetting(&pt->cfg->Vip[k].Melody, 0, &pt->cfg->Vip[k].Vol);
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "Transmit:To Handset")) {
                    k = pt->menuTree->GetCurrNodeIdx();           
                    tPBDlg.Initial(&pt->cfg->pb);
                    state = TRANSMIT_PHONE_BOOK;
                }
                else if (NodeIs(pt->menuTree->currNode, "Transmit:To Base")) {
                    tPBDlg.Initial(&ft->cfg.pb);
                    state = TRANSMIT_PHONE_BOOK;
                }
                else if (pt->menuTree->currNode->getFirstChild() != 0) {
                    Right();
                }
                break;
            }
            break;
        }
        break;
    case SEARCH_PHONE_BOOK:
        if (inputTextDlg.Input() == EXIT_DLG) {
            pt->lcd->DisplayCurrentMenu(pt->menuTree);
            state = BROWSING_PHONE_BOOK_TREE;
        }
        else {
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    k = pb->QuickSearch(inputTextDlg.Text());
                    EnterSelectPhoneBookEntry(k, pb);
                    break;
                case KEY_EXIT:  
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                }
                break;
            }
        }
        break;
    case SELECT_PHONE_BOOK_ENTRY:
        switch (substate) {
        case PICK_UP:
            selectPhoneBookEntryDlg.Input();
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    if (NodeIs(pt->menuTree->currNode->Parent, "Delete:One by one")) {
                        k = selectPhoneBookEntryDlg.CurrentIdx();
                        *pt->lcd->line[0] = "Erase?";
                        *pt->lcd->line[1] = pb->Item(k).name;
                        substate = ERASE_CONFIRM;
                    }
                    else {
                        *pt->lcd->line[1] = selectPhoneBookEntryDlg.CurrentItem().tel.office;
                        timeOut = 0;
                        substate = SHOW_TEL;
                    }
                    break; 
                case KEY_ON_OFF:  
                    *pt->lcd->line[1] = selectPhoneBookEntryDlg.CurrentItem().tel.office;
                    pt->lcd->icon[LINE]->image->Visible = true;
                    pt->lcd->icon[HOOK]->image->Visible = true;
                    substate = DIAL_TEL;
                    break;
                case KEY_EXIT:
                    ExitSelectPhoneBookEntry();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                case KEY_RIGHT:
                    EnterOption(); // state = ENTER_OPTION; substate = BROWSING_MENU;
                    break;
                }
                break;
            }
            break;
        case SHOW_TEL:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_ON_OFF:
                    pt->lcd->icon[LINE]->image->Visible = true;
                    pt->lcd->icon[HOOK]->image->Visible = true;
                    substate = DIAL_TEL;
                    break;
                case KEY_EXIT:
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    selectPhoneBookEntryDlg.Initial(pb, k);
                    substate = PICK_UP;
                    break;
                }
                break;
            }
            break;
        case DIAL_TEL:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_ON_OFF:
                    ExitPhoneBook();
                    pt->lcd->icon[LINE]->image->Visible = false;
                    pt->lcd->icon[HOOK]->image->Visible = false;
                    pt->lcd->icon[FUNCTION_SETTING]->image->Visible = false;
                    EnterMainMenu();
                    break;
                case KEY_EXIT:     
                    pt->lcd->icon[LINE]->image->Visible = false;
                    pt->lcd->icon[HOOK]->image->Visible = false;
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    selectPhoneBookEntryDlg.Initial(pb, k);
                    substate = PICK_UP;
                    break;
                }
                break;
            }
            break;   
        case ERASE_CONFIRM:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    pb->Delete(k);
                    *pt->lcd->line[1] = "OK";
                    pt->buzzar->PlayConfirmTone();
                    timeOut = 0;
                    substate = SHOW_ERASE_OK;
                    break;
                case KEY_EXIT:
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                }
                break;
            }
            break;
        case SHOW_ERASE_OK:
            switch (pt->act_event) {
            case TIME_OUT_500MS:
                if (timeOut > 2) {
                    timeOut = 0;
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                }
                break;
            }
            break;
        }
        break;
    case ENTER_OPTION:
        switch (substate) {
        case BROWSING_MENU:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_UP:
                    Up();
                    break;
                case KEY_DOWN:
                    Down();
                    break;
                case KEY_EXIT:
                    if (pt->menuTree->currNode->Parent != pt->menuTree->root) {
                        Left();
                    }
                    else {
                        ExitOption();
                    }
                    break;
                case KEY_F:
                    if (pt->menuTree->currNode->Text == "Edit") {
                        inputTextDlg.Initial("Name:", "", 20);
                        substate = INPUT_NAME;
                    }
                    else if (pt->menuTree->currNode->Text == "Erase") {
                        k = selectPhoneBookEntryDlg.CurrentIdx();
                        *pt->lcd->line[0] = "Erase?";
                        *pt->lcd->line[1] = pb->Item(k).name;
                        timeOut = 0;
                        substate = ERASE_CONFIRM;
                    }   
                    else if (pt->menuTree->currNode->Parent->Text == "Location") {
                        k = selectPhoneBookEntryDlg.CurrentIdx();
                        if (pt->menuTree->currNode->Text == "Office") {
                            if (pb->Item(k).tel.office == "") {
                                inputDigitsDlg.Initial("Office Number:", pb->Item(k).tel.office, 20);
                            }
                            else {
                                inputDigitsDlg.Initial("(O)" + pb->Item(k).name, pb->Item(k).tel.office, 20);
                            }
                        }
                        else if (pt->menuTree->currNode->Text == "Mobile") {
                            if (pb->Item(k).tel.mobile == "") {
                                inputDigitsDlg.Initial("Mobile Number:", pb->Item(k).tel.mobile, 20);
                            }
                            else {
                                inputDigitsDlg.Initial("(M)" + pb->Item(k).name, pb->Item(k).tel.mobile, 20);
                            }
                        }
                        else if (pt->menuTree->currNode->Text == "Fax") {
                            if (pb->Item(k).tel.fax == "") {
                                inputDigitsDlg.Initial("FAX Number:", pb->Item(k).tel.fax, 20);
                            }
                            else {
                                inputDigitsDlg.Initial("(F)" + pb->Item(k).name, pb->Item(k).tel.fax, 20);
                            }
                        }
                        timeOut = 0;
                        substate = LOCATION_INPUT_TEL;
                    }  
                    else if (pt->menuTree->currNode->Parent->Text == "VIP") {
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    else if (pt->menuTree->currNode->getFirstChild() != 0) {
                        Right();
                    }
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ExitOption();
                break;
            }
            break;
        case INPUT_NAME:
            if (inputTextDlg.Input() == EXIT_DLG) {
                pt->lcd->DisplayCurrentMenu(pt->menuTree);
                substate = BROWSING_MENU;
            }
            else {
                switch (pt->act_event) {
                case DR_KEYPAD_IND:
                    switch (pt->ptxx_dr_key) {
                    case KEY_F:
                        pbe.name = inputTextDlg.Text();
                        inputDigitsDlg.Initial("Number:", "", 20);
                        substate = EDIT_INPUT_TEL;
                        break;
                    case KEY_EXIT:
                        pt->lcd->DisplayCurrentMenu(pt->menuTree);
                        substate = BROWSING_MENU;
                        break;
                    }
                    break;
                }
            }
            break;
        case EDIT_INPUT_TEL:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pbe.tel.office = inputDigitsDlg.Digits(); 
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    pb->Delete(k);
                    pb->Add(pbe);
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    timeOut = 0;
                    inputTextDlg.BackToInput();
                    substate = INPUT_NAME;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ExitOption();
                k = pb->QuickSearch(pbe.name);
                selectPhoneBookEntryDlg.Initial(pb, k);
                break;
            }
            break;
        case ERASE_CONFIRM:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    pb->Delete(k);
                    *pt->lcd->line[1] = "OK";
                    substate = HOLD_BEFORE_EXIT_OPTION;
                    break;
                case KEY_EXIT:
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    substate = BROWSING_MENU;
                    break;
                }
                break;
            }
            break;
        case HOLD_BEFORE_EXIT_OPTION:
            switch (pt->act_event) {
            case TIME_OUT_500MS:
                if (timeOut > 2) {
                    ExitOption();
                    pt->buzzar->PlayConfirmTone();
                    timeOut = 0;
                }
                break;
            }
            break;
        case LOCATION_INPUT_TEL:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    k = selectPhoneBookEntryDlg.CurrentIdx();
                    if (pt->menuTree->currNode->Text == "Office") {
                        pb->UpdateOfficeTel(k, inputDigitsDlg.Digits());
                    }
                    else if (pt->menuTree->currNode->Text == "Mobile") {
                        pb->UpdateMobileTel(k, inputDigitsDlg.Digits());
                    }
                    else if (pt->menuTree->currNode->Text == "Fax") {
                        pb->UpdateFaxTel(k, inputDigitsDlg.Digits());
                    }        
                    pt->lcd->line[1]->StopBlink();
                    *pt->lcd->line[0] = "Wait";
                    *pt->lcd->line[1] = "to updating";
                    timeOut = 0;
                    substate = HOLD_BEFORE_EXIT_OPTION;
                    break;
                case KEY_EXIT:
                    timeOut = 0;
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    substate = BROWSING_MENU;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case ADD_PHONE_BOOK_ENTRY:
        switch (substate) {
        case INPUT_NAME:
            inputTextDlg.Input();
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pbe.name = inputTextDlg.Text();
                    inputDigitsDlg.Initial("Number:", "", 20);
                    substate = INPUT_TEL;
                    break;
                case KEY_EXIT:
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_TEL:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->lcd->line[0]->StopBlink();
                    pbe.tel.office = inputDigitsDlg.Digits();
                    k = pb->Find(pbe.name);
                    if (k == -1) {
                        pb->Add(pbe);
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    else { 
                        ShowMessageForOneSec("Name exist", "Setting Fail!!");
                    }
                    break;
                case KEY_EXIT:
                    inputTextDlg.BackToInput();
                    timeOut = 0;
                    substate = INPUT_NAME;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                if (k == -1) {        
                    k = pb->Find(pbe.name);
                    EnterSelectPhoneBookEntry(k, pb);
                }
                else {    
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                }
                break;
            }
            break;
        }
        break;
    case DELETE_ALL_PHONE_BOOK:
        switch (substate) {
        case CONFIRM:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    inputPinDlg.Initial("Security PIN:", "");
                    substate = INPUT_PIN;
                    break;
                case KEY_EXIT:
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_PIN:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:   
                inputPinDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    if (CheckPin(inputPinDlg.Pin(), ft->cfg.pin)) {
                        pb->DeleteAll();
                        ShowMessageForOneSec("Erasing memory", "OK!!");
                        pt->buzzar->PlayConfirmTone();
                    }
                    else {
                        ShowMessageForOneSec("PIN Error", "");
                        pt->buzzar->PlayFailTone();
                    }
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                if (CheckPin(inputPinDlg.Pin(), ft->cfg.pin)) {
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_PHONE_BOOK_TREE;
                }
                else {
                    inputPinDlg.Initial("Security PIN:", "");
                    substate = INPUT_PIN;
                }
                break;
            }
            break;
        }
        break;
    case TRANSMIT_PHONE_BOOK:
        tPBDlg.Input();
        if (tPBDlg.State() == EXIT_DLG) {
            state = BROWSING_PHONE_BOOK_TREE;
        }
        break;
    case BROWSING_FUNCTION_MENU_TREE:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_UP:
                Up();
                break;
            case KEY_DOWN:
                Down();
                break;
            case KEY_EXIT:
                if (pt->menuTree->currNode->Parent != pt->menuTree->root) {
                    Left();
                }
                else {
                    ExitFunctionMenu();
                    EnterMainMenu();
                }
                break;
            case KEY_F:
                if (pt->menuTree->currNode->Text == "Call ID") {
                    if (ft->cfg.cl.Size() == 0) {
                        *pt->lcd->line[0] = "    No Calls";
                        *pt->lcd->line[1] = "_";
                        pt->buzzar->PlayConfirmTone();
                        substate = SHOW_NO_CALLS;
                    }
                    else {
                        *pt->lcd->line[0] = "New " + IntToStr(ft->cfg.cl.TotalOfNew());
                        *pt->lcd->line[1] = "Old " + IntToStr(ft->cfg.cl.TotalOfOld());
                        pt->buzzar->PlayConfirmTone();
                        substate = SHOW_TOTAL_NEW_OLD;
                    }
                    state = VIEW_CALL_ID;
                    timeOut = 0;
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Answer Phone:Auto Answer")) {
                    // Auto Answer Setting
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    select = &pt->cfg->AnswerPhone_AutoAnswer;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Answer Phone:Any Key Answer")) {
                    // Any Key Answer Setting
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    select = &pt->cfg->AnswerPhone_AnyKeyAnswer;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Register HS")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    msDlg.Initial(pt->cfg->HaveRegisteredBase, 4, 0);
                    state = REGISTER_BASE;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Select Base")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    ssDlg.Initial(pt->cfg->SelectBase, 5, 0);
                    state = SELECT_BASE;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Idle Display:Display Type")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    if (pt->cfg->IdleDisplayOn) {
                        ssDlg.Initial(0, 2, 0);
                    }
                    else {
                        ssDlg.Initial(1, 2, 0);
                    }
                    state = IDLE_DISPLAY_TYPE;
                    substate = TURN_ON_OFF;
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "HS Setting:Idle Display:Change BS Name")) {
                    EnterChangeName("Enter Name:", "", 20);
                }
                else if (NodeIs(pt->menuTree->currNode, "HS Setting:Select Language")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    ssDlg.Initial(pt->cfg->SelectLanguage, 5, 0);
                    state = SELECT_LANGUAGE;
                } 
                else if (NodeIs(pt->menuTree->currNode, "BS Setting")) {
                    inputPinDlg.Initial("Enter PIN:", "");
                    pt->buzzar->PlayConfirmTone();
                    state = AUTHENTICATE_PIN;
                    timeOut = 0;
                }
                else if (NodeIs(pt->menuTree->currNode, "BS Setting:Change Pin")) {
                    inputPinDlg.Initial("New PIN:", "");
                    confirmInputPin = false;
                    pt->buzzar->PlayConfirmTone();
                    state = CHANGE_PIN;
                    substate = INPUT_PIN;
                    timeOut = 0;
                }
                else if (NodeIs(pt->menuTree->currNode, "BS Setting:Remove HS")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    for (int i = 0; i < 5; i++) {
                        if (ft->cfg.hsId[i].empty()) {
                            haveRegisterHS[i] = false;
                        }
                        else {
                            haveRegisterHS[i] = true;
                        }
                    }
                    msDlg.Initial(haveRegisterHS, 5, 0);
                    state = REMOVE_HS;
                }
                else if (NodeIs(pt->menuTree->currNode->Parent, "BS Setting:More Features")) {
                    if (pt->menuTree->currNode->Text == "Auto Prefix") {
                        inputDigitsDlg.Initial("Auto Prefix", "", 20);
                        pt->buzzar->PlayConfirmTone();
                        state = SET_PREFIX;
                    }
                    else {
                        if (pt->menuTree->currNode->Text == "Dial Mode") {
                            select = &ft->cfg.DialMode;
                            size = 2;
                        }
                        else if (pt->menuTree->currNode->Text == "Flash Time") {
                            select = &ft->cfg.FlashTime;
                            size = 4;
                        }
                        else if (pt->menuTree->currNode->Text == "Pause Time") {
                            select = &ft->cfg.PauseTime;
                            size = 4;
                        }
                        else if (pt->menuTree->currNode->Text == "Call Barring") {
                            select = &ft->cfg.CallBarring;
                            size = 4;
                        }
                        pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                        pt->menuTree->currNode->Selected = true;
                        ssDlg.Initial(*select, size, 0);
                        state = SET_MORE_FEATURE;
                        substate = PICK_UP;
                    }
                }
                else if (NodeIs(pt->menuTree->currNode, "BS Setting:HS Priority")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    msDlg.Initial(ft->cfg.HighPrioHS, 5, 0);
                    state = SET_HS_PRIORITY;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "BS Setting:SOS Setting")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    ssDlg.Initial(ft->cfg.SOSOnOff, 2, 0);
                    state = SOS_SETTING;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "Sound Setting:Ext. Ring")) {
                    // Ext.Ring Setting
                    EnterRingSetting(&pt->cfg->ExtRingMelody, 0, &pt->cfg->ExtRingVol);
   // ringSettingDlg.Initial(pMelody, beginMelody, pVol);
                }
                else if (NodeIs(pt->menuTree->currNode, "Sound Setting:Int. Ring")) {
                    // Int.Ring Setting
                    EnterRingSetting(&pt->cfg->IntRingMelody, 0, &pt->cfg->IntRingVol);
                }
                else if (NodeIs(pt->menuTree->currNode, "Sound Setting:Tone:Keyclick")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    select = &pt->cfg->Click;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "Sound Setting:Tone:Charging Tone")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    select = &pt->cfg->ChargingTone;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "Sound Setting:Tone:Confirm Tone")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    select = &pt->cfg->ConfirmTone;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "Clock Setting:Date")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    ssDlg.Initial(ft->cfg.Date.Format, 3, 0);
                    state = SET_DATE;
                    substate = PICK_UP;
                } 
                else if (NodeIs(pt->menuTree->currNode, "Clock Setting:Time")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    ssDlg.Initial(ft->cfg.Time.Format, 2, 0);
                    state = SET_TIME;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "More Features:Alarm Setting")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    ssDlg.Initial(pt->cfg->AlarmOnOff, 2, 0);
                    state = SET_ALARM;
                    substate = PICK_UP;
                }
                else if (NodeIs(pt->menuTree->currNode, "More Features:Appoints. Setting")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    select = &pt->cfg->AppointOnOff;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }    
                else if (NodeIs(pt->menuTree->currNode, "More Features:Memo Note")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    select = &pt->cfg->MemoNoteOnOff;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                } 
                else if (NodeIs(pt->menuTree->currNode, "More Features:Room Monitor")) {
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Selected = true;
                    select = &pt->cfg->RoomMonitorOnOff;
                    ssDlg.Initial(*select, 2, 0);
                    state = SET_ON_OFF;
                    substate = PICK_UP;
                }
                else if (pt->menuTree->currNode->getFirstChild() != 0) {
                    Right();
                }
            }
            break;
        }
        break;
    case VIEW_CALL_ID:
        switch (substate) {
        case SHOW_NO_CALLS:
            switch (pt->act_event) {
            case TIME_OUT_500MS:
                if (timeOut > 2) {
                    timeOut = 0;
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                }
                break;
            }
            break;
        case SHOW_TOTAL_NEW_OLD:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_EXIT:
                    timeOut = 0;
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                case KEY_DOWN:
                    iter.First();
                    if (!iter.IsDone()) {
                        cid = iter.CurrentItem();
                        *pt->lcd->line[0] = IntToStr(iter.CurrentIdx() + 1) + " " + AnsiString(cid.name);
                        *pt->lcd->line[1] = cid.tel;
                        page = 0;
                        substate = VIEW_CALL_LIST;
                    }
                    else {
                        timeOut = 0;
                        pt->lcd->DisplayCurrentMenu(pt->menuTree);
                        state = BROWSING_FUNCTION_MENU_TREE;
                    }
                    break;
                }
                break;
            }
            break;
        case VIEW_CALL_LIST:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_REDIAL:
                    timeOut = 0;
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                case KEY_DOWN:
                    iter.Next();
                    if (!iter.IsDone()) {
                        cid = iter.CurrentItem();
                        *pt->lcd->line[0] = IntToStr(iter.CurrentIdx() + 1) + " " + AnsiString(cid.name);
                        *pt->lcd->line[1] = cid.tel;
                    } 
                    else {
                        timeOut = 0;
                        pt->lcd->DisplayCurrentMenu(pt->menuTree);
                        state = BROWSING_FUNCTION_MENU_TREE;
                    }
                    page = 0;
                    break;
                case KEY_RIGHT:
                    if (!iter.IsDone()) {
                        if (page == 0) {
                            *pt->lcd->line[1] = cid.date.ToAnsiString().SubString(6, 5) + ":" + " " + cid.time.ToAnsiString().SubString(1, 5);
                            page = 1;
                        }
                        else { // page == 1
                            *pt->lcd->line[1] = cid.tel;
                            page = 0;
                        }
                    }
                    break;
                }
                break;
            }
            break;
        }
        break;
    case SET_ON_OFF:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    *select = ssDlg.Current();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(*select, 2, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case REGISTER_BASE:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                msDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                    case KEY_F:
                        *pt->lcd->line[0] = "Searching      " + IntToStr(msDlg.Current() + 1);
                        *pt->lcd->line[1] = "";
                        pt->lcd->icon[SELECT1]->image->Visible = false;
                        pt->lcd->icon[SELECT2]->image->Visible = false;
                        timeOut = 0;
                        substate = SEARCH_BASE;
                        break;
                    case KEY_EXIT:
                        BackToBrowsingFunctionMenu();
                        state = BROWSING_FUNCTION_MENU_TREE;
                        break;
                }
                break;
            }
            break;
        case SEARCH_BASE:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                    case KEY_EXIT:
                        msDlg.Initial(pt->cfg->HaveRegisteredBase, 4, msDlg.Current());
                        substate = PICK_UP;
                        break;
                }
                break;
            case TIME_OUT_500MS:
                if (timeOut > 4) {
                    // Search Base i
                    if (pt->cfg->FindBase[msDlg.Current()]) {
                        inputPinDlg.Initial("Enter PIN:", "");
                        pt->buzzar->PlayConfirmTone();
                        substate = INPUT_PIN;
                    }
                }
                break;
            }
            break;
        case INPUT_PIN:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputPinDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->lcd->line[1]->StopBlink();
                    if (CheckPin(inputPinDlg.Pin(), ft->cfg.pin)) {
                        pt->cfg->HaveRegisteredBase[msDlg.Current()] = true;
                        pt->buzzar->PlayConfirmTone();
                        msDlg.Initial(pt->cfg->HaveRegisteredBase, 4, msDlg.Current());
                        substate = PICK_UP;
                    }
                    else {
                        pt->lcd->line[1]->StopBlink();
                        inputPinDlg.Initial("Enter PIN:", "");
                        pt->buzzar->PlayFailTone();
                    }
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    msDlg.Initial(pt->cfg->HaveRegisteredBase, 4, msDlg.Current());
                    substate = PICK_UP;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case SELECT_BASE:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    *pt->lcd->line[0] = "Searching...";
                    *pt->lcd->line[1] = "";
                    pt->lcd->icon[SELECT1]->image->Visible = false;
                    pt->lcd->icon[SELECT2]->image->Visible = false;
                    timeOut = 0;
                    substate = SEARCH_BASE;
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            }
            break;
        case SEARCH_BASE:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case TIME_OUT_500MS:
                if (timeOut > 4) {
                    // Search Base i
                    if (ssDlg.Current() == 0 || pt->cfg->FindBase[ssDlg.Current() - 1]) {
                        pt->cfg->SelectBase = ssDlg.Current();
                        ShowMessageForOneSec("", "Setting OK");
                        timeOut = 0;
                    }
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(pt->cfg->SelectBase, 5, pt->cfg->SelectBase);
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case IDLE_DISPLAY_TYPE:
        switch (substate) {
        case TURN_ON_OFF:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    if (ssDlg.Current() == 0) { // "On"
                        pt->cfg->IdleDisplayOn = true;
                        pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                        ssDlg.Initial(pt->cfg->IdleDisplay, 2, 0);
                        substate = PICK_UP;
                    }
                    else { // "Off"
                        pt->cfg->IdleDisplayOn = false;
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(1, 2, 1);
                substate = TURN_ON_OFF;
                break;
            }
            break;
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->cfg->IdleDisplay = ssDlg.Current();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                if (pt->menuTree->currNode->Text != "Off") { // back to up two level
                    pt->menuTree->currNode = pt->menuTree->currNode->Parent;
                    pt->menuTree->currNode->Expanded = false;
                }
                BackToBrowsingFunctionMenu();
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        }
        break;
    case CHANGE_NAME:
        if (inputTextDlg.Input() == EXIT_DLG) {
            ExitChangeName();
        }
        else {
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->menuTree->currNode->Text = inputTextDlg.Text().c_str();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    ExitChangeName();
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ExitChangeName();
                break;
            }
        }
        break;
    case SELECT_LANGUAGE:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            ssDlg.Input(pt->ptxx_dr_key);
            switch (pt->ptxx_dr_key) {
            case KEY_F:
                pt->cfg->SelectLanguage = ssDlg.Current();
                pt->buzzar->PlayConfirmTone();
                ssDlg.Initial(pt->cfg->SelectLanguage, 5, pt->cfg->SelectLanguage);
                break;
            case KEY_EXIT:
                BackToBrowsingFunctionMenu();
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        }
        break;
    case AUTHENTICATE_PIN:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            inputPinDlg.Input();
            switch (pt->ptxx_dr_key) {
            case KEY_F:
                pt->lcd->line[1]->StopBlink();
                if (CheckPin(inputPinDlg.Pin(), ft->cfg.pin)) {
                    pt->lcd->icon[SELECT1]->image->Visible = false;
                    pt->lcd->icon[SELECT2]->image->Visible = false;
                    pt->lcd->line[1]->StopBlink();
                    pt->menuTree->currNode = pt->menuTree->currNode->getFirstChild();
                    pt->menuTree->currNode->Expanded = true;
                    pt->menuTree->currNode->Selected = true;
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    pt->buzzar->PlayConfirmTone();
                    state = BROWSING_FUNCTION_MENU_TREE;
                }
                else {
                    inputPinDlg.Initial("Enter PIN:", "");
                    pt->buzzar->PlayFailTone();
                }
                break;
            case KEY_EXIT:
                pt->lcd->icon[SELECT1]->image->Visible = false;
                pt->lcd->icon[SELECT2]->image->Visible = false;
                pt->lcd->line[1]->StopBlink();
                pt->lcd->DisplayCurrentMenu(pt->menuTree);
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        }
        break;
    case CHANGE_PIN:
        switch (substate) {
        case INPUT_PIN:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputPinDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->lcd->line[1]->StopBlink();
                    if (!confirmInputPin) {
                        confirmInputPin = true;
                        tempPin = inputPinDlg.Pin();
                        pt->lcd->line[1]->StopBlink();
                        inputPinDlg.Initial("Retype:", "");
                        pt->buzzar->PlayConfirmTone();
                    }
                    else {
                        if (CheckPin(inputPinDlg.Pin(), tempPin)) {
                            ft->cfg.pin = inputPinDlg.Pin();
                            ShowMessageForOneSec("", "Setting OK");
                        }
                        else {
                            pt->lcd->line[1]->StopBlink();
                            inputPinDlg.Initial("Retype:", "");
                            pt->buzzar->PlayFailTone();
                        }
                    }
                    break;
                case KEY_EXIT:
                    pt->lcd->icon[SELECT1]->image->Visible = false;
                    pt->lcd->icon[SELECT2]->image->Visible = false;
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                pt->lcd->DisplayCurrentMenu(pt->menuTree);
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        }
        break; 
    case REMOVE_HS:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            msDlg.Input(pt->ptxx_dr_key);
            switch (pt->ptxx_dr_key) {
            case KEY_F:
                if (ft->cfg.hsId[msDlg.Current()] != "") {
                    haveRegisterHS[msDlg.Current()] = false;
                    ft->cfg.hsId[msDlg.Current()] = "";
                    ShowMessageForOneSec("", "Setting OK");
                }
                else {
                    pt->buzzar->PlayFailTone();
                }
                break;
            case KEY_EXIT:
                BackToBrowsingFunctionMenu();
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        case EXIT_SHOW_MESSAGE:
            msDlg.Initial(haveRegisterHS, 5, msDlg.Current());
            state = BROWSING_FUNCTION_MENU_TREE;
            break;
        }
        break;
    case SET_PREFIX:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.Prefix = inputDigitsDlg.Digits();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                pt->lcd->DisplayCurrentMenu(pt->menuTree);
                state = BROWSING_FUNCTION_MENU_TREE;
                break;
            }
            break;
        }
        break;
    case SET_MORE_FEATURE:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    *select = ssDlg.Current();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(*select, size, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case SET_HS_PRIORITY:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                msDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    if (ft->cfg.HighPrioHS[msDlg.Current()]) {
                        ft->cfg.HighPrioHS[msDlg.Current()] = false;
                        msDlg.Initial(ft->cfg.HighPrioHS, 5, msDlg.Current());
                        pt->buzzar->PlayConfirmTone();
                    }
                    else {
                        ft->cfg.HighPrioHS[msDlg.Current()] = true;
                        inputDigitsDlg.Initial("Input Ringer:", "", 3);
                        pt->buzzar->PlayConfirmTone();
                        substate = INPUT_NUMBER;
                    }
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_NUMBER:  // input ring count
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.InputRingCount = StrToInt(inputDigitsDlg.Digits().c_str());
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                msDlg.Initial(ft->cfg.HighPrioHS, 5, msDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case SOS_SETTING:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.SOSOnOff = ssDlg.Current();
                    pt->lcd->icon[SELECT1]->image->Visible = false;
                    pt->lcd->icon[SELECT2]->image->Visible = false;
                    pt->buzzar->PlayConfirmTone();
                    if (ft->cfg.SOSOnOff == ON) {
                        inputDigitsDlg.Initial("Enter Number:", "", 20);
                        substate = INPUT_NUMBER;
                    }
                    else {
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break; 
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(ft->cfg.SOSOnOff, 2, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        case INPUT_NUMBER:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDigitsDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.SOSNo = inputDigitsDlg.Digits();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    pt->lcd->DisplayCurrentMenu(pt->menuTree);
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(ft->cfg.SOSOnOff, 2, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case RING_SETTING:
        ringSettingDlg.Input();
        if (ringSettingDlg.State() == EXIT_DLG) {
            ExitRingSetting();
        }
        break;
    case SET_DATE:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.Date.Format = ssDlg.Current();
                    inputDateDlg.Initial("Date:", ft->cfg.Date);
                    pt->buzzar->PlayConfirmTone();
                    substate = INPUT_DATE;
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_DATE:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputDateDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.Date = inputDateDlg.Date();
                    ShowMessageForOneSec("", "Setting OK");
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    ssDlg.Initial(ft->cfg.Date.Format, 3, ssDlg.Current());
                    substate = PICK_UP;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(ft->cfg.Date.Format, 3, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case SET_TIME:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    ft->cfg.Time.Format = ssDlg.Current();
                    inputTimeDlg.Initial("Time:", "", ft->cfg.Time);
                    pt->buzzar->PlayConfirmTone();
                    substate = INPUT_TIME;
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_TIME:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputTimeDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    time = inputTimeDlg.Time();
                    if (time.Hour < 1 || time.Hour > 24 || time.Minute > 59) {
                        pt->lcd->line[1]->StopBlink();
                        pt->buzzar->PlayFailTone();
                        inputTimeDlg.Initial("Time:", "HH:MM", inputTimeDlg.Time());
                    }
                    else {
                        ft->cfg.Time = inputTimeDlg.Time();
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    ssDlg.Initial(ft->cfg.Time.Format, 2, ssDlg.Current());
                    substate = PICK_UP;
                    break;
                }
                break;
            case TIME_OUT_500MS:
                if (ft->cfg.Time.Second == 0) { // show time at every 1 minute
                    *pt->lcd->line[1] = ft->cfg.Time;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(ft->cfg.Time.Format, 2, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case SET_ALARM:
        switch (substate) {
        case PICK_UP:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                ssDlg.Input(pt->ptxx_dr_key);
                switch (pt->ptxx_dr_key) {
                case KEY_F:
                    pt->cfg->AlarmOnOff = ssDlg.Current();
                    if (pt->cfg->AlarmOnOff == ON) {
                        pt->lcd->icon[SELECT1]->image->Visible = false;
                        pt->lcd->icon[SELECT2]->image->Visible = false;
                        *pt->lcd->line[0] = "Time:";
                        if (pt->cfg->AlarmTime.Hour == -1) {
                            inputTimeDlg.Initial("Time:", "HH:MM", pt->cfg->AlarmTime);
                        }
                        else {
                            inputTimeDlg.Initial("Time:", "", pt->cfg->AlarmTime);
                        }
                        substate = INPUT_TIME;
                    }
                    else {
                        ssDlg.Initial(pt->cfg->AlarmOnOff, 2, ssDlg.Current());
                    }
                    pt->buzzar->PlayConfirmTone();
                    break;
                case KEY_EXIT:
                    BackToBrowsingFunctionMenu();
                    state = BROWSING_FUNCTION_MENU_TREE;
                    break;
                }
                break;
            }
            break;
        case INPUT_TIME:
            switch (pt->act_event) {
            case DR_KEYPAD_IND:
                inputTimeDlg.Input();
                switch (pt->ptxx_dr_key) {
                case KEY_F:   
                    time = inputTimeDlg.Time();
                    if (time.Hour < 1 || time.Hour > 24 || time.Minute > 59) {
                        pt->buzzar->PlayFailTone();
                        inputTimeDlg.Initial("Time:", "", pt->cfg->AlarmTime);
                    }
                    else {
                        pt->cfg->AlarmTime = inputTimeDlg.Time();
                        ShowMessageForOneSec("", "Setting OK");
                    }
                    break;
                case KEY_EXIT:
                    pt->lcd->line[1]->StopBlink();
                    ssDlg.Initial(pt->cfg->AlarmOnOff, 2, ssDlg.Current());
                    substate = PICK_UP;
                    break;
                }
                break;
            case EXIT_SHOW_MESSAGE:
                ssDlg.Initial(pt->cfg->AlarmOnOff, 2, ssDlg.Current());
                substate = PICK_UP;
                break;
            }
            break;
        }
        break;
    case EDIT_SMS:
        break;
    case DIAL_NUMBER:
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            if (pt->ptxx_dr_key >= KEY_1 && pt->ptxx_dr_key <= KEY_HASH) {
                *pt->lcd->line[0] += pt->kp->key[pt->ptxx_dr_key].symbol;
                strcpy(cid.tel, pt->lcd->line[0]->ToAnsiString().c_str());
                if (offHook && strcmp(cid.tel, &ft->cfg.telNumber[0]) == 0) {
                    ft->OutgoingCall(cid);
                }
            }
            switch (pt->ptxx_dr_key) {
            case KEY_ON_OFF:
                if (offHook) {
                    offHook = false;
                    EnterMainMenu();
                }
                else {
                    ft->OutgoingCall(cid);
                }
                break;
            }
            break;
        case TIME_OUT_500MS:
         // return to MAIN_MENU, if user didn't enter  over 60 seconds.
            if (timeOut > 120) {
                pt->lcd->panel->Hidden();
                EnterMainMenu();
                timeOut = 0;
            }
            break;
        }
        break;
    }
    static AnsiString oldLine0, oldLine1;
    if (pt->act_event == EXT_INCOMING_CALL) {
        oldLine0 = pt->lcd->line[0]->ToAnsiString();
        oldLine1 = pt->lcd->line[1]->ToAnsiString();
        showIncomingCall = true;
        showTime = 0;
        pt->buzzar->PlayExtIncomingCallRing(RING_1);
        *pt->lcd->line[0] = pt->cid.name;
        *pt->lcd->line[1] = pt->cid.tel;
        pt->lcd->line[0]->blink = true;
        pt->lcd->line[1]->blink = true;
        pt->lcd->icon[HOOK]->blink = true;
    }
    if (showIncomingCall) {
        switch (pt->act_event) {
        case DR_KEYPAD_IND:
            switch (pt->ptxx_dr_key) {
            case KEY_ON_OFF:
                if (pt->lcd->icon[HOOK]->image->Visible) {
                    pt->lcd->icon[HOOK]->image->Visible = false;
                    *pt->lcd->line[0] = oldLine0;
                    *pt->lcd->line[1] = oldLine1;
                }
                else {
                    pt->lcd->icon[HOOK]->image->Visible = true;
                    pt->lcd->DisplayTime();
                    pt->lcd->line[0]->blink = false;
                    pt->lcd->line[1]->blink = false;
                } 
                pt->buzzar->Silence();
                showIncomingCall = false;
                showTime = 0;
                break;
            }
            break;
        case TIME_OUT_500MS:
            pt->lcd->DisplayTime();
            showTime++;
         // stop show caller id after 30 seconds from incoming call arrived.
            if (showTime > 60) {
                ft->SaveCallId();
                *pt->lcd->line[0] = oldLine0;
                *pt->lcd->line[1] = oldLine1;
                pt->lcd->line[0]->blink = false;
                pt->lcd->line[1]->blink = false;
                pt->lcd->icon[HOOK]->blink = false;
                pt->buzzar->Silence();
                showIncomingCall = false;
                showTime = 0;
            }
            break;
        }
    }
};

PT::PT() {
    cfg = new PTConfig;
    lcd = new Lcd(cfg);
    kp = new KeyPad;
    buzzar = new Buzzar;
    menuTree = new MenuTree();
    sms = new SMS;
    mmi = new PTMMI;
            
    Form1->TreeView1->Visible = false;

    Form1->Image1->Top = 0;
    Form1->Image1->Left = 0;

    Form1->Image2->Visible = false;

    Form1->Image3->Left = Form1->Image1->Left;
    Form1->Image3->Top = Form1->Image1->Top;
    Form1->Image3->Width = Form1->Image1->Width;
    Form1->Image3->Height = Form1->Image1->Height;
    Form1->Image3->Picture->LoadFromFile("bmp\\HS_red.bmp"); // gamma:need to change when porting.
    Form1->Image3->Visible = false;
};
     
PT::~PT() {
    delete mmi; 
    delete sms; 
    delete menuTree;
    delete buzzar; 
    delete kp; 
    delete lcd;
    cfg->SaveConfig();
    delete cfg;
};

void PT::IncomingCall(CID &data) {
    act_event = EXT_INCOMING_CALL;
    cid = data;
    mmi->Run();
};
