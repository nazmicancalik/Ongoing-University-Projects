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
    currentTime = 0;
    nextClosestEventTime = arrivalQueue.at(0)->getArrivalTime();
    while (isCpuBusy || !readyQueue.empty()) {
        putArrivedPCBToReadyQueue();
        getProcessOutOfCpu();
        takeProcessIntoCpu();
        executeProcess();

        //Update the time.
        currentTime = nextClosestEventTime;
        nextClosestEventTime = std::numeric_limits<int>::max();
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
    for (std::vector<std::shared_ptr<PCB>>::iterator it = arrivalQueue.begin(); it != arrivalQueue.end(); ++it) {
        std::shared_ptr<PCB> process = it->get();
        if (currentTime == process->getArrivalTime()) {
            readyQueue.push(process);
        }
        nextClosestEventTime = std::min(process->getArrivalTime(), nextClosestEventTime);
    }
}

void SchedulingManager::executeProcess() {
    remainingQuantumTime = quantum;
    while (canExecuteNextInstruction()) {
        remainingQuantumTime = quantum - currentProcessInCpu->getCurrentInstruction()->length;
        currentProcessInCpu->executeCurrentInstruction();
    }
}

void SchedulingManager::takeProcessIntoCpu() {

    //If the cpu is not busy and there is a process waiting for cpu.
    if (!isCpuBusy && !readyQueue.empty()) {
        currentProcessInCpu = readyQueue.front();
        readyQueue.pop();       //Consume the element.
        isCpuBusy = true;
    }
}

void SchedulingManager::getProcessOutOfCpu() {
    if (isCpuBusy && !canExecuteNextInstruction()) {
        isCpuBusy = false;
        readyQueue.push(currentProcessInCpu);
    }

    //TODO CONSIDER EXIT CASE.
}

bool SchedulingManager::canExecuteNextInstruction() {

    //TODO Make this one line return.
    if (isCpuBusy && currentProcessInCpu->getCurrentInstruction()->length <= remainingQuantumTime) {
        return true;
    }

    return false;
}


