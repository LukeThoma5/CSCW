#ifndef GUI_Headers_KeyboardScreen_h
#define GUI_Headers_KeyboardScreen_h
static void SSG_KS_Button_Return_Clicked();
void SSG_KS_Button_Definition_Clicked();
void GUI_keyboard_Handler();
void SSG_MS_Button_Keyboard_Clicked();
void keyboard_update_last_word(const std::string& attemptUpper, const std::string& wordString, const std::string& tViewName="SSG_KS_Text_LastWord");
void connectSignalsKeyboardScreen();
void SSG_KS_TextEntry_insert();
#endif
