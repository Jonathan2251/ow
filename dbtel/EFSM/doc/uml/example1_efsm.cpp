//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused

#include <string>
#include <list>
#include <iostream>

using namespace std;

enum {C = 1, G, A, B, H, D, F, E};
string stateName[] = {"", "C", "G", "A", "B", "H", "D", "F", "E"}; // for test print
#define STATES_SIZE 8
extern Event1, Event2, Event3;
extern Action1();
extern Action2();
extern Action3();

/*struct State {
	int id; //unique
	string name; //unique
    bool operator==(State& s) {
        if (id == s.id){
            return true;
        }
        else {
            return false;
        }
    };
};

class StateManager {
private:
	State _m[STATES_SIZE];
    int _currSize;
public:
    StateManager() : _currSize(0) {
    };
	int FindState(string& stateName) {
        for (int i = 0; i < _currSize; i++) {
            if (_m[i].name == stateName) {
                return i;
            }
        }
        return -1;
	};
	bool AddState(string stateName) {
        if (FindState(stateName) >= 0) {
            return false;
        }
        else {
            _m[_currSize].id = _currSize;
            _m[_currSize].name = stateName;
            _currSize++;
            return true;
        }
	};
};*/

class ActiveStateList {
private:
    struct Node {
        #define END 0
	    int stateId;
        Node* parent;
	    list<Node*> threads;  // threads : contains each subthread pointer
    	list<Node*>::iterator pt;  // pt : iterator for threads
	    Node() {
            threads.push_back(END);
		    pt = threads.begin();
    	}; 
        ~Node() { // release this node and all nodes origin from threads
            if (this) {
                list<Node*>::iterator i;
                for (i = this->threads.begin(); *i != END; i++) {
                    delete (*i);
                }
            }
        };
        void Reset() { // set pt to threads.begin(), first thread, and ask each node which the threads point to do that
            if (this) {
                list<Node*>::iterator i;
                for (i = this->threads.begin(); *i != END; i++) {
                    (*i)->Reset();
                }
                this->pt = this->threads.begin();
            }
        };
        void Print(int x) { // just for trace/debug the program
            int y = x;
            if (this) { 
                while (x > 0) {
                    cout << " ";
                    x--;
                }
                cout << stateName[this->stateId] << endl;
                list<Node*>::iterator i;
                for (i = this->threads.begin(); *i != END; i++) {
                    (*i)->Print(y+1);
                }
            }
        };
    };

	Node *_root, *_curr;

	void EnterSubDefaultState(int stateId) { // push the default states in s to list
        Node *p;
		Push(stateId);
	    switch(stateId) {
		    case E:
				EnterSubDefaultState(C);
				break;
    		case C:
                p = _curr;
				EnterSubDefaultState(G);
                _curr = p;
        //cout << "_curr->threads.size() = " << _curr->threads.size() << endl;  // gamma trace
        //cout << "push C " << (*_curr->pt)->stateId << endl;  // gamma trace
                _curr->pt++;
                p = _curr;
				EnterSubDefaultState(H);
                _curr = p;
        //cout << "_curr->threads.size() = " << _curr->threads.size() << endl;  // gamma trace
        //cout << "push C " << (*_curr->pt)->stateId << endl;  // gamma trace
                _curr->pt--;
				break;
	    	case G:
				EnterSubDefaultState(A);
				break;
    		case H:
				EnterSubDefaultState(D);
				break;
		    case F:
				EnterSubDefaultState(D);
				break;
		}
    };
public:
	ActiveStateList() : _root(0), _curr(0) {
    };
	Node* Root() {
		return _root;
	};
	Node* Current() {
		return _curr;
	};   
	void SetCurrent(Node* p) {
		_curr = p;
	};
    Node* Parent(int i) { // eg. when E->C->G->B and _curr = B, Parent(1) = G, Parent(2)->C, Parent(4) = 0
        Node *p = _curr;
        while (i > 0 && p) {
            p = p->parent;
            i--;
        }
        return p;
    };
    void Push(int id) { // attach new Node p to *(_curr->pt)
	    Node* p = new Node;
    	p->stateId = id;
        //cout << id << endl;//test
        p->parent = _curr;
        if (_curr) {
            if (*_curr->pt == END) {
                _curr->threads.insert(_curr->pt, p); // p insert before _curr->pt
                _curr->pt--;
            }
            else {
            	*(_curr->pt) = p;
            }
        }
        else {
            _root = p;
        }
        _curr = p;

    };
	void Up() { // set _curr to _curr->parent
        if (_curr != _root) {
    		_curr = _curr->parent;
        }
	};     
	void Cut() { // delete _curr and all subthreads deliver from it
        Node *p;
        if (_curr != _root) {
    		p = _curr->parent;
    		delete _curr;
            _curr = p;
        }
        else {
            if (_curr) {
    		    delete _curr;
                _root = _curr = 0;
            }
        }
	};
    void EnterState(int stateId) { // enter a new state, if the new state has default substate, push all default substate to _curr
        Node *p;
        if (_curr && *_curr->pt != END) {
            p = *(_curr->pt);
            delete p;
        }
        p =  _curr;
        EnterSubDefaultState(stateId);
        _curr = p;
    };
    bool SwitchToNextThread() { // change to the next thread
        if (*_curr->pt == END) {
            return false;
        }
        _curr->pt++;
	    if (*_curr->pt == END) {
            return false;
        }
        _curr = *_curr->pt;
        while (_curr->threads.front() != END) {
            _curr = _curr->threads.front();
        }
        return true;
    };
    void Begin() { // set each node after _root that pt = threads.begin(), and GoToTheMostLeftDown
	    _curr = _root;
        _curr->Reset(); // set each node after _root that pt = threads.begin()
	    while (_curr && _curr->threads.front() != END) {
            _curr = _curr->threads.front();
    	}
    };                 
    void Print() { // print to screen
	    _root->Print(0);
    };
};

