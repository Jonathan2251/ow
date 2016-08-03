#include<math.h>
#include<iostream>
#include<fstream>
#include<bitset>
#include<string>

#define PI 3.1416

enum CATEGORY_TYPE {DC_CATEGORY, AC_CATEGORY};

using namespace std;

struct _2DVector { // 二維的平面座標向量
	int x, y;
	_2DVector() { };
	_2DVector(const int aX, const int aY) : x(aX), y(aY) { };
	_2DVector operator+(const _2DVector aV) const { 
		static _2DVector temp1;
		temp1.x = x + aV.x;
		temp1.y = y + aV.y; 
		return temp1;
	};
	_2DVector operator-(const _2DVector aV) const { 
		static _2DVector temp2;
		temp2.x = x - aV.x;
		temp2.y = y - aV.y; 
		return temp2;
	};
	void operator+=(const _2DVector aV) { x += aV.x; y += aV.y; };
	bool operator==(const _2DVector aV) const {
		if ((x == aV.x)&&(y == aV.y)) return true;
		else return false;
	}; 
	friend void operator<<(ostream& aOs, _2DVector& aV) {
		aOs << "(" << aV.x << "," << aV.y << ")";
	};
};

// abstract class, interface for searching step by step.  
template <class T>
class Iterator {
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual T current() const = 0;
protected:
    Iterator() { };
};

enum Way {NE, SW};

class ZigZagIterator : public Iterator<_2DVector> { // 1  2  6  7
private:											// 3  5  8
	int row;										// 4  9 13
	int col;										//10 12
	_2DVector pos;									//11
	Way way;
	bool isdone;
public:
	ZigZagIterator(int aRow, int aCol) : row(aRow), col(aCol) {};
	void first() {
		pos.x = 0; pos.y = 0; way = NE; isdone = false;
	};
	void next() {
		if (pos.x == row - 1 && pos.y == col - 1) {
			isdone = true;
			return;
		}
		switch(way) {
			case NE :
				pos.x--;
				pos.y++;
				if (pos.x < 0 && pos.y > col - 1) {
					pos.x += 2;
					pos.y++;
					way = SW;
				}
				else if (pos.x < 0) {
					pos.x++;
					way = SW;
				}
				else if (pos.y > col - 1) {
					pos.x += 2;
					pos.y--;
					way = SW;
				}
				break;
			case SW :
				pos.x++;
				pos.y--;
				if (pos.x > row - 1 && pos.y < 0) {
					pos.x--;
					pos.y += 2;
					way = NE;
				}
				else if (pos.x > row - 1) {
					pos.x--;
					pos.y += 2;
					way = NE;
				}
				else if (pos.y < 0) {
					pos.y++;
					way = NE;
				}
				break;
		}
	};
	bool isDone() const { return isdone; };
    _2DVector current() const { return pos; };
};

struct Quantization { // quantization table
	int v[8][8];
	Quantization() { // 將quantization value從檔上讀進
    	ifstream is("quantized_table.txt");
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				is >> v[i][j];
	    is.close();
	};
};

struct DC_code {
	bitset<9> baseCode;
	int baseCodeLength;
	int length;
};

struct AC_code {
	bitset<26> baseCode;
	int baseCodeLength;
	int length;
};

