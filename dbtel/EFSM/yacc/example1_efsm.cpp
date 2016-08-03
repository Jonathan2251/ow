#include <string>
#include <stack>
#include <iostream>
#include <cstdio>

using namespace std;

enum {C = 1, G, A, B, H, D, F, E};
string stateName[] = {"", "C", "G", "A", "B", "H", "D", "F", "E"}; // for test print
#define STATES_SIZE 8
/*
extern int Event1, Event2, Event3;
extern int Action1();
extern int Action2();
extern int Action3();*/

int Event1, Event2, Event3;
int Action1() {
    cout << "Action1()" << endl;
    return 0;
};
int Action2() {
    cout << "Action2()" << endl;
    return 0;
};
int Action3() {
    cout << "Action3()" << endl;
    return 0;
};
// ©â¶H€ÆÃþ§O,Ž£šÑ€@­P©Ê€¶­±¥H¥h¹CŸú¬Y€@ª«¥ó
// abstract class, interface for searching step by step.
template <class T>
class Iterator
{
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T Current() const = 0;
protected:
    Iterator() { };
};

template <class T>
struct BinTree { //€G€žŸðžê®Æµ²ºc,°£€F°ò¥»žê®Æµ²ºcªºŸÞ§@¥~,ÁÙŽ£šÑŽX­Ó€£ŠP¹CŸú€G€žŸðªº€èŠ¡:1.level order
	T data;      //		2.inorder 3.postorder
	BinTree<T> *left, *right, *parent;
    BinTree() {
        left = 0;
        right = 0;
    };   
    BinTree(T aData, BinTree<T> *aParent) {
        left = 0;
        right = 0;
        data = aData;
        parent = aParent;
    };
    // check the ith parent of data is aData.
    bool CheckParent(int i, T aData) {
        BinTree<T> *p = this;
        while (i > 0 && p) {
            p = p->parent;
            i--;
        }
        if (i == 0 && p->data == aData) {
            return true;
        }
        else {
            return false;
        }
    };
    void Print(int x) { // just for trace/debug the program
        int y = x;
        if (this) {
            while (x > 0) {
                cout << " ";
                x--;
            }
            cout << stateName[data] << endl;
            if (left) {
                left->Print(y+1);
            }
            if (right) {
                right->Print(y);
            }
        }
    };
};

template <class T>
class InorderIterator : public Iterator<BinTree<T>*> { //šÌLVR(¥ª€lŸð¡B€€¡B¥k€lŸð)travse binary tree
private:
    BinTree<T> *_root, *_curr;
    stack<BinTree<T>*> _s;
public:
    InorderIterator(BinTree<T>* aTree) : _root(aTree) { };
    void First() {
        for (_curr = _root ; _curr; _curr = _curr->left) _s.push(_curr);
        if(!_s.empty()) {
            _curr = _s.top();
        }
        else {
            _curr = 0;
        }
    };
    void Next() {
        if (!_s.empty()) {
            _s.pop();
            _curr = _curr->right;
            for ( ; _curr; _curr = _curr->left) _s.push(_curr);
            if (!_s.empty()) {
                _curr = _s.top();
            }
            else {
                _curr = 0;
            }
        }
    };
    bool IsDone() const {
        return _s.empty();
    };
    BinTree<T>* Current() const {
        return _curr;
    };
};

template <class T>
class PreorderIterator : public Iterator<BinTree<T>*> { //šÌVLR(€€¡B¥ª€lŸð¡B¥k€lŸð)travse binary tree
private:
    #define DOWN 0
    #define UP   1
    BinTree<T> *_root, *_curr;
public:
    int direct;
    PreorderIterator(BinTree<T>* aTree) : _root(aTree) { };
    void First() {
        _curr = _root ;
        direct = DOWN;
        return;
    };
    void Next() {
        while (_curr) {
            if (direct == DOWN) {
                if (_curr->left) {
                    _curr = _curr->left;
                    return;
                }
                else if (_curr->right) {
                    _curr = _curr->right;
                    return;
                }
                else {
                    direct = UP;
                }
            }
            else { //direct == UP
                if (_curr->right) {
                    _curr = _curr->right;
                    direct = DOWN;
                    return;
                }
            }
            _curr = _curr->parent;
        }
        return;
    };
    bool IsDone() const {
        return _curr == 0;
    };
    BinTree<T>* Current() const {
        return _curr;
    };
    void SetCurrent(BinTree<T>* p) {
        _curr = p;
    };
};

