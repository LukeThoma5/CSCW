#include <vector>

using namespace std;

vector<int> splitVector(const vector<int>& inVector, int mode)
{
	int start;
	int end;
	if (!mode) //if 0
	{
		start = 0;
		end = inVector.size()/2;
	}
	else
	{
		//if not 0
		start = inVector.size()/2;
		end = inVector.size();
	}
	vector<int> returnVector;
	for (int i=start; i<end; i++)
		returnVector.push_back(inVector[i]); //fill the vector with the values from original
	return returnVector;
}
