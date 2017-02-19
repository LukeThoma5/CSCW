#ifndef windowContainer_H
#define windowContainer_H

#include <gtkmm.h>

class windowContainer
{
public:
    Gtk::Window* MainScreen = nullptr;
    Gtk::Window* SpellingScreen = nullptr;
    Gtk::Window* AnalysisScreen = nullptr;
    Gtk::Window* HangmanScreen = nullptr;
    Gtk::Window* KeyboardScreen = nullptr;
    Gtk::Window* OptionsScreen = nullptr;
    Gtk::Window* PasswordReset = nullptr;
    Gtk::Window* ResetData = nullptr;

    void addWindows(Glib::RefPtr<Gtk::Builder>& refBuilder)
    {
        //From the Gtk::Builder initialise all the window pointers
        //If it fails the pointer will be left null
        refBuilder->get_widget("SSG_Main_Screen",MainScreen);
        refBuilder->get_widget("SSG_Spelling_Screen",SpellingScreen);
        refBuilder->get_widget("SSG_Analysis_Screen",AnalysisScreen);
        refBuilder->get_widget("SSG_Hangman_Screen",HangmanScreen);
        refBuilder->get_widget("SSG_Keyboard_Screen",KeyboardScreen);
        refBuilder->get_widget("SSG_Options_Window",OptionsScreen);
        refBuilder->get_widget("SSG_PassWord_Reset",PasswordReset);
        refBuilder->get_widget("SSG_Reset_Data",ResetData);
    }
};

#endif
