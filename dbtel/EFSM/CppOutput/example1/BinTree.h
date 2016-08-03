namespace binaryTree {
	const int null(0);
}

#include<queue>
#include<stack>

using namespace std;

// 抽象化類別,提供一致性介面以去遊歷某一物件
// abstract class, interface for searching step by step.  
template <class T>
class Iterator
{
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual T current() const = 0;
protected:
    Iterator() { };
};

template <class T>
struct BinTree { //二元樹資料結構,除了基本資料結構的操作外,還提供幾個不同遊歷二元樹的方式:1.level order
	T data;      //		2.inorder 3.postorder
	BinTree<T> *leftChild, *rightChild;
	class LevelOrderIterator : public Iterator<BinTree<T>*> { //依BFS(breadth first search)travse binary tree
	private:
		BinTree<T>* curr;
		queue<pair<BinTree<T>*,int> > q;
		int level;
	public:
		void operator=(BinTree<T>* aP) { curr = aP; level = 1; };
		void first() {
			pair<BinTree<T>*, int> t;
			t.first = curr;
			t.second = level;
			q.push(t);
		};
		void next() {
			pair<BinTree<T>*, int> t;
			if (!q.empty()) {
				q.pop();
				if (curr->leftChild) {
					t.first = curr->leftChild;
					t.second = level + 1;
					q.push(t);
				}
				if (curr->rightChild) {
					t.first = curr->rightChild;
					t.second = level + 1;
					q.push(t);
				}
				if (!q.empty()) {
					t = q.front();
					curr = t.first;
					level = t.second;
				}
				else {
					curr = binaryTree::null;
					level = 0;
				}
			}
		};
		bool isDone() const { return q.empty(); };
		BinTree<T>* current() const { return curr; };
		int currentLevel() const { return level; };
	};
	class InorderIterator : public Iterator<BinTree<T>*> { //依LVR(左子樹、中、右子樹)travse binary tree
	private:
		BinTree<T>* curr;
		stack<BinTree<T>*> s;
	public:
		void operator=(BinTree<T>* aP) { curr = aP; };
		void first() {
			for ( ; curr; curr = curr->leftChild) s.push(curr);
			if(!s.empty()) {
				curr = s.top();
			}
			else curr = binaryTree::null;
		};
		void next() {
			if (!s.empty()) {
				s.pop();
				curr = curr->rightChild;
				for ( ; curr; curr = curr->leftChild) s.push(curr);
				if (!s.empty()) {
					curr = s.top();
				}
				else curr = binaryTree::null;
			}
		};
		bool isDone() const { return s.empty(); };
		BinTree<T>* current() const { return curr; };
	};
	class PostorderIterator : public Iterator<BinTree<T>*> { //依LRV(左子樹、右子樹、中)travse binary tree
	private:
		BinTree<T>* curr;
		queue<BinTree<T>*> q;
		void postorder(BinTree<T>* curr) {
			if (curr) {
				postorder(curr->leftChild);
				postorder(curr->rightChild);
				q.push(curr);
			}
		};
	public:
		void operator=(BinTree<T>* aP) { curr = aP; };
		void first() { postorder(curr);	};
		void next() { if (!q.empty()) q.pop(); };
		bool isDone() const { return q.empty(); };
		BinTree<T>* current() const { return q.front(); };
	};
};
