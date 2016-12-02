#! /bin/bash

g++ tests.cpp word.cpp badWord.cpp masterSyllableList.cpp wordContainer.cpp badwordContainer.cpp wordCC.cpp -o tests `pkg-config gtkmm-3.0 --cflags --libs`
