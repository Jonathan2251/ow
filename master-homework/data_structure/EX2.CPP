#define WALL '1'
#define PASS '0'

#include<fstream>
#include<iostream>
#include<string>
#include<stack>
#include<list>

using namespace std;

struct _2DVector { // 二維的平面座標向量
	int x, y;
	_2DVector() { };
	_2DVector(int aX, int aY) : x(aX), y(aY) { };
	_2DVector operator+(_2DVector aV)	{ 
		static _2DVector temp1;
		temp1.x = x + aV.x;
		temp1.y = y + aV.y; 
		return temp1;
	};
	_2DVector operator-(_2DVector aV)	{ 
		static _2DVector temp2;
		temp2.x = x - aV.x;
		temp2.y = y - aV.y; 
		return temp2;
	};
	void operator+=(_2DVector aV) { x += aV.x; y += aV.y; };
	bool operator==(_2DVector aV) {
		if ((x == aV.x)&&(y == aV.y)) return true;
		else return false;
	}; 
	friend void operator<<(ostream& aOs, _2DVector& aV) {
		aOs << "(" << aV.x << "," << aV.y << ")";
	};
};

const _2DVector UP(-1,0);
const _2DVector RIGHT(0,1);
const _2DVector DOWN(1,0);
const _2DVector LEFT(0,-1);
const _2DVector way[] = {UP,RIGHT,DOWN,LEFT};
typedef int Direction;
 
struct PosAndDir { // 座標點與方向
	_2DVector pos;
	Direction dir; // 配合way[]使用,例如：way[0] = UP
};

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

class Maze { //二維座標的迷宮
private:
	int row, col;
	char** v;
	_2DVector end;
public:
	class input_error { };
	Maze(istream aIs) {
		int i, j;

		aIs >> row;
		aIs >> col;
		v = new char*[row + 2];
		for (i = 0; i < row + 2; i++) {
			v[i] = new char[col + 2];
		}
		for (j = 0; j < col + 2; j++) v[0][j] = WALL;
		for (i = 1; i < row + 1; i++) {
			v[i][0] = WALL;
			v[i][col + 1] = WALL;
			for (j = 1; j < col + 1; j++) {
				aIs >> v[i][j];
				if ((v[i][j] != PASS)&&(v[i][j] != WALL)) throw input_error();
			}
		}
		for (j = 0; j < col + 2; j++) v[row + 1][j] = WALL;
		aIs >> end.x; // 取得迷宮終點位置
		aIs >> end.y;
	};
	int Row() { return row; };
	int Col() { return col; };
	bool pass(_2DVector* aPos) { return v[aPos->x][aPos->y] == PASS; }; //告知座標點為通路或障礙
	_2DVector End() { return end; };
};

class Mouse { //二維座標上的老鼠
private:
	_2DVector position; //老鼠目前所在位置
public:
	_2DVector pos() { return position; };
	void moveTo(_2DVector* aPos) { position = (*aPos); }; //將老鼠移至另一位置
};

class Path { //路徑
private:
	list<_2DVector> tr; //記錄所有走過的路徑
	stack<PosAndDir> ri; //記錄正確的路徑
	int row, col;
	bool **walk; //記錄各座標點上的位置是否走過
	friend class MazeIterator;
public:
	void clearStack() { while (!ri.empty()) ri.pop(); }; //清除stack
	void clearList() { tr.clear(); }; //清除list
	void clearWorked() { //清除walk[][]
		int i, j;
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)  walk[i][j] = false;
	};
	void clear() { clearStack(); clearList(); clearWorked(); }; //清除stack,list與walk[][]
	Path(Maze *aMaze) { //依迷宮大小建構walk[][],walk[][]設為false表未走過
		int i, j;

		tr.clear();
		clearStack();
		row = aMaze->Row() + 2;
		col = aMaze->Col() + 2;
		walk = new bool*[row];
		for (i = 0; i < row; i++) {
			walk[i] = new bool[col];
			for (j = 0; j < col; j++)  walk[i][j] = false;
		}
	};
	int Row() { return row; };
	int Col() { return col; };
	
	void push(_2DVector aPos) { //將走過的位置加入list,stack中，walk[pos.x][pos.y]設為true表走過
		PosAndDir p;
		_2DVector v;

		tr.insert(tr.end(), aPos);
		if (!ri.empty()) {
			v = aPos - ri.top().pos;
			for (int i = 0; i < 4; i++) {
				if (v == way[i]) {
					ri.top().dir = i;
					break;
				}
			}
		}
		p.pos = aPos;
		p.dir = 0;
		ri.push(p);
		walk[p.pos.x][p.pos.y] = true;
	};
	PosAndDir back() { //退回前一點
		static PosAndDir t1;
		ri.pop();
		if (!ri.empty()) {
			t1 = ri.top();
			t1.dir++;
			tr.insert(tr.end(), t1.pos);
		}
		else {
			t1.pos.x = -1;
			t1.pos.y = -1;
			t1.dir = -1;
		}
		return t1;
	};
	bool walked(_2DVector* aPos) { return walk[aPos->x][aPos->y]; };
	list<_2DVector>::iterator getTrace() { return tr.begin(); };
	void printTrace(ostream* aOs) {
	  // <1>輸出起點到終點所有走過的路徑(包含錯誤後走回頭的路徑)。
		int i;
		list<_2DVector>::iterator p;

		for (i = 1, p = tr.begin(); p != tr.end(); i++, p++) {
			*aOs << *p;
			if (i % 12 == 0) *aOs << '\n';
		}
		*aOs << '\n';
	};
	bool inRight(_2DVector& aPos) { //告知該座標點是否為正確路徑之一（亦即是否存在於stack內）
		stack<PosAndDir> s(ri);
		for (;!s.empty(); s.pop())
			if (s.top().pos == aPos)
				return true;
		return false;
	};		
};

