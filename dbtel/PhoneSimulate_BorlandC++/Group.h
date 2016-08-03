#include "Unit1.h"
#include <string>
#include <stack>

using namespace std;

enum {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_REDIAL, KEY_ON_OFF,
    KEY_HANDSFREE, KEY_TOP_MESSAGE, KEY_FT_SERVICES, KEY_SOS, KEY_MENU,
    KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_STAR, KEY_0, KEY_HASH, KEY_SOS_SMS, KEY_SMS, KEY_ON_OFF_LONG_PRESS};
#define KEY_SIZE KEY_ON_OFF_LONG_PRESS+1
#define NO_KEY -1
#define KEY_PHONE_BOOK KEY_UP
#define KEY_F KEY_MENU
#define KEY_EXIT KEY_REDIAL

enum {FUNCTION_SETTING, TAM, LINE, HOOK, INTERCOM, MUTE, BATTERY, ANTENNA,
    SPEAKER, KMESSAGE, BELL, SELECT1, SELECT2
};
#define ICON_SIZE SELECT2+1

enum {RING_1
};

enum Event {NO_EVENT = -1, DR_KEYPAD_IND = 0, TIME_OUT_500MS,
    EXT_INCOMING_CALL, EXIT_SHOW_MESSAGE
};

enum {OFFICE, MOBILE, FAX
};

class DataTransferError {};
class OpenConfigFileFail {};  // for exception
 
struct GDate {
    int Format;
    int Year;
    int Month;
    int Day;
    char *ToStr();
    AnsiString ToAnsiString();
    bool operator>(GDate &date);
    bool operator==(GDate &date);
    void operator=(string &s);
};

struct GTime {
    int Format;
    int Hour;
    int Minute;
    int Second;
    char *ToStr();  
    AnsiString ToAnsiString();
    bool operator>(GTime &time);
    bool operator==(GTime &time);
    void operator=(string &s);
};

struct CID {
    char name[20];
    GDate date;
    GTime time;
    char tel[20];
};

class CallList {
private:
    CID _entry[40];
    int _size;
    void Insert(int k, CID &entry); // insert entry before _entry[k]
public:
    bool IsNew[40];
    CallList();
    CID Item(int i);
    int Size();
    int TotalOfNew();
    int TotalOfOld();
    void Add(CID &entry); // insert cid according (date, time)
    void Delete(int k);
};

// abstract class, interface for searching step by step.
template <class T>
class Iterator
{
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T CurrentItem() const = 0;
protected:
    Iterator() { };
};

class ViewCallListIterator : public Iterator<CID> {
private:
    CallList *_cl;
    int _curr;
    bool _isDone;
    bool _haveViewed[40];
public:
    ViewCallListIterator(CallList *cl);
    void First();
    void Next();
    bool IsDone() const;
    CID CurrentItem() const;
    int CurrentIdx() const;
};

struct TelEntry {
    string office;
    string mobile;
    string fax;
};

struct PhoneBookEntry {
    string name;
    TelEntry tel;
    bool operator>(PhoneBookEntry &entry); 
    bool operator==(PhoneBookEntry &entry);
};

class PhoneBook {
private:
    PhoneBookEntry _entry[40];
    int _size;
    void Insert(int k, PhoneBookEntry &entry); // insert entry before _entry[k]
public:
    int current;
    PhoneBook();
    PhoneBookEntry Item(int i);
    int Size();
    int Find(string name);
    void Add(PhoneBookEntry &entry); // insert entry according (name, tel)
    void Delete(int k);
    void UpdateOfficeTel(int k, string tel);
    void UpdateMobileTel(int k, string tel);
    void UpdateFaxTel(int k, string tel);
    int QuickSearch(string s);
    void DeleteAll();
};

struct FTConfig {
    string pin;
    string hsId[5]; // registration for HS
    int DialMode; // 0:Tone, 1:Pulse
    int FlashTime; // 0:80ms, 1:200ms, 2:500ms, 3:900ms
    int PauseTime; // 0:1.0s, 1:2.0s, 2:3.0s, 3:4.0s
    int CallBarring; // 0:base1, 1:base2, 2:base3, 3:base4
    string Prefix;
    bool HighPrioHS[5];
    int InputRingCount;
    int SOSOnOff; // 0:On, 1:Off
    string SOSNo;
    GDate Date;
    GTime Time;
    int listenPort;
    string sendIP;
    int sendPort;
    string telNumber;
    CallList cl;
    PhoneBook pb;
    FTConfig();
    ~FTConfig();
};

