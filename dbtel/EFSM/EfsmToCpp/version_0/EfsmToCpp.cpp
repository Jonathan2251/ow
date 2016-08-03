/* This program transfer *.efsm to *.cpp
for example:

input file test.efsm as follows

*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#include <fstream>
#include <list>
#include <vector>
#include <string>

using namespace std;

enum {STATE = 1, SUBSTATE, THREAD, TRANSITION, FROM, TO, COLON,
    COMMA, CONDITION, ACTION, ID, PRIORITY, COMMENT
};
/*
bool operator==(string& str1, char *str2) {
    cout << str1.size() << " " << strlen(str2) << endl;
    if (str1.size() < strlen(str2)) {
        return false;
    }
    else {
        if (strcmp(str1.substr(0, strlen(str2)).c_str(), str2) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
} */

struct Token {
    int id;
    char value[1000];
};

class LexialAnalyser {
private:
	ifstream _ifs;
    Token _token;
    Token _prevToken;
    bool _isDone;
    long bp;

    bool GetColon() {
        char ch;
        _ifs >> ch;
        if (ch == ':') {
            return true;
        }
        else {
            return false;
        }
    };  
    bool GetComma() {
        char ch;
        _ifs >> ch;
        if (ch == ',') {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetState() {
        string s;
        _ifs >> s;
        if (s == "state") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetSubstate() {
        string s;
        _ifs >> s;
        if (s == "substate") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetThread() {
        string s;
        _ifs >> s;
        if (s == "thread") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetTransition() {
        string s;
        _ifs >> s;
        if (s == "transition") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetFrom() {
        string s;
        _ifs >> s;
        if (s == "from") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetTo() {
        string s;
        _ifs >> s;
        if (s == "to") {
            return true;
        }
        else {
            return false;
        }
    };
    bool GetCondition() {
        char ch;
        _ifs >> ch;
        if (ch == '?') {
            _ifs >> ch;
            int i;
            for (i = 0; ch != '?' && !_ifs.eof(); i++) { 
                _token.value[i] = ch;
                _ifs >> ch;
            }
            _token.value[i] = '\0';
            if (ch == '?') {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    };
    bool GetAction() {
        char ch;
        _ifs >> ch;
        if (ch == '!') {
            _ifs >> ch;
            int i;
            for (i = 0; ch != '!' && !_ifs.eof(); i++) { 
                _token.value[i] = ch;
                _ifs >> ch;
            }
            _token.value[i] = '\0';
            if (ch == '!') {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    };
    bool GetId() {
        char ch;
        _ifs.get(ch);
        if (isalpha(ch) || ch == '_') { 
            _token.value[0] = ch;
            int i;  
            long pos = _ifs.tellg();
            _ifs.get(ch);
            for (i = 1; (isalpha(ch) || ch == '_' || isdigit(ch)) && !_ifs.eof(); i++) {
                _token.value[i] = ch;
                pos = _ifs.tellg();
                _ifs.get(ch);
            }
            if (ch == ' ' || ch == ',' || ch == ':' || ch == '\n') {
                _token.value[i] = '\0';
                _ifs.seekg(pos);
                //cout << "_token.value = " << _token.value << endl;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    };   
    bool GetPriority() {
        int i = 0;
        _ifs >> i;
        if (i != 0) {
            itoa(i, _token.value, 10);
            return true;
        }
        else {
            return false;
        }
    };
    bool GetComment() {
        char ch;
        _ifs >> ch;
        if (ch == '/') {
            _ifs >> ch;
            if (ch == '/') {
                char s[1001];
                _ifs.get(s, 1000, '\n');
                return true;
            }
        }
        return false;
    }
public:
	LexialAnalyser() {
        _ifs.open("test.efsm");
	};
	Token Token() {
        return _token;
	};
    void First() {
        _ifs.close();
        _ifs.open("test.efsm");
        _isDone = false;
        _prevToken.id = 0;
        Next();
    };
	void Next() {
        _prevToken = _token;
        bp = _ifs.tellg();
        while (GetComment()) { // skip comment
            bp = _ifs.tellg();
        }
        _ifs.seekg(bp);

        string s;  // gamma trace
        _ifs >> s;
        cout << s << endl;
        /*if (s == "substate") {
            char ch;
            cin >> ch;
        }*/

        _ifs.seekg(bp);

        if (GetColon()) {
            _token.id = COLON;
            return;
        }   
        _ifs.seekg(bp);
        if (GetComma()) {
            _token.id = COMMA;
            return;
        }
        _ifs.seekg(bp);
        if (GetState()) {
            _token.id = STATE;
            return;
        }
        _ifs.seekg(bp);
        if (GetSubstate()) {
            _token.id = SUBSTATE;
            return;
        }
        _ifs.seekg(bp);
        if (GetThread()) {
            _token.id = THREAD;
            return;
        }         
        _ifs.seekg(bp);
        if (GetTransition()) {
            _token.id = TRANSITION;
            return;
        }           
        _ifs.seekg(bp);
        if (GetFrom()) {
            _token.id = FROM;
            return;
        }           
        _ifs.seekg(bp);
        if (GetTo()) {
            _token.id = TO;
            return;
        }           
        _ifs.seekg(bp);
        if (GetCondition()) {
            _token.id = CONDITION;
            return;
        }   
        _ifs.seekg(bp);
        if (GetAction()) {
            _token.id = ACTION;
            return;
        }
        _ifs.seekg(bp);
        if (GetId()) {
            _token.id = ID;
            return;
        }  
        if (GetPriority()) {
            _token.id = PRIORITY;
            return;
        }
        _isDone = true;
	};
    bool IsDone() {
        return _isDone;
    };
	long Pos() {
		return _ifs.tellg();
	};
	void SetPos(long x) {
		_ifs.seekg(x);   // set read pointer to offset x from file start point
	};
    void Prev() { // back to position of previous token
        _ifs.seekg(bp);
        _token = _prevToken;
    };
};
 
struct StateManager {
    vector<string> state;
    vector<int> nextSiblingIdx;
    int Find(const string &aState) {
        for (unsigned int i = 0; i < state.size(); i++) {
            if (aState == state[i]) {
                return int(i);
            }
        }
        return -1;
    };
    void Add(char *aState) {
        state.push_back(string(aState));
    };
    void Create(LexialAnalyser *scanner) {
        int lastId = 0;
        for (scanner->First(); !scanner->IsDone(); scanner->Next()) {
            if (lastId == STATE && scanner->Token().id == ID) {
                if (Find(scanner->Token().value) == -1) {
                    Add(scanner->Token().value);
                }
            }
            lastId = scanner->Token().id;
        }
        nextSiblingIdx.resize(state.size());
        for (unsigned int i = 0; i < state.size(); i++) {
            nextSiblingIdx[i] = -1;
        }
    };
    void PrintAllState() {
        for (unsigned int i = 0; i < state.size(); i++) {
            if (state[i] != "") {
                cout << "(" << state[i] << ", " << nextSiblingIdx[i] << ")" << endl;
            }
        }
    };
};
StateManager sm;
/*
class ConditionManager {
public:
    list<string> _conds;
    void Add(string condition) {
        list<string>::iterator p;
        for (p = _conds.begin(); p != _conds.end(); p++) {
            if (*p == condition) {
                return;
            }
        }
        _conds.push_back(condition);
    };
};
ConditionManager cm;

class ActionManager {
public:
    list<string> _acts;
    void Add(string action) {
        list<string>::iterator p;
        for (p = _acts.begin(); p != _acts.end(); p++) {
            if (*p == action) {
                return;
            }
        }
        _acts.push_back(action);
    };
};
ActionManager am;
*/
class Code_Declare {
private:
    void OutputHead(ofstream &os) {
        os << "//---------------------------------------------------------------------------" << endl;
        os << endl;
        os << "#include <vcl.h>" << endl;
        os << "#pragma hdrstop" << endl;
        os << endl;
        os << "//---------------------------------------------------------------------------" << endl;
        os << endl;
        os << "#pragma argsused" << endl;
        os << endl;
        os << "#include <string>" << endl;
        os << "#include <stack>" << endl;
        os << "#include <iostream>" << endl;
        os << endl;
        os << "using namespace std;" << endl;
        os << endl;
    };
public:
    void OutputCode(ofstream &os) {
        unsigned int i;
        OutputHead(os);
        if (sm.state.size() > 0) {
            os << "enum {" << sm.state[0] << " = 1";
            for (i = 1; i < sm.state.size(); i++) {
                os << ", " << sm.state[i];
            }
            os << "};" << endl;

            os << "string stateName[] = {" << "\"\", \"" << sm.state[0] << "\"";
            for (i = 1; i < sm.state.size(); i++) {
                os << ", \"" << sm.state[i] << "\"";
            }
            os << "};" << endl;
        }
        os << "#define STATES_SIZE " << sm.state.size() << endl;
    };
};

class Code_EFSM {
private:
    class Code_CreateBinTree {
    public:
        vector<string> caseSt;
        Code_CreateBinTree() {
        };
        void OutputCode(ofstream& ofs) {
            ofs << "BinTree<int>* EFSM::CreateBinTree(int stateId, BinTree<int> *aParent) { // create BinTree for the default states" << endl;
            ofs << "\t" << "BinTree<int> *s, *t, *p;" << endl;
            ofs << "\t" << "s = new BinTree<int>;" << endl;
            ofs << "\t" << "s->data = stateId;" << endl;
            ofs << "\t" << "s->parent = aParent;" << endl;
            ofs << "\t" << "switch(stateId) {" << endl;

            unsigned int i;
            for (i = 0; i < caseSt.size(); i++) {
                if (caseSt[i] != "") {
                    ofs << "\t\t" << "case " << sm.state[i] << ":" << endl;
                    ofs << caseSt[i];
                }
            }

            ofs << "\t" << "}" << endl;
            ofs << "\t" << "return s;" << endl;
            ofs << "};" << endl;
        };
    };

    class Code_Run {
    public:
        vector<string> ACaseSt;
        vector<string> BCaseSt;
        void OutputCode(ofstream& ofs) {
            ofs << "void EFSM::Run() { // input event and change ActiveStates" << endl;
            ofs << "\t" << "// process action()" << endl;
            ofs << "\t" << "InorderIterator<int> it(_root);" << endl;
            ofs << "\t" << "it.First();" << endl;
            ofs << "\t" << "while (!it.IsDone()) {" << endl;
            ofs << "\t\t" << "BinTree<int> *curr = it.Current();" << endl;
            ofs << "\t\t" << "//cout << it.Current()->data << endl;//test for trace state" << endl;
            ofs << "\t\t" << "switch(curr->data) {" << endl;
            for (unsigned int i = 0; i < ACaseSt.size(); i++) {
                if (ACaseSt[i] != "") {
                    ofs << "\t\t\t" << "case " << sm.state[i] << ":" << endl;
                    ofs << ACaseSt[i];
                    ofs << "\t\t\t\t" << "break;" << endl;
                }
            }
            ofs << "\t\t" << "}" << endl;
            ofs << "\t\t" << "it.Next();" << endl;
            ofs << "\t" << "}" << endl;
            ofs << endl;

            ofs << "\t" << "// change to next states according event" << endl;
            ofs << "\t" << "PreorderIterator<int> it1(_root);" << endl;
            ofs << "\t" << "it1.First();" << endl;
	        ofs << "\t" << "while (!it1.IsDone()) {" << endl;
            ofs << "\t\t" << "bool changeState = false;" << endl;
            ofs << "\t\t" << "BinTree<int> *p;" << endl;
            ofs << "\t\t" << "BinTree<int> *curr = it1.Current();" << endl;
            ofs << "\t\t" << "//cout << stateName[curr->data] << endl;" << endl;
            ofs << "\t\t" << "switch(curr->data) {" << endl;
            for (unsigned int i = 0; i < BCaseSt.size(); i++) {
                if (BCaseSt[i] != "") {
                    ofs << "\t\t\t" << "case " << sm.state[i] << ":" << endl;
                    ofs << BCaseSt[i];
                    ofs << "\t\t\t\t" << "break;" << endl;
                }
            }
            ofs << "\t\t" << "} // end of switch" << endl;
            ofs << "\t\t" << "if (changeState) {" << endl;
            ofs << "\t\t\t" << "it1.SetCurrent(curr);" << endl;
            ofs << "\t\t\t" << "it1.direct = UP;" << endl;
            ofs << "\t\t" << "}" << endl;
            ofs << "\t\t" << "it1.Next();" << endl;
            ofs << "\t" << "}" << endl;
            ofs << "}" << endl;
        };
    };
public:
    Code_CreateBinTree cbt;
    Code_Run cr;

    Code_EFSM() {
    };
    void OutputCode(ofstream& ofs) {
        cbt.OutputCode(ofs);
        ofs << endl;
        cr.OutputCode(ofs);
    };
};

class Parser {
private:
	LexialAnalyser _scanner;
	ofstream _ofs;

    Code_Declare _cd;
    Code_EFSM _cefsm;

    bool codeGeneration;
    string stateName;
    string threadName;
    string substate[200];
    int size;
    string condition;
    string action;
    string nestedState[20];
    int nestedSize;

    void OutputTemplate(ifstream &ifs, ofstream &ofs) {
        while (!ifs.eof()) {
            char ch;
            ifs.get(ch);
            ofs.put(ch);
        }
    };
    bool StateName() {
        if (_scanner.Token().id == ID) {
            return true;
        }
        else {
            return false;
        }
    };
    bool NestedStateName() {
        long pos = _scanner.Pos();
        int i;
        if (codeGeneration) {
            nestedSize = 0;
        }
        for (i = 0; i < 20 && StateName(); i++) {
            if (codeGeneration) {
                nestedState[i] = _scanner.Token().value;
                nestedSize = i + 1;
            }
            pos = _scanner.Pos();
            _scanner.Next();
            if (_scanner.Token().id != COLON) {
                _scanner.Prev();
                return true;
            }
            else { 
                _scanner.Next();
            }
        }
        _scanner.SetPos(pos);
        return false;
    };
    bool ThreadName() {
        if (_scanner.Token().id != ID) {
            return false;
        }
        return true;
    };
    bool Condition() {
        if (_scanner.Token().id != CONDITION) {
            return false;
        }
        long pos = _scanner.Pos();
        condition = _scanner.Token().value;
        return true;
    };
    bool Action() {
        if (_scanner.Token().id != ACTION) {
            return false;
        }
        action = _scanner.Token().value;
        return true;
    };

    bool Thread() {
		if (_scanner.Token().id != THREAD) {
			return false;
        }
        long pos = _scanner.Pos();
		_scanner.Next();
        if (ThreadName()) {
			return true;
        }
        else {
            _scanner.SetPos(pos);
            return false;
        }
    };

    bool Substates() {
		if (_scanner.Token().id != SUBSTATE) {
			return false;
        }
        if (codeGeneration) {
            size = 0;
        }
        long pos = _scanner.Pos();
		_scanner.Next();
        int i = 0;
        while (StateName()) {
            if (codeGeneration) {
                substate[size] = _scanner.Token().value;
                size++;
            }
            i++;
            pos = _scanner.Pos();
		    _scanner.Next();
            if (_scanner.Token().id == COMMA) {
		        _scanner.Next();
                continue;
            }
            else {
                if (codeGeneration) {
                    substate[size] = _scanner.Token().value;
                    size++;
                }
                _scanner.SetPos(pos);
                return true;
            }
        }
        if (codeGeneration) {
            size = i;
        }
        if (i > 0) {
            return true;
        }
        else {
            _scanner.SetPos(pos);
            return false;
        }
    };

    bool Transition() {
		if (_scanner.Token().id != TRANSITION) {
			return false;
        }
        long pos = _scanner.Pos();
		_scanner.Next();
        int priority = -1;
        if (_scanner.Token().id == PRIORITY) {
            priority = atoi(_scanner.Token().value);
        }
        else { 
            _scanner.SetPos(pos);
        }    
		_scanner.Next();
		if (!Condition()) {
            _scanner.SetPos(pos);
			return false;
        }
		_scanner.Next();
		if (!Action()) {
            _scanner.SetPos(pos);
			return false;
        }       
		_scanner.Next();
		if (_scanner.Token().id != FROM) {
            _scanner.SetPos(pos);
			return false;
        }
		_scanner.Next();
		if (!NestedStateName()) {
            _scanner.SetPos(pos);
			return false;
        }

        // from nestedState

        // for codeGeneration
        string s;
        int i;
        char temp[20];
        string sI;
        int k;

        if (codeGeneration) {
            s = string("\t\t\t\t") + "if (" + condition;
            for (i = 1; i < nestedSize; i++) {
                itoa(i, temp, 10); // transfer i into char* decimally
                sI = temp;
                s = s + " && curr->CheckParent(" + sI + ", " + nestedState[nestedSize-1-i] + ")";
            }
            itoa(i, temp, 10); // transfer i into char* decimally
            sI = temp;
            s = s + " && curr->CheckParent(" + sI + ", " + stateName + ")) {" + '\n';
            k = sm.Find(nestedState[nestedSize - 1]);
            if (k > -1) {
                _cefsm.cr.ACaseSt[k] = _cefsm.cr.ACaseSt[k] + s + "\t\t\t\t\t" + action + ";\n" + "\t\t\t\t" + "}\n";
                for (i = 0; i < nestedSize; i++) {
                    s += string("\t\t\t\t\t") + "curr = curr->parent;\n";
                }
                s += string("\t\t\t\t\t") + "delete curr->left;\n";
            }
            else {
                cout << "cannot find state " << nestedState[nestedSize - 1] << endl;
                return false;
            }
        }

		_scanner.Next();
		if (_scanner.Token().id != TO) {
            _scanner.SetPos(pos);
			return false;
        }
		_scanner.Next();
		if (!NestedStateName()) {
            _scanner.SetPos(pos);
			return false;
        }

        // to nestedState  
        if (codeGeneration) {
            for (i = 0; i < nestedSize - 1; i++) {
                s += string("\t\t\t\t\t") + "curr->left = new BinTree<int>(" + nestedState[i] + ", curr);\n";
                s += string("\t\t\t\t\t") + "p = curr->left;\n";
            }
            if (nestedSize > 1) {
                s += string("\t\t\t\t\t") + "p->left = CreateBinTree(" + nestedState[i] + ", p);\n";
            }
            else {
                s += string("\t\t\t\t\t") + "curr->left = CreateBinTree(" + nestedState[i] + ", curr);\n";
            }
            s += string("\t\t\t\t\t") + "changeState = true;\n";
        	s += string("\t\t\t\t\t") + "break;\n";
            s += string("\t\t\t\t") + "}\n";
            _cefsm.cr.BCaseSt[k] += s;
        }
        return true;
    };

	bool State() {
		if (_scanner.Token().id != STATE) {
			return false;
        }
        long pos = _scanner.Pos();
		_scanner.Next();
		if (!StateName()) {
            _scanner.SetPos(pos);
			return false;
        }
        if (codeGeneration) {
            stateName = _scanner.Token().value;
        }

        int i; // for codeGeneration
        if (codeGeneration) {
            i = sm.Find(stateName);
            if (i == -1) {
                _scanner.SetPos(pos);
                return false;
            }
        }

        pos = _scanner.Pos();
		_scanner.Next();
        string prevThreadName; // for codeGeneration
		if (Thread()) {
            prevThreadName = string(_scanner.Token().value);
            if (codeGeneration) {
    			_cefsm.cbt.caseSt[i] = string("\t\t\t") + "s->left = CreateBinTree(" + prevThreadName + ", s);\n";
                _cefsm.cbt.caseSt[i] += string("\t\t\t") + "break;\n";
            }
            pos = _scanner.Pos();
    		_scanner.Next();
            while (Thread()) {
                threadName = string(_scanner.Token().value);
                i = sm.Find(prevThreadName);
                if (i > -1) {
                    int j = sm.Find(threadName);
                    if (j > -1) {
                        sm.nextSiblingIdx[i] = j;
                        prevThreadName = threadName;
                        pos = _scanner.Pos();
                        _scanner.Next();
                    }
                    else {
                        cout << "problem on find " << threadName << endl;
                        return false;
                    }
                }
                else {
                    cout << "problem on find " << prevThreadName << endl;
                    return false;
                }
            }
            _scanner.SetPos(pos);
		}
        else {
            int ii = 0;
			while (Substates()) {
                if (codeGeneration) {
    			    _cefsm.cbt.caseSt[i] = string("\t\t\t") + "s->left = CreateBinTree(" + substate[0] + ", s);\n";
                    if (sm.nextSiblingIdx[i] != -1) {
            			_cefsm.cbt.caseSt[i] += string("\t\t\t") + "s->right = CreateBinTree(" + sm.state[sm.nextSiblingIdx[i]] + ", s->parent);\n";
                    }
                }  
                pos = _scanner.Pos();
        		_scanner.Next();
                ii++;
            }
            if (ii > 0) {
                if (codeGeneration) {
                    _cefsm.cbt.caseSt[i] += string("\t\t\t") + "break;\n";
                }
	    		while (Transition()) {
                    pos = _scanner.Pos();
                    _scanner.Next();
                };
                _scanner.SetPos(pos);
            }
            else {
                _scanner.SetPos(pos);
            }
        }
        return true;
	};
public:
	Parser() {
    };
    ~Parser() {
    };
    bool Parsing() {
        _ofs.open("test.cpp");
        int i;
        sm.Create(&_scanner);
        _cd.OutputCode(_ofs);
        _cefsm.cbt.caseSt.resize(sm.state.size());
        _cefsm.cr.ACaseSt.resize(sm.state.size());
        _cefsm.cr.BCaseSt.resize(sm.state.size());
        codeGeneration = false;
        for (_scanner.First(); State(); _scanner.Next()) {
            int i = 0;
        };
        sm.PrintAllState();
        char ch;
        cin >> ch;

        //code generation
        ifstream ifs;
        ifs.open("BinTree_template.cpp");
        OutputTemplate(ifs, _ofs);
        ifs.close();
        ifs.open("Iterator_template.cpp");
        OutputTemplate(ifs, _ofs);
        ifs.close();
        ifs.open("EfsmHead_template.cpp");
        OutputTemplate(ifs, _ofs);
        ifs.close();
        codeGeneration = true;
        for (_scanner.First(); State(); _scanner.Next()) {
            int i = 0;
        };
        _cefsm.OutputCode(_ofs);
        return true;
    };
};

// transfer *.efsm to *.cpp
#pragma argsused
int main(int argc, char* argv[])
{
    char ch;
    //ifstream ifs("test.efsm");
    //ofstream ofs("test.cpp");
 	Parser parser;
    if (parser.Parsing()) {
        cout << "success" << endl;
    }
    else {
        cout << "fail" << endl;
    }
    cin >> ch;
    return 0;
}
//---------------------------------------------------------------------------