class MazeIterator : public Iterator<int> { //遊歷迷宮
private:
	bool done; //是否已遊歷完迷宮
	int curr; //目前遊歷了幾條路徑
public:
	Maze* maze;
	Mouse* mouse;
	Path* path;
	MazeIterator(Maze* aMaze, Mouse* aMouse, Path* aPath) : maze(aMaze), mouse(aMouse), path(aPath) { curr = 0; done = false; };
    void first() { // 找出第一條路徑
		int i, j;
		PosAndDir p;
		Direction k;

		done = false;
		for (i = 0; i < path->Row(); i++)
			for (j = 0; j < path->Col(); j++)  path->walk[i][j] = false;
		_2DVector pos = mouse->pos();
		if (!maze->pass(&pos)) {
			done = true;
			return;
		}
		else {
			path->push(pos);
			p.pos = pos;
			p.dir = 0;
		}
		while (!path->ri.empty()) {
			for (k = p.dir; k < 4; k++) {
				pos = p.pos + way[k];
				if (pos == maze->End()) { //若為出口,則停止
					path->push(pos);
					mouse->moveTo(&pos);
					curr = 1;
					return;
				}
				if ((!path->walked(&pos))&&(maze->pass(&pos))) { //若pos未走過且為通的,則將mouse移至pos
					path->push(pos);
					mouse->moveTo(&pos);
					p.pos = pos;
					p.dir = 0;
					break;
				}
			}
			if (k > 3) { //若四個方向都是障礙或已走過,則退至前一個位置
				p = path->back();
				if (!path->ri.empty()) {
					mouse->moveTo(&p.pos);
				}
				else  break;
			}
		}
		curr = 0; // 找不到路徑
		done = true;
	};
    void next() { // 找出下一條路徑
		PosAndDir p;
		Direction k;
		_2DVector pos = path->ri.top().pos;
		path->walk[pos.x][pos.y] = false;
		p = path->back();
		path->tr.pop_back();
		path->tr.pop_back();
		while (!path->ri.empty()) {
			for (k = p.dir; k < 4; k++) {
				pos = p.pos + way[k];
				if (pos == maze->End()) { //若為出口,則停止
					path->push(pos);
					mouse->moveTo(&pos);
					curr++;
					return;
				}
				if ((!path->walked(&pos))&&(maze->pass(&pos))) { //若pos未走過且為通的,則將mouse移至pos
					path->push(pos);
					mouse->moveTo(&pos);
					p.pos = pos;
					p.dir = 0;
					break;
				}
			}
			if (k > 3) { //若四個方向都是障礙或已走過,則退至前一個位置
				p = path->back();
				if (!path->ri.empty()) {
					mouse->moveTo(&p.pos);
				}
				else  break;
			}
		}
		done = true;
	};
    bool isDone() const { return done; }; //是否已遊歷完迷宮
    int current() const { return curr; }; //目前遊歷了幾條路徑
};

//輸出結果<2>：輸出原始迷宮內容，其中障礙部份以"X"顯示，可走部份以"O"顯示。
void print2(ostream* aOs, Maze* aMaze) {
	_2DVector v;

	for (v.x = 1; v.x <= aMaze->Row(); v.x++) {
		for (v.y = 1; v.y <= aMaze->Col(); v.y++)
			if (aMaze->pass(&v))
				*aOs << 'O';
			else
				*aOs << 'X';
		*aOs << '\n';
	}
}

