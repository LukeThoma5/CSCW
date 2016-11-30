#ifndef windowContainer_H
#define windowContainer_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <gtkmm.h>

class windowContainer
{
public:
    Gtk::Window* MainScreen = nullptr;
    Gtk::Window* SpellingScreen = nullptr;

    void addWindows(Glib::RefPtr<Gtk::Builder>& refBuilder)
    {
        refBuilder->get_widget("SSG_Main_Screen",MainScreen);
        refBuilder->get_widget("SSG_Spelling_Screen",SpellingScreen);
    }
};

#endif
