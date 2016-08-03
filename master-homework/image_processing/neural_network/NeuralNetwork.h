#pragma warning(disable:4786)
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<math.h>
#include "Matrix.h"
//#include "test.h"

using namespace std;

#define LEARNING_RATE 1;

class NeuralNetwork {
private:
	int NO_OF_INPUTS, NO_OF_HIDDEN, NO_OF_OUTPUTS, NO_OF_PATTERNS;
	double THRESHOLD;
	Matrix<double> y[3];
	Matrix<double> net[3];
	Matrix<double> w[3];
	Matrix<double> errorOfDerivateW[3];
	Matrix<double> *desire;
	Matrix<double> head_z[3], z[3];
	void initWeight() {
		for (int i = 1; i <= 2; i++) {
			for (int j = 0; j < w[i].row(); j++) {
				for (int k = 0; k < w[i].col(); k++)
					w[i][j][k] = (double(rand())/RAND_MAX*2 - 1)/15;
			}
		}
	};
	void initWeight(ifstream& is) {
		for (int i = 1; i <= 2; i++) {
			for (int j = 0; j < w[i].row(); j++) {
				for (int k = 0; k < w[i].col(); k++)
					is >> w[i][j][k];
			}
		}
	};
	void inputPattern(Matrix<double>& pattern) {
		y[0] = pattern;
	};
	double f(double net) {
		static double temp;
		temp = 1.0/(1.0 + exp((-1)*(net)));
		return temp;
	};
	Matrix<double>& f(Matrix<double>& net) {
		static Matrix<double> temp;
		temp.init(net.row(), net.col());
		for (int i = 0; i < net.row(); i++)
			temp[i][0] = 1.0/(1.0 + exp((-1)*(net[i][0])));
		return temp;
	};
	double derivate_of_f(double net) {
		static double temp;
		temp = f(net) * (1 - f(net));
		return temp;
	};
	Matrix<double>& derivate_of_f(Matrix<double>& net) {
		static Matrix<double> temp;
		temp.init(net.row(), net.col());
		for (int i = 0; i < net.row(); i++)
			temp[i][0] = f(net[i][0]) * (1 - f(net[i][0]));
		return temp;
	};
	void forwardPropagation() {
		net[1] = w[1] * y[0];
		y[1] = f(net[1]);
		net[2] = w[2] * y[1];
		y[2] = f(net[2]);
	};
	void backwardPropagation(int k) {
		head_z[2] = desire[k] - y[2];
		for (int i = 0; i < head_z[2].row(); i++) 
			z[2][i][0] = head_z[2][i][0] * derivate_of_f(net[2][i][0]);
		head_z[1] = w[2].transpose() * z[2];
		for (i = 0; i < head_z[1].row(); i++) 
			z[1][i][0] = head_z[1][i][0] * derivate_of_f(net[1][i][0]);
		errorOfDerivateW[1] += z[1] * y[0].transpose(); //step4
		errorOfDerivateW[2] += z[2] * y[1].transpose();
	};
	void adjustWeights() {
		for (int i = 1; i <= 2; i++)
			w[i] += errorOfDerivateW[i] * LEARNING_RATE;
	};
public:
	NeuralNetwork(int noOfInputs, int noOfHidden, int noOfOutputs, int noOfPatterns, double threshold) {
		srand( (unsigned)time( NULL ) );
		NO_OF_INPUTS = noOfInputs; NO_OF_HIDDEN = noOfHidden;
		NO_OF_OUTPUTS = noOfOutputs; NO_OF_PATTERNS = noOfPatterns;
		THRESHOLD = threshold;
		y[0].init(NO_OF_INPUTS, 1);
		y[1].init(NO_OF_HIDDEN, 1);
		y[2].init(NO_OF_OUTPUTS, 1);
		net[1].init(NO_OF_HIDDEN, 1);
		net[2].init(NO_OF_OUTPUTS, 1);
		w[1].init(NO_OF_HIDDEN, NO_OF_INPUTS);
		w[2].init(NO_OF_OUTPUTS, NO_OF_HIDDEN);
		errorOfDerivateW[1].init(NO_OF_HIDDEN, NO_OF_INPUTS);
		errorOfDerivateW[2].init(NO_OF_OUTPUTS, NO_OF_HIDDEN);
		desire = new Matrix<double>[NO_OF_PATTERNS];
		for (int i = 0; i < NO_OF_PATTERNS; i++) {
			desire[i].init(NO_OF_OUTPUTS, 1);
			desire[i][i][0] = 1;
		}
		head_z[2].init(NO_OF_OUTPUTS, 1);
		head_z[1].init(NO_OF_HIDDEN, 1);
		z[2].init(NO_OF_OUTPUTS, 1);
		z[1].init(NO_OF_HIDDEN, 1);
		ifstream is("weights.dat");
		if (is) {
			initWeight(is);
			is.close();
		}
		else initWeight();
	};
	void learning(Matrix<double>* pattern, ostream& os) {
		int k = 0;
		Matrix<double> err(1, 1);
		err[0][0] = 90;
		os << "error = " << endl;
		while (err[0][0] > THRESHOLD) {
			err[0][0] = 0;
			for (int i = 0; i < NO_OF_PATTERNS; i++) {
				errorOfDerivateW[1].init(NO_OF_HIDDEN, NO_OF_INPUTS);
				errorOfDerivateW[2].init(NO_OF_OUTPUTS, NO_OF_HIDDEN);
				inputPattern(pattern[i]);
				forwardPropagation();
				backwardPropagation(i);
				err = err + ((desire[i] - y[2]).transpose() * (desire[i] - y[2]));
				adjustWeights();
			}
			err[0][0] = err[0][0] / 2.0;
			os << err[0][0] << " ";
			cout << err[0][0] << endl;
			k++;
			if (k > 4) {
				os << endl;
				k = 0;
			}
		}
		os << endl;
		if (err[0][0] <= THRESHOLD) {
			ofstream os("weights.dat"); //save weights
			os << w[1];
			os << endl;
			os << w[2];
			os.close();
			return;
		}
	};
	int classify(Matrix<double>& pattern) {
		inputPattern(pattern);
		forwardPropagation();
		int max = 0;
		for (int i = 1; i < NO_OF_OUTPUTS; i++) {
			if (y[2][i][0] > y[2][max][0]) max = i;
		}
		return max + 1;
	};
	Matrix<double>& getOutput() { return y[2]; };
};
