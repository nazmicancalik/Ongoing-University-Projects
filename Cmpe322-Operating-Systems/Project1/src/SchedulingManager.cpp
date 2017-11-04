#include "SchedulingManager.hpp"

//Default Constructor
SchedulingManager::SchedulingManager()
{
}

//Paremeterized Constructor
SchedulingManager::SchedulingManager(std::string &aProcessFile)
    : processFile(aProcessFile)
{
}

//Destructor
SchedulingManager::~SchedulingManager()
{
}

//Simulator method
void SchedulingManager::simulate()
{
    initializeArrivalQueue();

    //run the round robin
}

//Initializes the processes and puts them in arrivalQueue.
void SchedulingManager::initializeArrivalQueue()
{
    //Get the tokens.
    std::vector<std::string> tokens;
    tokens = IOManager::readTokens(processFile);

    for (int i = 0; i < tokens.size() - 2; i += 3)
    {
        //Create a process.
        PCB process;
        int arrivalTime = std::stoi(tokens.at(i + 2));
        process = PCB(tokens.at(i), tokens.at(i + 1), arrivalTime);

        //Put the process in arrivalQueue.
        std::cout << "Process name: " << process.getProcessName()
                  << "\nProcess instruction file name: "
                  << process.getInstructionFileName()
                  << "\nProcess arrival time: " << process.getArrivalTime()
                  << std::endl;
        arrivalQueue.push(&process);
    }
}