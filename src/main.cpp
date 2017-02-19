#include <gtkmm.h>

#include "../headers/randng.h" //random number generator header
#include "../headers/masterSyllableList.h" //MSL declaration
#include "../headers/masterSyllableListTree.h"
#include "../headers/masterSyllableListMap.h"
#include "../headers/wordCC.h"
#include "../headers/hLog.h"

#include "../objects/windowContainer.cpp"

using namespace std;

namespace SSG {
	//SSG SpellingSuiteGlobalVariables
	masterSyllablesListTree physicalMSL; //Define the MSLTree object that will fufill the MSL interface
	//masterSyllablesListMap physicalMSL;
	masterSyllablesList* MSL = &physicalMSL; //MasterSyllablesList //Upcast either MSLTree or MSLMap to MSL so objects can interact with it.
	Glib::RefPtr<Gtk::Builder> refBuilder; //Gtk object needed to retrieve widgets from the gtk builder (object that loads the xml file)
	windowContainer winContainer; //Object to keep the Gtk::window pointers active
	wordCC SpellingWords("./Data/finalDictwithDef.txt", "./Data/wrongWords.txt");
	//wordCC SpellingWords("./Data/shortDict.txt", "./Data/shortwrongWords.txt"); //Load the words into memory and store in a wordCC (wordContainerContainer)
	hLog histLog("./Data/SSGHistFile.txt"); //Load the historyLog from a file
	extern const time_t sessionStartTime = time(0); //Set a constant that has external linkage (can be seen by other translation units) to the time at startup of the program
}

void createRandomWordWrongCounts(); //Defined in src/Misc/MSLHelpers
void connectSignals(); //Defined in src/GUI.cpp

int main (int argc, char **argv)
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example"); //Create a gtk Application, pass the argv to gtk so it can extract the relevant options eg gtk themes

    //Load the GtkBuilder file and instantiate its widgets:
    SSG::refBuilder = Gtk::Builder::create();
    try
    {
      SSG::refBuilder->add_from_file("SSG_Gui_rev2.glade");
    }
    catch(const Glib::FileError& ex)
    {
	  //File not found
      std::cerr << "FileError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
	  //Error in the file structure
      std::cerr << "MarkupError: " << ex.what() << std::endl;
      return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
	  //Error in the file (wrongly defined item)
      std::cerr << "BuilderError: " << ex.what() << std::endl;
      return 1;
    }

	//Add the windows from the Gtk::builder
	SSG::winContainer.addWindows(SSG::refBuilder);
	connectSignals(); //Add the signal handlers to the signals that may be created during program execution

	if (SSG::MSL->hasNoValues()) //If the first run eg no wrong counts loaded
		createRandomWordWrongCounts(); //Set up some randomly until the user gets some wrong

	const std::string pipeName = "/tmp/SSGFIFO"; //The pipes location in the file structure
    mkfifo(pipeName.c_str(), 0666); //Create the pipe in memory with 666 permissions

	app->run(*(SSG::winContainer.MainScreen)); //Run the GUI, when the user closes main window continue execution in main

	cout << "Returning to OS" << endl; //Message to determine object destruction about to occur

	unlink(pipeName.c_str()); //Remove the pipe from the file system

	return 0; //Give a success signal to the operating system
}
