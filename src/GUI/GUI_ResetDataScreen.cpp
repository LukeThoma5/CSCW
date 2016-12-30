#include <iostream>
#include <string>
#include <vector>

#include "../../headers/sha256.h" //External code, not mine

#include <gtkmm.h>

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
	Gtk::Entry* pEntry = nullptr;
    SSG::refBuilder->get_widget("SSG_RD_TextEntry_Password_Attempt",pEntry);
    string attempt = pEntry->get_text();

	vector<string> passVector = readPasswordFile();
	if (passVector.size() > 1)
	{
		string passwordAttempt = saltPassword(attempt,passVector[0]);
		if (passwordAttempt != passVector[1])
			cout << "PASSWORDS DO NOT MATCH" << endl;
		else
			SSG::histLog.clearLog();
	}
}
static void SSG_RD_Button_Close_Clicked()
{
	SSG::winContainer.ResetData->close();
}

static void SSG_OP_Button_Clear_Data_Clicked()
{
	cout << "Showing" << endl;
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
