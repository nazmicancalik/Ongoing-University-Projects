#include "PCB.h"

//Default constructor
PCB::PCB() {
}

//Constructor with parameters Every PCB initializes its own instructions by itself.
PCB::PCB(std::string &aProcessName, std::string &anInstructionFileName, int anArrivalTime)
        : processName(aProcessName), instructionFileName(anInstructionFileName), arrivalTime(anArrivalTime) {
    initializeInstructions();
    currentInstructionNumber = 0;
}

//Destructor
PCB::~PCB() {
}

//Initializes the instructions vector of the process.
void PCB::initializeInstructions() {
    std::shared_ptr<std::vector<std::string>> tokens;
    tokens = IOManager::readTokens(instructionFileName);
    //Parse all the instructions.
    for (int i = 0; i < tokens->size() - 1; i += 2) {
        //Get the instruction
        instruction instruction1;
        instruction1.name = tokens->at(i);
        instruction1.length = std::stoi(tokens->at(i + 1));

        //Push the instruction
        instructions.push_back(instruction1);
    }
}

//Getter for process name.
std::string PCB::getProcessName() {
    return processName;
}

//Returns a pointer to the adress of the instrucitons vector.
std::vector<instruction> *PCB::getInstructions() {
    return &instructions;
}

//Returns the name of the instruction file name.
std::string PCB::getInstructionFileName() {
    return instructionFileName;
}

int PCB::getArrivalTime() {
    return arrivalTime;
}

void PCB::executeCurrentInstruction() {
    currentInstructionNumber++;
}

instruction *PCB::getCurrentInstruction() {
    return &instructions.at(currentInstructionNumber);
}

int PCB::getFinishTime() {
    return finishTime;
}

void PCB::setFinishTime(int aFinishTime) {
    finishTime = aFinishTime;
}
