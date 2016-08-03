#include "model.h"
#include "item.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char stationsDesc[2000];
extern int stationsDescLen;
extern StationArray stations;
extern ItemArray items;
extern char cVars[10000];

void Model::Reset() {
	_name[0] = '\0';
	_desc[0] = '\0';
	_descLen = 0;
	_stations.Reset();
}

Model::Model() {
	Reset();
}

char* Model::Name() {
//	static char name[100];
//	strcpy(name, _name);
	return _name;
}

StationArray* Model::Stations() {
	return &_stations;
}

void Model::SetName(char *name) {
	strcpy(_name, name);
}

void Model::AddStations(StationArray *pStations) {
	_stations = *pStations;
}

void Model::AddDescription(char* desc) {
	strcpy(&desc[_descLen+1], desc);
	_descLen += strlen(desc) + 1;
}

void Model::OutputErrCodeList(FILE* out) {
	if (strlen(_name) > 0) {
		fprintf(out, "MODEL = %s\n", _name);
	}
	_stations.OutputErrCodeList(out);
	fprintf(out, "\n\n");
}

void Model::OutputStationList(FILE* out) {
	if (strlen(_name) > 0) {
		fprintf(out, "MODEL = %s\n", _name);
	}
	_stations.OutputStationList(out);
	fprintf(out, "\n\n");
}

void Model::OutputCreateStation(FILE* out, char *leadingChar) {
	int i;
	char* p;
	
	stationsDesc[0] = '\0';
	stationsDescLen = 0;
	fprintf(out, "%s#ifdef CONFIG_DISPLAY_STATION_INFO\n", leadingChar);
	for (i = 0; i < MAX_NR_STATIONS; i++) {
		if (_stations.Exist(i)) {
			strcpy(stationsDesc, _stations.GetStation(i)->Description());
			fprintf(out, "%sstrcpy(stationsDesc, \"%s\");\n", leadingChar, _stations.GetStation(i)->Description());
			stationsDescLen = strlen(stationsDesc);
			fprintf(out, "%sstationsDescLen = strlen(stationsDesc);\n", leadingChar);
			p = stationsDesc + stationsDescLen + 1;
			fprintf(out, "%sp = stationsDesc + stationsDescLen + 1;\n", leadingChar);
			break;
		}
	}
	if (i >= MAX_NR_STATIONS) {
		return;
	}
	for ( i++; i < MAX_NR_STATIONS; i++) {
		if (_stations.Exist(i)) {
			strcpy(p, stations.GetStation(i)->Description());
			fprintf(out, "%sstrcpy(p, \"%s\");\n", leadingChar, stations.GetStation(i)->Description());
			p += strlen(stations.GetStation(i)->Description()) + 1;
			fprintf(out, "%sp += strlen(\"%s\") + 1;\n", leadingChar, stations.GetStation(i)->Description());
		}
	}
	
	fprintf(out, "%sp = stationsDesc;\n", leadingChar);
	stationsDescLen = p - stationsDesc;
	fprintf(out, "%s#endif // CONFIG_DISPLAY_STATION_INFO\n\n", leadingChar);
	_stations.OutputCreateStation(out, leadingChar);
}

// Print to console window, for debug & trace.
void Model::Print() {
	int i = 0;
	
	printf("model {\n");
	printf("name = %s\n", _name);
	_stations.Print();
	printf("}\n");
}

void ModelArray::Reset() {
	_size = 0;
}

void ModelArray::AddModel(Model* pModel) {
	if (_size < 10) {
		_model[_size] = *pModel;
		_size++;
	}
	else {
		printf("ERROR!!!   Model size > 10\n");
		exit(1);
	}
	//printf("\nModel._size = %d\n", _size);
}
	
void ModelArray::OutputErrCodeList(FILE* out) {
	int i;
	
	fprintf(out, "--------- ERROR CODE LIST ---------\n\n");
	if (_size > 0) {
		for (i = 0; i < _size; i++) {
			_model[i].OutputErrCodeList(out);
		}
	}
}

void ModelArray::OutputStationList(FILE* out) {
	int i;
	
	fprintf(out, "---------- STATION LIST ----------\n\n");
	if (_size > 0) {
		for (i = 0; i < _size; i++) {
			_model[i].OutputStationList(out);
		}
	}
	else {
		stations.OutputStationList(out);
	}
}

