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
    //Create and initialize the processes along with their instructions.
    initializeArrivalQueue();

    //printProcesses();
    //Execute the round robin.
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
    std::sort(arrivalQueue.begin(), arrivalQueue.end(),
    [](const std::shared_ptr<PCB> & a, const std::shared_ptr<PCB> & b) -> bool
    {
        return a->getArrivalTime() < b->getArrivalTime();
    });
}

void SchedulingManager::printProcesses()
{
    for (int i = 0; i < arrivalQueue.size(); i++)
    {
        std::shared_ptr<PCB> process = arrivalQueue.at(i);
        std::cout << "Name of the process is: " << process->getProcessName()
                  << "The instruction file of it is: " << process->getInstructionFileName()
                  << std::endl;
        for (int j=0;j<process->getInstructions()->size();j++)
        {
            std::cout << "Name of the "<< i << " instruction is: " << process->getInstructions()->at(j).name << std::endl;
            std::cout << "Length of the "<< i << " instruction is: " << process->getInstructions()->at(j).length << std::endl;
        }
    }
}
/* DEPRECATED FOR NOW. PROCESS ITSELF DOES THIS WHEN CREATED.
//Initializes the instructions(vector) of the processes for each process in arrival queue.
void SchedulingManager::initializeProcessInstructions()
{
    for(int i = 0;i < arrivalQueue.size();i++){
        std::shared_ptr<PCB> process = arrivalQueue.at(i);
        process->initializeInstructions();
    }
}
*/