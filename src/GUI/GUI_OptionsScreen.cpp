#include <iostream>

#include <gtkmm.h>

#include "../../objects/windowContainer.cpp"

using namespace std;

void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

namespace SSG {
	extern windowContainer winContainer;
}

static void SSG_AS_Button_Options_Clicked()
{
	SSG::winContainer.OptionsScreen->show();
}

static void SSG_OP_Button_Close_Clicked()
{
	cout << "closing" << endl;
	SSG::winContainer.OptionsScreen->hide();
}

void connectSignalsOptionsScreen()
{
    if (SSG::winContainer.OptionsScreen) //Only connect signals if window initialised
    {
        std::vector<std::string> widgetNames = {
            "SSG_AS_Button_Options",
            "SSG_OP_Button_Close"};
        std::vector<sigc::slot<void>> funcPointers = {
            sigc::ptr_fun(SSG_AS_Button_Options_Clicked),
            sigc::ptr_fun(SSG_OP_Button_Close_Clicked)};

        connectBasicSignalHandersButton(widgetNames,funcPointers);
    }
}
