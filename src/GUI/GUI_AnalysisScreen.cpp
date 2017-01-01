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

#define useConnectGraphFunc //Use connect function + 1 helper over 3 parameterless functions

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
static void SSG_AS_Combo_changed();
static void SSG_MS_Button_Analysis_Clicked()
{
	SSG::winContainer.AnalysisScreen->show();
	SSG_AS_Combo_changed(); //Update AS text
}

static void SSG_AS_Combo_changed()
{
	//Get the output buffer
	Gtk::TextView* pSSG_AS_Text_Status = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Text_Status",pSSG_AS_Text_Status);
	Glib::RefPtr<Gtk::TextBuffer> StatusBuffer =  pSSG_AS_Text_Status->get_buffer();
	//Get the combo box to extract the wanted time
	Gtk::ComboBoxText* pSSG_AS_Combo_Timeframe = nullptr;
	SSG::refBuilder->get_widget("SSG_AS_Combo_Timeframe", pSSG_AS_Combo_Timeframe);
	string timeFrame = pSSG_AS_Combo_Timeframe->get_active_text();
	//Determine time based off string value
	time_t startTime;
	if (timeFrame == "This session")
		startTime=SSG::sessionStartTime;
	else
		if (timeFrame == "Last week")
			startTime=(time(0)-604800); //604800 is 1 week in seconds
		else
			if (timeFrame == "Last month")
				startTime=(time(0)-2419200); //4 Weeks (Month)
			else
				startTime=0; //Start of time (1 Jan 1970)

	SSG::currentASComboTime = startTime; //Update the wanted time for other functions to use
	StatusBuffer->set_text(SSG::histLog.getEventString(startTime)); //Calculate text output and place in buffer
}

static void SSG_ASG_MSL_Clicked()
{
	//MSL assumed sorted due to AS opening [now removed]
	SSG::MSL->sortList();
	vector<string> syllables; //X values
    vector<int> syllableWCount; //Y values

	syllables.reserve(SSG::MSL->size()); //Reduce mem allocs
	syllableWCount.reserve(SSG::MSL->size());

    for (int i=0; i<SSG::MSL->size(); i++) //Add every syllable and wrong count
    {
        syllables.push_back((*SSG::MSL)[i]);
        syllableWCount.push_back(SSG::MSL->getSyllableWCount(syllables.back()));
    }
	//createBarGraph output file, title, x values, y values
    SSG::histLog.createBarGraph("./Data/graphData/SyllableData.csv","Syllable Wrong Counts", syllables, syllableWCount);
}

static void SSG_ASGK_MovingAvg_Toggled()
{
	SSG::AnalysisMovAvg = !SSG::AnalysisMovAvg; //Toggle the value, set it equal to itself not'ed
}

static void SSG_ASG_IncorrectWords_Clicked()
{
	SSG::histLog.graphIncorrectWords(SSG::currentASComboTime);
}

#ifndef useConnectGraphFunc
static void SSG_ASGK_Mistakes_Clicked()
{
	if (SSG::AnalysisMovAvg) //If Moving Average has been toggled on, generate moving average, else don't
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
#endif

static void SSG_AS_Button_Return_Clicked()
{
	if(SSG::winContainer.AnalysisScreen)
		SSG::winContainer.AnalysisScreen->hide(); //hide() will close the window but keep the program running
}

#ifdef useConnectGraphFunc
static void SSG_ASGK_BasicGraphHandler(const int keyboardValue,const string& title, const string& saveLocation)
{
	if (SSG::AnalysisMovAvg)
		SSG::histLog.graphKeyboardMovingAvg(SSG::currentASComboTime,keyboardValue,title + " (1 Week Moving Average)", saveLocation + "AVG.csv");
	else
		SSG::histLog.graphKeyboard(SSG::currentASComboTime,keyboardValue,title, saveLocation + ".csv");
}

static void connectGraphSignalHandlers(const std::vector<std::string>& widgetNames,
		const vector<sigc::slot<void,const int,const string&,const string&>>& funcPointers,
		const std::vector<int>& keyboardValues, const std::vector<const std::string*>& titles,
		const std::vector<const std::string*>& saveLocations)
{
    if (widgetNames.size() == funcPointers.size())
    {
		Gtk::Button* pButton;
        for (int iter=0, end=widgetNames.size(); iter<end; iter++)
        {
            pButton = nullptr;
            SSG::refBuilder->get_widget(widgetNames[iter], pButton);
            if(pButton)
                {
					pButton->signal_clicked().connect( sigc::bind<const int,const std::string&,const std::string&>( funcPointers[iter], keyboardValues[iter], *titles[iter], *saveLocations[iter]));
                    cout << "Connecting " << widgetNames[iter] << endl;
                }
        }
    }
    else
    {
    	cout << "Warning unable to connect signals, function mismatch" << endl;
    }
}

struct graphKeepers
{
	const string WPMTitle = "WPM";
	const string WPMSL = "./Data/graphData/wordsPerMinute";
	const string MistakesTitle = "Mistakes per test";
	const string MistakesSL = "./Data/graphData/keyboardMistakes";
	const string Mistakes100Title = "Mistakes per 100 characters";
	const string Mistakes100SL = "./Data/graphData/keyboard100Mistakes";
} graphAlias;

#endif

void connectSignalsAnalysisScreen()
{
    if (SSG::winContainer.AnalysisScreen) //Only connect signals if window initialised
    {
        connectBasicSignalHandersButton(
			{"SSG_AS_Button_Return",
            "SSG_ASG_MSL",
			"SSG_MS_Button_Analysis",
			"SSG_ASG_IncorrectWords"
			#ifndef useConnectGraphFunc
            ,"SSG_ASGK_Mistakes",
             "SSG_ASGK_Mistakes100",
             "SSG_ASGK_WPM"
			 #endif
		 	},
			 {sigc::ptr_fun(SSG_AS_Button_Return_Clicked),
             sigc::ptr_fun(SSG_ASG_MSL_Clicked),
			 sigc::ptr_fun(SSG_MS_Button_Analysis_Clicked),
			 sigc::ptr_fun(SSG_ASG_IncorrectWords_Clicked)
			 #ifndef useConnectGraphFunc
            ,sigc::ptr_fun(SSG_ASGK_Mistakes_Clicked),
             sigc::ptr_fun(SSG_ASGK_Mistakes100_Clicked),
             sigc::ptr_fun(SSG_ASGK_WPM_Clicked)
			 #endif
		 	});

		#ifdef useConnectGraphFunc
		vector<string> widgetNames = {
			"SSG_ASGK_Mistakes",
			"SSG_ASGK_Mistakes100",
			"SSG_ASGK_WPM"};

		vector<sigc::slot<void,const int,const string&,const string&>> funcPointers = {
			sigc::ptr_fun(SSG_ASGK_BasicGraphHandler),
			sigc::ptr_fun(SSG_ASGK_BasicGraphHandler),
			sigc::ptr_fun(SSG_ASGK_BasicGraphHandler)};

		vector<int> keyboardValues = {2,3,5};
		vector<const string*> titles = {
			&graphAlias.MistakesTitle,
			&graphAlias.Mistakes100Title,
			&graphAlias.WPMTitle};

		vector<const string*> saveLocations = {
			&graphAlias.MistakesSL,
			&graphAlias.Mistakes100SL,
			&graphAlias.WPMSL};

		connectGraphSignalHandlers(widgetNames,funcPointers,keyboardValues,titles,saveLocations);
		#endif

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