class FT {
private: 
    TServerSocket *receiver;
    CID msg;
public:
    TClientSocket *sender;
    FTConfig cfg;
    FT(TServerSocket *aReceiver, TClientSocket *aSender);
    bool IsRegistered(string& aHsId);
// data include name, date, time, tel number
    bool OutgoingCall(CID &data);
    void IncomingCall(CID &data);
    void SaveCallId();
};

struct MelodyVolume {
    int Melody;
    int Vol;
};

// data read from config.txt and must rewrite to file after the program end.
struct PTConfig {
    enum {ON, OFF};
    int AnswerPhone_AutoAnswer; // 0:On, 1:Off
    int AnswerPhone_AnyKeyAnswer; // 0:On, 1:Off
    int Click; // 0:On, 1:Off
    int ChargingTone; // 0:On, 1:Off
    int ConfirmTone; // 0:On, 1:Off

    bool HaveRegisteredBase[4];
    bool FindBase[4];
    bool SuccessRegisterBase[4];

    int SelectBase; //0..3
    string PriorityCallAlert;
    bool IdleDisplayOn;
    int IdleDisplay;
    string IdleDisplay_Product;
    string IdleDisplay_Base;
    int SelectLanguage;
    int ExtRingMelody;
    int ExtRingVol;
    int IntRingMelody;
    int IntRingVol;
    int AlarmOnOff;
    GTime AlarmTime;
    int AppointOnOff;
    int MemoNoteOnOff;
    int RoomMonitorOnOff;
    string batteryStatus;
    PhoneBook pb;
    MelodyVolume Vip[5];
    PTConfig();
    void SaveConfig();
};

struct CChar {
    TLabel* ch;
    bool blink; //Decide that each character in TLabel is need to blink or not.
    CChar(int x, int y, int w, int h);
    void operator=(char c);
    bool operator==(char c);
    void StopBlink(); //Stop character blinking.
};

class Line {
private:
    int length;
public:
    bool blink;
    CChar *pos[16];
    Line(int x, int y);
    void operator=(char *s);
    void operator=(string s);
    void operator=(AnsiString s);  
    void operator=(GDate &date);  
    void operator=(GTime time);
    int Length();
    AnsiString ToAnsiString();
    string Tostring();
    GDate ToDate(int dateFormat); 
    GTime ToTime(int timeFormat);
    AnsiString operator+(char *s);
    void operator+=(char *s);
    void StopBlink(); //Stop all characters on Line blinking.
    void Visible(); // visible all pos[] in line
    void Invisible(); // invisible all pos[] in line
};

class Icon {
public:
    TImage *image;
    bool blink;
    Icon();
    void StopBlink();
};

class SymbolPanel {
public:
    int idx; // idx for user select
    int size;   // value is depend on key pressed. eg. KEY_2 is 4 (ABC2),
                // KEY_7 is 5 (PQRS7).
    TStaticText *ch[7];

    SymbolPanel(Line *line[2]);
    bool Visible();

// Load Letter and Symbol using aKey information. eg. KEY_2 load ABC.
    void Load(int aKey, bool upperCase);
    void Show();
    void Hidden();
    char ActiveSymbol();
};
               
struct MenuTree {
    TTreeNode *root; // point to Node "Main Menu"
    TTreeNode *currNode; // point to Node Current Travel Node
    HTREEITEM returnNodeId;
    MenuTree();
    int GetCurrNodeIdx(); 
    void EnterPhoneBookMenu();
    void ExitPhoneBookMenu();
    void EnterFunctionMenu();
    void ExitFunctionMenu();
    void EnterOptionMenu();
    void ExitOptionMenu();
};

// Lcd have 2 lines and each line can display 16 characters.
// Each character be displayed through a dedicate TLabel.
struct Lcd {
    Line *line[2];
    Icon *icon[ICON_SIZE];
    SymbolPanel *panel;

    Lcd(PTConfig *aCfg);
    void Clear();
    void DisplaySearching();
    void DisplayMainMenu();
    void DisplayTime();
// Display currNode and NextSibling of currNode on lcd
    void DisplayCurrentMenu(MenuTree *aMenuTree);
// Timer1 call Trigger() at every 0.5 second, just for handle blink fuction
    void Trigger(); //Timer1 call Trigger() at every 0.5 second just for process blink.
    void StopBlink(); //Stop all characters on Lcd blinking.
};

