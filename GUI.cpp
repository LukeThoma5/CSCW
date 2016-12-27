#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include "sha256.h" //External code, not mine

#include <gtkmm.h>

#include "masterSyllableList.h" //MSL declaration
#include "word.h"
#include "badWord.h"
#include "SSG.h"
#include "wordContainer.h"
#include "badwordContainer.h"
#include "wordCC.h"

#include "windowContainer.cpp"

using namespace std;

void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string makeUpperCase(const string& attempt);
string saltPassword(string password, string salt);
string makeSalt();
vector<string> readPasswordFile();

namespace SSG {
	std::time_t currentASComboTime = 0;
	bool AnalysisMovAvg = false;
}

static void SSG_SC_Button_Return_Clicked()
{
	if(SSG::winContainer.SpellingScreen)
		SSG::winContainer.SpellingScreen->hide(); //hide() will close the window but keep the program running
	SSG::SpellingWords.userEndSpellingTest(); //Create log event if appropriate
}

static void SSG_KS_Button_Return_Clicked()
{
	if(SSG::winContainer.KeyboardScreen)
		SSG::winContainer.KeyboardScreen->hide(); //hide() will close the window but keep the program running
}

static void SSG_AS_Button_Return_Clicked()
{
	if(SSG::winContainer.AnalysisScreen)
		SSG::winContainer.AnalysisScreen->hide(); //hide() will close the window but keep the program running
}

void SSG_MS_Button_Quit_Clicked()
{
	if (SSG::winContainer.MainScreen)
		SSG::winContainer.MainScreen->hide();
}

void SSG_MS_Button_Games_Clicked()
{
	//SSG::SpellingWords.generatewScore();
	//SSG::SpellingWords.findHardest();
	SSG::SpellingWords.findSpellingWords();
}

void definitionHelper(const string& widgetName)
{
	Gtk::TextView* pText_DefintionBox = nullptr;
    SSG::refBuilder->get_widget(widgetName,pText_DefintionBox);
    Glib::RefPtr<Gtk::TextBuffer> DefinitionBuffer =  pText_DefintionBox->get_buffer();
    DefinitionBuffer->set_text(SSG::SpellingWords.getCurrentWord()->getDefinition());
}

void SSG_SC_Button_Definition_Clicked()
{
  definitionHelper("SSG_SC_Text_DefinitionBox");
}

void SSG_KS_Button_Definition_Clicked()
{
	definitionHelper("SSG_KS_Text_DefinitionBox");
}

void SSG_SC_Button_Play_Clicked()
{
	string wordToSpell = SSG::SpellingWords.getCurrentWord()->getWord();
	speak(wordToSpell,false);
}

void SSG_AS_Button_Options_Clicked()
{
	SSG::winContainer.OptionsScreen->show();
}

