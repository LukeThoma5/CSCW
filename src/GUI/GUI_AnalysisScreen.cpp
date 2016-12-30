#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>

#include "../../headers/masterSyllableList.h" //MSL declaration
#include "../../headers/word.h"
#include "../../headers/badWord.h"
#include "../../headers/wordCC.h"
#include "../../headers/hLog.h"

#include "../../objects/windowContainer.cpp"

using namespace std;

void connectBasicSignalHandersButton(const std::vector<std::string>& widgetNames,const std::vector<sigc::slot<void>>& funcPointers);

namespace SSG {
	//Static variables have internal linkage, only accessible in this translation unit (.cpp file)
	static std::time_t currentASComboTime = 0;
	static bool AnalysisMovAvg = false;

	//Objects stored in main.cpp
	extern windowContainer winContainer;
	extern wordCC SpellingWords;
	extern Glib::RefPtr<Gtk::Builder> refBuilder;
	extern std::time_t sessionStartTime;
	extern hLog histLog;
	extern masterSyllablesList* MSL; //MasterSyllablesList
}

static void SSG_AS_Button_Return_Clicked()
{
	if(SSG::winContainer.AnalysisScreen)
		SSG::winContainer.AnalysisScreen->hide(); //hide() will close the window but keep the program running
}

static void SSG_AS_Combo_changed()
{
	Gtk::TextView* pSSG_AS_Text_Status = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Text_Status",pSSG_AS_Text_Status);
	Glib::RefPtr<Gtk::TextBuffer> StatusBuffer =  pSSG_AS_Text_Status->get_buffer();
	Gtk::ComboBoxText* pSSG_AS_Combo_Timeframe = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe", pSSG_AS_Combo_Timeframe);
	string timeFrame = pSSG_AS_Combo_Timeframe->get_active_text();
	time_t startTime;
	if (timeFrame == "This session")
		startTime=SSG::sessionStartTime;
	else
		if (timeFrame == "Last week")
			startTime=(time(0)-604800);
		else
			startTime=0;
	SSG::currentASComboTime = startTime;
	StatusBuffer->set_text(SSG::histLog.getEventString(startTime));
	//SSG::histLog.graphIncorrectWords(startTime);
}

static void SSG_ASGK_MovingAvg_Toggled()
{
	SSG::AnalysisMovAvg = !SSG::AnalysisMovAvg;
	cout << "State Toggled!" << SSG::AnalysisMovAvg << endl;
}

static void SSG_ASG_MSL_Clicked()
{
	//MSL assumed sorted due to AS opening [now removed]
	SSG::MSL->sortList();
	vector<string> syllables;
    vector<int> syllableWCount;

	syllables.reserve(SSG::MSL->size()); //Reduce mem allocs
	syllableWCount.reserve(SSG::MSL->size());

    for (int i=0; i<SSG::MSL->size(); i++)
    {
        syllables.push_back((*SSG::MSL)[i]);
        syllableWCount.push_back(SSG::MSL->getSyllableWCount(syllables.back()));
    }

    SSG::histLog.createBarGraph("./Data/graphData/SyllableData.csv","Syllable Wrong Counts", syllables, syllableWCount);
}

static void SSG_ASG_IncorrectWords_Clicked()
{
	SSG::histLog.graphIncorrectWords(SSG::currentASComboTime);
}

static void SSG_ASGK_Mistakes_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,2,"Mistakes per test (1 Week Moving Average)", "./Data/graphData/keyboardMistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,2,"Mistakes per test", "./Data/graphData/keyboardMistakes.csv");
}

static void SSG_ASGK_Mistakes100_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,3,"Mistakes per 100 characters (1 Week Moving Average)","./Data/graphData/keyboard100MistakesAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,3,"Mistakes per 100 characters","./Data/graphData/keyboard100Mistakes.csv");
}

static void SSG_ASGK_WPM_Clicked()
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,5,"WPM (1 Week Moving Average)", "./Data/graphData/wordsPerMinuteAVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,5,"WPM", "./Data/graphData/wordsPerMinute.csv");
}

static void SSG_MS_Button_Analysis_Clicked()
{
	SSG::winContainer.AnalysisScreen->show();
	SSG_AS_Combo_changed();

}

void connectSignalsAnalysisScreen()
{
    if (SSG::winContainer.AnalysisScreen) //Only connect signals if window initialised
    {
        std::vector<std::string> widgetNames = {
            "SSG_AS_Button_Return",
            "SSG_ASG_MSL",
            "SSG_ASG_IncorrectWords",
            "SSG_MS_Button_Analysis",
            "SSG_ASGK_Mistakes",
             "SSG_ASGK_Mistakes100",
             "SSG_ASGK_WPM"};
        std::vector<sigc::slot<void>> funcPointers = {
            sigc::ptr_fun(SSG_AS_Button_Return_Clicked),
            sigc::ptr_fun(SSG_ASG_MSL_Clicked),
            sigc::ptr_fun(SSG_ASG_IncorrectWords_Clicked),
            sigc::ptr_fun(SSG_MS_Button_Analysis_Clicked),
            sigc::ptr_fun(SSG_ASGK_Mistakes_Clicked),
            sigc::ptr_fun(SSG_ASGK_Mistakes100_Clicked),
            sigc::ptr_fun(SSG_ASGK_WPM_Clicked)};

        connectBasicSignalHandersButton(widgetNames,funcPointers);

		Gtk::ToggleButton* pToggle = nullptr;
		SSG::refBuilder->get_widget("SSG_ASGK_MovingAvg_Toggle", pToggle);
		if (pToggle)
			{pToggle->signal_toggled().connect( sigc::ptr_fun(SSG_ASGK_MovingAvg_Toggled) );}

		Gtk::ComboBoxText* pCombo = nullptr;
		SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe",pCombo);
		if (pCombo)
			{pCombo->signal_changed().connect( sigc::ptr_fun(SSG_AS_Combo_changed) );}
    }
}
