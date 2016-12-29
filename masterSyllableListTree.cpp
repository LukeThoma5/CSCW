#include "masterSyllableListTree.h"

using namespace std;

std::vector<std::string> masterSyllablesList::findUniqueSyllables(const std::vector<std::string>& inSyllables)
{
    cout << "masterSyllablesList::findUniqueSyllables" << endl;
    vector<string> returnVector;
    for (int i=0; i<inSyllables.size(); i++)
    {
        try
        {
            root->findNode(inSyllables[i]);
        }
        catch (int toCatch)
        {
            if (toCatch == -1)
                returnVector.push_back(inSyllables[i]);
        }
    }
    return returnVector;
}


void masterSyllablesList::addSyllables(const std::vector<std::string>& inSyllables)
{
    if (isSortedOnWrongCount)
        makeAlphabetical();
    cout << "masterSyllablesList::addSyllables" << endl;
    if (root)
    {
        vector<string> unique = findUniqueSyllables(inSyllables);
        for (int i=0; i<unique.size(); i++)
        {
            syllableNode* toAdd = new syllableNode(unique[i],0,1);
            allNodes.push_back(toAdd);
            root->addValueOnSyllable(toAdd);
        }
    }
    else
    {
        root = new syllableNode(inSyllables[0],0,1); //Initialise root
        allNodes.push_back(root);
        //Slice the old list
        vector<string> nonRootToAdd;
        for (int i=1; i<inSyllables.size(); i++)
            nonRootToAdd.push_back(inSyllables[i]);
        //Add the remaining syllables
        addSyllables(nonRootToAdd);
    }
}

void masterSyllablesList::addToTotal(const std::string& syllableToFind, const int& amountToInc)
{
    if (isSortedOnWrongCount)
        makeAlphabetical();
    cout << "masterSyllablesList::addToTotal" << endl;
    try
    {
        root->findNode(syllableToFind)->addToTotal(amountToInc);
    }
    catch (int i)
    {
        if (i==-1)
        {
            syllableNode* toAdd = new syllableNode(syllableToFind,amountToInc,1.0);
            allNodes.push_back(toAdd);
            root->addValueOnSyllable(toAdd);
        }
    }
}

int masterSyllablesList::size()
{
    cout << "masterSyllablesList::size" << endl;
    return allNodes.size();
}

void masterSyllablesList::sortList()
{
    cout << "masterSyllablesList::sortList" << endl;
    root->clearPointers();
    isSortedOnWrongCount = true;
    for (int i=0; i<allNodes.size(); i++)
    {
        if (allNodes[i] != root)
            root->addValueOnWrongCount(allNodes[i]);
    }
    allNodes.clear();
    root->addInOrder(allNodes);
}

void masterSyllablesList::makeAlphabetical()
{
    root->clearPointers();
    isSortedOnWrongCount = false;
    for (int i=0; i<allNodes.size(); i++)
    {
        if (allNodes[i] != root)
            root->addValueOnSyllable(allNodes[i]);
    }
}

bool masterSyllablesList::hasNoValues()
{
    cout << "masterSyllablesList::hasNoValues" << endl;
    return root->hasNoValues();
}

void masterSyllablesList::print()
{
    cout << "masterSyllablesList::print" << endl;
    root->printInOrder();
}

int masterSyllablesList::getSyllableWCount(int syllableToGet)
{
    cout << "masterSyllablesList::getSyllableWCount" << endl;
    return allNodes[syllableToGet]->getSyllableWCount();
}

int masterSyllablesList::getSyllableWCount(const std::string syllableToFind)
{
    if (isSortedOnWrongCount)
        makeAlphabetical();
    cout << "masterSyllablesList::getSyllableWCount" << endl;
    return root->findNode(syllableToFind)->getSyllableWCount();
}

masterSyllablesList::~masterSyllablesList()
{
    cout << "Destroying MSL" << endl;
    delete root;
}

int main(int argc, char const *argv[]) {
    masterSyllablesList MSL;
    MSL.addSyllables({"AH0","B","D"});
    cout << MSL.getSyllableWCount("AH0") << endl;
    MSL.addToTotal("AH0",5);
    cout << MSL.getSyllableWCount("AH0") << endl;
    MSL.addToTotal("D",3);
    MSL.print();
    MSL.sortList();
    MSL.print();
    MSL.addToTotal("B",15);
    MSL.print();
    MSL.sortList();
    MSL.print();
    return 0;
}
