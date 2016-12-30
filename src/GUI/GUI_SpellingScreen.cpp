#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>

#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordCC.h"

#include "../../objects/windowContainer.cpp"

using namespace std;

void speak(const string& wordToSay, const bool isCorrect);
void say(const string& sentence);
string makeUpperCase(const string& attempt);
void definitionHelper(const string& widgetName);
void keyboard_update_last_word(const string& attemptUpper, const string& wordString, const string& tViewName="SSG_KS_Text_LastWord");

namespace SSG {
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
}

void SSG_MS_Button_Spelling_Clicked()
{
	SSG::winContainer.SpellingScreen->show();
	SSG::SpellingWords.findSpellingWords();
}

static void SSG_SC_Button_Return_Clicked()
{
	if(SSG::winContainer.SpellingScreen)
		SSG::winContainer.SpellingScreen->hide(); //hide() will close the window but keep the program running
	SSG::SpellingWords.userEndSpellingTest(); //Create log event if appropriate
}

void SSG_SC_Button_Definition_Clicked()
{
  definitionHelper("SSG_SC_Text_DefinitionBox");
}

void SSG_SC_Button_Play_Clicked()
{
	string wordToSpell = SSG::SpellingWords.getCurrentWord()->getWord();
	speak(wordToSpell,false);
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

void connectSignalsSpellingScreen()
{
    if (SSG::winContainer.SpellingScreen) //Only connect signals if window initialised
    {
        Gtk::Button* pButton = nullptr;
    	SSG::refBuilder->get_widget("SSG_SC_Button_Return", pButton);
    	if(pButton)
    		{pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Return_Clicked) );}

        pButton = nullptr;
        SSG::refBuilder->get_widget("SSG_MS_Button_Spelling", pButton);
        if(pButton)
            {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_MS_Button_Spelling_Clicked) );}

        pButton = nullptr;
        SSG::refBuilder->get_widget("SSG_SC_Button_Definition", pButton);
        if(pButton)
            {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Definition_Clicked) );}

        pButton = nullptr;
        SSG::refBuilder->get_widget("SSG_SC_Button_Play", pButton);
        if(pButton)
            {pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Play_Clicked) );}

        Gtk::Entry* pEntry = nullptr;
        SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
        if (pEntry)
            {pEntry->signal_activate().connect( sigc::ptr_fun(SSG_SC_TextEntry_activate) );}
    }
}
