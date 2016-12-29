#include "../headers/syllableNode.h"

using namespace std;

void syllableNode::addToTotal(const int& amountToInc)
{
    cout << "syllableNode::addToTotal" << endl;
    wrongCount += amountToInc;
}

syllableNode::syllableNode(const std::string& Syllable, const int WrongCount, const float Weight)
{
    cout << "syllableNode::syllableNode" << endl;
    syllable=Syllable;
    wrongCount=WrongCount;
    weight=Weight;
}

bool syllableNode::hasNoValues()
{
    cout << "syllableNode::hasNoValues" << endl;
    if (wrongCount == 0)
        return true;
    return false;
}

int syllableNode::getSyllableWCount() const
{
    cout << "syllableNode::getSyllableWCount" << endl;
    return wrongCount;
}

std::string syllableNode::getSyllableValue() const
{
    cout << "syllableNode::getSyllableValue" << endl;
    return syllable;
}

void syllableNode::addValueOnSyllable(syllableNode* toAdd)
{
    cout << "syllableNode::addValueOnSyllable" << endl;
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
    cout << "syllableNode::addValueOnWrongCount" << endl;
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
    cout << "syllableNode::addInOrder" << endl;
    if (left)
        left->addInOrder(nodes);
    nodes.push_back(this);
    if (right)
        right->addInOrder(nodes);
}

syllableNode* syllableNode::findNode(const string& toFind)
{
    cout << "syllableNode::findNode" << endl;
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
    cout << "syllableNode::~syllableNode" << endl;
    if (left)
        delete left;
    if (right)
        delete right;
}

void syllableNode::printInOrder()
{
    cout << "syllableNode::printInOrder" << endl;
    if (left)
        left->printInOrder();
    cout << syllable << " with wrongCount " << wrongCount << endl;
    if (right)
        right->printInOrder();
}
