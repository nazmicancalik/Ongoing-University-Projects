#ifndef PCB_H
#define PCB_H

#include "IOManager.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>

struct instruction {
    std::string name;
    int length;
};

class PCB {
public:
    PCB();

    ~PCB();

    PCB(std::string &name, std::string &instructionFileName, int arrivalTime);

    std::string getProcessName();

    std::string getInstructionFileName();

    std::vector<instruction> *getInstructions();

    instruction *getCurrentInstruction();

    void executeCurrentInstruction();

    int getArrivalTime();

    int getFinishTime();

    void setFinishTime(int aFinishTime);

private:
    void initializeInstructions();

    std::string processName;
    std::string instructionFileName;
    std::vector<instruction> instructions;
    int currentInstructionNumber;
    int arrivalTime;
    int finishTime = std::numeric_limits<int>::max();
};

#endif