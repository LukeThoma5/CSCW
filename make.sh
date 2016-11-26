#! /bin/bash

g++ main.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp -o a.out
#g++ -W GUI.cpp -o backlessGUI `pkg-config gtkmm-3.0 --cflags --libs`
