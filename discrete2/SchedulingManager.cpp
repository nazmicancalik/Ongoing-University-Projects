#include "SchedulingManager.h"


//Default Constructor
SchedulingManager::SchedulingManager()
{
}

//Paremeterized Constructor
SchedulingManager::SchedulingManager(std::string &aProcessFile, std::string &anOutputFile)
    : processFile(aProcessFile), outputFile(anOutputFile)
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

    //Execute the round robin.
    executeRoundRobin();
}

//Initializes Semaphores.
void SchedulingManager::initializeSemaphores() {
    for(int i=0; i < 10; i++){
        std::shared_ptr<Semaphore> semaphore =
                std::make_shared<Semaphore>(Semaphore(i));
        semaphores.push_back(semaphore);
    }
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

    //Sort the arrivalQueue according to arrival time using lambda function.
    std::sort(arrivalQueue.begin(), arrivalQueue.end(),
              [](const std::shared_ptr<PCB> &a, const std::shared_ptr<PCB> &b) -> bool {
                  return a->getArrivalTime() < b->getArrivalTime();
              });
}

//Executes the round robin algorithm on ready queue.
void SchedulingManager::executeRoundRobin()
{
    //Put the arrived processes to the ready queue.
    currentTime = 0;
    int processCount = arrivalQueue.size();
    nextClosestEventTime = std::numeric_limits<int>::max();

    while (finishedProcesses.size() != processCount)
    {
        //Execute discrete events.
        putArrivedPCBToReadyQueue();
        getProcessOutOfCpu();
        takeProcessIntoCpu();
        executeInstruction();

        //Update the time.
        currentTime = nextClosestEventTime;
        nextClosestEventTime = std::numeric_limits<int>::max();
    }
}

//Take arriving processes to the ready queue.
void SchedulingManager::putArrivedPCBToReadyQueue()
{
    for (int i = 0; i < arrivalQueue.size(); i++)
    {
        if (currentTime == arrivalQueue.at(i)->getArrivalTime())
        {
            readyQueue.push_back(arrivalQueue.at(i));
            arrivalQueue.erase(arrivalQueue.begin() + i);
            //Because we deleted an element
            i--;
        }
    }
    for (int i = 0; i < arrivalQueue.size(); i++)
    {
        nextClosestEventTime = std::min(arrivalQueue.at(i)->getArrivalTime(), nextClosestEventTime);
    }
}

void SchedulingManager::executeInstruction()
{
    int passedTime = 0;
    bool someTimePassed = false;
    while (canExecuteNextInstruction())
    {
        //If the process in the cpu is executing its last instruction.
        if (currentProcessInCpu->getFinishTime() != std::numeric_limits<int>::max())
        {
            break;
        }
        
        //For examining the next closest event.
        someTimePassed = true;
        
        remainingQuantumTime = remainingQuantumTime - currentProcessInCpu->getCurrentInstruction()->length;
        passedTime += currentProcessInCpu->getCurrentInstruction()->length;
        
        if (currentProcessInCpu->getCurrentInstruction()->name == "exit")
        {
            currentProcessInCpu->setFinishTime(currentTime + passedTime);
        }
        else
        {
            currentProcessInCpu->executeCurrentInstruction();
        }
    }
    if (someTimePassed)
    {
        cpuFinishTime = currentTime + passedTime;
        nextClosestEventTime = std::min(cpuFinishTime, nextClosestEventTime);
    }
}

//Takes process into cpu.
void SchedulingManager::takeProcessIntoCpu()
{

    //If the cpu is not busy and there is a process waiting for cpu.
    if (!isCpuBusy && !readyQueue.empty())
    {
        remainingQuantumTime = quantum;
        currentProcessInCpu = readyQueue.front();
        printQueue();
        readyQueue.pop_front(); //Consume the element. //was pop
        isCpuBusy = true;
    }
}

void SchedulingManager::getProcessOutOfCpu()
{
    if (isCpuBusy)
    {
        if (currentProcessInCpu->getFinishTime() == currentTime)
        {
            finishedProcesses.push_back(currentProcessInCpu);
            isCpuBusy = false;

            //Only for printing the last process finished. Empty Queue.
            if (readyQueue.empty() && arrivalQueue.empty())
            {
                printQueue();
            }
        }
    }
    if (isCpuBusy && (currentTime == cpuFinishTime))
    {
        isCpuBusy = false;
        readyQueue.push_back(currentProcessInCpu);
    }
    else if (isCpuBusy)
    {
        nextClosestEventTime = std::min(cpuFinishTime, nextClosestEventTime);
    }
}

//Returns true if Cpu can execute the next instruction.
bool SchedulingManager::canExecuteNextInstruction()
{
    return (isCpuBusy && remainingQuantumTime > 0);
}

//Prints the ready queue contents to the output file.
void SchedulingManager::printQueue()
{

    IOManager::write(outputFile,std::to_string(currentTime));
    IOManager::write(outputFile,"::HEAD-");
    std::string processNames;
    for (int i = 0; i < readyQueue.size(); i++)
    {
        IOManager::write(outputFile,readyQueue.at(i)->getProcessName());
        IOManager::write(outputFile,"-");

        processNames += readyQueue.at(i)->getProcessName() + "-";
    }

    IOManager::write(outputFile,"TAIL\n");
}
