#pragma warning(disable:4786)
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include "NeuralNetwork.h"
//#include "Matrix.h"

using namespace std;

void main() {
	int noOfInputs = 30, noOfHidden = 15, noOfOutputs = 15, noOfPatterns = 15;
	double threshold = 0.05;
	
	ifstream is("classifyinp.txt");
	if (!is) {
		cout << "找不到該輸入檔案classifying.txt" << endl;
		return;
	}
	int noOfData;
	is >> noOfData;
	Matrix<double>* data = new Matrix<double>[noOfData];
	for (int i = 0; i < noOfData; i++) data[i].init(noOfInputs, 1);
	for (i = 0; i < noOfData; i++) {
		for (int j = 0; j < noOfInputs; j++) {
			is >> data[i][j][0];
		}
	}
	is.close();
	NeuralNetwork neural(noOfInputs, noOfHidden, noOfOutputs, noOfPatterns, threshold);
	for (i = 0; i < noOfData; i++) {
		cout << "	data " << i << " : " << data[i].transpose();
		int answer = neural.classify(data[i]);
		cout << "	y[2] = " << neural.getOutput().transpose();
		cout << "	判定為 pattern " << answer << endl;
	}
}