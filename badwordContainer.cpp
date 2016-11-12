#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "word.h"
#include "badWord.h"
#include "wordContainer.h"
#include "badwordContainer.h"

using namespace std;

badwordContainer::badwordContainer(): wordContainer()
{
    cout << "badwordContainer created!" << endl;
    containsBadWords = true;
}
