#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include <unistd.h>

using namespace std;

int maskOperand(int& instruction)
{
	int opcode = (instruction & 65280);
	cout << "Operand: " << hex << opcode << endl;
	return opcode;
}

int maskOpcode(int& instruction)
{
	int operand = (instruction & 255);
	cout << "Opcode: " << hex << operand << endl;
	return operand;
}

void JMP(int& operand, int* sregisters)
{
	sregisters[1] = operand-1;
}

void CALL(int& operand, int* sregisters, int* registers)
{
	registers[sregisters[2]] = sregisters[1]; //Add the PC to stack
	//cout << registers[sregisters[2]] << endl;
	sregisters[2]--;
	//cout << registers[sregisters[2]] << endl;
	JMP(operand, sregisters);
}

void checkZero(int* sregisters)
{
	if (sregisters[0] == 0)
	{
		//cout << "Setting zero!" << endl;
		sregisters[3] = (sregisters[3] | 4);
	}
	else
	{
		//cout << "Clearing zero!" << endl;
		sregisters[3] = (sregisters[3] & 251);
	}
}

void checkCarry(int* sregisters)
{
	if (sregisters[0] > 255)
	{
		sregisters[0] -= 255;
		sregisters[3] = (sregisters[3] | 1);
	}
	else
	{
		if (sregisters[0] < 0)
		{
			//cout << "The value of W is: " << sregisters[0] << endl;
			sregisters[0] += 255;
			sregisters[3] = (sregisters[3] | 1);
		}
		else
			sregisters[3] = (sregisters[3] & 254);
	}
}

void ADDW(int& operand, int* sregisters)
{
	//cout << "Adding " << hex << operand << " to " << hex << sregisters[0] << endl;
	sregisters[0] = sregisters[0] + operand;
	//cout << sregisters[0] << endl;
	checkZero(sregisters);
	//cout << sregisters[0] << endl;
	checkCarry(sregisters);
	//cout << sregisters[0] << endl;
}

void ANDW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] & operand);
	checkZero(sregisters);
	checkCarry(sregisters);
}

void ORW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] | operand);
	checkZero(sregisters);
	checkCarry(sregisters);
}

void XORW(int& operand, int* sregisters)
{
	sregisters[0] = (sregisters[0] ^ operand);
	checkZero(sregisters);
	checkCarry(sregisters);
}

void SUBW(int& operand, int* sregisters){
	sregisters[0] = (sregisters[0] - operand);
	checkZero(sregisters);
	checkCarry(sregisters);
}

void INC(int& operand, int* registers, int* sregisters)
{
	registers[operand]++;
	checkZero(sregisters);
}

void DEC(int& operand, int* registers, int* sregisters)
{
	registers[operand]--;
	checkZero(sregisters);
}

void MOVW(int& operand, int* sregisters)
{
	sregisters[0] = operand;
	checkZero(sregisters);
}

void MOVWR(int& operand, int* registers, int* sregisters)
{
	registers[operand] = sregisters[0];
	checkZero(sregisters);
}

void MOVRW(int& operand, int* registers, int* sregisters)
{
	sregisters[0] = registers[operand];
	checkZero(sregisters);
}

void RET(int* sregisters, int* registers)
{
	//cout << "Jumping to location1: " << registers[sregisters[2]] << endl;
	sregisters[2]++;
	//cout << "Jumping to location: " << registers[sregisters[2]] << endl;
	sregisters[1] = registers[sregisters[2]]; //JMP
	registers[sregisters[2]] = 0x0000;
}

void JPZ(int& operand, int* sregisters)
{
	if (sregisters[3] & 4)
	{
		JMP(operand,sregisters);
	}
}

void JPC(int& operand, int* sregisters)
{
	if (sregisters[3] & 1)
	{
		JMP(operand,sregisters);
	}
}

void decodeInstruction(int& instruction, int* registers, int* sregisters)
{
	int opcode = maskOperand(instruction);
	int operand = maskOpcode(instruction);
	cout << "Current Instruction: ";
	switch (opcode)
	{
		case 0x0000: 
			if (instruction == 0x0008)
			{
				cout << "RET" << endl; RET(sregisters,registers); break;
			}
			else
			{
				cout << "NOP" << endl; break;
			}

		case 0x2000: cout << "CALL" << endl; CALL(operand,sregisters,registers); break;

		case 0x0A00: cout << "INC" << endl; INC(operand, registers, sregisters); break;

		case 0x0300: cout << "DEC" << endl; DEC(operand, registers, sregisters); break;

		case 0x3E00: cout << "ADDW" << endl; ADDW(operand,sregisters); break;

		case 0x3900: cout << "ANDW" << endl; ANDW(operand, sregisters); break;

		case 0x3C00: cout << "SUBW" << endl; SUBW(operand, sregisters); break;

		case 0x3800: cout << "ORW" << endl; ORW(operand, sregisters); break;

		case 0x3A00: cout << "XORW" << endl; XORW(operand, sregisters); break;

		case 0x2800: cout << "JMP" << endl; JMP(operand,sregisters); break;

		case 0x2900: cout << "JPZ" << endl; JPZ(operand,sregisters); break;

		case 0x2A00: cout << "JPC" << endl; JPC(operand,sregisters); break;

		case 0x0100: cout << "MOVWR" << endl; MOVWR(operand, registers, sregisters); break;

		case 0x3000: cout << "MOVW" << endl; MOVW(operand, sregisters); break;

		case 0x0800: cout << "MOVRW" << endl; MOVRW(operand, registers, sregisters); break;

		case 0xEE00: cout << "HALT" << endl; exit(0); break;

		default: cout << "Invalid instruction" << endl; break;
	}
}