//輸出結果<3>：將迷宮中從起點到終點所有走過的錯誤路徑顯示為"*"，其中正確路徑
//            （不含錯誤後走回頭的路徑）顯示為"@"，輸出迷宮搜尋過的狀態。
void print3(ostream* aOs, MazeIterator* aMI) {
	_2DVector v;

	for (v.x = 1; v.x <= aMI->maze->Row(); v.x++) {
		for (v.y = 1; v.y <= aMI->maze->Col(); v.y++)
			if (aMI->path->inRight(v))
				*aOs << '@';
			else if (aMI->path->walked(&v))
				*aOs << '*';
			else if (aMI->maze->pass(&v))
				*aOs << 'O';
			else
				*aOs << 'X';
		*aOs << '\n';
	}
}

void main() {
	int choice;
	string file;
	_2DVector start;
	char again;

	ofstream os("ex2out.txt");
	try {
		cout << "1.基本功能;2.擴展功能:";
		os   << "1.基本功能;2.擴展功能:";
		cin >> choice;
		os << choice << "\n";
		if (choice != 1 && choice != 2)
			return;
		cout << "Please input the file of maze:"; // 將迷宮內容從檔上讀進maze中
		os   << "Please input the file of maze:"; 
		cin >> file;
		os << file << "\n";
		ifstream is(&file[0]);
		if (!is) {
			os << "找不到該輸入檔案\n";
			return;
		}
		Maze maze(is);
		is.close();
		Mouse mouse;
		Path path(&maze);
		MazeIterator mI(&maze, &mouse, &path);

		while (1) {
			cout << "Please input the starting position:\n"; // 輸入迷宮起點位置
			os   << "Please input the starting position:\n";
			cout << "Please input the row position:";
			cin >> start.x;
			os   << "Please input the row position:" << start.x << "\n";
			cout << "Please input the column position:";
			cin >> start.y;
			os << "Please input the column position:" << start.y << "\n";
			if (start.x > maze.Row() || start.y > maze.Col()) {
				os << "輸入起始位置超過迷宮範圍\n";
				continue;
			}
			mouse.moveTo(&start);
			path.clear();

			mI.first();
			// 輸出結果:
			if (!mI.isDone()) {
			// <1>輸出起點到終點所有走過的路徑(包含錯誤後走回頭的路徑)。
				os << "<1>輸出起點到終點所有走過的路徑(包含錯誤後走回頭的路徑)。\n";
				os << "第1條路徑:\n";
				path.printTrace(&os);
			// <2>輸出原始迷宮內容，其中障礙部份以"X"顯示，可走部份以"O"顯示。
				os << "<2>輸出原始迷宮內容，其中障礙部份以X顯示，可走部份以O顯示。\n";
				print2(&os, &maze);
				// <3>將迷宮中從起點到終點所有走過的錯誤路徑顯示為"*"，其中正確路徑
				//   （不含錯誤後走回頭的路徑）顯示為"@"，輸出迷宮搜尋過的狀態。
				os << "<3>將迷宮中從起點到終點所有走過的錯誤路徑顯示為*，其中正確路徑\n";
				os << "  （不含錯誤後走回頭的路徑）顯示為@，輸出迷宮搜尋過的狀態。\n";
				print3(&os, &mI);
				if (choice == 2) { // 擴展功能
					for (mI.next(); !mI.isDone(); mI.next()) {
					// <1>輸出起點到終點所有走過的路徑(包含錯誤後走回頭的路徑)。
						os << "第" << mI.current() << "條路徑:\n";
						path.printTrace(&os);
					}
				}
			}
			else {
			// <4>找不到路徑必須顯示訊息"Can't find a path!"，一樣要顯示
			//    <1><2><3>的輸出，但<3>的輸出不必有正確路徑。
				os << "Can't find a path!\n"; 
				os << "<2>輸出原始迷宮內容，其中障礙部份以X顯示，可走部份以O顯示。\n";
				print2(&os, &maze);
				os << "<3>將迷宮中從起點到終點所有走過的錯誤路徑顯示為*，其中正確路徑\n";
				os << "  （不含錯誤後走回頭的路徑）顯示為@，輸出迷宮搜尋過的狀態。\n";
				print3(&os, &mI);
			}

			while (1) {
				cout << "Do you want to try again? (y/n)";
				cin >> again;
				os   << "Do you want to try again? (y/n)" << again << "\n";
				if (again != 'y' && again != 'Y' && again != 'n' && again != 'N') {
					cout << "輸入字元錯誤 ( 不是 Y,y 或 N,n )\n";
					os   << "輸入字元錯誤 ( 不是 Y,y 或 N,n )\n";
				}
				else break;
			}
			if (again == 'N' || again == 'n')  break;
		}
	}
	catch (Maze::input_error) {
		cerr << "輸入檔迷宮的內容必須為0或1\n";
		return;
	}
	catch (bad_alloc) {
		cerr << "記憶體不足\n";
		return;
	}
	os.close();
}
