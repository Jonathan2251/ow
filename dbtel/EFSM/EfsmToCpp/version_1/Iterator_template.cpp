
// 抽象化類別,提供一致性介面以去遊歷某一物件
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
class InorderIterator : public Iterator<BinTree<T>*> { //依LVR(左子樹、中、右子樹)travse binary tree
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
class PreorderIterator : public Iterator<BinTree<T>*> { //依VLR(中、左子樹、右子樹)travse binary tree
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
