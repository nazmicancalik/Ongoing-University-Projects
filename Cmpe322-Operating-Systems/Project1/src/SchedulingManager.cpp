#include "SchedulingManager.h"

//Default Constructor
SchedulingManager::SchedulingManager() {
}

//Paremeterized Constructor
SchedulingManager::SchedulingManager(std::string &aProcessFile)
        : processFile(aProcessFile) {
}

//Destructor
SchedulingManager::~SchedulingManager() {
}

//Simulator method
void SchedulingManager::simulate() {
    //Create and initialize the processes along with their instructions.
    initializeArrivalQueue();

    //Execute the round robin.
    executeRoundRobin();
}

//Initializes the processes and puts them in arrivalQueue.
void SchedulingManager::initializeArrivalQueue() {
    //Get the tokens.
    std::shared_ptr<std::vector<std::string>> tokens;
    tokens = IOManager::readTokens(processFile);


    for (int i = 0; i < tokens->size() - 2; i += 3) {
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
void SchedulingManager::executeRoundRobin() {
    //Put the arrived processes to the ready queue.
    //currentTime = 0;
    currentTime = arrivalQueue.at(0)->getArrivalTime();
    nextClosestEventTime = std::numeric_limits<int>::max();
    bool isFirstTime = true;
    //putArrivedPCBToReadyQueue();
    printQueue();
    while (isCpuBusy || !readyQueue.empty() || isFirstTime) {
        putArrivedPCBToReadyQueue();
        getProcessOutOfCpu();
        takeProcessIntoCpu();
        executeInstruction();

        //Update the time.
        currentTime = nextClosestEventTime;
        nextClosestEventTime = std::numeric_limits<int>::max();
        isFirstTime = false;

    }
}

void SchedulingManager::printProcesses() {
    for (int i = 0; i < arrivalQueue.size(); i++) {
        std::shared_ptr<PCB> process = arrivalQueue.at(i);
        std::cout << "Name of the process is: " << process->getProcessName() << "\n"
                  << "The instruction file of it is: " << process->getInstructionFileName()
                  << std::endl;
        for (int j = 0; j < process->getInstructions()->size(); j++) {
            std::cout << "Name of the " << j << " instruction is: " << process->getInstructions()->at(j).name
                      << std::endl;
            std::cout << "Length of the " << j << " instruction is: " << process->getInstructions()->at(j).length
                      << std::endl;
        }
    }
}

void SchedulingManager::putArrivedPCBToReadyQueue() {
    /*Deprecated iterator.
     * for (std::vector<std::shared_ptr<PCB>>::iterator it = arrivalQueue.begin(); it != arrivalQueue.end(); it++) {
        std::shared_ptr<PCB> process = *it;
        if (currentTime == process->getArrivalTime()) {
            readyQueue.push(process);
            arrivalQueue.erase(it);
        }
    }

    for (std::vector<std::shared_ptr<PCB>>::iterator it = arrivalQueue.begin(); it != arrivalQueue.end(); it++) {
        std::shared_ptr<PCB> process = *it;
        nextClosestEventTime = std::min(process->getArrivalTime(), nextClosestEventTime);
    }*/
    //Burdaki silme de uzunluk değiştiği için sıkıntılar olabilir girmesi gereken bi elemean girmeyebilir.
    for(int i = 0;i<arrivalQueue.size();i++)
    {
        if (currentTime == arrivalQueue.at(i)->getArrivalTime()) {
            readyQueue.push_back(arrivalQueue.at(i));//was push
            arrivalQueue.erase(arrivalQueue.begin()+i);
            i--; //Because we deleted an element
        }
    }
    printQueue();
    for(int i = 0;i<arrivalQueue.size();i++)
    {
        nextClosestEventTime = std::min(arrivalQueue.at(i)->getArrivalTime(), nextClosestEventTime);
    }
}

void SchedulingManager::executeInstruction() {
    if (canExecuteNextInstruction()) {
        std::string currentInstructionName1 = currentProcessInCpu->getCurrentInstruction()->name;
        int currentInstructionLength1 = currentProcessInCpu->getCurrentInstruction()->length;
        remainingQuantumTime = remainingQuantumTime - currentProcessInCpu->getCurrentInstruction()->length;
        if (currentProcessInCpu->getCurrentInstruction()->name == "exit") {
            finishedProcesses.push_back(currentProcessInCpu);
            isCpuBusy = false;
        }
        nextClosestEventTime = std::min(currentTime + currentProcessInCpu->getCurrentInstruction()->length,nextClosestEventTime); // Update the current time.
        currentProcessInCpu->executeCurrentInstruction();
    }
}

void SchedulingManager::takeProcessIntoCpu() {

    //If the cpu is not busy and there is a process waiting for cpu.
    if (!isCpuBusy && !readyQueue.empty()) {
        remainingQuantumTime = quantum;
        currentProcessInCpu = readyQueue.front();
        readyQueue.pop_front();       //Consume the element. //was pop
        isCpuBusy = true;
        printQueue();
    }
}

void SchedulingManager::getProcessOutOfCpu() {
    if (isCpuBusy && !canExecuteNextInstruction()) {
        isCpuBusy = false;
        readyQueue.push_back(currentProcessInCpu);  //was push
        printQueue();
    }
}

bool SchedulingManager::canExecuteNextInstruction() {
    return (isCpuBusy && currentProcessInCpu->getCurrentInstruction()->length <= remainingQuantumTime);
}

void SchedulingManager::printQueue() {
    std::string processNames;
    for(int i = 0;i<readyQueue.size();i++)
    {
        processNames += readyQueue.at(i)->getProcessName() + " - ";
    }
    std::cout << currentTime << " : HEAD-" << processNames << "TAIL" <<std::endl;
}


