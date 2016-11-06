#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include <unistd.h>

//#define HEADLESS //Uncomment to remove printing to console
#define limitHz //Uncomment to remove frequency limiting
//#define debug //Uncommment to get a tick on each complete amount of Hz

//Luke Foreman 05/11/16 AQA Microcontroller Emulator

using namespace std;

int maskOperand(int& instruction)
{
	int opcode = (instruction & 65280); //Bitwise AND with 0xFF00
	#ifndef HEADLESS
	cout << "Operand: " << hex << opcode << endl; //Print to console
	#endif
	return opcode; //Returns the opcode
}

int maskOpcode(int& instruction)
{
	int operand = (instruction & 255); //Bitwise AND with 0x00FF to get last 2 digits
	#ifndef HEADLESS
	cout << "Opcode: " << hex << operand << endl; //Print to console
	#endif
	return operand; //Returns the operand
}

void JMP(int& operand, int* sregisters)
{
	sregisters[1] = operand-1; //Sets the PC to the value of the operand-1. The -1 is nessissary as the PC is always incremented. Potential for improvement
}

void CALL(int& operand, int* sregisters, int* registers)
{
	registers[sregisters[2]] = sregisters[1]; //Add the PC to stack
	//cout << registers[sregisters[2]] << endl;
	sregisters[2]--; //Set the pointer to the next location to put memory in
	//cout << registers[sregisters[2]] << endl;
	JMP(operand, sregisters); //Execute a jump to the called subroutine
}

void checkZero(int* sregisters)
{
	if (sregisters[0] == 0) //If W is 0
	{
		//cout << "Setting zero!" << endl;
		sregisters[3] = (sregisters[3] | 4); //Set the Z bit to 1
	}
	else
	{
		//cout << "Clearing zero!" << endl;
		sregisters[3] = (sregisters[3] & 251); //Clear the Z bit
	}
}

void checkCarry(int* sregisters)
{
	if (sregisters[0] > 255) //If W is greater than 255 eg greater than an 8 bit int can handle
	{
		sregisters[0] -= 255; //Remove a full 0xFF, this emulates what would have happened if an unsigned int was 8 bits big. This is required as C++ mandates at least 16 bits for ints.
		sregisters[3] = (sregisters[3] | 1); //Sets the carry bit
	}
	else
	{
		if (sregisters[0] < 0) //If W has underflowed
		{
			//cout << "The value of W is: " << sregisters[0] << endl;
			sregisters[0] += 255;
			sregisters[3] = (sregisters[3] | 1); //Set the carry bit
		}
		else //If it hasn't over or underflowed then clear the bit
			sregisters[3] = (sregisters[3] & 254); //Clear the carry bit
	}
}

void ADDW(int& operand, int* sregisters)
{
	//cout << "Adding " << hex << operand << " to " << hex << sregisters[0] << endl;
	sregisters[0] = sregisters[0] + operand; //Add the operand to W
	//cout << sregisters[0] << endl;
	checkZero(sregisters); //Update Z bit
	//cout << sregisters[0] << endl;
	checkCarry(sregisters); //Update C bit
	//cout << sregisters[0] << endl;
}

void ANDW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] & operand); //Bitwise AND W with the operand, store the result in W
	checkZero(sregisters);
	checkCarry(sregisters);
}

void ORW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] | operand); //Bitwise OR W with the operand, store the result in W
	checkZero(sregisters);
	checkCarry(sregisters);
}

void XORW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] ^ operand); //Bitwise XOR W with the operand, store the result in W
	checkZero(sregisters);
	checkCarry(sregisters);
}

void SUBW(int& operand, int* sregisters){
	sregisters[0] = (sregisters[0] - operand); //Subtract operand from W
	checkZero(sregisters);
	checkCarry(sregisters);
}

void INC(int& operand, int* registers, int* sregisters)
{
	registers[operand]++; //Increment memory location given by operand by 1;
	checkZero(sregisters);
}

void DEC(int& operand, int* registers, int* sregisters)
{
	registers[operand]--; //Decrease memory location given by operand by 1;
	checkZero(sregisters);
}

void MOVW(int& operand, int* sregisters)
{
	sregisters[0] = operand; //Copy the value of the operand into W
	checkZero(sregisters);
}

void MOVWR(int& operand, int* registers, int* sregisters)
{
	registers[operand] = sregisters[0]; //Copy the value of W into the memory location specified by operand.
	checkZero(sregisters);
}