class HuffmanTable {
private:
	DC_code dc[12];
	AC_code ac[16][11];
public:
	HuffmanTable() {
		ifstream is1("JPEG_Default_DC_Code.txt"); // load JPEG default DC code table
		for (int i = 0; i < 12; i++) {
			is1 >> dc[i].baseCode;
			is1 >> dc[i].baseCodeLength;
			is1 >> dc[i].length;
		}
		is1.close();
		ifstream is("JPEG_Default_AC_Code.txt"); // load JPEG default AC code table
		for (int j = 0; j < 11; j++) {
			is >> ac[0][j].baseCode;
			is >> ac[0][j].baseCodeLength;
			is >> ac[0][j].length;
		}
		for (i = 1; i < 15; i++) {
			for (j = 1; j < 11; j++) {
				is >> ac[i][j].baseCode;
				is >> ac[i][j].baseCodeLength;
				is >> ac[i][j].length;
			}
		}
		for (j = 0; j < 11; j++) {
			is >> ac[15][j].baseCode;
			is >> ac[15][j].baseCodeLength;
			is >> ac[15][j].length;
		}
		is.close();
	};
	int getCodingCategory(int aI, CATEGORY_TYPE aType) const {
		int result;
		if (aType != DC_CATEGORY && aType != AC_CATEGORY)
			return -1;
		int k = abs(aI);
		for (double i = 0; i < 12 && k >= pow(2.0, i); i++);
		result = i;
		if (aType == DC_CATEGORY && result < 12)
			return result;
		if (aType == AC_CATEGORY && result > 0 && result < 11)
			return result;
		return -1;
	};
	DC_code getDC_code(int aCategory) const { return dc[aCategory]; }; //取得DC term的huffman code
	AC_code getAC_code(int aRun, int aCategory) const { return ac[aRun][aCategory]; }; //取得AC term的huffman code
	void printDC() const {
		for (int i = 0; i < 12; i++) { //test
			cout << i;
			cout << " " << dc[i].baseCode;
			cout << " " << dc[i].baseCodeLength;
			cout << " " << dc[i].length << endl;
		}
	};
	void printAC() const { //test
		char ch;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 11; j++) {
				cout << i << " " << j;
				cout << " " << ac[i][j].baseCode;
				cout << " " << ac[i][j].baseCodeLength;
				cout << " " << ac[i][j].length << endl;
			}
			cin >> ch;
		}
	};
};

class FBitStream { // 提供一個檔案IO的介面機制，將bit匯集，直到滿一個byte才輸出到檔案
private:
	char b8;
	int curr;
	ofstream *os;
	void outputBuffer() {
		if (curr != 0) {
			b8 <<= (8 - curr);
			os->write(&b8, 1);
			curr = 0;
		}
	};
public:
	FBitStream(ofstream* outs) : os(outs), curr(0) { };
	inline void operator<<(bitset<1> bit) {
		b8 <<= 1;
		if (bit[0])
			b8 += 1;
		curr++;
		if (curr > 7) {
			os->write(&b8, 1);
			curr = 0;
		}
	};
	~FBitStream() { outputBuffer(); };
};

class JPEGEncode {
private:
	Quantization z;
	int pDC; //previous DC term
	char tmp[8][512]; //原始影像
	unsigned char v[8][8]; //存所要處理的8*8大小的block影像
	int n[8][8]; //執行完normalize()後的block值
	int d[8][8]; //做DCT()轉換後的block值
	int q[8][8]; //做quantize()後的block值
	HuffmanTable huf;
	ZigZagIterator *zIter;
	FBitStream *bs;
	void getImageBlock(int k) { //從input檔中讀取8*8的block
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				v[i][j] = unsigned char(tmp[i][k*8+j]);
			}
	};
	void normalize() { //每一灰階值減去128
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				n[i][j] = (int)v[i][j] - 128;
			}
		}
	};
	int round(const double v) { //四捨五入
		if (v - floor(v) < 0.5)
			return floor(v);
		else
			return ceil(v);
	};
	void DCT() { //做DCT轉換
		float ru, rv;
		for (int u = 0; u < 8; u++) {
			if (u == 0)
				ru = 1/sqrt(8);
			else
				ru = 1/sqrt(4);
			for (int v = 0; v < 8; v++) {
				float sum = 0;
				if (v == 0)
					rv = 1/sqrt(8);
				else
					rv = 1/sqrt(4);
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						sum += n[x][y]*cos(float(2*x+1)*u*PI/16)*cos(float(2*y+1)*v*PI/16);
					}
				}
				d[u][v] = round(ru*rv*sum);
			}
		}
	};
	void quantize() { //d[i][j]/z.v[i][j]後之值四捨五入
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				q[i][j] = round(double(d[i][j]) / double(z.v[i][j]));
	};
	template <class T> //依取得的huffman code編碼後輸出到檔上
	void outputBits(const int k, const T baseCode, const int baseCodeLength, const int length) {
		static bitset<1> b1;
		static bitset<26> b;
		for (int i = baseCodeLength - 1; i >= 0; i--) { //輸出編碼
			b1[0] = baseCode[i];
			*bs << b1;
		}
		b = abs(k); //將欲編碼的整數轉成二進位
		if (k < 0) b.flip(); //若為負數取1系補數
		for (i = length - baseCodeLength - 1; i >= 0; i--) {
			b1[0] = b[i];
			*bs << b1;
		}
	};
	void huffmanCoding() { //依huffman table進行編碼
		int previousZeros = 0;
		zIter->first();
		_2DVector pos = zIter->current(); //block中第一個pixel編碼
		int k = huf.getCodingCategory(q[pos.x][pos.y] - pDC, DC_CATEGORY); //減去前一個DC term後，依DC category編碼
		if (k < 0) {
			cout << "getCodingCategory error!" << endl;
			exit(1);
		}
		DC_code tdc = huf.getDC_code(k); //取得DC term的huffman code
		outputBits(q[pos.x][pos.y] - pDC, tdc.baseCode, tdc.baseCodeLength, tdc.length); //依取得的huffman code編碼後輸出到檔上
		for (zIter->next(); !zIter->isDone(); zIter->next()) { //依ZigZagIterator方向對block進行Huffman coding
			pos = zIter->current();
			if (q[pos.x][pos.y] == 0)
				previousZeros++;
			else {
				while (previousZeros > 15) { //連續16個0以上
					AC_code tac = huf.getAC_code(15, 0); //取得AC term的huffman code
					outputBits(0, tac.baseCode, tac.baseCodeLength, tac.length);
					previousZeros -= 16;
				}
				int k = huf.getCodingCategory(q[pos.x][pos.y], AC_CATEGORY);
				if (k < 0) {
					cout << "getCodingCategory error!" << endl;
					exit(1);
				}
				AC_code tac = huf.getAC_code(previousZeros, k);
				outputBits(q[pos.x][pos.y], tac.baseCode, tac.baseCodeLength, tac.length);
				previousZeros = 0;
			}
		}
		AC_code tac = huf.getAC_code(0, 0); //輸出EOB
		outputBits(0, tac.baseCode, tac.baseCodeLength, tac.length);
		previousZeros = 0;
	};
