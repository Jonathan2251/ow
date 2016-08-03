#ifndef _MODEL_H
#define _MODEL_H

#include "station.h"

class Model {
private:
	char _name[100];
	StationArray _stations;
	char _desc[2000];
	int _descLen;
	
public:
	void Reset();
	Model();
	char* Name();
	char Description();
	StationArray* Stations();
	void SetName(char* name);
	void AddStations(StationArray* pStations);
	void AddDescription(char* desc);
	void OutputErrCodeList(FILE* out);
	void OutputStationList(FILE* out);
	void OutputCreateStation(FILE* out, char* leadingChar);
	void Print();
};

class ModelArray {
private:
	Model _model[10];
	int _size;
	
	int WhichModelDescriptionLengthIsLongest();
	
public:
	void Reset();
	void AddModel(Model* pModel);
	void OutputErrCodeList(FILE* out);
	void OutputStationList(FILE* out);
	void OutputCreateStation(FILE* out);
	void Print();
};

#endif	// _MODEL_H
