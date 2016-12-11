#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstdlib> //Declare system() which comes from a c library

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

static void SSG_SC_Button_Return_Clicked()
{
	if(SSG::winContainer.SpellingScreen)
		SSG::winContainer.SpellingScreen->hide(); //hide() will close the window but keep the program running
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

void SSG_SC_Button_Definition_Clicked()
{
  cout << "Insert definition here" << endl;
  Gtk::TextView* pSSG_SC_Text_DefintionBox = nullptr;
  SSG::refBuilder->get_widget("SSG_SC_Text_DefinitionBox",pSSG_SC_Text_DefintionBox);
  Glib::RefPtr<Gtk::TextBuffer> DefinitionBuffer =  pSSG_SC_Text_DefintionBox->get_buffer();
  word* cWord = SSG::SpellingWords.getCurrentWord();

  DefinitionBuffer->set_text(cWord->definition);
 // pSSG_SC_Text_DefintionBox->set_buffer(m_refTextBuffer2);
}

void SSG_SC_Button_Play_Clicked()
{
	string wordToSpell = SSG::SpellingWords.getCurrentWord()->getWord();
	speak(wordToSpell,false);
}

void SSG_MS_Button_Spelling_Clicked()
{
	SSG::winContainer.SpellingScreen->show();
	SSG::SpellingWords.findSpellingWords();
}

void SSG_MS_Button_Keyboard_Clicked()
{
	SSG::SpellingWords.findKeyboardWords();
	SSG::winContainer.KeyboardScreen->show();
	Gtk::TextView* pSSG_KS_WordList = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_WordList",pSSG_KS_WordList);
	Glib::RefPtr<Gtk::TextBuffer> WordListBuffer =  pSSG_KS_WordList->get_buffer();
	WordListBuffer->set_text(SSG::SpellingWords.getKeyboardWords());
	//pSSG_KS_WordList->set_wrap_mode(Gtk::Wrap_Word);
}

void SSG_MS_Button_Analysis_Clicked()
{
	SSG::winContainer.AnalysisScreen->show();
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

void SSG_SC_TextEntry_activate()
{
    //cout << "Entered function" << endl;
    Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();
    cout << attempt << endl;

	SSG::SpellingWords.spellingAttempt(attempt);
	
	EntryBuffer->set_text("");
}

void SSG_KS_TextEntry_insert()
{
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();
    cout << attempt << endl;
	if (SSG::SpellingWords.keyboardAttempt(attempt))
		EntryBuffer->set_text("");
	/*To create a tag Gtj::TextBuffer::create_tag(string) https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextBuffer.html#ad42f4e41a4cb2d5a824e2f0ffa78e973
	  use apply_tag to with the tagref, iterator start and iterator end https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextBuffer.html#ad42f4e41a4cb2d5a824e2f0ffa78e973 https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextTag.html*/
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
		SSG::refBuilder->get_widget("SSG_SC_TextEntry",pEntry);

	pEntry = nullptr;
	SSG::refBuilder->get_widget("SSG_KS_TextEntry",pEntry);
	if (pEntry)
		{pEntry->signal_changed().connect( sigc::ptr_fun(SSG_KS_TextEntry_insert) );}
}
}
