#! /bin/bash

#g++ tests.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp GUI.cpp hLog.cpp logEvent.cpp -o tests `pkg-config gtkmm-3.0 --cflags --libs`
g++ ./src/tests.cpp\
 ./objects/word.cpp\
 ./objects/word/wordWScore.cpp\
 ./objects/word/wordWordWrong.cpp\
 ./objects/badWord.cpp\
 ./objects/masterSyllableListTree.cpp\
 ./objects/masterSyllableListMap.cpp\
 ./objects/syllableNode.cpp\
 ./objects/wordContainer.cpp\
 ./objects/wordContainer/wordContainerBinSearch.cpp\
 ./objects/wordContainer/wordContainerSort.cpp\
 ./objects/wordContainer/wordContainerMaintenance.cpp\
 ./objects/wordContainer/wordContainerExtract.cpp\
 ./objects/badwordContainer.cpp\
 ./objects/wordCC.cpp\
 ./objects/wordCC/wordCCSpelling.cpp\
 ./objects/wordCC/wordCCKeyboard.cpp\
 ./objects/wordCC/wordCCMaintenance.cpp\
 ./objects/wordCC/wordCCCorrect.cpp\
 ./objects/wordCC/wordCCWrong.cpp\
 ./objects/wordCC/wordCCSort.cpp\
 ./src/GUI.cpp\
 ./src/GUI/GUI_SpellingScreen.cpp\
 ./src/GUI/GUI_KeyboardScreen.cpp\
 ./src/GUI/GUI_HangmanScreen.cpp\
 ./src/GUI/GUI_AnalysisScreen.cpp\
 ./src/GUI/GUI_OptionsScreen.cpp\
 ./src/GUI/GUI_PasswordScreen.cpp\
 ./src/GUI/GUI_ResetDataScreen.cpp\
 ./src/Tests/MSLAddSyllablesTest.cpp\
 ./src/Tests/MSLAddToTotalTest.cpp\
 ./src/Tests/MSLSortingTest.cpp\
 ./src/Tests/MSLSWCOverTest.cpp\
 ./src/Tests/MSLTestUtils.cpp\
 ./src/Tests/WordContainerSearchTest.cpp\
 ./src/Tests/hLogTests.cpp\
 ./src/Tests/WordTests.cpp\
 ./objects/hLog.cpp\
 ./objects/hLog/hLogASText.cpp\
 ./objects/hLog/hLogGraphHelpers.cpp\
 ./objects/hLog/hLogGraphs.cpp\
 ./objects/hLog/hLogKeyboardGraphs.cpp\
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
