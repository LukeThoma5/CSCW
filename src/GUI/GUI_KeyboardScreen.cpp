#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include "../../headers/sha256.h" //External code, not mine

#include <gtkmm.h>

#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordContainer.h"
#include "../../headers/badwordContainer.h"
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
void definitionHelper(const string& widgetName);

namespace SSG {
	extern std::time_t currentASComboTime;
	extern bool AnalysisMovAvg;
	extern string HMwordToGuess;
	extern string HMhiddenLine;
	extern std::time_t hangmanStartTime;
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern std::time_t sessionStartTime;
	extern hLog histLog;
	extern masterSyllablesList* MSL; //MasterSyllablesList
}

static void SSG_KS_Button_Return_Clicked()
{
	if(SSG::winContainer.KeyboardScreen)
		SSG::winContainer.KeyboardScreen->hide(); //hide() will close the window but keep the program running
}

void SSG_KS_Button_Definition_Clicked()
{
	definitionHelper("SSG_KS_Text_DefinitionBox");
}

void GUI_keyboard_Handler()
{
	if (SSG::SpellingWords.size() < 200) //If not enough words, close the screen
	{
		SSG::winContainer.KeyboardScreen->hide();
		cout << "Not enough words to create the test, exiting" << endl;

		Gtk::MessageDialog dialog(*SSG::winContainer.MainScreen, "Not enough words!"); //Create a dialog with MainScreen as the parent, Main text of Not enough words
		dialog.set_secondary_text("You cannot use the keyboard mode with less than 200 words, only " + to_string(SSG::SpellingWords.size()) + " words have been loaded. Please change the loaded dictionary to access this mode.");
		dialog.run(); //Run the dialog, when ok'ed continue execution
	}
	else
	{
		SSG::SpellingWords.findKeyboardWords();
		Gtk::TextView* pSSG_KS_WordList = nullptr;
		SSG::refBuilder->get_widget("SSG_KS_WordList",pSSG_KS_WordList);
		Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  pSSG_KS_WordList->get_buffer();
		WordListBuffer->set_text(SSG::SpellingWords.getKeyboardWords());
	}
}

void SSG_MS_Button_Keyboard_Clicked()
{
	SSG::winContainer.KeyboardScreen->show();
	GUI_keyboard_Handler();
	//Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch = Gtk::TextBuffer::Tag::create();
	//refTagMatch->property_background() = "orange";
	Gtk::TextView* testView = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_Text_LastWord", testView);
	Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  testView->get_buffer();
	WordListBuffer->insert_with_tag(WordListBuffer->begin(),"red","redtag");
	WordListBuffer->insert_with_tag(WordListBuffer->begin(),"orange","orangetag");
	WordListBuffer->insert_with_tag(WordListBuffer->begin(),"green","greentag");
}

void keyboard_update_last_word(const string& attemptUpper, const string& wordString, const string& tViewName="SSG_KS_Text_LastWord")
{
	int end = wordString.size(); //initialise end to max size
	if (attemptUpper.size() < end); //If attempt smaller than max reduce end
		end = attemptUpper.size();

	Gtk::TextView* KSTextView = nullptr;
	SSG::refBuilder->get_widget(tViewName, KSTextView); //Get the TextView widget
	Glib::RefPtr<Gtk::TextBuffer> TBuffer =  KSTextView->get_buffer(); //Set reference pointer to the textBuffer
	TBuffer->set_text(""); //Remove previous text
	if (attemptUpper == wordString) //If all correct, no need to check individual letters, mark all green
	{
		TBuffer->insert_with_tag(TBuffer->begin(),wordString,"greentag");
	}
	else
	{
		string buffer=""; int bufferStart=0; bool wasLastCorrect=true; //variables for the loop, that need to be accessed outside loop
		for (int i=0; i<end; i++)
		{
			if (attemptUpper[i] != wordString[i]) //If letter incorrect
			{
				if (wasLastCorrect) //If the buffer is for correct letters
				{
					if (buffer != "") //If the buffer is not empty
						TBuffer->insert_with_tag(TBuffer->get_iter_at_offset(bufferStart),buffer,"greentag"); //Flush the buffer to screen with green text
					bufferStart=i; //Update the offset for the insert iterator
					buffer=attemptUpper[i]; //Clear the buffer and set first value to new incorrect value
					wasLastCorrect=false; //Mark buffer as incorrect
				}
				else //If buffer was previously wrong, add to buffer
					buffer+=attemptUpper[i];
				continue; //Soft break loop, don't execute next code
			}

			//letter must now be correct
			if (wasLastCorrect) //If buffer correct, add to buffer
				buffer+=attemptUpper[i];
			else
			{
				if (buffer != "") //If buffer not empty flush bad characters
					TBuffer->insert_with_tag(TBuffer->get_iter_at_offset(bufferStart),buffer,"redtag");
				bufferStart=i;
				buffer=attemptUpper[i];
				wasLastCorrect=true;
			}
		}

		if (buffer != "") //If still got buffer to flush, flush appropriately
		{
			if (wasLastCorrect)
				TBuffer->insert_with_tag(TBuffer->get_iter_at_offset(bufferStart),buffer,"greentag");
			else
				TBuffer->insert_with_tag(TBuffer->get_iter_at_offset(bufferStart),buffer,"redtag");
		}
	}
}

void SSG_KS_TextEntry_insert()
{
	string currentWordString = SSG::SpellingWords.getCurrentWord()->getWord();
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();
    cout << attempt << endl;

	if (SSG::SpellingWords.keyboardAttempt(attempt))
	{
		keyboard_update_last_word(makeUpperCase(attempt), currentWordString);
		EntryBuffer->set_text("");

		Gtk::TextView* KSTextView = nullptr;
		SSG::refBuilder->get_widget("SSG_KS_Text_CurrentWord", KSTextView); //Get the TextView widget
		Glib::RefPtr<Gtk::TextBuffer> TBuffer =  KSTextView->get_buffer(); //Set reference pointer to the textBuffer
		TBuffer->set_text(SSG::SpellingWords.getCurrentWord()->getWord()); //Set buffer to the new word

	}
	/*To create a tag Gtj::TextBuffer::create_tag(string) https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextBuffer.html#ad42f4e41a4cb2d5a824e2f0ffa78e973
	  use apply_tag to with the tagref, iterator start and iterator end https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextBuffer.html#ad42f4e41a4cb2d5a824e2f0ffa78e973 https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextTag.html*/
}

void connectSignalsKeyboardScreen()
{
    Gtk::Button* pButton = nullptr;
	Gtk::Entry* pEntry = nullptr;

    SSG::refBuilder->get_widget("SSG_KS_Button_Return", pButton);
    if(pButton)
        {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_KS_Button_Return_Clicked) );}

    pButton = nullptr;
    SSG::refBuilder->get_widget("SSG_MS_Button_Keyboard", pButton);
    if(pButton)
        {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Keyboard_Clicked) );}

    pButton = nullptr;
    SSG::refBuilder->get_widget("SSG_KS_Button_Definition", pButton);
    if(pButton)
        {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_KS_Button_Definition_Clicked) );}

    pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
    if (pEntry)
        {pEntry->signal_changed().connect( sigc::ptr_fun(SSG_KS_TextEntry_insert) );}
}
