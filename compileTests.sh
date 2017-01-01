#! /bin/bash

#g++ tests.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp GUI.cpp hLog.cpp logEvent.cpp -o tests `pkg-config gtkmm-3.0 --cflags --libs`
g++ ./src/tests.cpp\
 ./objects/word.cpp\
 ./objects/badWord.cpp\
 ./objects/masterSyllableListTree.cpp\
 ./objects/masterSyllableListMap.cpp\
 ./objects/syllableNode.cpp\
 ./objects/wordContainer.cpp\
 ./objects/badwordContainer.cpp\
 ./objects/wordCC.cpp\
 ./src/GUI.cpp\
 ./src/GUI/GUI_SpellingScreen.cpp\
 ./src/GUI/GUI_KeyboardScreen.cpp\
 ./src/GUI/GUI_HangmanScreen.cpp\
 ./src/GUI/GUI_AnalysisScreen.cpp\
 ./src/GUI/GUI_OptionsScreen.cpp\
 ./src/GUI/GUI_PasswordScreen.cpp\
 ./src/GUI/GUI_ResetDataScreen.cpp\
 ./objects/hLog.cpp\
 ./objects/logEvent.cpp\
 ./src/externalCode.cpp\
 ./src/SHA256.cpp\
 ./src/speech.cpp\
 ./src/crypto.cpp\
 ./src/Misc/printVector.cpp\
 ./src/Misc/mergeSortHelpers.cpp\
 ./src/Misc/MSLHelpers.cpp\
 ./src/Misc/Utils.cpp\
 -o tests\
 `pkg-config gtkmm-3.0 --cflags --libs`
