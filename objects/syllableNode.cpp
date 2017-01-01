#include "../headers/syllableNode.h"

using namespace std;

//#define syllableNodeDEBUG //Uncomment to enable debug messages

void syllableNode::addToTotal(const int& amountToInc)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addToTotal" << endl;
    #endif
    wrongCount += amountToInc;
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
    if (wrongCount == 0)
        return true;
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
    string comp = toAdd->getSyllableValue();
    if (comp<syllable)
        if (left==nullptr)
            left=toAdd;
        else
            left->addValueOnSyllable(toAdd);
    else
        if (right==nullptr)
            right=toAdd;
        else
            right->addValueOnSyllable(toAdd);
}

void syllableNode::addValueOnWrongCount(syllableNode* toAdd)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addValueOnWrongCount" << endl;
    #endif
    int comp = toAdd->getSyllableWCount();
    if (comp>=wrongCount)
        if (left==nullptr)
            left=toAdd;
        else
            left->addValueOnWrongCount(toAdd);
    else
        if (right==nullptr)
            right=toAdd;
        else
            right->addValueOnWrongCount(toAdd);
}

void syllableNode::addInOrder(std::vector<syllableNode*>& nodes)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::addInOrder" << endl;
    #endif
    if (left)
        left->addInOrder(nodes);
    nodes.push_back(this);
    if (right)
        right->addInOrder(nodes);
}

syllableNode* syllableNode::findNode(const string& toFind)
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::findNode" << endl;
    #endif
    if (toFind == syllable)
        return this;
    if (toFind < syllable)
        if (left)
            return left->findNode(toFind);
        else
            {cout << "throwing"<<endl;throw -1;}
    if (right)
        return right->findNode(toFind);

    cout << "throwing"<<endl;throw -1;
}

void syllableNode::clearPointers()
{
    if (left)
    {
        left->clearPointers();
        left=nullptr;
    }
    if (right)
    {
        right->clearPointers();
        right=nullptr;
    }

}

syllableNode::~syllableNode()
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::~syllableNode" << endl;
    #endif
    if (left)
        delete left;
    if (right)
        delete right;
}

void syllableNode::printInOrder()
{
    #ifdef syllableNodeDEBUG
    cout << "syllableNode::printInOrder" << endl;
    #endif
    if (left)
        left->printInOrder();
    cout << syllable << " with wrongCount " << wrongCount << endl;
    if (right)
        right->printInOrder();
}
