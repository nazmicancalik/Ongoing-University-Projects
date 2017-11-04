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
    std::shared_ptr<std::vector<std::string>> tokens;
    tokens = IOManager::readTokens(processFile);

    for (int i = 0; i < tokens->size() - 2; i += 3)
    {
        //Convert the arrival time to integer.
        int arrivalTime = std::stoi(tokens->at(i + 2));
        //Create a shared pointer to the newly created object.
        std::shared_ptr<PCB> process =
            std::make_shared<PCB>(PCB(tokens->at(i), tokens->at(i + 1), arrivalTime));
        //Push to arrival queue.
        arrivalQueue.push_back(process);
    }

    //Sort the arrivalQueue according to arrival time.
    std::sort(arrivalQueue.begin(),arrivalQueue.end());
    
}

//Initializes the instructions(vector) of the processes for each process in arrival queue.
void SchedulingManager::initializeProcessInstructions()
{
    for(int i = 0;i < arrivalQueue.size();i++){
        std::shared_ptr<PCB> process = arrivalQueue.at(i);

    }
}