class EFSM { // EFSM_E
private:
    ActiveStateList asl;
public:
    void EnterState(int id); // initial to enter state id, the asl reset to keep the list of default substates
    void Run();
    ActiveStateList *ActiveStates() {
        return &asl;
    };
};

void EFSM::EnterState(int id) {
    asl.SetCurrent(asl.Root());
    asl.Cut();
    asl.EnterState(id);
};

void EFSM::Run() {
    asl.Begin();
	while (asl.Current()->stateId != E) {
        //cout << asl.Current()->stateId << endl;//test for trace state
        switch(asl.Current()->stateId) {
	        case D:
	    	    if (Event2 && asl.Parent(1)->stateId == F && asl.Parent(2)->stateId == E) {
		    	    Action1();
        			asl.Up(); //escape D
	        		asl.Up(); //escape F
		        	asl.EnterState(B);
    	    	    break;
        		}

		    // Two same Event3 on the same state D need to be different
	    	    if (Event3 && asl.Parent(1)->stateId == F && asl.Parent(2)->stateId == E) { //asl.Parent(1)->stateId :meaning F include D
		    	    Action3();
        			asl.Up (); //escape D
	        		asl.Up (); //escape F
		        	asl.EnterState(C);
    	    	    break;
        		}
	        	if (Event3 && asl.Parent(1)->stateId == H) {
		        	Action2();
    			    asl.Up (); //escape D
    	    		asl.EnterState(B);
    	    	    break;
    	    	}
                asl.Up(); //escape D
	    	    break;
        	case A:
		        if (Event2 && asl.Parent(1)->stateId == G) {
			        Action2();
	        		asl.Up(); //escape A
    	    		asl.EnterState(B); // add all nested default substate to asl list.
    	    	    break;
    	    	}
        	case B:
		        if (Event1 && asl.Parent(1)->stateId == G && asl.Parent(2)->stateId == C && asl.Parent(3)->stateId == E) {
			        Action1();
		        	asl.Up(); //escape B
		        	asl.Up(); //escape G, to C
		        	asl.Cut(); //delete C
			        asl.Push(F);
    	    		asl.EnterState(D); // add all nested default substate to asl list.
    	    	    break;
    	    	}
	        	if (Event3 && asl.Parent(1)->stateId == G) {
		        	Action2();
    			    asl.Up (); //escape B
    	    		asl.EnterState(A);
    	    	    break;
	    	    }
                asl.Up(); //escape B
                break;
    	    case G:
	    	    asl.Up(); //escape G
    		    break;
        	case H:
	        	asl.Up(); //escape H
		        break;
        	case C:
                //cout << "threads size = " << asl.Current()->threads.size() << endl;//gamma trace
	        	if (asl.SwitchToNextThread()) { // turn to H
                    break;
                }
	        	asl.Up(); //escape C
                break;
        	default:
	        	asl.Up();
                break;
        }
    }
}

/*
Proof:
1.	Two same Event on the same state is different.
2.
*/

int main(int argc, char* argv[])
{
    EFSM efsm;

    Event1 = 0;
    Event2 = 1;
    Event3 = 1;
    efsm.EnterState(E);
    efsm.ActiveStates()->Print();
    efsm.Run();
    efsm.ActiveStates()->Print();

    Event1 = 1;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.ActiveStates()->Print();

    Event1 = 1;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.ActiveStates()->Print();

    char ch;
    cin >> ch;

    return 0;
}
//---------------------------------------------------------------------------
