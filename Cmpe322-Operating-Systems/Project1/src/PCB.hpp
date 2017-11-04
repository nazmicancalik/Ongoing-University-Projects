#ifndef PCB_H
#define PCB_H

#include <iostream>
#include <vector>
#include <string>

class PCB
{
public:
	PCB();
	~PCB();
	PCB(std::string &name, std::string &instructionFileName, int arrivalTime);
	std::string getProcessName();
	std::string getInstructionFileName();
	std::vector<int> *getInstructions();
	int getCurrentInstructionNumber();
	int getArrivalTime();
	bool operator<(const PCB &other);
private:
	void initializeInstructions();
	std::string processName;
	std::string instructionFileName;
	std::vector<int> instructions;
	int currentInstructionNumber;
	int arrivalTime;
};
#endif