void printRegisters(int* reg, int* sreg)
{
	const int size = 4;
	cout << "W : " << hex << sreg[0] << "                    ";
	cout << "PC: " << hex << sreg[1] << '\n';
	cout << "SP: " << hex << sreg[2] << "                    ";
	cout << "SR: " << hex << sreg[3] << "\n ";
	for (int i=0; i<16; i++)
	{
		cout << hex << i*16 << "  ";
		for (int j=0; j<16; j++)
		{
			int value = *(reg+(i*16+j));
			string padding = "";
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
			cout << padding << hex << value << " ";
		}
		cout << '\n';
	}
	cout <<	"                                            TRSA PRTA TRSB PRTB TRSC PRTC PRE  TMR" << endl;
}

void loadFile(vector<string>& filelines)
{
	ifstream file("assembly.txt");
	string currentline;
	while (getline(file, currentline))
	{
		filelines.push_back(currentline);
	}
}

vector<string> split(const string& s)
{
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss,item,' '))
	{
		if (!item.empty())
		{
			elems.push_back(item);
		}
	}
	return elems;
}

int getOpcode(string& opcode)
{
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
	string data = operand.substr(2,2);
	unsigned int x = stoul(data,nullptr,16);
	cout << x << endl;
	return x;
}

bool isOperandHex(string& operand)
{
	string data = operand.substr(0,2);
	if (data == "0x")
	{
		return true;
	}
	return false;
}

bool checkIfLabel(string& line)
{
	if (line.back() == ':')
	{
		cout << "Label Found" << endl;
		cout << line << endl;
		return true;
	}
	return false;
}

void assembleCode(vector<string> filelines, int* registers, vector<string>& labels, vector<int>& labelPos)
{
	int* regStart = registers;
	vector<int> positionsToFix;
	vector<string> labelPositionsToPutin;
	for (unsigned int i=0; i<filelines.size(); i++)
	{
		cout << filelines[i] << endl;
		vector<string> elems = split(filelines[i]);
		int opcode = getOpcode(elems[0]);
		cout << opcode << endl;
		if (opcode)
		{
			if (opcode==0x0008 || opcode==0xEE00)
			{
				*registers = opcode;
				registers++;
				continue;
			}
			if (opcode != 0xFFFF and opcode != 0x2000 and opcode != 0x0A00 and opcode != 0x3000 and opcode != 0x0100 and opcode != 0x0800 and opcode != 0x2800 and opcode != 0x2900 and opcode != 0x2A00)
			{
				*registers = opcode + getOperand(elems[1]);
				registers++;
				continue;
			}

			if (opcode == 0xFFFF)
			{
				cout << "Checking potential Label" << endl;
				if(checkIfLabel(elems[0]))
				{
					labels.push_back(elems[0]);
					labelPos.push_back(int(registers-regStart));
					cout << labelPos[0] << endl;
				}
				continue;
			}

			
			*registers = opcode;
			if (isOperandHex(elems[1]))
			{
				*registers += getOperand(elems[1]);
				registers++;
				continue;
			}

			positionsToFix.push_back(int(registers-regStart));
			registers++;				
			labelPositionsToPutin.push_back(elems[1] + ':');
			//cout << positionsToFix[0] << endl;
			
			
		}
		else
		{
			//if NOP eg no operand
			*registers = 0x0000;
			registers++;
		}
	}

	for(int i=0; i<positionsToFix.size(); i++)
	{
		cout << "Fixing memory location: " << positionsToFix[i] << endl;
		cout << "Searching for label: " << labelPositionsToPutin[i] << endl;
		for (int j=0; j<labels.size(); j++)
		{
			if (labels[j] == labelPositionsToPutin[i])
			{
				cout << labels[j] << " found at location: " << labelPos[j] << endl;
				//cout << *(regStart + positionsToFix[i]);
				*(regStart + positionsToFix[i]) = *(regStart + positionsToFix[i]) + labelPos[j];
				//cout << *(regStart + positionsToFix[i]) << endl;
			}
		}

	}

}

int main(int argc, char const *argv[])
{
	int freq = 2;
	if (argc == 2)
		freq = atoi(argv[1]);
	vector<string> filelines;
	vector<string> labels = {"TRISA:","PORTA:","TRISB:","PORTB:","TRISC:","PORTC:","PRE:","TMR:"};
	vector<int> labelPos = {248,249,250,251,252,253,254,255};
	loadFile(filelines);
	int registers[256] = {0};
	int sregisters[4] = {0};
	sregisters[2] = 247;
	const int sleeptime = 1000000/freq;
	// 0: W
	// 1: PC
	// 2: SP
	// 3: SR

	//decodeInstruction(registers[0]);
	registers[1] = 0x2010;
	//decodeInstruction(registers[1]);
	assembleCode(filelines,registers,labels,labelPos);

	while (true)
	{
		usleep(sleeptime);
		printRegisters(registers,sregisters);
		decodeInstruction(registers[sregisters[1]],registers,sregisters);
		sregisters[1]++;
	}

	return 0;
}