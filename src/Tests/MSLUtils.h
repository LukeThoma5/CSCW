#ifndef MSLUTILS_H
#define MSLUTILS_H
void initMSL(const vector<string>& initialState, const vector<int>& syllableWCount, masterSyllablesList* MSL);
bool validateMSLState(masterSyllablesList* MSL, const vector<string>& syllablePos, const vector<int>& syllableValue);
void printMSL();
bool verifyMSLSingleInstance(const vector<string>& syllables, masterSyllablesList* MSL);
vector<string> mergeAllDataToOneVector(const vector<string>& initialState, const vector<vector<string>>& testData);
#endif
