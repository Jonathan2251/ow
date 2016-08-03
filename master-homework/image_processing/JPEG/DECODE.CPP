#pragma warning(disable:4786)
#include<math.h>
#include<iostream>
#include<fstream>
#include<bitset>
#include<string>
#include<map>
#include<cstdio>

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

struct DC {
	int baseCodeLength;
	int length;
};

struct AC {
	int run;
	int category;
	int baseCodeLength;
	int length;
};

class HuffmanDecoder {
private:
	map<unsigned int,DC> dc;
	map<unsigned long,AC> ac;
public:
	HuffmanDecoder() {
		string str;
		string::reverse_iterator p;

		ifstream is1("JPEG_Default_DC_Code.txt"); // load JPEG default DC code table
		for (int i = 0; i < 12; i++) {
			unsigned int baseCode = 0;
			is1 >> str;
			for (p = str.rbegin(); p != str.rend(); p++) {
				baseCode >>= 1;
				if (*p == '1') baseCode += 0x8000;
			}
			is1 >> dc[baseCode].baseCodeLength;
			is1 >> dc[baseCode].length;
		}
		is1.close();
		ifstream is("JPEG_Default_AC_Code.txt"); // load JPEG default AC code table
		for (int j = 0; j < 11; j++) {
			unsigned long baseCode = 0;
			is >> str;
			for (p = str.rbegin(); p != str.rend(); p++) {
				baseCode >>= 1;
				if (*p == '1') baseCode += 0x80000000;
			}
			ac[baseCode].run = 0;
			ac[baseCode].category = j;
			is >> ac[baseCode].baseCodeLength;
			is >> ac[baseCode].length;
		}
		for (i = 1; i < 15; i++) {
			for (j = 1; j < 11; j++) {
				unsigned long baseCode = 0;
				is >> str;
				for (p = str.rbegin(); p != str.rend(); p++) {
					baseCode >>= 1;
					if (*p == '1') baseCode += 0x80000000;
				}
				ac[baseCode].run = i;
				ac[baseCode].category = j;
				is >> ac[baseCode].baseCodeLength;
				is >> ac[baseCode].length;
			}
		}
		for (j = 0; j < 11; j++) {
			unsigned long baseCode = 0;
			is >> str;
			for (p = str.rbegin(); p != str.rend(); p++) {
				baseCode >>= 1;
				if (*p == '1') baseCode += 0x80000000;
			}
			ac[baseCode].run = 15;
			ac[baseCode].category = j;
			is >> ac[baseCode].baseCodeLength;
			is >> ac[baseCode].length;
		}
		is.close();
	};
	inline bool getDC_code(unsigned int baseCode, int baseCodeLength, DC& aDc) { //判斷是否屬於DC term的huffman leading code
		map<unsigned int,DC>::iterator p = dc.find(baseCode);
		if (p != dc.end()) {
			if (baseCodeLength == dc[baseCode].baseCodeLength) {
				aDc.baseCodeLength = dc[baseCode].baseCodeLength;
				aDc.length = dc[baseCode].length;
				return true;
			}
			else return false;
		}
		else return false;
	};
	inline bool getAC_code(unsigned long baseCode, int baseCodeLength, AC& aAc) {//判斷是否屬於AC term的huffman leading code
		map<unsigned long,AC>::iterator p = ac.find(baseCode);
		if (p != ac.end()) {
			if (baseCodeLength == ac[baseCode].baseCodeLength) {
				aAc.run = ac[baseCode].run;
				aAc.category = ac[baseCode].category;
				aAc.baseCodeLength = ac[baseCode].baseCodeLength;
				aAc.length = ac[baseCode].length;
				return true;
			}
			else return false;
		}
		else {
			return false;
		}
	};
};

class IFBitStream { // 提供一個檔案IO的介面機制，一次輸入一個bit
private:
	char b8;
	int curr;
	ifstream *is;
public:
	IFBitStream(ifstream* aIs) : is(aIs), curr(0) { };
	inline void operator>>(bitset<1>& bit) {
		if (curr == 0)	is->read(&b8, 1);
		if (int(b8) < 0) bit[0] = 1;
		else bit[0] = 0;
		b8 <<= 1;
		curr++;
		if (curr > 7) {
			curr = 0;
		}
	};
	inline bool eof() {return is->eof()&&(curr == 0); };
};

