#! /bin/bash

#g++ main.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp GUI.cpp hLog.cpp logEvent.cpp externalCode.cpp -o a.out `pkg-config gtkmm-3.0 --cflags --libs`
g++ main.cpp word.cpp badWord.cpp masterSyllableListTree.cpp masterSyllableListMap.cpp syllableNode.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp GUI.cpp hLog.cpp logEvent.cpp externalCode.cpp -o a.out `pkg-config gtkmm-3.0 --cflags --libs`
#g++ -W GUI.cpp -o backlessGUI `pkg-config gtkmm-3.0 --cflags --libs`
