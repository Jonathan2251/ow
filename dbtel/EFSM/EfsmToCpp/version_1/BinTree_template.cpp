
template <class T>
struct BinTree { //二元樹資料結構,除了基本資料結構的操作外,還提供幾個不同遊歷二元樹的方式:1.level order
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
