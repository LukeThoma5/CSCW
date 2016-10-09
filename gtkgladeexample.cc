#include <gtkmm.h>
#include <iostream>
#include <cstdlib> //Declare system() which comes from a c library
#include <string>

Gtk::Window* pDialog = nullptr;
Glib::RefPtr<Gtk::Builder> refBuilder;

static
void on_button_clicked()
{
  if(pDialog)
    pDialog->hide(); //hide() will cause main::run() to end.
}

void my_button_clicked()
{
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

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  //Load the GtkBuilder file and instantiate its widgets:
  refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("guitest.glade");
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
  refBuilder->get_widget("Mywindow", pDialog);
  if(pDialog)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pButton = nullptr;
    refBuilder->get_widget("Quit", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(on_button_clicked) );
    }

    pButton = nullptr;
    refBuilder->get_widget("Play", pButton);
    if(pButton)
    {
      pButton->signal_clicked().connect( sigc::ptr_fun(my_button_clicked) );
    }


    app->run(*pDialog);
  }

  delete pDialog;

  return 0;
}

