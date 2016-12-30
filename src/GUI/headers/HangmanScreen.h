#ifndef GUI_Headers_HangmanScreen_h
#define GUI_Headers_HangmanScreen_h
static void SSG_HM_Return_Clicked();
std::string createDash(const int& length);
void SSG_MS_Button_Games_Clicked();
bool revealHangmanWord(const std::string& wordToGuess, std::string& hiddenLine, const char attempt);
void ensureCharUpperCase(char& toUpper);
void SSG_HM_TextEntry_activate();
void connectSignalsHangmanScreen();
#endif
