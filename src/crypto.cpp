#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../headers/randng.h" //random number generator header

using namespace std;

string makeSalt()
{
	string salt;
	for (int i=0; i<256; i++) //Add 256 characters (output size of SHA256)
	{
		salt += static_cast<char>(randNG(65,128)); //Cast the int to a char then Add the ASCII character to the string
	}
	cout << salt << endl;
	return salt;
}

string saltPassword(const string& password, const string& salt)
{
	string saltedPass = "";//Return string, password + salt
	//Cache the string sizes due to many comparisons
	int passSize = password.size();
	int saltSize = salt.size();
	//Default some values to remove need of else statements
	bool addMoreSalt = true;
	int end = passSize;
	if (passSize != saltSize) //If we can't go to the end of both without one overruning
	{
		if (passSize > saltSize) //If the password is greater than 256 (unlikely)
		{
			end = saltSize; //Set the end to 256
			addMoreSalt = false; //Tell it to add the rest of the password after salting as best as possible
		}
	}

	for (int i=0; i<end; ++i) //For the length of the smallest string
	{
		saltedPass += password[i] + salt[i]; //Add the password char and salt char to the saltedPassword
	}

	if (passSize != saltSize) //If we need to add some back
		if (addMoreSalt)
			saltedPass += salt.substr(end,saltSize); //Add the rest of the salt
		else
			saltedPass += password.substr(end,passSize); //Add the rest of the password

	return saltedPass;
}

vector<string> readPasswordFile()
{
	ifstream passFile("./Data/userPassword.hash"); //Open password file that is stored in Data subdir
	vector<string> retVec;
	string line;
	while (getline(passFile, line)) //while not at end of file
	{
		retVec.push_back(line); //Add the line read in to the vector
	}
	return retVec;
}
