#pragma warning(disable:4786)
#include<fstream>
#include<iostream>
#include<string>
#include<queue>
#include<stack>
#include<map>
#include "BinTree.h"

using namespace std;
using namespace binaryTree;

enum Expression_Type {Prefix, Infix, Postfix};

class Expression { //運算式類別：
private:
	BinTree<char> *exp; //指標：指向運算式（運算式以二元樹結構儲存）
	map<char, double> varValue; //存變數與其值，eg. (A,2) => A = 2
	string::iterator iter;
	BinTree<char>* createBinTree() {
		BinTree<char>* t = new BinTree<char>;
		if (isOperand(*iter)) {
			t->data = *iter;
			t->leftChild = null;
			t->rightChild = null;
		}
		else
			if (isOperator(*iter)) {
				t->data = *iter;
				iter++;
				t->leftChild = createBinTree();
				iter++;
				t->rightChild = createBinTree();
			}	
		return t;
	};
	BinTree<char>* fromPrefix(string& st) { //依prefix order的string造出二元樹
		string::iterator p;
		for (p = st.begin(); p != st.end(); p++) //若運算子不為+-*/，或運算元不為a..z,A..Z，則丟出symbolError()例外
			if (!isOperator(*p) && !isOperand(*p)) throw symbolError();
		if (operatorNo(st) != operandNo(st) - 1) throw expressionError(); //運算子數目必須比運算元少1，否則丟出expressionError()例外
		iter = st.begin();
		return createBinTree();
	};
public:
	class expressionError 
	{
	public:
		const char *what()
		{
			static char *msg = "expression error !";
			return msg;
		} 
	};
	class symbolError { };
	bool isOperator(char aCh) { //return是否為運算子
		if (aCh == '+' || aCh == '-' || aCh == '*' || aCh == '/')
			return true;
		else
			return false;
	};
	bool isOperand(char aCh) { //return是否為運算元
		if (isalpha(int(aCh)))
			return true;
		else
			return false;
	};
	int operatorNo(string& str) { //return運算子個數
		string::iterator p;
		int sum = 0;
		for (p = str.begin(); p != str.end(); p++)
			if (isOperator(*p)) sum++;
		return sum;
	};
	int operandNo(string& str) { //return運算元個數
		string::iterator p;
		int sum = 0;
		for (p = str.begin(); p != str.end(); p++)
			if (isOperand(*p)) sum++;
		return sum;
	};
	Expression(string& str, Expression_Type type) {
		if (type == Prefix) exp = fromPrefix(str);
	};
	BinTree<char>* getExpression() { return exp; };
	void getVariables() { //輸入運算式變數之值
		map<char, bool> traves;
		cout << "Please input the values for the variables:" << endl;
		BinTree<char>::InorderIterator i; //以inorder順序遊歷二元樹
		i = getExpression();
		for (i.first(); !i.isDone(); i.next()) {
			char ch = i.current()->data;
			if (isOperand(ch) && (!traves[ch])) {
				cout << "Please input the value for variable " << ch << ":";
				cin >> varValue[ch];
				traves[ch] = true;
			}
		}
	};
	double calculate() { //輸出運算結果
		stack<double> s;
		BinTree<char>::PostorderIterator i; //以inorder順序遊歷二元樹
		i = getExpression();
		for (i.first(); !i.isDone(); i.next()) {
			char tokens = i.current()->data;
			if (isOperand(tokens)) {
				s.push(varValue[tokens]);
			}
			else {
				double op2 = s.top();
				s.pop();
				double op1 = s.top();
				s.pop();
				switch(tokens) {
				case '+':
					s.push(op1 + op2);
					break;
				case '-':
					s.push(op1 - op2);
					break;
				case '*':
					s.push(op1 * op2);
					break;
				case '/':
					s.push(op1 / op2);
					break;
				}
			}
		}
		return s.top();
	};
};	


void main() {
	cout << "Prease input the file of prefix:"; //輸入檔名並由該檔案取得prefix運算式
	string str;
	cin >> str;
	ifstream is(&str[0]);
	if (!is) {
		cout << "找不到該輸入檔案" << endl;
		return;
	}
	is >> str;
	is.close();

	try {
		Expression e(str, Prefix); //依取得的prefix運算式字串造出運算式物件

		{ //以level orde順序輸出此二元樹之內容
			BinTree<char>::LevelOrderIterator i; //以level orde順序遊歷二元樹
			cout << "Level order to display the tree:";
			i = e.getExpression();
			int level = 0;
			for (i.first(); !i.isDone(); i.next()) {
				if (level != i.currentLevel()) { //輸出同一level的運算子及運算元
					level = i.currentLevel();
					cout << endl << "Level " << level << ":";
				}
				cout << i.current()->data;
			}
			cout << endl;
		}

		{//以inorde順序輸出此二元樹之內容
			BinTree<char>::InorderIterator i; //以inorder順序遊歷二元樹
			cout << "Infix exporession:";
			i = e.getExpression();
			for (i.first(); !i.isDone(); i.next()) { //以inorder順序輸出運算子及運算元
				cout << i.current()->data;
			}
			cout << endl;
		}

		{//以postorde順序輸出此二元樹之內容
			BinTree<char>::PostorderIterator i; //以postorder順序遊歷二元樹
			cout << "Postfix exporession:";
			i = e.getExpression();
			for (i.first(); !i.isDone(); i.next()) { //以postorder順序輸出運算子及運算元
				cout << i.current()->data;
			}
			cout << endl;
		}

		//讓使用者指定各變數之值後，以postorder的順序來計算此運算式的結果
		char again = 'Y'; 
		while (again == 'Y' || again == 'y') {
			e.getVariables(); //輸入運算式變數之值
			cout << "The evaluating result = " << e.calculate() << endl; //輸出運算結果
			while (1) {
				cout << "Do you want to try again? (y/n)";
				cin >> again;
				if (again != 'y' && again != 'Y' && again != 'n' && again != 'N') {
					cout << "輸入字元錯誤 ( 不是 Y,y 或 N,n )" << endl;
				}
				else break;
			}
		}
	}
	catch(Expression::expressionError) {
		cout << "輸入prefix運算式錯誤" << endl;
	}
	catch(Expression::symbolError) {
		cout << "輸入prefix運算式錯誤" << endl;
	}
	catch(overflow_error) {
		cout << "overflow" << endl;
	}
	catch(exception& e) {
		cout << e.what() << endl;
	}
	catch(...) {
		cout << "some exception throw out" << endl;
	}
}