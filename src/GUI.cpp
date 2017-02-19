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
	extern windowContainer winContainer; //Contains the window pointers
	extern wordCC SpellingWords; //Handles the words in memory
	extern Glib::RefPtr<Gtk::Builder> refBuilder; //Used to extract widgets
}

static void SSG_MS_Button_Quit_Clicked()
{
	if (SSG::winContainer.MainScreen)
		SSG::winContainer.MainScreen->hide();
}

void definitionHelper(const string& widgetName)
{
	Gtk::TextView* pText_DefintionBox = nullptr;
    SSG::refBuilder->get_widget(widgetName,pText_DefintionBox); //Get the textView of the widget passed in to the function
    Glib::RefPtr<Gtk::TextBuffer> DefinitionBuffer =  pText_DefintionBox->get_buffer(); //Get the textView buffer
    DefinitionBuffer->set_text(SSG::SpellingWords.getCurrentWord()->getDefinition()); //Set the contents of the buffer to the definition stored in the currently active word.
}

void addTags(string textName)
{
	Gtk::TextView* testView = nullptr;
	SSG::refBuilder->get_widget(textName, testView); //Get the textView of the passed in widget name
	Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  testView->get_buffer(); //Get its buffer
	//WordListBuffer->insert_with_tag(WordListBuffer->begin(),"Test",refTagMatch);
	//Add 3 tags to the buffer which can be applied when inserting text
	WordListBuffer->create_tag("redtag");
	WordListBuffer->create_tag("orangetag");
	WordListBuffer->create_tag("greentag");
	//Get the textViewBuffer tagtable (table with 3 above tags in)
	Glib::RefPtr<Gtk::TextTagTable> mytagtable = WordListBuffer->get_tag_table();
	//Find the tags created above and set the background property appropraitely
	Glib::RefPtr<Gtk::TextTag> myTag = mytagtable->lookup("redtag");
	myTag->property_background() = "red";
	myTag = mytagtable->lookup("orangetag");
	myTag->property_background() = "orange";
	myTag = mytagtable->lookup("greentag");
	myTag->property_background() = "green";
}

void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers)
{
    if (widgetNames.size() == funcPointers.size()) //If the same amount of widgets as signal handlers
    {
        Gtk::Button* pButton;
        for (int iter=0, end=widgetNames.size(); iter<end; ++iter)
        {
            pButton = nullptr; //Null the pointer to prevent two handlers being attached to the same widget
            SSG::refBuilder->get_widget(widgetNames[iter], pButton); //Get the widget of this iteration
            if(pButton) //If the button could be found
            {
                pButton->signal_clicked().connect( funcPointers[iter] ); //Attach the function pointer to the widget
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
	//Connect the quit button to the signal handler
	//Before connectBasicSignalHandersButton all handlers where attached like this
	if(SSG::winContainer.MainScreen)
	{
		Gtk::Button* pButton = nullptr;
		SSG::refBuilder->get_widget("SSG_MS_Button_Quit", pButton);
		if(pButton)
			{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Quit_Clicked) );}
	}

	//Connect the handlers of the individual screens
	connectSignalsSpellingScreen();
	connectSignalsKeyboardScreen();
	connectSignalsHangmanScreen();
	connectSignalsAnalysisScreen();
	connectSignalsOptionsScreen();
	connectSignalsPasswordScreen();
	connectSignalsResetDataScreen();

	//Add tags to the Keyboard and Spelling screen
	addTags("SSG_KS_Text_LastWord");
	addTags("SSG_SC_Text_AttemptedSpelling");
}
