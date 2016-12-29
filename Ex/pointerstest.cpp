#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	int i=0;
	int* ip = &i;

	*ip += 1;
	cout << *ip << endl;
	return 0;
}