void SSG_MS_Button_Spelling_Clicked()
{
	SSG::winContainer.SpellingScreen->show();
	SSG::SpellingWords.findSpellingWords();
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

void SSG_AS_Combo_changed()
{
	Gtk::TextView* pSSG_AS_Text_Status = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Text_Status",pSSG_AS_Text_Status);
	Glib::RefPtr<Gtk::TextBuffer> StatusBuffer =  pSSG_AS_Text_Status->get_buffer();
	Gtk::ComboBoxText* pSSG_AS_Combo_Timeframe = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe", pSSG_AS_Combo_Timeframe);
	string timeFrame = pSSG_AS_Combo_Timeframe->get_active_text();
	time_t startTime;
	if (timeFrame == "This session")
		startTime=SSG::sessionStartTime;
	else
		if (timeFrame == "Last week")
			startTime=(time(0)-604800);
		else
			startTime=0;
	SSG::currentASComboTime = startTime;
	StatusBuffer->set_text(SSG::histLog.getEventString(startTime));
	//SSG::histLog.graphIncorrectWords(startTime);
}

void SSG_MS_Button_Analysis_Clicked()
{
	SSG::winContainer.AnalysisScreen->show();
	SSG_AS_Combo_changed();

}

string seperateWord(const string& wordToSep)
{
	cout << wordToSep << endl;
	string retString;

	for (int i=0; i<wordToSep.size(); i++)
	{
		retString = retString + wordToSep[i] + ' ';
	}

	/*
	for (int i=0; i<syllables.size(); i++)
	{
		retString += syllables[i] + " ";
	}
	*/
	cout << retString << endl;
	return retString;
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

void SSG_SC_TextEntry_activate()
{
	//Get attempt from TextEntry
    Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();

	//Update textview to indicate success
	string correctSpelling = SSG::SpellingWords.getCurrentWord()->getWord();
	EntryBuffer->set_text("");
	Gtk::TextView* pText = nullptr;
	SSG::refBuilder->get_widget("SSG_SC_Text_CorrectSpelling", pText);
	Glib::RefPtr<Gtk::TextBuffer> TextBuffer = pText->get_buffer();
	TextBuffer->set_text(correctSpelling);
	keyboard_update_last_word(makeUpperCase(attempt), correctSpelling, "SSG_SC_Text_AttemptedSpelling");

	//Update word Logic
	SSG::SpellingWords.spellingAttempt(attempt);
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

void addTags(string textName)
{
	Gtk::TextView* testView = nullptr;
	SSG::refBuilder->get_widget(textName, testView);
	Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  testView->get_buffer();
	//WordListBuffer->insert_with_tag(WordListBuffer->begin(),"Test",refTagMatch);
	WordListBuffer->create_tag("redtag");
	WordListBuffer->create_tag("orangetag");
	WordListBuffer->create_tag("greentag");
	Glib::RefPtr<Gtk::TextTagTable> mytagtable = WordListBuffer->get_tag_table();
	Glib::RefPtr<Gtk::TextTag> myTag = mytagtable->lookup("redtag");
	myTag->property_background() = "red";
	myTag = mytagtable->lookup("orangetag");
	myTag->property_background() = "orange";
	myTag = mytagtable->lookup("greentag");
	myTag->property_background() = "green";
}

void SSG_ASGK_MovingAvg_Toggled()
{
	SSG::AnalysisMovAvg = !SSG::AnalysisMovAvg;
	cout << "State Toggled!" << SSG::AnalysisMovAvg << endl;
}

void SSG_ASG_MSL_Clicked()
{
	//MSL assumed sorted due to AS opening
	vector<string> syllables;
    vector<int> syllableWCount;

	syllables.reserve(SSG::MSL.size()); //Reduce mem allocs
	syllableWCount.reserve(SSG::MSL.size());

    for (int i=0; i<SSG::MSL.size(); i++)
    {
        syllables.push_back(SSG::MSL[i]);
        syllableWCount.push_back(SSG::MSL.getSyllableWCount(syllables.back()));
    }

    SSG::histLog.createBarGraph("SyllableData.txt","Syllable Wrong Counts", syllables, syllableWCount);
}

void SSG_ASG_IncorrectWords_Clicked()
{
	SSG::histLog.graphIncorrectWords(SSG::currentASComboTime);
}

void SSG_ASGK_Mistakes_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,2,"Mistakes per test (1 Week Moving Average)", "keyboardMistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,2,"Mistakes per test", "keyboardMistakes.csv");
}

void SSG_ASGK_Mistakes100_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,3,"Mistakes per 100 characters (1 Week Moving Average)","keyboard100MistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,3,"Mistakes per 100 characters","keyboard100Mistakes.csv");
}

void SSG_ASGK_WPM_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,5,"WPM (1 Week Moving Average)", "wordsPerMinuteAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,5,"WPM", "wordsPerMinute.csv");
}

void SSG_OP_Button_Password_Clicked()
{
	cout << "Showing" << endl;
	SSG::winContainer.PasswordReset->show();
}
void SSG_OP_Button_Clear_Data_Clicked()
{
	cout << "Showing" << endl;
	SSG::winContainer.ResetData->show();
}
void SSG_OP_Button_Close_Clicked()
{
	cout << "closing" << endl;
	SSG::winContainer.OptionsScreen->hide();
}

