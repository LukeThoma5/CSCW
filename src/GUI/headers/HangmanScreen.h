#ifndef GUI_Headers_HangmanScreen_h
#define GUI_Headers_HangmanScreen_h
static void SSG_HM_Return_Clicked();
static std::string createDash(const int& length);
static void SSG_MS_Button_Games_Clicked();
static bool revealHangmanWord(const std::string& wordToGuess, std::string& hiddenLine, const char attempt);
static void SSG_HM_TextEntry_activate();
void ensureCharUpperCase(char& toUpper);
void connectSignalsHangmanScreen();
#endif
