#include <iostream>
#include <string>
#include <vector>

using namespace std;

class node
{
	node* left = nullptr;
	node* right = nullptr;
	int value = 0;
public:
	void setLeft(node* leftp) {left=leftp;}

	void setRight(node* rightp) {right=rightp;}

	node* getLeft() {return left;}

	node* getRight() {return right;}

	int getValue() {return value;}

	node(int val)
	{
		value=val;
		left = nullptr;
		right = nullptr;
	}

	node(int val, node* leftp, node* rightp)
	{
		value=val;
		left=leftp;
		right=rightp;
	}

	void addValue(int val)
	{
		if (val < value)
			if (left == nullptr)
				left = new node(val);
			else
				left->addValue(val);
		else
			if (right == nullptr)
				right = new node(val);
			else
				right->addValue(val);
	}

	void printChildren()
	{
		cout << "--------------------" << endl;
		if (left)
		{
			cout << value <<  ": Left value " << left->getValue() << endl;
			left->printChildren();
		}
		if (right)
		{
			cout << value << ": Right value " << right->getValue() << endl;
			right->printChildren();
		}
	}

	

	void printPreOrder()
	{
		cout << value << ",";
		if (left)
			left->printPreOrder();
		if (right)
			right->printPreOrder();
	}

	void printInOrder()
	{
		if (left)
			left->printInOrder();
		cout << value << ",";
		if (right)
			right->printInOrder();
	}

	void printPostOrder()
	{
		if (left)
			left->printPostOrder();
		if (right)
			right->printPostOrder();
		cout << value << ",";
	}

	node* findValue(int val)
	{
		cout << "Searching node " << value << " for the value " << val << endl;
		if (val == value)
			return this;
		if (val < value)
			if (left)
				return left->findValue(val);
			else
				throw  -1;
		if (val > value)
			if (right)
				return right->findValue(val);
			else
				throw -1;
	}

	~node()
	{
		cout << "Entered " << value << " destructor!" << endl;
		if (left)
		{
			//left->~node();
			delete left;
		}
		if (right)
		{
			//right->~node();
			delete right;
		}
	}

};

node* makeTree(const vector<int>& list)
{
	node* root = new node(list[0]);
	for (int i=1; i<list.size(); i++)
	{
		root->addValue(list[i]);
	}
	return root;
}

int main(int argc, char const *argv[])
{
	vector<int> treeValues = {17,8,4,12,22,19,14,5,30,25};
	node* myRoot = makeTree(treeValues);
	cout << myRoot->getValue() << endl;
	
	myRoot->printChildren();

	myRoot->printPreOrder();
	cout << endl;
	myRoot->printInOrder();
	cout << endl;
	myRoot->printPostOrder();
	cout << endl;

	myRoot->findValue(12)->printChildren();

	try
	{
		node* test = myRoot->findValue(1);
		test->printChildren();
	}
	catch (int i)
	{
		if (i==-1)
			cout << "Not found!" << endl;
	}

	delete myRoot;
	
	return 0;
}