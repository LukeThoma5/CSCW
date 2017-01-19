//Needed for say functin and pipe functionality
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

void say(const string& sentence)
{
    const std::string pipeName = "/tmp/SSGFIFO"; //The pipes location in the file structure
    //Pipe created by main function
    int fileDescriptor = open(pipeName.c_str(), O_WRONLY); //Open the pipe for writing
    //write to the fileDescriptor opened above the array of char in sentence with a max size of 100
    write(fileDescriptor,sentence.c_str(),100); //Write a maximum of 100 characters to the buffer
    close(fileDescriptor); //Close the pipe so other program can read from it
}

void speak(const string& wordToSay, const bool isCorrect)
{
	if (isCorrect)
		say("That is correct, please spell the word " + wordToSay);
	else
		say("Please spell the word " + wordToSay);
}

string seperateWord(const string& wordToSep)
{
	cout << wordToSep << endl;
	string retString;

	for (int i=0; i<wordToSep.size(); ++i) //For every letter in the word
	{
		retString = retString + wordToSep[i] + ' '; //Add the letter with a space to seperate
	}

	cout << retString << endl;
	return retString;
}
