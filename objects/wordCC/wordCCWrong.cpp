#include <ctime>

#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

namespace SSG {
	extern hLog histLog;
}

using namespace std;

void wordCC::wordWrong(const int& wordPosition,const string& attempt)
//Only called during a spelling test
{
    spellingWrongWordCount++; //Increase the number of incorrect words this run
	//Badword check must go first as a goodWord would be made into a badword which would make both sections run
    if (goodBadPos[wordPosition] == true) //If a badWord
    {
        cout << "badWord wordWrong will be called" << endl;
        getWord(wordPosition)->wordWrong(attempt);  //Increase the MSL and badWord weight
        getWord(wordPosition)->determineScore(); //Regenerate the score but don't change any words positions
    }
    if (goodBadPos[wordPosition] == false) //If a goodWord
    {
        cout << "goodWord wordWrong will be called" << endl;
        goodWordWrong(wordPosition,attempt); //Call the function to move goodWord to badWords
    }
}

void wordCC::goodWordWrong(const int& wordPosition,const std::string& attempt)
{
    //Create new logEvent
    vector<string> eventData;
    word* eventWordp = getWord(wordPosition);
    eventData.push_back(eventWordp->getWord()); //Push back the word in question
    eventData.push_back(to_string(eventWordp->getwScore())); //Add its score
    eventData.push_back(attempt); //And the attempt at the word
    SSG::histLog.addEvent(eventData,time(0),"GoodWordWrong"); //Create the event with the current time stamp

    //Begin main function purpose
    cout << "The word position is " << wordPosition << endl;
    string badWordLine = getWord(wordPosition)->wordWrong(attempt); //Generate the badwordline for the word that was wrong, wordPosition is the wordCC abs index, getword converts to wordContainer abs int then asks for the word behind that abs.
    cout << badWordLine << endl;
    badWord* badWordToAdd = new badWord(getWord(wordPosition),badWordLine); //Create a badword from the word in memory and the generated badwordline
    cout << "bad word Created" << endl;

    int badWordLocation = badWords.addWord(badWordToAdd); //Add the badword to the badwordList
    int goodWordLocation = wordPos[wordPosition];

    cout << "Begining fixing abstraction" << endl;
    goodWords.deleteWord(goodWords.getABSIndex(wordPos[wordPosition])); //Remove the word from the goodWord list
    goodBadPos[wordPosition] = true; //Set the goodBadPos to say the word is bad
    wordPos[wordPosition] = badWordLocation; //Update the abstraction to point to the location in badWords

    fixwordPos(goodWordLocation); //Fix the abstraction
}
