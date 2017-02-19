#include "../headers/randng.h" //random number generator header
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//http://stackoverflow.com/posts/13445752/revisions
int randNG(int low, int high)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(low,high);
	return dist6(rng);
}

//modified from here http://stackoverflow.com/questions/236129/split-a-string-in-c
void split(const std::string &s,const char delim, std::vector<std::string>& elems) {
    stringstream ss(s); //create a stringstream object from the string parameter as getline requires istream object
    string item; //string to hold the newly found item
    while (getline(ss, item, delim)) { //while still data left in the stringstream read until you encounter the delimiter, store the stream in the item string, execute loop, repeat
        if (!item.empty())
            elems.push_back(item);
    }
}

vector<string> split(const std::string &s, const char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
