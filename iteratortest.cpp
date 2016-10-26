#include <iostream>
#include <vector>

using namespace std;

class test {
public:
		int i=0;

		test() {;}
};

class test2 : public test {
public:
	test2() : test() {i=2;}
};

int main(int argc, char const *argv[])
{
	vector<int> myVec = {1,2,3,4,5,6,7,8,9};
	cout << myVec[4] << endl;
	myVec.erase(myVec.begin()+4);
	cout << myVec[4] << endl;

	vector<test*> testVec;
	testVec.push_back(new test2);
	testVec.push_back(new test2);
	testVec.erase(testVec.begin()+1);
	return 0;
}