#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include <iostream>

const int MAX_BUF = 4096; //Declare max buffer size, must be the same on client and server

int main()
{
    /* open, read, and display the message from the FIFO */
    char buf[MAX_BUF]; //Create an array in memory to read the pipe into
    std::string lastInput = ""; //Create a string to hold the last read info
    std::string buffer = ""; //Creat a string to hold the current buffer to create the speack command with
    const std::string SSGFIFO = "/tmp/SSGFIFO"; //Declare the name of the pipe
    while (1) //While the program runs
    {
    	int fileDescriptor = open(SSGFIFO.c_str(), O_RDONLY); //Open the pipe in read only mode
    	read(fileDescriptor, buf, MAX_BUF); //Read the pipe and store it in the char buffer
    	close(fileDescriptor); //Close the pipe so that the main program can write data to it
    	buffer = buf; //Set the buffer string to the content of the buffer
    	if (buffer != lastInput) //If something new to say
    	{
    		printf("Received: %sEND\n", buf); //Print what has been read in to check for garbled data
    		std::string command = "espeak \"" + buffer + '\"'; //create the command to call the speak program to say the buffer contents
    		system(command.c_str()); //Execute the command using the system shell
            lastInput = buffer; //Only update the lastInput if something changed
    	}
    	sleep(0.05); //Limit pipe reading to 20Hz
    }
    return 0; //Tell OS program sucessful
}