struct Key {
    int Left, Top, Height, Width; // rectangle for displaying the Pressed Key Image.
    TColor color; // each different key has different color
    AnsiString bitMapFile; // name of the Bitmap File for display the effect of the key be pressed.
    char symbol[5]; // ascii character for display on lcd

    void ShowKeyPressed(TImage *image2);
};

struct KeyPad{
    Key key[KEY_SIZE];
    bool keyPressed;
    int pressTime; // 0.5 second unit
    KeyPad();
    int WhichKeyPressed(int x, int y);
};

class Buzzar {
private:
    const string dir;
    string voiceFile[KEY_SMS+11];
public:
    Buzzar(); 
    ~Buzzar();
    void PlayKeyPressedVoice(int aKey);
    void PlayPowerOnMelody();
    void PlayPowerOffMelody();
    void PlayConfirmTone();
    void PlayFailTone();
    void PlayDialTone();
    void PlayRing(int melody, int volIdx);
    void PlayExtIncomingCallRing(int ringType);
    void SetVol(DWORD vol);
    void Silence();
};

struct SMS {
    string writeBuf;
};

class PTMMI {
private:
    enum {ON, OFF};
    class SingleSelectDlg {
    private:
        int _select;
        int _size;
        int _curr;
        void Display();
    public:
        void Initial(int select, int size, int curr);
        void Input(int key);
        int Current();
    };
    class MultiSelectDlg {
    private:
        bool _select[10];
        int _size;
        int _curr;
    public:  
        void Display();
        void Initial(bool *select, int size, int curr);
        void Input(int key);
        int Current();
    };
    class InputTextDlg {
    private:
        string _title;
        string _text;
        unsigned int _begin; // idx for lcd->line[1] = &_text[_begin];
        unsigned int _pos; // pos for cursor at lcd->line[1] range is 0..15
        unsigned int _maxSize; // max size of _text
        PhoneBook *_pb;
        int _selTime;
        int _timeOut;
        int _lastKey;
        bool _uppercase;
    public:
        void Initial(string title, string text, unsigned int maxSize);
        int Input();
        string Text();
        void BackToInput();
    };
    class InputDigitsDlg {
    private:
        string _digits;
        unsigned int _begin; // idx for lcd->line[1] = &_digits[_begin];
        unsigned int _pos; // pos for cursor at lcd->line[1] range is 0..15
        unsigned int _maxSize; // max size of _digits
    public:
        void Initial(string title, string digits, unsigned int maxSize);
        void Input();
        string Digits();
    };
    class SelectPhoneBookEntryDlg {
    private:
        PhoneBook *_pb;
        int _curr;
    public:
        void Initial(PhoneBook *pb, int curr);
        void Input();
        int CurrentIdx();
        PhoneBookEntry CurrentItem();
    };
    class InputPinDlg {
    private:
        string _pin;
        unsigned int _begin; // idx for lcd->line[1] = &_pin[_begin];
        unsigned int _pos; // pos for cursor at
    public:
        void Initial(string title, string pin);
        void Input();
        string Pin();
    };
    class InputDateDlg {
    private:
        GDate _date;
        int _pos;
    public:
        void Initial(string title, GDate date);
        void Input();
        GDate Date();
    };
    class InputTimeDlg {
    private:
        GTime _time;
        int _pos;
    public:
        void Initial(string s1, string s2, GTime date);
        void Input();
        GTime Time();
    };    
    class AdjustScaleDlg {
    private:
        int _scale;
    public:
        void Initial(string title, int scale);
        void Input();
        int Scale();
    };
    class RingSettingDlg {
    private:
        AdjustScaleDlg _adjustScaleDlg;
        int _state;
        int *_melody;
        int _melodyBegin; // offset for melody. eg. External Ring offset is 0, VIP Ring offset is 10 = VIP1
        int *_vol;
        int _timeOut;
    public:
        void Initial(int *melody, int melodyBegin, int *vol);
        void Input();
        int State();
    };
    class TransmitPhoneBookDlg {
    private:
        MultiSelectDlg _msDlg;
        PhoneBook *_toPB;
        bool _select[20];
        int _state;
        int _timeOut;
        void RemovePBFromTree();
    public:
        void Initial(PhoneBook *toPB);
        void Input();
        int State();
    };
    struct States {
        int state;
        int substate;
    };
    int state;
    int substate;
    stack<States> trace;
    bool offHook;
    SingleSelectDlg ssDlg;
    MultiSelectDlg msDlg;
    InputTextDlg inputTextDlg;
    InputDigitsDlg inputDigitsDlg;
    SelectPhoneBookEntryDlg selectPhoneBookEntryDlg; 
    InputPinDlg inputPinDlg;
    InputDateDlg inputDateDlg;
    InputTimeDlg inputTimeDlg;
    AdjustScaleDlg adjustScaleDlg;
    RingSettingDlg ringSettingDlg;
    TransmitPhoneBookDlg tPBDlg;