public:
	JPEGEncode(ifstream& aIs, ofstream* aOs) : pDC(0), zIter(new ZigZagIterator(8, 8)), bs(new FBitStream(aOs)) {
		char str[256];
		for (int i = 0; i < 3; i++) { //bypass 前三個line, *.pgm的格式(head of file)
			aIs.getline(str, 256, '\n');
		}
		int row, col;
		aIs >> row; //第四行為rows and columns of image
		aIs >> col;
		aIs.getline(str, 256, '\n'); //bypass end of line(第四行)
		aIs.getline(str, 256, '\n'); //bypass 第五個line
		unsigned int t = col / 8; //計算水平方向分成幾個block(t值),將其存入檔頭
		char ch = char(t >> 8);
		aOs->write(&ch, 1);
		ch = char(t);
		aOs->write(&ch, 1);
		int blocks = (row / 8) * (col / 8);
		if(!aIs.eof()) {
			aIs.read(&tmp[0][0], 8*col);
			for (i = 0; !aIs.eof(); i++) {
				for (int j = 0; j < col/8; j++) {
					getImageBlock(j); //從input檔中讀取8*8的block
					normalize(); //每一灰階值減去128
					DCT(); //做DCT轉換
					quantize(); //d[i][j]/z.v[i][j]後之值四捨五入
					huffmanCoding(); //依huffman table進行編碼
					pDC = q[0][0];
				}
				aIs.read(&tmp[0][0], 8*col);
			}
		}
		delete bs; //輸出未滿16 bits的編碼
		delete zIter;
	};
};

void main(int argc,char *argv[]) {
	if (argc != 3) {
		cout << "encode source target" << endl;
		cout << "    source : 欲壓縮的影像檔檔名。" << endl;
		cout << "    target : 壓縮後的影像檔檔名。" << endl;
		return;
	}
	ifstream is(argv[1],ios::binary);
	if (!is) {
		cerr << "cannot open input file " << argv[1] << endl;
		return;
	}
	ofstream os(argv[2],ios::binary);
	if (!os) {
		cerr << "cannot open output file " << argv[2] << endl;
		return;
	}
	JPEGEncode jpeg(is, &os); //讀進未壓縮的source image，然後用JPEG壓縮方法壓縮
	is.close();
	os.close();
}