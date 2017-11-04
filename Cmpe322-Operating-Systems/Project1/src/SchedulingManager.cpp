#include "SchedulingManager.hpp"

#include <memory>

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
        int arrivalTime = std::stoi(tokens.at(i + 2));
        std::shared_ptr<PCB> process =
            std::make_shared<PCB>(PCB(tokens.at(i), tokens.at(i + 1), arrivalTime));
        arrivalQueue.push(process);
    }
}