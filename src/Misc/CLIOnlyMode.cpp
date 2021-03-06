#include <iostream>
#include "../../headers/word.h"
#include "../../headers/wordCC.h"
using namespace std;

void say(const string& sentence);
void speak(const string& wordToSay, const bool isCorrect);

void SpellingTest(wordCC& SpellingWords)
{
	SpellingWords.findSpellingWords();

	for (int i=0; i<10; ++i) //For 10 words
	{
		string wordToSpell = SpellingWords.getCurrentWord()->getWord();
		speak(wordToSpell,false);
		string userInput;
		cin >> userInput;
		if (userInput == "exit")
			exit(0);
		if (userInput != wordToSpell)
		{
			cout << "You spelt it: " << userInput << " correct spelling is " << wordToSpell << endl;
			SpellingWords.wordWrong(i,userInput);
		}
		else
		{
			cout << "Correct!" << endl;
		}

	}
}
