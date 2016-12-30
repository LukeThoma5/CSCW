#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

#include "../headers/sha256.h" //External code, not mine

#include <gtkmm.h>

#include "../headers/masterSyllableList.h" //MSL declaration
#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordContainer.h"
#include "../headers/badwordContainer.h"
#include "../headers/wordCC.h"
#include "../headers/hLog.h"

#include "../objects/windowContainer.cpp"

#include "./GUI/headers/SpellingScreen.h"
#include "./GUI/headers/KeyboardScreen.h"
#include "./GUI/headers/HangmanScreen.h"

using namespace std;

void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string makeUpperCase(const string& attempt);
string saltPassword(const string& password, const string& salt);
string makeSalt();
vector<string> readPasswordFile();

namespace SSG {
	std::time_t currentASComboTime = 0;
	bool AnalysisMovAvg = false;
	string HMwordToGuess;
	string HMhiddenLine;
	std::time_t hangmanStartTime;
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern std::time_t sessionStartTime;
	extern hLog histLog;
	extern masterSyllablesList* MSL; //MasterSyllablesList
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

void definitionHelper(const string& widgetName)
{
	Gtk::TextView* pText_DefintionBox = nullptr;
    SSG::refBuilder->get_widget(widgetName,pText_DefintionBox);
    Glib::RefPtr<Gtk::TextBuffer> DefinitionBuffer =  pText_DefintionBox->get_buffer();
    DefinitionBuffer->set_text(SSG::SpellingWords.getCurrentWord()->getDefinition());
}

void SSG_AS_Button_Options_Clicked()
{
	SSG::winContainer.OptionsScreen->show();
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

	cout << retString << endl;
	return retString;
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
	//MSL assumed sorted due to AS opening [now removed]
	SSG::MSL->sortList();
	vector<string> syllables;
    vector<int> syllableWCount;

	syllables.reserve(SSG::MSL->size()); //Reduce mem allocs
	syllableWCount.reserve(SSG::MSL->size());

    for (int i=0; i<SSG::MSL->size(); i++)
    {
        syllables.push_back((*SSG::MSL)[i]);
        syllableWCount.push_back(SSG::MSL->getSyllableWCount(syllables.back()));
    }

    SSG::histLog.createBarGraph("./Data/graphData/SyllableData.csv","Syllable Wrong Counts", syllables, syllableWCount);
}

void SSG_ASG_IncorrectWords_Clicked()
{
	SSG::histLog.graphIncorrectWords(SSG::currentASComboTime);
}

void SSG_ASGK_Mistakes_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,2,"Mistakes per test (1 Week Moving Average)", "./Data/graphData/keyboardMistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,2,"Mistakes per test", "./Data/graphData/keyboardMistakes.csv");
}

void SSG_ASGK_Mistakes100_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,3,"Mistakes per 100 characters (1 Week Moving Average)","./Data/graphData/keyboard100MistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,3,"Mistakes per 100 characters","./Data/graphData/keyboard100Mistakes.csv");
}

void SSG_ASGK_WPM_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,5,"WPM (1 Week Moving Average)", "./Data/graphData/wordsPerMinuteAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,5,"WPM", "./Data/graphData/wordsPerMinute.csv");
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
			ofstream passFile("./Data/userPassword.hash", std::ofstream::out);
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
	Gtk::Entry* pEntry = nullptr;
	connectSignalsSpellingScreen();
	connectSignalsKeyboardScreen();	
	connectSignalsHangmanScreen();

	
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
	SSG::refBuilder->get_widget("SSG_MS_Button_Analysis", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Analysis_Clicked) );}

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

	

	Gtk::ComboBoxText* pCombo = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe",pCombo);
	if (pCombo)
		{pCombo->signal_changed().connect( sigc::ptr_fun(SSG_AS_Combo_changed) );}
}

addTags("SSG_KS_Text_LastWord");
addTags("SSG_SC_Text_AttemptedSpelling");

}
