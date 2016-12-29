#include <gtkmm.h>
#include <iostream>
#include <cstdlib> //Declare system() which comes from a c library
#include <string>

Gtk::Window* pDialog = nullptr;
Glib::RefPtr<Gtk::Builder> refBuilder;
Gtk::Window* pDialogToRun = nullptr;
bool programContinue = true;

using namespace std;

static
void SSG_SC_Button_Return_Clicked()
{
  if(pDialog)
    pDialog->hide(); //hide() will cause main::run() to end.
}

void SSG_SC_Button_Definition_Clicked()
{
  cout << "Insert definition here" << endl;
  Gtk::TextView* pSSG_SC_Text_DefintionBox = nullptr;
  refBuilder->get_widget("SSG_SC_Text_DefinitionBox",pSSG_SC_Text_DefintionBox);
  Glib::RefPtr<Gtk::TextBuffer> DefinitionBuffer =  pSSG_SC_Text_DefintionBox->get_buffer();
  DefinitionBuffer->set_text("Insert definition here");
 // pSSG_SC_Text_DefintionBox->set_buffer(m_refTextBuffer2);
}

void SSG_MS_Button_Spelling_Clicked()
{
    if(pDialog)
    {
        refBuilder->get_widget("SSG_Spelling_Screen", pDialogToRun);
        programContinue = false;
        pDialogToRun->show();
        //pDialog->hide();
    }

}

void SSG_SC_TextEntry_activate()
{
    cout << "Entered function" << endl;
    Gtk::Entry* pEntry = nullptr;
    refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
    Glib::RefPtr<Gtk::EntryBuffer> EntryBuffer =  pEntry->get_buffer();
    string attempt = pEntry->get_text();
    cout << attempt << endl;
    EntryBuffer->set_text("");
}

/*
void my_button_clicked()
{
  std::cout << "Button Clicked!" << std::endl;
  if(pDialog) //if the window has been initialised
  {
    std::cout << "You have pressed the top button, well done!" << std::endl;
    //system("mpv /mnt/shit/home/l/Music/Youtube/Nano/BeFree.mkv");
    Gtk::Entry* textp = nullptr;
    refBuilder->get_widget("SongTitle", textp);

    if (textp)
    {
      std::string entryTextString = textp->get_text();
      std::cout << entryTextString << std::endl;

      refBuilder->get_widget("SongDirectory", textp);

      if(textp)
      {
        std::string enterDirectory = textp->get_text();
        entryTextString = "mpv " + enterDirectory + entryTextString;
        system(entryTextString.c_str());
        //entryTextString = "flite -voice slt -t \"" + entryTextString + "\"";
        //std::cout << entryTextString << std::endl;
        //system(entryTextString.c_str());

      }


    }
  }
}
*/

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  //Load the GtkBuilder file and instantiate its widgets:
  refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("SSG_Gui_rev2.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated Dialog:
  refBuilder->get_widget("SSG_Spelling_Screen", pDialog);
  if(pDialog)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = nullptr;
    refBuilder->get_widget("SSG_SC_Button_Return", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Return_Clicked) );
    }

    pButton = nullptr;
    refBuilder->get_widget("SSG_SC_Button_Definition", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(SSG_SC_Button_Definition_Clicked) );
    }

    Gtk::Entry* pEntry = nullptr;
    refBuilder->get_widget("SSG_SC_TextEntry",pEntry);
    //pEntry->signal_changed().connect( sigc::ptr_fun(SSG_SC_TextEntry_insert) );
    pEntry->signal_activate().connect( sigc::ptr_fun(SSG_SC_TextEntry_activate) );

    /*
    pButton = nullptr;
    refBuilder->get_widget("Play", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(my_button_clicked) );
     // gtk_builder_connect_signals(refBuilder.operator->(), NULL); Not possible to do in c++
    }
    */
        app->run(*pDialog);
  }

  delete pDialog;

  return 0;
}
