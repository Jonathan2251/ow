#pragma warning(disable:4786)
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "NeuralNetwork.h"

using namespace std;

void main() {
	int noOfInputs = 30, noOfHidden = 15, noOfOutputs = 15, noOfPatterns = 15;
	double threshold = 0.05;
	
	ifstream is("learninp.txt");
	if (!is) {
		cout << "找不到該輸入檔案" << endl;
		return;
	}
	Matrix<double>* pattern = new Matrix<double>[noOfPatterns];
	for (int i = 0; i < noOfPatterns; i++) pattern[i].init(noOfInputs, 1);
	for (i = 0; i < noOfPatterns; i++) {
		for (int j = 0; j < noOfInputs; j++) {
			is >> pattern[i][j][0];
		}
	}
	cout << "record file : ";
	string rec;
	cin >> rec;
	ofstream os(&rec[0]);
	if (!os) {
		cout << "找不到該輸出檔案" << endl;
		return;
	}
	cout << "hidden nodes = ";
	cin >> noOfHidden;
	NeuralNetwork neural(noOfInputs, noOfHidden, noOfOutputs, noOfPatterns, threshold);

    char tmpbuf[128];
	_strtime( tmpbuf );
    os << "start time:" << tmpbuf << endl; //display start time for learning
	neural.learning(pattern, os);
	_strtime( tmpbuf );
    os << "end time:" << tmpbuf << endl; //display end time for learning
}