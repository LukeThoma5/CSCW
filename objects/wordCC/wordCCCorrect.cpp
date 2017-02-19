#include <ctime>

#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

namespace SSG {
	extern hLog histLog;
}

using namespace std;

void wordCC::wordCorrect(const int& wordPosition)
{
	//If the word is stored in goodWords
    if (goodBadPos[wordPosition] == false)
    {
        cout << "goodWord wordCorrect will be called" << endl;
        goodWords.wordCorrect(wordPos[wordPosition]); //Make wordContainer call the words wordCorrect function and output useful debug info
        printTop(0,10); //Useful debug info for small tests
    }
	//If a badWord stored in badWords
    if (goodBadPos[wordPosition] == true)
    {
        cout << "Badword wordCorrect will be called" << endl;
		//Call special function as if the weight drops to 0, the badword needs to be be changed into a goodWord
        badwordCorrect(wordPosition);
    }
}

void wordCC::badwordCorrect(const int& wordPosition)
{
    //wordContainer addword fixes its abstraction and places the location of the new word at the end of the wordPos vector Regardless of score or location in wordList. Adding word is fine, update the goodbadpos and set the value to the end of the wordContainer.
    //wordContainer remove word all the indexes are updated to point to new locations and the words index is removed from the wordPos vector and wordlist, all wordPos indexes after the word point need to be shifted one to the right. The underlying indexes are correct but the wordPos of wordCC needs updating.

	//If the badWordContainer says the weight has dropped to 0
    bool needsFixing = badWords.wordCorrect(wordPos[wordPosition]);

    if (needsFixing)
    {
        //Create data for logEvent;
        vector<string> dataItems;
        badWord* bwordToFix = badWords.getBadWord(wordPos[wordPosition]);
        dataItems.push_back(bwordToFix->getWord()); //Add the word
        vector<int> bwordSWC = bwordToFix->getSyllableWrongCount();
        for (unsigned int i=0, end=bwordSWC.size(); i<end; ++i) //Add the syllableWrongCounts
            dataItems.push_back(to_string(bwordSWC[i]));
        SSG::histLog.addEvent(dataItems, time(0), "bwordToCorrect"); //Create the event

        //We need to move badword from badWords to a word in goodWords
		//Debug statements, toggleable via PreProcessor directive
		#ifdef WORDCCDEBUG
        cout << "Score has fallen to 0! need to delete!" << endl;
        printwordCC(); //Help with debug
        cout << "Word is bad and has value " << getWord(wordPosition)->getWord() << endl;
		#endif
		//Generate a new word from the badWord by slicing the object.
        word* turnedGoodWord = new word(getWord(wordPosition));
        cout << turnedGoodWord->getWord() << " has been created!" << endl;
        goodWords.addWord(turnedGoodWord); //Adds word to goodWords, has no major change to abs as it is added to the end.
        int goodWordSize = goodWords.size(); //cast to signed int
        cout << goodWords.at(goodWordSize-1)->getWord() << " at the end of goodWords" << endl; //Verify it is at the end

        badWords.deleteWord(badWords.getABSIndex(wordPos[wordPosition])); //Delete the word at the index
        goodBadPos[wordPosition] = false; //Minor update the wordCC index, set it to be a goodWord
        wordPos[wordPosition] = goodWordSize-1; //Set the position to be the end of the goodWordList

        cout << "Word is now good and has value " << getWord(wordPosition)->getWord() << endl;

        //Fix abstraction
        for (int i=0; i<goodBadPos.size(); ++i)
        {
            if (goodBadPos[i]==true) //if a bad word
                if (i>wordPosition) //if later in the list than the deleted word
                    wordPos[i]--; //Update the position
        }

        printwordCC();
    }
    else
    {
        cout << "word still got score > 0.0" << endl;
    }

}
