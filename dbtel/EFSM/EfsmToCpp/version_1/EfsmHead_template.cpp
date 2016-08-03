
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
