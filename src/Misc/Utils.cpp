#include <string>
using namespace std;

string makeUpperCase(const string& attempt)
{
    //Should be ok but some shortcuts were made for efficency. May cause errors
    string upperString;
    for (unsigned int i=0, end=attempt.size(); i<end; ++i)
    {
        int charInt = static_cast<int>(attempt[i]); //Cast the current character to an int
        if (charInt>96) //If later than 'a'
        {
            charInt -= 32; //Remove 32 to make it 'A'
        }
        upperString += static_cast<char>(charInt); //Cast int back to character
    }
    return upperString;
}
