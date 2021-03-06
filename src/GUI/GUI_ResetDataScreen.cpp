#include <iostream>
#include <string>
#include <vector>
#include <gtkmm.h>

#include "../../headers/sha256.h" //External code, not mine

#include "../../headers/hLog.h"
#include "../../objects/windowContainer.cpp"

using namespace std;

string saltPassword(const string& password, const string& salt);
vector<string> readPasswordFile();
void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

namespace SSG {
	extern windowContainer winContainer;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern hLog histLog;
}

static void SSG_RD_Button_Clear_Data_Confirm_Clicked()
{
	//Get the password attempt
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_RD_TextEntry_Password_Attempt",pEntry);
    string attempt = pEntry->get_text();
	//Load the passwordFile into the vector [0] is the salt [1] sakted hash
	vector<string> passVector = readPasswordFile();
	if (passVector.size() > 1) //If the password file not corrupt
	{
		string passwordAttempt = saltPassword(attempt,passVector[0]); //salt the password attempt
		passwordAttempt = sha256(passwordAttempt); //Hash the salted password
		if (passwordAttempt != passVector[1]) //If the hashes don't match eg wrong password entered
			cerr << "PASSWORDS DO NOT MATCH" << endl; //Print an error message
		else
			SSG::histLog.clearLog(); //Tell the log to clear
	}
}
static void SSG_RD_Button_Close_Clicked()
{
	SSG::winContainer.ResetData->close();
}

static void SSG_OP_Button_Clear_Data_Clicked()
{
	SSG::winContainer.ResetData->show();
}

void connectSignalsResetDataScreen()
{
    if (SSG::winContainer.ResetData)
    {
        connectBasicSignalHandersButton(
			{"SSG_OP_Button_Clear_Data",
	        "SSG_RD_Button_Clear_Data_Confirm",
	        "SSG_RD_Button_Close"},
			{sigc::ptr_fun(SSG_OP_Button_Clear_Data_Clicked),
		    sigc::ptr_fun(SSG_RD_Button_Clear_Data_Confirm_Clicked),
		    sigc::ptr_fun(SSG_RD_Button_Close_Clicked)});
    }
}