void MOVRW(int& operand, int* registers, int* sregisters)
{
	sregisters[0] = registers[operand]; //Copy the value stored in the memory location specified by the operand into W
	checkZero(sregisters);
}

void RET(int* sregisters, int* registers)
{
	//cout << "Jumping to location1: " << registers[sregisters[2]] << endl;
	sregisters[2]++; //Set the stack pointer to the current top of the stack
	//cout << "Jumping to location: " << registers[sregisters[2]] << endl;
	sregisters[1] = registers[sregisters[2]]; //JMP to location on the top of the stack
	registers[sregisters[2]] = 0x0000; //Clear the top of the stack
}

void JPZ(int& operand, int* sregisters)
{
	if (sregisters[3] & 4) //If Z bit is 1
	{
		JMP(operand,sregisters); //Do a normal jump
	}
}

void JPC(int& operand, int* sregisters)
{
	if (sregisters[3] & 1) //If C bit is 1
	{
		JMP(operand,sregisters); //Do a normal jump
	}
}

void decodeInstruction(int& instruction, int* registers, int* sregisters)
{
	int opcode = maskOperand(instruction); //mask instruction to get opcode
	int operand = maskOpcode(instruction); //mask instruction to get operand
	string OPstring;
	switch (opcode) //Evaluate opcode to int so it can jump straight to the correct code without going through a bunch of ifs
	{
		case 0x0000: //NOP and RET have same opcode :(, RET is 0x0008 anything else is a NOP
			if (instruction == 0x0008)
			{
				#ifndef HEADLESS
				OPstring = "RET"; 
				#endif
				RET(sregisters,registers); 
				break;
			}
			else
			{
				#ifndef HEADLESS
				OPstring = "NOP";
				#endif
				break;
			}

		//On opcode, write the operation to the screen, call the appropriate function and break to prevent fallthrough.
		case 0x2000: OPstring = "CALL"; CALL(operand,sregisters,registers); break;

		case 0x0A00: OPstring = "INC"; INC(operand, registers, sregisters); break;

		case 0x0300: OPstring = "DEC"; DEC(operand, registers, sregisters); break;

		case 0x3E00: OPstring = "ADDW"; ADDW(operand,sregisters); break;

		case 0x3900: OPstring = "ANDW"; ANDW(operand, sregisters); break;

		case 0x3C00: OPstring = "SUBW"; SUBW(operand, sregisters); break;

		case 0x3800: OPstring = "ORW"; ORW(operand, sregisters); break;

		case 0x3A00: OPstring = "XORW"; XORW(operand, sregisters); break;

		case 0x2800: OPstring = "JMP"; JMP(operand,sregisters); break;

		case 0x2900: OPstring = "JPZ"; JPZ(operand,sregisters); break;

		case 0x2A00: OPstring = "JPC"; JPC(operand,sregisters); break;

		case 0x0100: OPstring = "MOVWR"; MOVWR(operand, registers, sregisters); break;

		case 0x3000: OPstring = "MOVW"; MOVW(operand, sregisters); break;

		case 0x0800: OPstring = "MOVRW"; MOVRW(operand, registers, sregisters); break;

		case 0xEE00: cout << "HALT" << endl; exit(0); break;

		default: cout << "Invalid instruction" << endl; break;
	}
	#ifndef HEADLESS
	cout << "Current Instruction: " << OPstring << endl;
	#endif
}


void printRegisters(int* reg, int* sreg)
{
	cout << "W : " << hex << sreg[0] << "                    "; //Output W in hex form and padd the line ready for PC
	cout << "PC: " << hex << sreg[1] << '\n'; //Print PC in hex form and start a new line but don't flush to the screen
	cout << "SP: " << hex << sreg[2] << "                    "; // " "
	cout << "SR: " << hex << sreg[3] << "\n "; // " " start a new line and indent it one so 0: is in line with the other lines
	for (int i=0; i<16; i++) //For every row
	{
		cout << hex << i*16 << "  "; //Print out the row name
		for (int j=0; j<16; j++) //For every collumn
		{
			int value = *(reg+(i*16+j)); //Dereference the pointer which was updated to point to the current memory location we are interseted in.
			string padding = ""; //Pad the values so the grid is uniform
			if (value < 4096)
			{
				if (value < 256)
				{
					if (value < 16)
					{
						padding = "000";
					}
					else
					{
						padding = "00";
					}
				}
				else
				{
					padding = "0";
				}
			}
			cout << padding << hex << value << " "; //Print out the value of the memory location
		}
		cout << '\n'; //Start a new line but don't flush
	}
	cout <<	"                                            TRSA PRTA TRSB PRTB TRSC PRTC PRE  TMR" << endl; //Add the final line with Reserved locations memory labels and FINALLY flush to screen
}

