#include "../headers/syllableNode.h"

using namespace std;

//#define syllableNodeDEBUG //Uncomment to enable debug messages

void syllableNode::addToTotal(const int& amountToInc)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addToTotal" << endl;
    #endif
    wrongCount += amountToInc; //Increase wrongCount by parameter
}

syllableNode::syllableNode(const std::string& Syllable, const int WrongCount, const float Weight)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::syllableNode" << endl;
    #endif
    syllable=Syllable;
    wrongCount=WrongCount;
    weight=Weight;
}

bool syllableNode::hasNoValues()
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::hasNoValues" << endl;
    #endif
    if (wrongCount == 0) //If no values
        return true; //Add random values
    return false;
}

int syllableNode::getSyllableWCount() const
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::getSyllableWCount" << endl;
    #endif
    return wrongCount;
}

std::string syllableNode::getSyllableValue() const
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::getSyllableValue" << endl;
    #endif
    return syllable;
}

void syllableNode::addValueOnSyllable(syllableNode* toAdd)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addValueOnSyllable" << endl;
    #endif
    string comp = toAdd->getSyllableValue(); //Cache string value to compare with
    if (comp<syllable) //If lower in alphabet add to left subtree
        if (left==nullptr) //If no node on left
            left=toAdd; //Set it to left
        else
            left->addValueOnSyllable(toAdd); //Add to left subtree
    else //If higher in alphabet
        if (right==nullptr) //If no subtree
            right=toAdd; //Make subtree
        else
            right->addValueOnSyllable(toAdd); //Add the subtree
}

void syllableNode::addValueOnWrongCount(syllableNode* toAdd)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addValueOnWrongCount" << endl;
    #endif
    int comp = toAdd->getSyllableWCount(); //Cache the wrongCount
    if (comp>=wrongCount) //If the node to add has a greater wrong count add on left subtree (So inorder traversal prints in decending order)
        if (left==nullptr) //If no subtree
            left=toAdd; //Start the subtree
        else
            left->addValueOnWrongCount(toAdd); //Add to the subtree
    else
        if (right==nullptr) //If no subtree
            right=toAdd; //Start the subtree
        else
            right->addValueOnWrongCount(toAdd); //Add to the subtree
}

void syllableNode::addInOrder(std::vector<syllableNode*>& nodes)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addInOrder" << endl;
    #endif
    if (left)
        left->addInOrder(nodes); //Add all the nodes recursively on the left hand side
    nodes.push_back(this); //Add this node to the list
    if (right) //Add all the nodes recursively on the right sub tree
        right->addInOrder(nodes);
}

syllableNode* syllableNode::findNode(const string& toFind)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::findNode" << endl;
    #endif
    if (toFind == syllable) //If we are the searched for syllable return our position
        return this;
    if (toFind < syllable) //If lower in alphabet check left tree
        if (left)
            return left->findNode(toFind); //Search left subtree and return the result up the stack
        else
            {/*cout << "throwing"<<endl;*/throw -1;} //If no sub tree, node not in tree throw an error
    if (right)
        return right->findNode(toFind); //Search right subtree and return the result up the stack
    //No else needed, if it has gotten here it will always need to do this statement
    /*cout << "throwing"<<endl;*/throw -1; //If no right subtree throw
}

void syllableNode::clearPointers()
{
    if (left) //If not null
    {
        left->clearPointers(); //Clear the subtree of their pointers
        left=nullptr; //Clear the left pointer
    }
    if (right) //If not null
    {
        right->clearPointers(); //Recursively clear the subtree of their pointers
        right=nullptr; //Clear right pointer
    }

}

syllableNode::~syllableNode()
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::~syllableNode" << endl;
    #endif
    if (left)
        delete left; //Recursively delete right subtree
    if (right)
        delete right; //Recursively delete left subtree
}

void syllableNode::printInOrder()
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::printInOrder" << endl;
    #endif
    if (left) //If left subtree
        left->printInOrder(); //Print left subtree
    cout << syllable << " with wrongCount " << wrongCount << endl; //Print current node value
    if (right) //If right subtree
        right->printInOrder(); //Print right subtree
}
