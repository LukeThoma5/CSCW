#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../headers/randng.h" //random number generator header

using namespace std;

string makeSalt()
{
	string salt;
	for (int i=0; i<256; i++)
	{
		salt += static_cast<char>(randNG(65,128));
	}
	cout << salt << endl;
	return salt;
}

string saltPassword(const string& password, const string& salt)
{
	string saltedPass = "";
	int passSize = password.size();
	int saltSize = salt.size();
	bool addMoreSalt = true;
	int end = passSize;
	if (passSize != saltSize)
	{
		if (passSize > saltSize)
		{
			end = saltSize;
			addMoreSalt = false;
		}
	}

	for (int i=0; i<end; i++)
	{
		saltedPass += password[i] + salt[i];
	}

	if (passSize != saltSize)
		if (addMoreSalt)
			saltedPass += salt.substr(end,saltSize);
		else
			saltedPass += password.substr(end,passSize);

	return saltedPass;
}

vector<string> readPasswordFile()
{
	ifstream passFile("./Data/userPassword.hash");
	vector<string> retVec;
	string line;
	while (getline(passFile, line))
	{
		retVec.push_back(line);
	}
	return retVec;
}
