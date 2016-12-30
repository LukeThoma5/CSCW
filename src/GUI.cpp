#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>

#include "../headers/word.h"
#include "../headers/badWord.h"
#include "../headers/wordCC.h"

#include "../objects/windowContainer.cpp"

#include "./GUI/headers/SpellingScreen.h"
#include "./GUI/headers/KeyboardScreen.h"
#include "./GUI/headers/HangmanScreen.h"
#include "./GUI/headers/AnalysisScreen.h"
#include "./GUI/headers/OptionsScreen.h"
#include "./GUI/headers/PasswordScreen.h"
#include "./GUI/headers/ResetDataScreen.h"

using namespace std;

namespace SSG {
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
}

static void SSG_MS_Button_Quit_Clicked()
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
	if(SSG::winContainer.MainScreen)
	{
		Gtk::Button* pButton = nullptr;
		SSG::refBuilder->get_widget("SSG_MS_Button_Quit", pButton);
		if(pButton)
			{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Quit_Clicked) );}
	}

	//Get the GtkBuilder-instantiated Button, and connect a signal handler:
	connectSignalsSpellingScreen();
	connectSignalsKeyboardScreen();	
	connectSignalsHangmanScreen();
	connectSignalsAnalysisScreen();
	connectSignalsOptionsScreen();
	connectSignalsPasswordScreen();
	connectSignalsResetDataScreen();

	addTags("SSG_KS_Text_LastWord");
	addTags("SSG_SC_Text_AttemptedSpelling");

}