void SSG_PWR_Button_Accept_Clicked()
{
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt1",pEntry);
    string attempt1 = pEntry->get_text();
	SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt2",pEntry);
    string attempt2 = pEntry->get_text();

	if (attempt1 != attempt2)
	{
		Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Text fields do not match"); //Create a dialog with PasswordReset as the parent
		dialog.set_secondary_text("To set a password the fields must match!");
		dialog.run(); //Run the dialog, when ok'ed continue execution
	}
	else
	{

		if (attempt1.size() < 6)
		{
			Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Password must be at least 6 characters long!"); //Create a dialog with PasswordReset as the parent
			dialog.run(); //Run the dialog, when ok'ed continue execution
		}

		else
		{
			//cout << "Password: " << attempt1 << " hash: " << sha256(attempt1) << endl;
			string salt = makeSalt();
			string password = saltPassword(attempt1, salt);
			cout << password << endl;
			ofstream passFile("userPassword.hash", std::ofstream::out);
			passFile << salt << endl;
			passFile << password << endl;
			SSG::winContainer.PasswordReset->hide();
		}
	}
}

void SSG_RD_Button_Clear_Data_Confirm_Clicked()
{
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_RD_TextEntry_Password_Attempt",pEntry);
    string attempt = pEntry->get_text();

	vector<string> passVector = readPasswordFile();
	if (passVector.size() > 1)
	{
		string passwordAttempt = saltPassword(attempt,passVector[0]);
		if (passwordAttempt != passVector[1])
			cout << "PASSWORDS DO NOT MATCH" << endl;
		else
			SSG::histLog.clearLog();
	}
}
void SSG_RD_Button_Close_Clicked()
{
	SSG::winContainer.ResetData->close();
}

void connectSignals()
{
if(SSG::winContainer.SpellingScreen)
{
	//Get the GtkBuilder-instantiated Button, and connect a signal handler:
	Gtk::Button* pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_SC_Button_Return", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Return_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_Button_Return", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_KS_Button_Return_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Button_Return", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_AS_Button_Return_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Button_Options", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_AS_Button_Options_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Quit", pButton);
	if(pButton)
		{cout << "MainScreen made!" << endl; pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Quit_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Spelling", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Spelling_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Keyboard", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Keyboard_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Analysis", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Analysis_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Games", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Games_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_OP_Button_Password", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_OP_Button_Password_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_OP_Button_Clear_Data", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_OP_Button_Clear_Data_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_OP_Button_Close", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_OP_Button_Close_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_PWR_Button_Accept", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_PWR_Button_Accept_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_RD_Button_Clear_Data_Confirm", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_RD_Button_Clear_Data_Confirm_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_RD_Button_Close", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_RD_Button_Close_Clicked) );}


	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_SC_Button_Definition", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Definition_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_Button_Definition", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_KS_Button_Definition_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_SC_Button_Play", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Play_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_ASG_MSL", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_ASG_MSL_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_ASG_IncorrectWords", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_ASG_IncorrectWords_Clicked) );}

	Gtk::ToggleButton* pToggle = nullptr;
	SSG::refBuilder->get_widget("SSG_ASGK_MovingAvg_Toggle", pToggle);
	if (pToggle)
		{pToggle->signal_toggled().connect( sigc::ptr_fun(SSG_ASGK_MovingAvg_Toggled) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_ASGK_Mistakes", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_ASGK_Mistakes_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_ASGK_Mistakes100", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_ASGK_Mistakes100_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_ASGK_WPM", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_ASGK_WPM_Clicked) );}

	Gtk::Entry* pEntry = nullptr;
	SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
	if (pEntry)
		{pEntry->signal_activate().connect( sigc::ptr_fun(SSG_SC_TextEntry_activate) );}
		SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);

	pEntry = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
	if (pEntry)
		{pEntry->signal_changed().connect( sigc::ptr_fun(SSG_KS_TextEntry_insert) );}

	Gtk::ComboBoxText* pCombo = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe",pCombo);
	if (pCombo)
		{pCombo->signal_changed().connect( sigc::ptr_fun(SSG_AS_Combo_changed) );}
}

addTags("SSG_KS_Text_LastWord");
addTags("SSG_SC_Text_AttemptedSpelling");

}
