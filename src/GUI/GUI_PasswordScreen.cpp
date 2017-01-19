#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../../headers/sha256.h" //External code, not mine

#include <gtkmm.h>

#include "../../objects/windowContainer.cpp"

using namespace std;

namespace SSG {
	extern windowContainer winContainer;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
}

string saltPassword(const string& password, const string& salt);
string makeSalt();
void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

static void SSG_PWR_Button_Accept_Clicked()
{
	//Get the attempts from the GUI
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt1",pEntry);
    string attempt1 = pEntry->get_text();
	SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt2",pEntry);
    string attempt2 = pEntry->get_text();

	if (attempt1 != attempt2) //If misstyped the password
	{
		Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Text fields do not match"); //Create a dialog with PasswordReset as the parent
		dialog.set_secondary_text("To set a password the fields must match!");
		dialog.run(); //Run the dialog, when ok'ed continue execution
	}
	else //If passwords match
	{

		if (attempt1.size() < 6)
		{
			Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Password must be at least 6 characters long!"); //Create a dialog with PasswordReset as the parent
			dialog.run(); //Run the dialog, when ok'ed continue execution
		}

		else //If not too short
		{
			//cout << "Password: " << attempt1 << " hash: " << sha256(attempt1) << endl;
			string salt = makeSalt(); //Generate a random 256 char hash from /dev/random
			string password = saltPassword(attempt1, salt); //Mix the salt and password together
			cout << password << endl;
			ofstream passFile("./Data/userPassword.hash", std::ofstream::out); //Open the password File for writing, previous data auto cleared
			passFile << salt << endl; //Write the salt to the file with a newline \n
			passFile << password << endl; //Write the salted hash to the file
			SSG::winContainer.PasswordReset->hide(); //Stop password screen execution
		}
	}
}

static void SSG_OP_Button_Password_Clicked()
{
	SSG::winContainer.PasswordReset->show();
}

void connectSignalsPasswordScreen()
{
    if (SSG::winContainer.AnalysisScreen) //Only connect signals if window initialised
    {
        connectBasicSignalHandersButton(
			{"SSG_OP_Button_Password",
	        "SSG_PWR_Button_Accept"},
			{sigc::ptr_fun(SSG_OP_Button_Password_Clicked),
	        sigc::ptr_fun(SSG_PWR_Button_Accept_Clicked)});
    }
}
