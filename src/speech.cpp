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
    std::string myfifo = "/tmp/SSGFIFO"; //The pipes location in the file structure
    /* create the FIFO (named pipe) */
    mkfifo(myfifo.c_str(), 0666);

	/* write my message to the Pipe (FIFO) */
    int fileDescriptor = open(myfifo.c_str(), O_WRONLY);
    //write to the fileDescriptor opened above the array of char in sentence with a max size of 100
    write(fileDescriptor,sentence.c_str(),100); //Write a maximum of 100 characters to the buffer
    close(fileDescriptor); //Close the pipe

    /* unlick the FIFO */
    unlink(myfifo.c_str());
}

void speak(const string& wordToSay, const bool isCorrect)
{
	/*
	//string Command = "flite -voice slt -t \"Please spell the word " + wordToSay + "\"";
	string Command = "flite -t \"Please spell the word " + wordToSay + "\"";
    system(Command.c_str());
	*/
	if (isCorrect)
		say("That is correct, please spell the word " + wordToSay);
	else
		say("Please spell the word " + wordToSay);
}

string seperateWord(const string& wordToSep)
{
	cout << wordToSep << endl;
	string retString;

	for (int i=0; i<wordToSep.size(); i++)
	{
		retString = retString + wordToSep[i] + ' ';
	}

	cout << retString << endl;
	return retString;
}