void ModelArray::OutputCreateStation(FILE* out) {
	int i, j;
	
	fprintf(out, "#include \"CreateStation.h\"\n");
	fprintf(out, "\nStation station[MAX_NR_STATIONS];\n\n");
	fprintf(out, "\n%s\n\n", cVars);
	
	fprintf(out, "#ifdef CONFIG_DISPLAY_STATION_INFO\n");
		
	fprintf(out, "Item gItem[256] = {\n");
		
	for (i = 0; i < 256; i++) {
		char sType[20];
		
		if (items[i]->type == TEST_ITEM_TYPE) {
			strcpy(sType, "TEST_ITEM_TYPE");
		}
		else if (items[i]->type == ACTION_ITEM_TYPE) {
			strcpy(sType, "ACTION_ITEM_TYPE");
		}
		else if (items[i]->type == CONTROL_ITEM_TYPE) {
			strcpy(sType, "CONTROL_ITEM_TYPE");
		}
		else {
			strcpy(sType, "NO_ITEM_TYPE");
		}
		fprintf(out, "{%d, %s, \"%s\"},\n", items[i]->id, sType, items[i]->name);
	}
	fprintf(out, "};\n\n");
	
	int maxLen = -1;
	int modelSelect = -1;
	
	if (_size > 0) {
		for (i = 0; i < _size; i++) {
			int descLen = 0;
			for (j = 0; j < MAX_NR_STATIONS; j++) {
				if (_model[i].Stations()->Exist(j)) {
					if (strlen(_model[i].Stations()->GetStation(j)->Description()) > 0) {
						descLen += strlen(_model[i].Stations()->GetStation(j)->Description());
					}
					descLen++;
				}
			}
			if (descLen > maxLen) {
				maxLen = descLen;
				modelSelect = i;
			}
		}
	}
	fprintf(out, "S8 stationsDesc[%d];\n", maxLen);
	fprintf(out, "int stationsDescLen;\n\n");
	
	fprintf(out, "#else\n");
	
	fprintf(out, "Item gItem[256] = {\n");
		
	for (i = 0; i < 256; i++) {
		char sType[20];
		
		if (items[i]->type == TEST_ITEM_TYPE) {
			strcpy(sType, "TEST_ITEM_TYPE");
		}
		else if (items[i]->type == ACTION_ITEM_TYPE) {
			strcpy(sType, "ACTION_ITEM_TYPE");
		}
		else if (items[i]->type == CONTROL_ITEM_TYPE) {
			strcpy(sType, "CONTROL_ITEM_TYPE");
		}
		else {
			strcpy(sType, "NO_ITEM_TYPE");
		}
		fprintf(out, "{%d, %s},\n", items[i]->id, sType);
	}
	fprintf(out, "};\n\n");
	
	fprintf(out, "#endif // CONFIG_DISPLAY_STATION_INFO\n\n");
	
	fprintf(out, "void StationsInit() {\n");
	
	fprintf(out, "\t#ifdef CONFIG_DISPLAY_STATION_INFO\n");
	fprintf(out, "\tS8* p = stationsDesc;\n");
	fprintf(out, "\t#endif // CONFIG_DISPLAY_STATION_INFO\n\n");
	
	if (_size > 0) {
		if (_size == 1 && strcmp(_model[0].Name(), "") == 0) {
			_model[0].OutputCreateStation(out, "\t");
		}
		else {
			fprintf(out, "\tif (IsProductModel(%s)) {\n", _model[0].Name());
			_model[0].OutputCreateStation(out, "\t\t");
			fprintf(out, "\t}\n");
			for (i = 1; i < _size; i++) {
				fprintf(out, "\telse if (IsProductModel(%s)) {\n", _model[i].Name());
				_model[i].OutputCreateStation(out, "\t\t");
				fprintf(out, "\t}\n");
			}
		}
	}
	fprintf(out, "}\n");
}

// Print to console window, for debug & trace.
void ModelArray::Print() {
	int i = 0;
	
	printf("models {\n");
	for (i = 0; i < _size; i++) {
		_model[i].Print();
	}
	printf("}\n");
}

Model model;
ModelArray models;
