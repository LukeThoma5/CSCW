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
#include "./GUI/headers/AnalysisScreen.h"
#include "./GUI/headers/OptionsScreen.h"
#include "./GUI/headers/PasswordScreen.h"

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

void SSG_OP_Button_Clear_Data_Clicked()
{
	cout << "Showing" << endl;
	SSG::winContainer.ResetData->show();
}

void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers)
{
    if (widgetNames.size() == funcPointers.size())
    {
        Gtk::Button* pButton;
        for (int iter=0, end=widgetNames.size(); iter<end; iter++)
        {
            pButton = nullptr;
            SSG::refBuilder->get_widget(widgetNames[iter], pButton);
            if(pButton)
                {
                    pButton->signal_clicked().connect( funcPointers[iter] );
                    cout << "Connecting " << widgetNames[iter] << endl;
                }
        }
    }
    else
    {
    	cout << "Warning unable to connect signals, function mismatch" << endl;
    }
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
	connectSignalsAnalysisScreen();
	connectSignalsOptionsScreen();
	connectSignalsPasswordScreen();

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_MS_Button_Quit", pButton);
	if(pButton)
		{cout << "MainScreen made!" << endl; pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Quit_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_OP_Button_Clear_Data", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_OP_Button_Clear_Data_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_RD_Button_Clear_Data_Confirm", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_RD_Button_Clear_Data_Confirm_Clicked) );}

	pButton = nullptr;
	SSG::refBuilder->get_widget("SSG_RD_Button_Close", pButton);
	if(pButton)
		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_RD_Button_Close_Clicked) );}
}

addTags("SSG_KS_Text_LastWord");
addTags("SSG_SC_Text_AttemptedSpelling");

}
