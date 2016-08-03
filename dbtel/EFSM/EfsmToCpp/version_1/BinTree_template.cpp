
template <class T>
struct BinTree { //�G�����Ƶ��c,���F�򥻸�Ƶ��c���ާ@�~,�ٴ��ѴX�Ӥ��P�C���G���𪺤覡:1.level order
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
