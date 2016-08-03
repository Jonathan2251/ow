#include "Group.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vcl.h> // for FTConfig::FTConfig()

using namespace std;

extern PT *pt;

ostream& operator<<(ostream& os, GDate& date) {
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
    return os << s;
};

ostream& operator<<(ostream& os, GTime& time) {
    char s[9];
    AnsiString temp;
    strcpy(s, "00:00:00");
    temp = IntToStr(time.Hour);
    if (temp.Length() <= 2) {
        memcpy(s + 2 - temp.Length(), temp.c_str(), temp.Length());
    }
    temp = IntToStr(time.Minute);
    if (temp.Length() <= 2) {
        memcpy(s + 5 - temp.Length(), temp.c_str(), temp.Length());
    }
    temp = IntToStr(time.Second);
    if (temp.Length() <= 2) {
        memcpy(s + 8 - temp.Length(), temp.c_str(), temp.Length());
    }
    return os << s;
};

char *GDate::ToStr() {
    static char s[20];
    if (Format == 0) {
        sprintf(s, "%d/%d/%d", Year, Month, Day);
    }
    else if (Format == 1) {
        sprintf(s, "%d/%d/%d", Month, Day, Year);
    }
    else if (Format == 2) {
        sprintf(s, "%d/%d/%d", Day, Month, Year);
    }  
    s[19] = '\0';
    return s;
};
 
AnsiString GDate::ToAnsiString() {
    static AnsiString s;
    s = ToStr();
    return s;
};

bool GDate::operator>(GDate &date) {
    if (Year > date.Year) {
        return true;
    }
    else if (Year == date.Year) {
        if (Month > date.Month) {
            return true;
        }
        else if (Month == date.Month) {
            if (Day > date.Day) {
                return true;
            }
        }
    }
    return false;
};

bool GDate::operator==(GDate &date) {
    if (Year == date.Year && Month == date.Month && Day == date.Day) {
        return true;
    }
    else {
        return false;
    }
};

void GDate::operator=(string &s) {
    if (Format == 0) {
        Year = StrToInt(s.substr(0, 4).c_str());
        Month = StrToInt(s.substr(5, 2).c_str());
        Day = StrToInt(s.substr(8, 2).c_str());
        if (s[4] != '/' || s[7] != '/') {
            throw DataTransferError();
        }
    }
    else if (Format == 1) {
        Month = StrToInt(s.substr(0, 2).c_str());
        Day = StrToInt(s.substr(3, 2).c_str());
        Year = StrToInt(s.substr(6, 2).c_str());
        if (s[2] != '/' || s[5] != '/') {
            throw DataTransferError();
        }
    }
    else if (Format == 2) {
        Day = StrToInt(s.substr(0, 2).c_str());
        Month = StrToInt(s.substr(3, 2).c_str());
        Year = StrToInt(s.substr(6, 2).c_str());
        if (s[2] != '/' || s[5] != '/') {
            throw DataTransferError();
        }
    }
    else {
        throw DataTransferError();
    }
};
  
char *GTime::ToStr() {
    static char s[20];
    sprintf(s, "%d:%d:%d", Hour, Minute, Second);
    s[19] = '\0';
    return s;
};

AnsiString GTime::ToAnsiString() {
    static AnsiString s;
    s = ToStr();
    return s;
};

bool GTime::operator>(GTime &time) {
    if (Hour > time.Hour) {
        return true;
    }
    else if (Hour == time.Hour) {
        if (Minute > time.Minute) {
            return true;
        }
        else if (Minute == time.Minute) {
            if (Second > time.Second) {
                return true;
            }
        }
    }
    return false;
};

bool GTime::operator==(GTime &time) {
    if (Hour == time.Hour && Minute == time.Minute && Second == time.Second) {
        return true;
    }
    else {
        return false;
    }
};

