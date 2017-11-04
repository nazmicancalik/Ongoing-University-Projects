#ifndef PCB_H
#define PCB_H

#include <iostream>
#include <vector>
#include <string>

struct instruction
{
	std::string name;
	int length;
};

class PCB
{
public:
	PCB();
	~PCB();
	PCB(std::string &name, std::string &instructionFileName, int arrivalTime);
	std::string getProcessName();
	std::string getInstructionFileName();
	std::vector<instruction> *getInstructions();
	int getCurrentInstructionNumber();
	int getArrivalTime();
	bool operator<(const PCB &other);
private:
	void initializeInstructions();
	std::string processName;
	std::string instructionFileName;
	std::vector<instruction> instructions;
	int currentInstructionNumber;
	int arrivalTime;
};
#endif