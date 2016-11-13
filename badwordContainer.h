#ifndef badwordContainer_h
#define badwordContainer_h

#include <vector>
#include <string>

#include "word.h"
#include "badWord.h"
#include "wordContainer.h"

class badwordContainer : public wordContainer {
public:
    badwordContainer();

    badwordContainer(wordContainer& fullWordList, std::string filename);

};

#endif
