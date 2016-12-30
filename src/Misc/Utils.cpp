#include <string>
using namespace std;

string makeUpperCase(const string& attempt)
{
    //Should be ok but some shortcuts were made for efficency. May cause errors
    string upperString;
    for (unsigned int i=0; i<attempt.size(); i++)
    {
        char currentChar = attempt[i];
        int charInt = static_cast<int>(currentChar);
        if (charInt>96)
        {
            charInt -= 32;
        }
        upperString += static_cast<char>(charInt);
    }
    return upperString;
}
