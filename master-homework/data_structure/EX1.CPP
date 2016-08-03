#pragma warning(disable:4786)
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<utility>

using namespace std;

template <class T>
struct Code {
	T first;
	int second;
};

// abstract class, interface for searching step by step.  
template <class T>
class Iterator {
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual T::iterator Current() const = 0;
protected:
    Iterator() { }
};

// Pattern 為一串字元與失敗函數
// pair<字元,失敗函數>
template <class T>
class Pattern {
private:
    int *fail;
public:
    T* ch;

	int size() { return ch->size(); };
	int Fail(int aPos) { return fail[aPos]; };
    Pattern(T* aCh) : ch(aCh) { fail = new int[ch->size()]; };
	void setFailFun() { // 計算failure function
		int i, j;

	    int n = size();
		if (n > 0) {
			fail[0] = -1;
		    for (j = 1; j < n; j++) {
				i = fail[j - 1];
				while (((*ch)[j] != (*ch)[i + 1])&&(i >= 0))
					i = fail[i];
				if ((*ch)[j] == (*ch)[i + 1])
					fail[j] = i + 1;
				else
					fail[j] = -1;
			}
		}
	}
};

// PatternMatch 提供欲比對的本文與所要比對的字串之比對工作
template <class T1, class T2>
class PatternMatch {
public:
    T1* text; // 欲比對的本文
    Pattern<T2>* pat; // 所要比對的字串及失敗函數
    PatternMatch(T1* aText, Pattern<T2>* aPat) : text(aText), pat(aPat) { };

  // 從本文的begin位置到end位置,找到第一個Pattern發生之起始位置
    T1::iterator pMatch(T1::iterator begin, T1::iterator end) {
	    T1::iterator i;
		int j;

		for (i = begin, j = 0; ((i != end) && (j < pat->size())); ) {
			if (*i == (*pat->ch)[j]) {
				i++; j++;
			}
			else {
				if (j == 0)  i++;
				else  j = pat->Fail(j - 1) + 1;
			}
		}
		return ( (j == pat->size()) ? i - pat->size() : end );
	}
};

// PatternMatchIterator 從本文的第一個字到最後一個字找與Pattern相同之子字串
template <class T1, class T2>
class PatternMatchIterator : public Iterator<T1> {
private:
    T1::iterator current;
public:
	PatternMatch<T1,T2>* pM;
	PatternMatchIterator(PatternMatch<T1,T2>* aPM) : pM(aPM) { };
    void first() { // 找到第一次字串的位置
		current = pM->pMatch(pM->text->begin(), pM->text->end());
	}
    void next() { // 從上一次找到的出現的結束位置往後找
		if (pM->pat->size() < pM->text->end() - current)
			current = pM->pMatch(current + pM->pat->size(), pM->text->end());
		else
			current = pM->text->end();
	};
    T1::iterator Current() const {return current;}; // return 目前所找到的子字串的起始位置
    bool isDone() const {return current == pM->text->end();}; // return 是否本文已搜尋完畢
};

void main() {
	string text, str,newStr;
	int i, j, choice;

	cout << "Input choice : (1 basic, 2 extend) ";
	cin >> choice;
	if ((choice != 1)&&(choice != 2)) // 輸入錯誤的choice,中斷
		return;

	// 由鍵盤輸入欲搜尋之子字串(pattern)後,create pattern object : pat
	cout << "Please input the searched pattern:";
	cin >> str;
	if (choice == 2) { // 若選擇擴展功能(2 extend)則讀進新字串
        cout << "Please input the replace string:";
		cin >> newStr;
	}
	else { // 否則新字串設為全部'*'
		newStr.resize(str.size());
		for (i = 0; i < newStr.size(); i++) newStr[i] = '*';
	}

    Pattern<string> pat(&str);
	pat.setFailFun();

	// 將欲搜尋的文字內容(本文)從text.dat讀進list<char>中
	ifstream is("text.txt");
	if (!is) {
		cerr << "cannot open input file : text.txt " << endl;
		return;
	}
	is >> text;
	is.close();

	// create a patternMatch object : pM
	PatternMatch<string,string> pM(&text, &pat);

	// create a patternMatchIterator object : pMI
	PatternMatchIterator<string,string>  pMI(&pM);

	// <1>輸出所有字串出現的開始及結束的位置
	for (i = 0, pMI.first(); !pMI.isDone(); pMI.next(), i++) {
		j = (int)(pMI.Current() - pMI.pM->text->begin());
        cout << "(" << j << "," 
			 << j + (int)pMI.pM->pat->size() - 1 << ") ";
    }
	cout << '\n';
	// <4>若找不到此子字串顯示"找不到此子字串"
	if (i == 0)  cout << "找不到此子字串" << '\n';

	// <2>輸出原始被搜尋文字檔內容
	cout << *pMI.pM->text << '\n';

	// <3>將被搜尋文字檔內容中所有子字串以新字串蓋過,並輸出結果
	for (pMI.first(); !pMI.isDone(); pMI.next() ) {
		i = (int)(pMI.Current() - pMI.pM->text->begin() );
		pMI.pM->text->replace(i, pMI.pM->pat->size(), newStr);
    }
	cout << *pMI.pM->text << '\n';
}