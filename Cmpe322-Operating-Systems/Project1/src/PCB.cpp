#include "PCB.hpp"

//Default constructor
PCB::PCB()
{
}

//Constructor with parameters
PCB::PCB(std::string &aProcessName, std::string &anInstructionFileName, int anArrivalTime)
	: processName(aProcessName), instructionFileName(anInstructionFileName), arrivalTime(anArrivalTime)
{
	initializeInstructions();
}

//Destructor
PCB::~PCB()
{
}

//Initializes the instructions vector of the process.
void PCB::initializeInstructions()
{
	std::shared_ptr<std::vector<std::string>> tokens;
	std::string name = "./input/" + instructionFileName;
	tokens = IOManager::readTokens(instructionFileName);
	//Parse all the instructions.
	for (int i = 0; i < tokens->size()-1; i+=2)
	{
		//Get the instruction
		instruction instruction1;
		instruction1.name = tokens->at(i);
		instruction1.length = std::stoi(tokens->at(i+1));
		std::cout<<"looptayım"<<std::endl;
		//Push the instruction
		instructions.push_back(instruction1);
	}
}

//Getter for process name.
std::string PCB::getProcessName()
{
	return processName;
}

//Returns a pointer to the adress of the instrucitons vector.
std::vector<instruction> *PCB::getInstructions()
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