void GTime::operator=(string &s) {
    Hour = StrToInt(s.substr(0, 2).c_str());
    Minute = StrToInt(s.substr(3, 2).c_str());
    Second = StrToInt(s.substr(6, 2).c_str());
    if (s[2] != ':' || s[5] != ':') {
        throw DataTransferError();
    }
};

CallList::CallList() : _size(0) {
};

// if _size == 40 then replace _entry[1] with _entry[2], _entry [2] with _entry[3],
//     ..., _entry[k-1] with _entry[k], _entry[k] with entry.
// else
//     replace _entry[_size] with _entry[_size-1], ...,
//         _entry[k+1] with _entry[k], _entry[k] with entry.
void CallList::Insert(int k, CID &entry) {
    if (_size == 40) {
        for (int i = 0; i < k; i++) {
            _entry[i] = _entry[i + 1];
        }
        _entry[k] = entry;
    }
    else { // _size < 40
        for (int i = _size; i > k; i--) {
            _entry[i] = _entry[i - 1];
        }
        _entry[k] = entry;
        _size++;
    }
};

CID CallList::Item(int i) {
    return _entry[i];
};

int CallList::Size() {
    return _size;
};

int CallList::TotalOfNew() {
    int sum = 0;
    for (int i = 0; i < _size; i++) {
        if (IsNew[i]) {
            sum++;
        }
    }
    return sum;
};

int CallList::TotalOfOld() {
    return _size - TotalOfNew();
};

//  add a entry to PhoneBook according the order of (date, time)
void CallList::Add(CID &entry) {
    for (int i = 0; i < _size; i++) {
        if (_entry[i].date > entry.date) {
            Insert(i, entry);
            return;
        }
        else if (_entry[i].date == entry.date && _entry[i].time > entry.time) {
            Insert(i, entry);
            return;
        }
    }
    Insert(_size, entry);
};
  
// delete _entry[k]
void CallList::Delete(int k) {
    if (k < _size) {
        for (int i = k; i < _size - 1; i++) {
            _entry[i] = _entry[i + 1];
        } 
        _size--;
    }
};

ViewCallListIterator::ViewCallListIterator(CallList *cl) {
    _cl = cl;
};

void ViewCallListIterator::First() {
    int i;
    for (i = 0; i < _cl->Size(); i++) {
        _haveViewed[i] = false;
    }
    if (_cl->TotalOfNew() > 0) {
        for (i = 0; i < _cl->Size(); i++) {
            if (_cl->IsNew[i]) {
                _curr = i;
                _haveViewed[_curr] = true;
                break;
            }
        }
        _isDone = false;
    }
    else if (_cl->TotalOfOld() > 0) {
        for (i = 0; i < _cl->Size(); i++) {
            if (!_cl->IsNew[i]) {
                _curr = i;  
                _haveViewed[_curr] = true;
                break;
            }
        }
        _isDone = false;
    }
    else {
        _isDone = true;
    }
};

void ViewCallListIterator::Next() {
    int begin = _curr;
    if (!_isDone) {
        int totalOfNew = _cl->TotalOfNew();
        if (totalOfNew > 0) {
            _cl->IsNew[_curr] = false;
            for (_curr = (_curr + 1) % _cl->Size(); _curr != begin; _curr = (_curr + 1) % _cl->Size()) {
                if (_cl->IsNew[_curr] && !_haveViewed[_curr]) {
                    _cl->IsNew[_curr] = false;
                    _haveViewed[_curr] = true;
                    break;
                }
            }
        }
        else {
            for (_curr = (_curr + 1) % _cl->Size(); _curr != begin; _curr = (_curr + 1) % _cl->Size()) {
                if (!_cl->IsNew[_curr] && !_haveViewed[_curr]) {
                    _haveViewed[_curr] = true;
                    break;
                }
            }
            if (_curr == begin) {
                _isDone = true;
            }
        }
    }
};

bool ViewCallListIterator::IsDone() const {
    return _isDone;
};

CID ViewCallListIterator::CurrentItem() const {
    return _cl->Item(_curr);
};
 
