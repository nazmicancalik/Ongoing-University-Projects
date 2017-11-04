#include "PCB.hpp"

//Default constructor
PCB::PCB()
{
}

//Constructor with parameters
PCB::PCB(std::string &aProcessName, std::string &anInstructionFileName, int anArrivalTime)
	: processName(aProcessName), instructionFileName(anInstructionFileName), arrivalTime(anArrivalTime)
{
}

//Destructor
PCB::~PCB()
{
}

//Operator overloading for sorting the arrival queue.
bool PCB::operator<(const PCB &other)
{
	return arrivalTime < other.arrivalTime;
}


//Initializes the instructions finish time vector.
void PCB::initializeInstructions()
{
	//According to the cases initialize the instructions vector of the process.
	//TODO
}
//Getter for process name.
std::string PCB::getProcessName()
{
	return processName;
}


//Returns a pointer to the adress of the instrucitons vector.
std::vector<int> *PCB::getInstructions()
{
	return &instructions;
}


//Returns the name of the instruction file name.
std::string PCB::getInstructionFileName()
{
	return instructionFileName;
}

//Return current instruction number.
int PCB::getCurrentInstructionNumber()
{
	return currentInstructionNumber;
}

int PCB::getArrivalTime()
{
	return arrivalTime;
}