class EFSM { // EFSM_E
private:
    BinTree<int> *_root;
 // create BinTree for the default states
    BinTree<int>* EFSM::CreateBinTree(int stateId, BinTree<int> *aParent);
public:
    EFSM();
    ~EFSM();
    void Run();
    void Print() { // print to screen
	    _root->Print(0);
    };
};

EFSM::EFSM() {
    _root = CreateBinTree(E, 0);
};
 
EFSM::~EFSM() {
    if (_root) {
        delete _root;
        _root = 0;
    }
};

BinTree<int>* EFSM::CreateBinTree(int stateId, BinTree<int> *aParent) { // create BinTree for the default states
    BinTree<int> *s;
    s = new BinTree<int>;
    s->data = stateId;
    s->parent = aParent;
    switch(stateId) {
        case E:
            s->left = CreateBinTree(C, s);
            break;
        case C:
            s->left = CreateBinTree(G, s);
            break;
        case G:
            s->left = CreateBinTree(A, s);
            s->right = CreateBinTree(H, s->parent);
            break;
        case H:
            s->left = CreateBinTree(D, s);
            break;
        case F:
            s->left = CreateBinTree(D, s);
            break;
    }
    return s;
};

void EFSM::Run() { // input event and change ActiveStates
    // process action()
    InorderIterator<int> it(_root);
    it.First();
	while (!it.IsDone()) {
        BinTree<int> *curr = it.Current();
        //cout << it.Current()->data << endl;//test for trace state
        switch(curr->data) {
        	case A:
		        if (Event2 && curr->CheckParent(1, G)) {
			        Action2();
    	    	}
                break;
        	case B:
		        if (Event1 && curr->CheckParent(1, G) && curr->CheckParent(2, C) && curr->CheckParent(3, E)) {
			        Action1();
    	    	}
	        	if (Event3 && curr->CheckParent(1, G)) {
		        	Action2();
	    	    }
                break;   
	        case D:
	    	    if (Event2 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
		    	    Action1();
        		}

		    // Two same Event3 on the same state D need to be different
	    	    if (Event3 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) { //curr->parent->stateId :meaning F include D
		    	    Action3();
        		}
	        	if (Event3 && curr->CheckParent(1, H)) {
		        	Action2();
    	    	}
                break;
        }
        it.Next();
    }

    // change to next states according event
    PreorderIterator<int> it1(_root);
    it1.First();
	while (!it1.IsDone()) {
        bool changeState = false;
        BinTree<int> *p;
        BinTree<int> *curr = it1.Current();
        cout << stateName[curr->data] << endl;
        switch(curr->data) {
        	case A:
		        if (Event2 && curr->CheckParent(1, G)) {
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = CreateBinTree(B, curr);
                    changeState = true;
    	    	    break;
    	    	} 
	    	    break;
        	case B:
		        if (Event1 && curr->CheckParent(1, G) && curr->CheckParent(2, C) && curr->CheckParent(3, E)) {
                    curr = curr->parent;
                    curr = curr->parent;
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = new BinTree<int>(F, curr);
                    p = curr->left;
					p->left = CreateBinTree(D, p);
                    changeState = true;
    	    	    break;
    	    	}
	        	if (Event3 && curr->CheckParent(1, G)) {
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = CreateBinTree(A, curr);
                    changeState = true;
    	    	    break;
	    	    }
	    	    break;  
	        case D:
	    	    if (Event2 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
                    curr = curr->parent;
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = CreateBinTree(B, curr);
                    changeState = true;
    	    	    break;
        		}

		    // Two same Event3 on the same state D need to be different
	    	    if (Event3 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
                    curr = curr->parent;
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = CreateBinTree(C, curr);
                    changeState = true;
    	    	    break;
        		}
	        	if (Event3 && curr->CheckParent(1, H)) {
                    curr = curr->parent;
                    delete curr->left;
                    curr->left = CreateBinTree(B, curr);
                    changeState = true;
    	    	    break;
    	    	}
	    	    break;
        } // end of switch
        if (changeState) {
            it1.SetCurrent(curr);
            it1.direct = UP;
        }
        it1.Next();
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
    efsm.Print();
    efsm.Run();
    efsm.Print();

    Event1 = 0;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.Print();

    Event1 = 1;
    Event2 = 0;
    Event3 = 1;
    efsm.Run();
    efsm.Print();

    char ch;
    cin >> ch;

    return 0;
}
//---------------------------------------------------------------------------
