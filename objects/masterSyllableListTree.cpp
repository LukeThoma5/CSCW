#include "../headers/masterSyllableListTree.h"

using namespace std;

//#define MSLTREEDEBUG Uncomment to allow function Sequence printing

std::vector<std::string> masterSyllablesListTree::findUniqueSyllables(const std::vector<std::string>& inSyllables)
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::findUniqueSyllables" << endl;
    #endif
    vector<string> returnVector;
    for (int i=0, inSyllSize=inSyllables.size(); i<inSyllSize; ++i)
    {
        try
        //Try to find the node, if the node can't be found -1 is thrown
        {
            root->findNode(inSyllables[i]);
        }
        catch (int toCatch)
        //If not found
        {
            if (toCatch == -1) //If expected throw value
                returnVector.push_back(inSyllables[i]); //ADd the syllables to the unique values
        }
    }
    return returnVector;
}


void masterSyllablesListTree::addSyllables(const std::vector<std::string>& inSyllables)
{
    if (isSortedOnWrongCount) //If not sorted on syllable value
        makeAlphabetical(); //Sort it on syllable value
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::addSyllables" << endl;
    #endif
    if (root) //If tree  initialised
    {
        vector<string> unique = findUniqueSyllables(inSyllables); //Find the unique syllables
        for (int i=0, uniqueSize=unique.size(); i<uniqueSize; ++i)
        //For every syllable to add
        {
            //cout << "Adding syllable with value " << unique[i] << endl;
            syllableNode* toAdd = new syllableNode(unique[i],0,1); //Create a new syllableNode
            allNodes.push_back(toAdd); //Add it to the allNodes list
            root->addValueOnSyllable(toAdd); //Add the node to the tree based of syllable value
        }
    }
    else
    //If no root
    {
        if (inSyllables.size() > 0)
        //As long as there is a syllable to make the root
        {
            root = new syllableNode(inSyllables[0],0,1); //Initialise root
            allNodes.push_back(root); //Add the root to allNodes
            //Slice the old list
            if (inSyllables.size() > 1) //If more than just the root
            {
                vector<string> nonRootToAdd; //List to store all the other syllables
                for (int i=1, inSyllSize=inSyllables.size(); i<inSyllSize; ++i)
                    nonRootToAdd.push_back(inSyllables[i]); //Add all values but the root to the list so that they can be added again
                //Add the remaining syllables
                addSyllables(nonRootToAdd);
            }
        }
    }
}

void masterSyllablesListTree::addToTotal(const std::string& syllableToFind, const int& amountToInc)
{
    //Ensure tree sorted on syllable
    if (isSortedOnWrongCount)
        makeAlphabetical();
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::addToTotal" << endl;
    #endif
    try
    {
        if (root) //As long as there is a root
        {
            root->findNode(syllableToFind)->addToTotal(amountToInc); //Find the node in the tree and increment its wrong count
        }
        else
        {
            //If no root
            syllableNode* toAdd = new syllableNode(syllableToFind,amountToInc,1.0); //Create a root node
            allNodes.push_back(toAdd); //Add the node to the list of all nodes
            root = toAdd; //Set the root to the new syllableNode
        }
    }
    catch (int i)
    //If no node has that syllable
    {
        if (i==-1)
        {
            syllableNode* toAdd = new syllableNode(syllableToFind,amountToInc,1.0); //Create a syllable with wrongCount of the amount to add
            allNodes.push_back(toAdd); //Add to the list
            root->addValueOnSyllable(toAdd); //Add to the tree
        }
    }
}

int masterSyllablesListTree::size()
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::size" << endl;
    #endif
    return allNodes.size(); //Return the length of the list
}

void masterSyllablesListTree::sortList()
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::sortList" << endl;
    #endif
    if (root) //If we have a tree
    {
        root->clearPointers(); //Clear all connections in the tree (all nodes still in the list)
        isSortedOnWrongCount = true; //Set the flag so it wont try and operate on the wrong value
        for (int i=0, allNodesSize=allNodes.size(); i<allNodesSize; ++i)
        //For every syllableNode created
        {
            if (allNodes[i] != root) //As long as it isn't the root
                root->addValueOnWrongCount(allNodes[i]); //Add the node to the tree based off wrongCount
        }
        //Now all nodes are back in the tree so its safe to remove the node pointers from the list
        allNodes.clear();
        root->addInOrder(allNodes); //Do an inorder traversel and add your node pointer to the allNodes list
        //allNodes now in order
        //To reduce tree restructures the tree is not made alphabetical until needed as there might be consecutive calls to sortList
    }
}

void masterSyllablesListTree::makeAlphabetical()
{
    if (root) //Aslong as we have a root
    {
        root->clearPointers(); //Remove all the pointers on the nodes
        isSortedOnWrongCount = false; //Set the flag
        for (int i=0, allNodesSize=allNodes.size(); i<allNodesSize; ++i)
        {
            if (allNodes[i] != root) //As long as it isn't the root
                root->addValueOnSyllable(allNodes[i]); //Add the current node to the tree based off syllable value
        }
    }
}

bool masterSyllablesListTree::hasNoValues()
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::hasNoValues" << endl;
    #endif
    if (root) //If there are some nodes
        return root->hasNoValues(); //Check all nodes for any wrong counts
    return false; //Return don't try and add values to 0 nodes
}

void masterSyllablesListTree::print()
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::print" << endl;
    #endif
    if (root)
        root->printInOrder(); //In-order traversel with printing
}

int masterSyllablesListTree::getSyllableWCount(int syllableToGet)
{
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::getSyllableWCount" << endl;
    #endif
    return allNodes[syllableToGet]->getSyllableWCount(); //Use the int to translate to a node and get its wrong count
}

int masterSyllablesListTree::getSyllableWCount(const std::string syllableToFind)
{
    if (isSortedOnWrongCount) //Ensure sorted on syllable
        makeAlphabetical();
    #ifdef MSLTREEDEBUG
    cout << "masterSyllablesList::getSyllableWCount" << endl;
    #endif
    if (root)
        return root->findNode(syllableToFind)->getSyllableWCount(); //Find the syllableNode based of syllable and extract the wrongCount
    return 0; //If called on an empty MSL return 0
}

masterSyllablesListTree::~masterSyllablesListTree()
{
    cout << "Destroying MSL" << endl;
    delete root; //Deleting a node deletes all its children
}
