#include <iostream>

#include <string>
using namespace std;
int main(int argc, char const *argv[])
{
	int a=5;
	std::cout << a << std::endl;

	string first = "applec";
	string second = "applee";
	cout << first.compare(second) << endl;

	cout << second.compare(first) << endl;

	return 0;
}