#! /bin/bash

#g++ main.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp GUI.cpp hLog.cpp logEvent.cpp externalCode.cpp -o a.out `pkg-config gtkmm-3.0 --cflags --libs`
g++ ./src/main.cpp\
 ./objects/word.cpp\
 ./objects/badWord.cpp\
 ./objects/masterSyllableListTree.cpp\
 ./objects/masterSyllableListMap.cpp\
 ./objects/syllableNode.cpp\
 ./objects/wordContainer.cpp\
 ./objects/badwordContainer.cpp\
 ./objects/wordCC.cpp\
 ./objects/wordCC/wordCCSpelling.cpp\
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
 -o a.out\
 `pkg-config gtkmm-3.0 --cflags --libs`
#g++ -W GUI.cpp -o backlessGUI `pkg-config gtkmm-3.0 --cflags --libs`
