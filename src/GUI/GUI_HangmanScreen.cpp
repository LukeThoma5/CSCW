#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <gtkmm.h>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

#include "../../objects/windowContainer.cpp"

using namespace std;

void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string makeUpperCase(const string& attempt);
string saltPassword(const string& password, const string& salt);
string makeSalt();
vector<string> readPasswordFile();
void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

namespace SSG {
	//Static variables have internal linkage, only accessible in this translation unit (.cpp file)
	static string HMwordToGuess;
	static string HMhiddenLine;
    static std::time_t hangmanStartTime;

	//Objects stored in main.cpp
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern hLog histLog;
}

static void SSG_HM_Return_Clicked()
{
	if (SSG::winContainer.HangmanScreen)
	{
		SSG::winContainer.HangmanScreen->hide();
		vector<string> eventData;
		eventData.push_back(to_string(time(0)-SSG::hangmanStartTime));//The amount of time in WRE
		eventData.push_back(to_string(SSG::SpellingWords.getCurrentPosition())); //The amount of words they played through
		SSG::histLog.addEvent(eventData,time(0),"HangmanComplete");
	}
}

static string createDash(const int& length)
{
	string retString;
	for (int i=0; i<length; i++)
		retString += '_';
	return retString;
}

static void SSG_MS_Button_Games_Clicked()
{
	SSG::winContainer.HangmanScreen->show();
	SSG::SpellingWords.findSpellingWords();
	SSG::HMwordToGuess = SSG::SpellingWords.getCurrentWord()->getWord();
	SSG::HMhiddenLine = createDash(SSG::HMwordToGuess.size());

	Gtk::TextView* HiddenText = nullptr;
	SSG::refBuilder->get_widget("SSG_HM_Text_Current_Guess", HiddenText);
	HiddenText->get_buffer()->set_text(SSG::HMhiddenLine);
	SSG::hangmanStartTime = time(0);
}

static bool revealHangmanWord(const string& wordToGuess, string& hiddenLine, const char attempt)
{
	bool wasMistake = true;

	for (int i=0; i<wordToGuess.size(); i++)
	{
		//cout << attempt << "," << wordToGuess[i];
		if (attempt == wordToGuess[i])
		{
			wasMistake = false;
			hiddenLine[i] = attempt;
		}
	}
	//cout << endl;
	return wasMistake;
}

void ensureCharUpperCase(char& toUpper)
{
	if (toUpper > 96)
		toUpper -= 32;
}

static void SSG_HM_TextEntry_activate()
{
	static int leftOverAttempts = 8; //Persistant value between function calls, initialised once.

	//Get the users attempt from the entry
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_HM_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();

	EntryBuffer->set_text(""); //Reset the text entry buffer to empty
	cout << attempt << endl;

	//Get the pointers to display outputs
	Gtk::TextView* Guesses = nullptr;
	SSG::refBuilder->get_widget("SSG_HM_Text_Guesses", Guesses);
	Gtk::TextView* HiddenText = nullptr;
	SSG::refBuilder->get_widget("SSG_HM_Text_Current_Guess", HiddenText);

	if (attempt.size() == 1) //If only a single character
	{
		ensureCharUpperCase(attempt[0]); //Remove need to have caps lock on when playing as word::wordC is capitalised

		if (revealHangmanWord(SSG::HMwordToGuess,SSG::HMhiddenLine,attempt[0])) //Reveal all instances of this letter, if none found enter {}
		{
			//TODO Add penalty code
			Guesses->get_buffer()->insert_at_cursor(attempt); //Add to incorrect guesses
			leftOverAttempts--; //Remove an attempt
		}
	}

	if (attempt == SSG::HMwordToGuess || SSG::HMwordToGuess == SSG::HMhiddenLine || leftOverAttempts==0) //If game over
	{
		SSG::SpellingWords.nextWord(); //Update the wordCC word
		//Reset the user interface
		SSG::HMwordToGuess = SSG::SpellingWords.getCurrentWord()->getWord();
		SSG::HMhiddenLine = createDash(SSG::HMwordToGuess.size());
		Guesses->get_buffer()->set_text("");
		leftOverAttempts = 8; //Reset the attempts
	}
	else
	{
		if (attempt.size() > 1) //If not a character eg not already been added to guess list
			Guesses->get_buffer()->insert_at_cursor('"' + attempt + '"'); //Add to guess list in quotes to differentiate between quesses
	}

	HiddenText->get_buffer()->set_text(SSG::HMhiddenLine); //Update UI to current state
}

void connectSignalsHangmanScreen()
{
    if (SSG::winContainer.HangmanScreen)
    {
        connectBasicSignalHandersButton(
			{"SSG_HM_Return",
            "SSG_MS_Button_Games"},
			{sigc::ptr_fun(SSG_HM_Return_Clicked),
	        sigc::ptr_fun(SSG_MS_Button_Games_Clicked)});

        Gtk::Entry* pEntry = nullptr;
        SSG::refBuilder->get_widget("SSG_HM_TextEntry",pEntry);
        if (pEntry)
            {pEntry->signal_activate().connect( sigc::ptr_fun(SSG_HM_TextEntry_activate) );}
    }
}