class JPEGDecode {
private:
	Quantization z;
	int pDC; //previous DC term
	int v[8][8]; //從input檔中讀取huffman code轉成8*8的bit map的8 bits gray level的block影像
	int q[8][8]; //做dequantize()後的block值
	int d[8][8]; //做inverse DCT()轉換後的block值
	int n[8][8]; //執行完denormalize()後的block值
	char buffer[8][512];
	HuffmanDecoder huf;
	ZigZagIterator *zIter;
	IFBitStream *bs;
	inline int round(const double v) { //四捨五入
		if (v - floor(v) < 0.5)
			return floor(v);
		else
			return ceil(v);
	};
	inline void decodeVarLen(int length) { //將huffman coding的數值轉成2係補數的int型式
		bitset<1> b1;					   //eg. 11 => +3, 010 => -5
		if (length > 0) {
			_2DVector pos = zIter->current();
			*bs >> b1;
			if (b1[0]) {
				v[pos.x][pos.y]++;
				for (int i = 1; i < length; i++) {
					*bs >> b1;
					v[pos.x][pos.y] <<= 1;
					if (b1[0]) {
						v[pos.x][pos.y]++;
					}
				}
			}
			else {
				v[pos.x][pos.y]++;
				for (int i = 1; i < length; i++) {
					*bs >> b1;
					v[pos.x][pos.y] <<= 1;
					if (!b1[0]) {
						v[pos.x][pos.y]++;
					}
				}
				v[pos.x][pos.y] *= -1;
			}
		}
	};
	inline bool getImageBlock(int aI) { //從input檔中讀取huffman code轉成8*8的bit map的8 bits gray level的block
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				v[i][j] = 0;
			}
		}
		bitset<1> b1;
		DC dc;
		AC ac;
		zIter->first();
		if(!bs->eof()) {
			unsigned int i = 0;
			unsigned long l = 0;
			*bs >> b1;
			if (b1[0]) i += 0x8000;
			for (int k = 1; !huf.getDC_code(i,k,dc); k++) { //取得DC term的leading code,1 bit 2 bit ...去試,直到找到為止
				if (k > 16) { //DC term leading code長度 <= 16 bits,所以>16表示有error
					cout << "cannot getDC_code" << endl;
					throw error();
				}
				*bs >> b1;
				if (b1[0]) i += (0x8000 >> k);
			}
			decodeVarLen(dc.length - dc.baseCodeLength);
			_2DVector pos = zIter->current();
			v[pos.x][pos.y] += pDC; //加上前一個DC term
			for(zIter->next(); !bs->eof(); zIter->next()) {
				l = 0;
				*bs >> b1;
				if (b1[0]) l += 0x80000000;
				for (int k = 1; !huf.getAC_code(l,k,ac); k++) { //取得DC term的leading code,1 bit 2 bit ...去試,直到找到為止
					if (k > 26) { //AC term leading code長度 <= 26 bits,所以>26表示有error
						cout << "cannot getAC_code" << endl;
						throw error();
					}
					*bs >> b1;
					if (b1[0]) l += (0x80000000 >> k);
				}
				if (ac.run == 0 && ac.category == 0) return true; //EOB
				for (int i = 0; i < ac.run; i++) zIter->next();
				if ((ac.run == 15)&&(ac.category == 0)) {//連續16個0
					zIter->next();
				}
				else
					decodeVarLen(ac.length - ac.baseCodeLength);
			}
		}
		return false;
	};
	inline void dequantize() { //d[i][j]*z.v[i][j]後之值四捨五入
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				q[i][j] = round(v[i][j] * z.v[i][j]);
	};
	inline void IDCT() { //做 inverse DCT轉換
		double ru, rv;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				double sum = 0;
				for (int u = 0; u < 8; u++) {
					if (u == 0)
						ru = 1/sqrt(8);
					else
						ru = 1/sqrt(4);
					for (int v = 0; v < 8; v++) {
						if (v == 0)
							rv = 1/sqrt(8);
						else
							rv = 1/sqrt(4);
						sum += ru*rv*q[u][v]*cos((2*x+1)*u*PI/16)*cos((2*y+1)*v*PI/16);
					}
				}
				d[x][y] = round(sum);
			}
		}
	};
	inline void denormalize() { //每一灰階值加上128
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				n[i][j] = (int)d[i][j] + 128;
			}
		}
	};
	inline void outputToBuffer(int k) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				buffer[i][k*8+j] = char(n[i][j]);
			}
		}
	};
public:
	class error { };
	JPEGDecode(ifstream* aIs, ofstream& aOs) : pDC(0), zIter(new ZigZagIterator(8, 8)), bs(new IFBitStream(aIs)) {
		char ch;
		aIs->read(&ch, 1); //檔頭前2byte為水平方向分成幾個block,將此資訊存入colBlocks
		unsigned int colBlocks = unsigned int(unsigned char(ch));
		colBlocks <<= 8;
		aIs->read(&ch, 1);
		colBlocks += unsigned int(unsigned char(ch));
		bool success = true;
		for (int i = 0; !bs->eof(); i++) {
			for (int j = 0; j < colBlocks; j++) {
				success = getImageBlock(j); //從input檔中讀取huffman code轉成8*8的bit map的8 bits gray level的block
				if (!success) break;
				dequantize(); //d[i][j]/z.v[i][j]後之值四捨五入
				IDCT(); //做inverse DCT轉換
				denormalize(); //每一灰階值加上128
				outputToBuffer(j); //將解碼後的block輸出到buffer中
				pDC = v[0][0];
			}
			if (success) aOs.write(&buffer[0][0], 4096); //將解碼後的blocks輸出到檔上
			else break;
		}
		cout << "blocks = " << i * colBlocks << endl;//test
		delete bs; //輸出未滿8 bits的編碼
		delete zIter;
	};
};

void main(int argc,char *argv[]) {
	if (argc != 3) {
		cout << "decode source target" << endl;
		cout << "    source : 壓縮的影像檔檔名。" << endl;
		cout << "    target : 解壓縮後的影像檔檔名。" << endl;
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
	try {
		JPEGDecode jpeg(&is, os); //讀進未壓縮的source image，然後用JPEG壓縮方法壓縮
		is.close();
		os.close();
	}
	catch(JPEGDecode::error) {
		is.close();
		os.close();
	}
}