int ViewCallListIterator::CurrentIdx() const {
    return _curr;
};

FTConfig::FTConfig() {
    ifstream ifs(ParamStr(2).c_str());
    while (!ifs.eof()) {
        string s;
        ifs >> s;
        if (s == "PIN") {
            ifs >> pin;
        } 
        if (s == "HS1") {
            ifs >> hsId[0];
        }
        else if (s == "HS2") {
            ifs >> hsId[1];
        }
        else if (s == "HS3") {
            ifs >> hsId[2];
        }
        else if (s == "HS4") {
            ifs >> hsId[3];
        }
        else if (s == "HS5") {
            ifs >> hsId[4];
        }
        else if (s == "DialMode") {
            ifs >> DialMode;
        }       
        else if (s == "FlashTime") {
            ifs >> FlashTime;
        }
        else if (s == "PauseTime") {
            ifs >> PauseTime;
        }    
        else if (s == "Prefix") {
            ifs >> Prefix;
        }
        else if (s == "CallBarring") {
            ifs >> CallBarring;
        }
        else if (s == "HighPriorityHS1") {
            ifs >> s;
            if (s == "true") {
                HighPrioHS[0] = true;
            }
            else {
                HighPrioHS[0] = false;
            }
        } 
        else if (s == "HighPriorityHS2") {
            ifs >> s;
            if (s == "true") {
                HighPrioHS[1] = true;
            }
            else {
                HighPrioHS[1] = false;
            }
        } 
        else if (s == "HighPriorityHS3") {
            ifs >> s;
            if (s == "true") {
                HighPrioHS[2] = true;
            }
            else {
                HighPrioHS[2] = false;
            }
        }
        else if (s == "HighPriorityHS4") {
            ifs >> s;
            if (s == "true") {
                HighPrioHS[3] = true;
            }
            else {
                HighPrioHS[3] = false;
            }
        }
        else if (s == "HighPriorityHS5") {
            ifs >> s;
            if (s == "true") {
                HighPrioHS[4] = true;
            }
            else {
                HighPrioHS[4] = false;
            }
        }
        else if (s == "InputRingCount") {
            ifs >> InputRingCount;
        }
        else if (s == "SOS_ON_OFF") {
            ifs >> SOSOnOff;
        } 
        else if (s == "SOS_Number") {
            ifs >> SOSNo;
        }
        else if (s == "Date::Format") {
            ifs >> Date.Format;
        }
        else if (s == "Date::Year") {
            ifs >> Date.Year;
        }
        else if (s == "Date::Month") {
            ifs >> Date.Month;
        }
        else if (s == "Date::Day") {
            ifs >> Date.Day;
        }  
        else if (s == "Time::Format") {
            ifs >> Time.Format;
        }
        else if (s == "Time::Hour") {
            ifs >> Time.Hour;
        }
        else if (s == "Time::Minute") {
            ifs >> Time.Minute;
        }
        else if (s == "Time::Second") {
            ifs >> Time.Second;
        }
        else if (s == "LISTEN_PORT") {
            ifs >> listenPort;
        }
        else if (s == "SEND_IP") {
            ifs >> sendIP;
        }
        else if (s == "SEND_PORT") {
            ifs >> sendPort;
        }
        else if (s == "TEL_NUMBER") {
            ifs >> telNumber;
        }
        else if (s == "[Caller_Id_List]") {
            ifs >> s;
            if (s == "{") {
                CID cid;
                for (int i = 0; i < 40;) {
                    ifs >> s;
                    if (s == "name") {
                        ifs >> s;
                        strcpy(cid.name, &s[0]);
                    }
                    else if (s == "date") {
                        ifs >> s;
                        cid.date.Format = 0;
                        cid.date = s;
                    }
                    else if (s == "time") {
                        ifs >> s;
                        cid.time.Format = 0;
                        cid.time = s;
                    }
                    else if (s == "tel") {
                        ifs >> s;
                        strcpy(cid.tel, &s[0]);
                    }
                    else if (s == "new") {
                        cl.IsNew[i] = true;
                        cl.Add(cid);
                        i++;
                    }
                    else if (s == "old") {
                        cl.IsNew[i] = false;
                        cl.Add(cid);
                        i++;
                    }
                    else if (s == "}") {
                        break;
                    }
                }
            }
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
    }
    return;
};

FTConfig::~FTConfig() {
    ofstream ofs(ParamStr(2).c_str());
    int i, j;
    ofs << "PIN " << pin << endl;
    for (i = 0; i < 5; i++) {
        if (!hsId[i].empty()) {
            ofs << "HS" << i+1 << " " << hsId[i] << endl;
        }
    }
    ofs << "DialMode " << DialMode << " // 0:Tone, 1:Pulse" << endl;
    ofs << "FlashTime " << FlashTime << " // 0:80ms, 1:200ms, 2:500ms, 3:900ms" << endl;
    ofs << "PauseTime " << PauseTime << " // 0:1.0s, 1:2.0s, 2:3.0s, 3:4.0s" << endl;   
    ofs << "CallBarring " << CallBarring << " // 0:base1, 1:base2, 2:base3, 3:base4" << endl;
    ofs << "Prefix " << Prefix << endl; 
    for (i = 0; i < 5; i++) {
        ofs << "HighPriorityHS" << i + 1;
        if (HighPrioHS[i]) {
            ofs << " true" << endl;
        }
        else { 
            ofs << " false" << endl;
        }
    }
    ofs << "InputRingCount " << InputRingCount << endl;
    ofs << "SOS_ON_OFF " << SOSOnOff << " // 0:On, 1:Off" << endl;
    ofs << "SOS_Number " << SOSNo << endl;
    ofs << "Date::Format " << Date.Format << endl;
    ofs << "Date::Year " << Date.Year << endl;
    ofs << "Date::Month " << Date.Month << endl;
    ofs << "Date::Day " << Date.Day << endl;
    ofs << "Time::Format " << Time.Format << endl;
    ofs << "Time::Hour " << Time.Hour << endl;
    ofs << "Time::Minute " << Time.Minute << endl;
    ofs << "Time::Second " << Time.Second << endl;
    ofs << "SEND_IP " << sendIP << endl;
    ofs << "SEND_PORT " << sendPort << endl;
    ofs << "TEL_NUMBER " << telNumber << endl;
    ofs << "[Caller_Id_List] {" << endl;
    for (i = 0; i < cl.Size(); i++) {
        ofs << "name " << cl.Item(i).name << endl;
        ofs << "date " << cl.Item(i).date << endl;
        ofs << "time " << cl.Item(i).time << endl;
        ofs << "tel " << cl.Item(i).tel << endl;
        if (cl.IsNew[i]) {
            ofs << "new" << endl;
        }
        else {
            ofs << "old" << endl;
        }
    }
    ofs << "}" << endl;

    ofs << "[Phone_Book] {" << endl;
    for (i = 0; i < pb.Size(); i++) {
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
};

FT::FT(TServerSocket *aReceiver, TClientSocket *aSender) {
    receiver = aReceiver;
    sender = aSender;
    receiver->Port = cfg.listenPort;
    sender->Host = cfg.sendIP.c_str();
    sender->Port = cfg.sendPort;
    receiver->Active = true;
    sender->Active = false;
};

bool FT::IsRegistered(string& aHsId) {
    int i;
    if (aHsId.empty()) {
        return false;
    }
    for (i = 0; i < 5; i++) {
        if (aHsId == cfg.hsId[i]) {
            return true;
        }
    }
    return false;
};

bool FT::OutgoingCall(CID &data) {
    sender->Socket->SendBuf((void*)&data, 80);
    return false;
};

void FT::IncomingCall(CID &data) {
    msg = data;
    pt->IncomingCall(data);
};
    
void FT::SaveCallId() {
    cfg.cl.Add(msg);
};

FT *ft;
