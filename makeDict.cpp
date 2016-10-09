#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
void split(const string &s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
    	if (!item.empty())
        	elems.push_back(item);
    }
}


vector<string> split(const string &s, const char delim=' ') {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

char toUpper(char chr)
{
	int chrint = (int)chr;
	if (chrint <= 122 and chrint >=97)
	{
		return char(chrint-32);
	}
	return chr;
}

string toUpper(string ustring)
{
	
	for (int i=0; i<ustring.size(); i++)
	{
		ustring[i] = toUpper(ustring[i]);
	}
	return ustring;
}

void addDefinition(vector<vector<string>>& CMUelements, string word, string definition, vector<vector<string>>& definedWords)
{
	static int MatchedItems = 0;
	//cout << "Searching for: " << word << word.size() << endl;
	for (int i=0; i < CMUelements.size(); i++)
	{
		if (CMUelements[i][0] == word)
		{
			//cout << word << " Found" << endl;
			if (CMUelements[i][CMUelements[i].size()-2] != "#DEF")
			{
				CMUelements[i].push_back("#DEF");
				CMUelements[i].push_back(definition);
				MatchedItems += 1;
				definedWords.push_back(CMUelements[i]);
			}			
		}
	}
	//cout << "Matched items: " << MatchedItems << endl;
}

string generateLine(const vector<string> line)
{
	string returnLine = "";
	for (int i=0; i<line.size()-1; i++)
	{
		returnLine += line[i] + "+";
	}
	returnLine += line[line.size()-1];
	return returnLine; 
}

void printVector(const vector<string>& sV)
{
	for (unsigned int i=0; i < sV.size(); i++)
	{
		cout << i << ":" << sV[i] << ' ';
	}
	cout << endl;
}

int main(int argc, char const *argv[])
{
	
	ifstream cmudic("cmudict.txt");
	vector<string> cmudictVec;
	string currentLine;
	while (getline(cmudic, currentLine))
	{
		cmudictVec.push_back(currentLine);
	}
	cout << "cmudict loaded!" << endl;
	vector<vector<string>> CMUelements;
	for (int i=0; i<cmudictVec.size(); i++)
	{
		CMUelements.push_back(split(cmudictVec[i]));
	}
	cout << "cmudict fully split!" << endl;
	
	
	ifstream defin("dictdef.txt");
	string definLine;
	vector<string> definFile;
	while (getline(defin, definLine))
	{
		definFile.push_back(definLine);
	}
	cout << "definition file fully loaded!" << endl;
	//string currentLine;
	string currentWord;
	string currentDef;
	vector<vector<string>> definedWords;
	bool needDef = false;
	for (int i=0; i< definFile.size(); i++)
	{
		currentLine = definFile[i];
		if (currentLine == toUpper(currentLine))
		{
			if (currentLine.size() != 1)
			{
				needDef = true;
				//cout << "Word found at line " << i << " " << currentLine << endl;
				currentLine.pop_back();//remove last character
				currentWord = currentLine;
			}

		}
		if (needDef)
		{
			if(currentLine.find("Defn: ") != std::string::npos)
			{
				//cout << "Definition found at " << i << endl;
				//currentLine.pop_back();
				string toAdd = currentLine.substr(6);
				toAdd.pop_back();
				currentDef += toAdd;
				for (int j=i+1; j<definFile.size(); j++)
				{
					if (definFile[j].size() != 1)
					{
						definFile[j].pop_back();
						currentDef += definFile[j];
					}
					else
						break;
				}
				//cout << "Definition: " << currentDef << endl;
				needDef = false;
				addDefinition(CMUelements, currentWord, currentDef, definedWords);
				currentDef = "";

			}
		}
	}

	//cmudictVec.clear();
	cout << "Saving now" << endl;
	ofstream finalDict("finalDict.txt");
	for (int i=0; i< CMUelements.size(); i++)
	{
		finalDict << generateLine(CMUelements[i]) << "\n";
	}

	ofstream finalDictDef("finalDictwithDef.txt");
	for (int i=0; i< definedWords.size(); i++)
	{
		finalDictDef << generateLine(definedWords[i]) << "\n";
	}

	//printVector(cmudictVec);


	return 0;
}