void loadFile(vector<string>& filelines)
{
	ifstream file("assembly.txt"); //Create infilestream from a file called assembly.txt which contains the code
	string currentline; //String object to hold the current line
	while (getline(file, currentline)) //Will continue to get lines until EOF when getline returns 0
	{
		filelines.push_back(currentline); //Add the current line to the string vector for later decoding
	}
}

vector<string> split(const string& s)
{
	vector<string> elems; //Resizable array to hold the opcodes, operands and comments
	stringstream ss(s); //Creates a string stream object from the passed in line
	string item; //temp string to hold the current item
	while (getline(ss,item,' ')) //While still text to get from the line
	{
		if (!item.empty()) //As long as the element isnt ""
		{
			elems.push_back(item); //Add it to the elements list
		}
	}
	return elems; //Return the delimited elements
}

int getOpcode(string& opcode)
{
	//Reverse of decode instruction, switch can't be used as a multiple character string can't be evaluated to an int.
	cout << "getOpcode:" << opcode << endl;
	if (opcode=="NOP") {return 0x00;}

	if (opcode=="CALL") {return 0x2000;}

	if (opcode=="INC") {return 0x0A00;}

	if (opcode=="DEC") {return 0x0300;}

	if (opcode=="ADDW") {return 0x3E00;}

	if (opcode=="ANDW") {return 0x3900;}

	if (opcode=="SUBW") {return 0x3C00;}

	if (opcode=="ORW") {return 0x3800;}

	if (opcode=="XORW") {return 0x3A00;}

	if (opcode=="JMP") {return 0x2800;}

	if (opcode=="JPZ") {return 0x2900;}

	if (opcode=="JPC") {return 0x2A00;}

	if (opcode=="MOVWR") {return 0x0100;}

	if (opcode=="MOVW") {return 0x3000;}

	if (opcode=="MOVRW") {return 0x0800;}

	if (opcode=="RET") {return 0x0008;}

	if (opcode=="HALT") {return 0xEE00;}

	return 0xFFFF;
}

int getOperand(string& operand)
{
	cout << operand << endl;
	string data = operand.substr(2,2); //Remove the 0x
	unsigned int x = stoul(data,nullptr,16); //convert the base 16 number to an int
	cout << x << endl;
	return x;
}

bool isOperandHex(string& operand)
{
	string data = operand.substr(0,2);
	if (data == "0x") //If first 2 characters are 0x then the operand is a hex value
	{
		return true;
	}
	return false;
}

bool checkIfLabel(string& line)
{
	if (line.back() == ':') //If the last character is a : the current line is a label
	{
		cout << "Label Found" << endl;
		cout << line << endl;
		return true;
	}
	return false;
}

