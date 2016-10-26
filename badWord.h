#ifndef badWord_h
#define badWord_h

class badWord : public word { //New class badWord inherits from word, keep datamembers public

protected:
	void constructorHelper(const string& badwordline)
	{
		//Assumes the word constructor has already been called
		vector<string> wordVector = split(badwordline);
		printVector(wordVector);

		if (wordVector[0] != wordC) //If trying to construct a badWord from the wrong word throw an error.
		{
			throw "WordSyncError";
		}

		//finish the badWord constructor
		cout << "Trying to convert " << wordVector[1] << " to float" << endl;
		weight = stof(wordVector[1]);

		for (unsigned int i=2; i<wordVector.size(); i++)
		{
			cout << "Trying to convert " << wordVector[i] << " to int" << endl;
			syllableWrongCount.push_back(stoi(wordVector[i]));
		}

		if (syllableWrongCount.size() != syllables.size())
		{
			cout << "Word mismatch found!" << endl;
			throw "WordSyllableCount";
		}

		//badWordCount++;
	}

public:

	vector<int> syllableWrongCount;
	float weight;

	badWord(const string& wordline, const string& badwordline) : word(wordline) //Call the word constructor to initialise default values
	{
		constructorHelper(badwordline);
	}

	badWord(const badWord& createFrom) : word(true)
	{
		//word(true);
		syllableWrongCount=createFrom.syllableWrongCount;
		weight=createFrom.weight ;
		wordC=createFrom.wordC;
		syllables=createFrom.syllables;
		definition=createFrom.definition;
		wordFlags=createFrom.wordFlags;
		syllablePositions=createFrom.syllablePositions;  
	}

	badWord(const word* createFrom, string badWordLine) : word(true)
	{
		wordC=createFrom->wordC;
		syllables=createFrom->syllables;
		definition=createFrom->definition;
		wordFlags=createFrom->wordFlags;
		syllablePositions=createFrom->syllablePositions;
		constructorHelper(badWordLine);
	}

	virtual void wordWrong(const string& attempt)
	{
		cout << "badWord wordWrong Called" << endl;
	}
};

#endif