    // temper variable
    PhoneBook *pb; // used by InputPhoneBookNameDlg()
    //int selTime;
    bool showIncomingCall;
    int showTime;

    bool NodeIs(TTreeNode *p, char *str);
    void BackToBrowsingFunctionMenu();
    bool CheckPin(string &pin1, string &pin2);
    //void InputPinDlg();
    void EnterMainMenu();
    void EnterPhoneBook();
    void ExitPhoneBook();
    void EnterFunctionMenu();
    void ExitFunctionMenu();
    void EnterSelectPhoneBookEntry(int k, PhoneBook *pb);
    void ExitSelectPhoneBookEntry();
    void EnterOption();
    void ExitOption();
    void ShowMessageForOneSec(string s1, string s2);
    void EnterChangeName(string s1, string s2, int size);
    void ExitChangeName();
    void EnterRingSetting(int *melody, int beginMelody, int *vol);
    void ExitRingSetting();
    void Up();
    void Down();
    void Left();
    void Right();
public:
    enum { // state
        POWER_OFF = 0, CONNECT_BASE, MAIN_MENU, BROWSING_PHONE_BOOK_TREE,
        SEARCH_PHONE_BOOK, ADD_PHONE_BOOK_ENTRY,
        ENTER_OPTION, DELETE_ALL_PHONE_BOOK, TRANSMIT_PHONE_BOOK,
        BROWSING_FUNCTION_MENU_TREE, VIEW_CALL_ID,
        SET_ON_OFF, SET_ON_OFF_COMPLETE, REGISTER_BASE, SELECT_BASE,
        IDLE_DISPLAY_TYPE, CHANGE_NAME, SELECT_LANGUAGE,
        AUTHENTICATE_PIN, CHANGE_PIN, REMOVE_HS, SET_PREFIX,
        SET_MORE_FEATURE, SET_HS_PRIORITY, SOS_SETTING,
        SET_DATE, SET_TIME, SET_ALARM,
        RING_SETTING,
        EDIT_SMS, DIAL_NUMBER, SHOW_MESSAGE,
        // substate
        PICK_UP, SEARCH_BASE, INPUT_PIN, TURN_ON_OFF,
        INPUT_NUMBER, INPUT_DATE, INPUT_TIME,
        SHOW_NO_CALLS, SHOW_TOTAL_NEW_OLD, VIEW_CALL_LIST,
        SHOW_TEL, DIAL_TEL, SHOW_ERASE_OK,
        BROWSING_MENU, INPUT_NAME, INPUT_TEL, ERASE_CONFIRM,
        EDIT_INPUT_TEL, HOLD_BEFORE_EXIT_OPTION, LOCATION_INPUT_TEL,
        CONFIRM,
        // state and TransmitPhoneBookDlg
        SELECT_PHONE_BOOK_ENTRY,
        // substate and RingSettingDlg
        MELODY_SETTING, VOL_SETTING,
        // RingSettingDlg
        FINISH_MELODY_SETTING, FINISH_VOL_SETTING,
        // Run() and Dialog
        CONTINUE, EXIT_DLG,
        // TransmitPhoneBookDlg
        SHOW_TOTAL_CHOOSE, WAIT_TRANSMIT, SHOW_TRANSMIT_OK
    };

    PTMMI();
    int State();
    void Run(); 
    void ExitShowMessageForOneSec();
};

class PT {
public:
    string id; // id for HS
    Event act_event;
    int ptxx_dr_key;
    PTMMI *mmi;
    MenuTree *menuTree;
    Lcd *lcd;
    KeyPad *kp;
    Buzzar *buzzar;
    /*   Battery _battery;
    Rf  _rf;*/
    PTConfig *cfg;
    SMS *sms;
    int state; // tell us what a status of the phone now is. eq. MMI state
    CID cid; // the last caller id

    PT();
    ~PT();
    void IncomingCall(CID &data);
};

