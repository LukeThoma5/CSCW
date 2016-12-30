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

void SSG_PWR_Button_Accept_Clicked()
{
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt1",pEntry);
    string attempt1 = pEntry->get_text();
	SSG::refBuilder->get_widget("SSG_PWR_TextEntry_Attempt2",pEntry);
    string attempt2 = pEntry->get_text();

	if (attempt1 != attempt2)
	{
		Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Text fields do not match"); //Create a dialog with PasswordReset as the parent
		dialog.set_secondary_text("To set a password the fields must match!");
		dialog.run(); //Run the dialog, when ok'ed continue execution
	}
	else
	{

		if (attempt1.size() < 6)
		{
			Gtk::MessageDialog dialog(*SSG::winContainer.PasswordReset, "Password must be at least 6 characters long!"); //Create a dialog with PasswordReset as the parent
			dialog.run(); //Run the dialog, when ok'ed continue execution
		}

		else
		{
			//cout << "Password: " << attempt1 << " hash: " << sha256(attempt1) << endl;
			string salt = makeSalt();
			string password = saltPassword(attempt1, salt);
			cout << password << endl;
			ofstream passFile("./Data/userPassword.hash", std::ofstream::out);
			passFile << salt << endl;
			passFile << password << endl;
			SSG::winContainer.PasswordReset->hide();
		}
	}
}

void SSG_OP_Button_Password_Clicked()
{
	cout << "Showing" << endl;
	SSG::winContainer.PasswordReset->show();
}

void connectSignalsPasswordScreen()
{
    if (SSG::winContainer.AnalysisScreen) //Only connect signals if window initialised
    {
        std::vector<std::string> widgetNames = {
            "SSG_OP_Button_Password",
            "SSG_PWR_Button_Accept"};
        std::vector<sigc::slot<void>> funcPointers = {
            sigc::ptr_fun(SSG_OP_Button_Password_Clicked),
            sigc::ptr_fun(SSG_PWR_Button_Accept_Clicked)};

        connectBasicSignalHandersButton(widgetNames,funcPointers);
    }
}