void assembleCode(vector<string> filelines, int* registers, vector<string>& labels, vector<int>& labelPos)
{
	int* regStart = registers; //The start of the array is saved as the register pointer is incremented throughout
	vector<int> positionsToFix; //Resizeable list to store the memory locations of instructions that require further processing, eg to add the memory location of a label
	vector<string> labelPositionsToPutin; //The same as above, this stores which label position is needed to be put in.
	for (unsigned int i=0; i<filelines.size(); i++) //For every line of the file.
	{
		cout << filelines[i] << endl; //Print the line being assembled
		vector<string> elems = split(filelines[i]); //Split the line up by whitespace
		int opcode = getOpcode(elems[0]); //get the opcode by masking it
		cout << opcode << endl;
		if (opcode) //if the opcode is not a NOP
		{
			if (opcode==0x0008 || opcode==0xEE00) //If a RET or HALT just place in the opcode as they take no operand
			{
				*registers = opcode;
				registers++;
				continue;
			}
			if (opcode != 0xFFFF and opcode != 0x2000 and opcode != 0x0A00 and opcode != 0x3000 and opcode != 0x0100 and opcode != 0x0800 and opcode != 0x2800 and opcode != 0x2900 and opcode != 0x2A00) //If not an instrunction potenitally dependent on a label
			{
				*registers = opcode + getOperand(elems[1]); //Save the instruction as the opcode + operand
				registers++; //Increase the pointer to point to next memory location to fill
				continue; //soft break, stop this execution but keep executng the loop
			}

			if (opcode == 0xFFFF) //If unknown instruction
			{
				cout << "Checking potential Label" << endl;
				if(checkIfLabel(elems[0])) //If the item is a label
				{
					labels.push_back(elems[0]); //And the label to the list of known labels
					labelPos.push_back(int(registers-regStart)); //Add the location of the label to the list so the when another instruction asks for the label it knows where to go to
					//cout << labelPos[0] << endl;
				}
				continue;
			}

			//If potentially dependant on label
			*registers = opcode; //Always set the instruction to the opcode
			if (isOperandHex(elems[1])) //If it is actually using a hex value 
			{
				*registers += getOperand(elems[1]); //just add the operand to the  instruction as normal
				registers++;
				continue;
			}

			//If dependant on a label
			positionsToFix.push_back(int(registers-regStart)); //Add the memory location to the list of items to fix
			registers++;				
			labelPositionsToPutin.push_back(elems[1] + ':'); //Add the label name that needs to be evaluated
			//cout << positionsToFix[0] << endl;
			
			
		}
		else
		{
			//if NOP eg no operand
			*registers = 0x0000;
			registers++;
		}
	}

	//Fixing all the labels
	for(int i=0; i<positionsToFix.size(); i++) //For all the locations that need fixing
	{
		cout << "Fixing memory location: " << positionsToFix[i] << endl;
		cout << "Searching for label: " << labelPositionsToPutin[i] << endl;
		for (int j=0; j<labels.size(); j++) //Search through the known labels
		{
			if (labels[j] == labelPositionsToPutin[i]) //If the needed label has a known position
			{
				cout << labels[j] << " found at location: " << labelPos[j] << endl;
				//cout << *(regStart + positionsToFix[i]);
				*(regStart + positionsToFix[i]) = *(regStart + positionsToFix[i]) + labelPos[j]; //update the troubled memory location by adding in the position of the label
				//cout << *(regStart + positionsToFix[i]) << endl;
			}
		}

	}

}

int main(int argc, char const *argv[])
{
	int freq = 2; //Default frequency if no commandline options
	if (argc == 2) //If a frequency specified in CML
		freq = atoi(argv[1]); //Set the frequency to the CML option
	vector<string> filelines; //List to store the users program in
	vector<string> labels = {"TRISA:","PORTA:","TRISB:","PORTB:","TRISC:","PORTC:","PRE:","TMR:"}; //List of all the default labels
	vector<int> labelPos = {248,249,250,251,252,253,254,255}; //And their respective memory locations
	loadFile(filelines); //Load in the users code
	int registers[257] = {0}; //int array to act as registers 0x00 to 0xFF with an extra location that contains JMP 0x00
	int sregisters[4] = {0}; //int array holding the special registers
	//to make the special registers accessible in the rest of the program make registers 4 bigger and make sregisters a pointer to memory location 256
	// 0: W
	// 1: PC
	// 2: SP
	// 3: SR
	sregisters[2] = 247; //Set the Stackpointer to the start of the stack
	const int sleeptime = 1000000/freq; //Set the amount of microseconds to sleep for
	registers[256] = 0x2800; //If PC gets one too big, set the instruction to JMP 0x00
	
	assembleCode(filelines,registers,labels,labelPos); //Assemble the code

	int pulses = 0;
	#ifdef debug
	int ontime = 0;
	#endif
	while (true) //Until you encounter a halt, tick every x times a second
	{
		#ifdef limitHz
		usleep(sleeptime); //Limit the frequency, is not active so not using cpu when not needed
		#endif
		#ifndef HEADLESS
		printRegisters(registers,sregisters); //Make the memory space visible
		#endif
		decodeInstruction(registers[sregisters[1]],registers,sregisters); //Execute the next instruction
		sregisters[1]++; //Increase the program counter
		if (registers[255]) //If the TMR is not 0
		{
			pulses++; //Increase the pulse count
			if (pulses == (1+registers[254])) //If the pulse count is equal to 1+ prescalar amount
			{
				registers[255]--; //Decrement TMR
				pulses = 0; //Reset the pulses
			}
		}
		#ifdef debug
		ontime++;
		if (ontime == freq)
		{
			cout << "Tick" << endl;
			ontime=0;
		}
		#endif
			
	}

	return 0; //Exit the program, is never used
}