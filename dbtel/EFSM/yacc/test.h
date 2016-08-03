#include <string>
#include <stack>
#include <iostream>

using namespace std;

enum {A = 1,
 B,
 D,
 G,
 H,
 C,
 F,
 E
};

string stateName[] = {"",
 "A",
 "B",
 "D",
 "G",
 "H",
 "C",
 "F",
 "E"
};

#define STATES_SIZE 8

extern int Event1, Event2, Event3;
extern int Action1();
extern int Action2();
extern int Action3();

template <class T>
struct BinTree { //trace binary tree by three path: 1.inorder 3.preorder
	T data;	
	BinTree<T> *child, *sibling, *parent;
	BinTree();
	BinTree(T aData, BinTree<T> *aParent);
	bool CheckParent(int i, T aData);
	void Print(int x);
};

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
class InorderIterator : public Iterator<BinTree<T>*> {
private:
	BinTree<T> *_root, *_curr;
	stack<BinTree<T>*> _s;
public:
	InorderIterator(BinTree<T>* t);
	void First();
	void Next();
	bool IsDone() const;
	BinTree<T>* Current() const;
};

template <class T>
class PreorderIterator : public Iterator<BinTree<T>*> {
private:
    #define DOWN 0
    #define UP   1
    BinTree<T> *_root, *_curr;
public:
    int direct;
	PreorderIterator(BinTree<T>* t);
	void First();
	void Next();
	bool IsDone() const;
	BinTree<T>* Current() const;
	void SetCurrent(BinTree<T>* p);
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
	void Print();
};

// BinTree begin ----------------------------------------------
template <class T>
BinTree<T>::BinTree() {
	child = 0;
	sibling = 0;
};  
 
template <class T>
BinTree<T>::BinTree(T aData, BinTree<T> *aParent) {
	child = 0;
	sibling = 0;
	data = aData;
	parent = aParent;
};

// check the ith parent of data is aData.
template <class T>
bool BinTree<T>::CheckParent(int i, T aData) {
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

template <class T>
void BinTree<T>::Print(int x) { // just for trace/debug the program
	int y = x;
	if (this) {
		while (x > 0) {
			cout << " ";
			x--;
		}
		cout << stateName[data] << endl;
		if (child) {
			child->Print(y+1);
		}
		if (sibling) {
			sibling->Print(y);
		}
	}
};

// InorderIterator begin ----------------------------------------------
template <class T>
InorderIterator<T>::InorderIterator(BinTree<T>* aTree) : _root(aTree) { };

template <class T>
void InorderIterator<T>::First() {
	for (_curr = _root ; _curr; _curr = _curr->child) _s.push(_curr);
	if(!_s.empty()) {
		_curr = _s.top();
	}
	else {
		_curr = 0;
	}
};

template <class T>
void InorderIterator<T>::Next() {
	if (!_s.empty()) {
		_s.pop();
		_curr = _curr->sibling;
		for ( ; _curr; _curr = _curr->child) _s.push(_curr);
		if (!_s.empty()) {
			_curr = _s.top();
		}
		else {
			_curr = 0;
		}
	}
};

template <class T>
bool InorderIterator<T>::IsDone() const {
	return _s.empty();
};

template <class T>
BinTree<T>* InorderIterator<T>::Current() const {
	return _curr;
};


// PreorderIterator begin ----------------------------------------------
template <class T>
PreorderIterator<T>::PreorderIterator(BinTree<T>* aTree) : _root(aTree) { };

template <class T>
void PreorderIterator<T>::First() {
	_curr = _root ;
	direct = DOWN;
	return;
};

template <class T>
void PreorderIterator<T>::Next() {
	while (_curr) {
		if (direct == DOWN) {
			if (_curr->child) {
				_curr = _curr->child;
				return;
			}
			else if (_curr->sibling) {
				_curr = _curr->sibling;
				return;
			}
			else {
				direct = UP;
			}
		}
		else { //direct == UP
			if (_curr->sibling) {
				_curr = _curr->sibling;
				direct = DOWN;
				return;
			}
		}
		_curr = _curr->parent;
	}
	return;
};

template <class T>
bool PreorderIterator<T>::IsDone() const {
	return _curr == 0;
};

template <class T>
BinTree<T>* PreorderIterator<T>::Current() const {
	return _curr;
};

template <class T>
void PreorderIterator<T>::SetCurrent(BinTree<T>* p) {
	_curr = p;
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

void EFSM::Print() { // print to screen
	_root->Print(0);
};

BinTree<int>* EFSM::CreateBinTree(int stateId, BinTree<int> *aParent) { // create BinTree for the default states
	BinTree<int> *s;
	s = new BinTree<int>;
	s->data = stateId;
	s->parent = aParent;
	switch(stateId) {
		case G:
			s->child = CreateBinTree(A, s);
			s->sibling = CreateBinTree(H, s->parent);
			break;
		case H:
			s->child = CreateBinTree(D, s);
			break;
		case C:
			s->child = CreateBinTree(G, s);
			break;
		case F:
			s->child = CreateBinTree(D, s);
			break;
		case E:
			s->child = CreateBinTree(C, s);
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
					Action3();
				}
				break;
			case B:
				if (Event3 && curr->CheckParent(1, G)) {
					Action2();
				}
				if (Event1 && curr->CheckParent(1, G) && curr->CheckParent(2, C) && curr->CheckParent(3, E)) {
					Action1();
				}
				if (Event2 && curr->CheckParent(1, E)) {
					Action1();
				}
				break;
			case D:
				if (Event3 && curr->CheckParent(1, H)) {
					Action2();
				}
				if (Event2 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
					Action1();
				}
				if (Event3 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
					Action3();
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
		//cout << stateName[curr->data] << endl;
		switch(curr->data) {
			case A:
				if (Event2 && curr->CheckParent(1, G)) {
					curr = curr->parent;
					delete curr->child;
					curr->child = CreateBinTree(B, curr);
					changeState = true;
					break;
				}
				break;
			case B:
				if (Event1 && curr->CheckParent(1, G) && curr->CheckParent(2, C) && curr->CheckParent(3, E)) {
					curr = curr->parent;
					curr = curr->parent;
					curr = curr->parent;
					delete curr->child;
					curr->child = new BinTree<int>(F, curr);
					p = curr->child;
					p->child = CreateBinTree(D, p);
					changeState = true;
					break;
				}
				if (Event2 && curr->CheckParent(1, E)) {
					curr = curr->parent;
					delete curr->child;
					curr->child = new BinTree<int>(F, curr);
					p = curr->child;
					p->child = CreateBinTree(D, p);
					changeState = true;
					break;
				}
				if (Event3 && curr->CheckParent(1, G)) {
					curr = curr->parent;
					delete curr->child;
					curr->child = CreateBinTree(A, curr);
					changeState = true;
					break;
				}
				break;
			case D:
				if (Event2 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
					curr = curr->parent;
					curr = curr->parent;
					delete curr->child;
					curr->child = CreateBinTree(B, curr);
					changeState = true;
					break;
				}
				if (Event3 && curr->CheckParent(1, F) && curr->CheckParent(2, E)) {
					curr = curr->parent;
					curr = curr->parent;
					delete curr->child;
					curr->child = CreateBinTree(C, curr);
					changeState = true;
					break;
				}
				if (Event3 && curr->CheckParent(1, H)) {
					curr = curr->parent;
					delete curr->child;
					curr->child = CreateBinTree(B, curr);
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
