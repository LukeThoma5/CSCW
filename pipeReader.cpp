#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include <iostream>

#define MAX_BUF 4096

int main()
{
    

    /* open, read, and display the message from the FIFO */
    char lastbuf[MAX_BUF];
    std::string lastInput = "";
    std::string buffer = "";
    while (1)
    {
    	int fd;
    	std::string myfifo = "/tmp/myfifo";
    	char buf[MAX_BUF];
    	fd = open(myfifo.c_str(), O_RDONLY);
    	read(fd, buf, MAX_BUF);
    	close(fd);
    	buffer = buf;
    	if (buffer != lastInput)
    	{
    		printf("Received: %sEND\n", buf);
    		std::string command = "espeak \"";
    		command += buf;
    		command += '\"';
    		std::cout << command << std::endl;
    		system(command.c_str());
    	}
    	
    	lastInput = buf;
    	sleep(0.05);
    }
    
    

    return 0;
}