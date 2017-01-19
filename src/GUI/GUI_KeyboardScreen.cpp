#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordCC.h"

#include "../../objects/windowContainer.cpp"

using namespace std;

string makeUpperCase(const string& attempt);
void definitionHelper(const string& widgetName);
void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

namespace SSG {
	//Objects initialised in main.cpp
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
}

static void SSG_KS_Button_Return_Clicked()
{
	if(SSG::winContainer.KeyboardScreen)
		SSG::winContainer.KeyboardScreen->hide(); //hide() will close the window but keep the program running
}

static void SSG_KS_Button_Definition_Clicked()
{
	definitionHelper("SSG_KS_Text_DefinitionBox");
}

void GUI_keyboard_Handler()
{
	if (SSG::SpellingWords.size() < 200) //If not enough words, close the screen
	{
		SSG::winContainer.KeyboardScreen->hide();
		cerr << "Not enough words to create the test, exiting" << endl;

		Gtk::MessageDialog dialog(*SSG::winContainer.MainScreen, "Not enough words!"); //Create a dialog with MainScreen as the parent, Main text of Not enough words
		dialog.set_secondary_text("You cannot use the keyboard mode with less than 200 words, only " + to_string(SSG::SpellingWords.size()) + " words have been loaded. Please change the loaded dictionary to access this mode.");
		dialog.run(); //Run the dialog, when ok'ed continue execution
	}
	else
	{
		SSG::SpellingWords.findKeyboardWords(); //Update the wordCC
		//Get the wordList and add the 200 words to it
		Gtk::TextView* pSSG_KS_WordList = nullptr;
		SSG::refBuilder->get_widget("SSG_KS_WordList",pSSG_KS_WordList);
		Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  pSSG_KS_WordList->get_buffer();
		WordListBuffer->set_text(SSG::SpellingWords.getKeyboardWords());
	}
}

static void SSG_MS_Button_Keyboard_Clicked()
{
	SSG::winContainer.KeyboardScreen->show();
	GUI_keyboard_Handler(); //Start a new game and update the screen
	Gtk::TextView* testView = nullptr; //Get the lastWord textView buffer
	SSG::refBuilder->get_widget("SSG_KS_Text_LastWord", testView);
	Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  testView->get_buffer();
	//Insert some dummy text to make the user aware of the TextView
	//Insert into this buffer, this text using a tag with this name
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

static void SSG_KS_TextEntry_insert()
{
	//Code run every time a change happens in the buffer
	//Get the current text
	string currentWordString = SSG::SpellingWords.getCurrentWord()->getWord();
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();

	if (SSG::SpellingWords.keyboardAttempt(attempt)) //If wordCC says to clear screen
	{
		keyboard_update_last_word(makeUpperCase(attempt), currentWordString); //Add last word to textView with correct colouring
		EntryBuffer->set_text(""); //Clear the entry buffer

		Gtk::TextView* KSTextView = nullptr;
		SSG::refBuilder->get_widget("SSG_KS_Text_CurrentWord", KSTextView); //Get the TextView widget
		Glib::RefPtr<Gtk::TextBuffer> TBuffer =  KSTextView->get_buffer(); //Set reference pointer to the textBuffer
		TBuffer->set_text(SSG::SpellingWords.getCurrentWord()->getWord()); //Set buffer to the new word

	}
}

void connectSignalsKeyboardScreen()
{
    if (SSG::winContainer.KeyboardScreen)
    {
        connectBasicSignalHandersButton(
			{"SSG_KS_Button_Return",
            "SSG_MS_Button_Keyboard",
            "SSG_KS_Button_Definition"},
			{sigc::ptr_fun(SSG_KS_Button_Return_Clicked),
            sigc::ptr_fun(SSG_MS_Button_Keyboard_Clicked),
            sigc::ptr_fun(SSG_KS_Button_Definition_Clicked)});

    	Gtk::Entry* pEntry = nullptr;
        SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
        if (pEntry)
            {pEntry->signal_changed().connect( sigc::ptr_fun(SSG_KS_TextEntry_insert) );}
    }
}
