#include "SchedulingManager.h"


//Default Constructor
SchedulingManager::SchedulingManager() {
}

//Paremeterized Constructor
SchedulingManager::SchedulingManager(std::string &aProcessFile, std::string &anOutputFile)
        : processFile(aProcessFile), outputFile(anOutputFile) {
}

//Destructor
SchedulingManager::~SchedulingManager() {
}

//Simulator method
void SchedulingManager::simulate() {
    //Create and initialize the processes along with their instructions.
    initializeArrivalQueue();

    //Initialize Semaphores
    initializeSemaphores();

    //Execute the round robin.
    executeRoundRobin();
}

//Initializes Semaphores.
void SchedulingManager::initializeSemaphores() {
    for (int i = 0; i < 10; i++) {
        std::shared_ptr<Semaphore> semaphore =
                std::make_shared<Semaphore>(Semaphore(i));
        semaphores.push_back(semaphore);
    }
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
        std::string ins_file_name = "/home/gray/workspace/Ongoing-University-Projects/discrete2/" + tokens->at(i + 1);
        std::shared_ptr<PCB> process =
                std::make_shared<PCB>(PCB(tokens->at(i), ins_file_name, arrivalTime));
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
    int processCount = arrivalQueue.size();
    nextClosestEventTime = std::numeric_limits<int>::max();

    while (finishedProcesses.size() != processCount) {
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
void SchedulingManager::putArrivedPCBToReadyQueue() {
    for (int i = 0; i < arrivalQueue.size(); i++) {
        if (currentTime == arrivalQueue.at(i)->getArrivalTime()) {
            readyQueue.push_back(arrivalQueue.at(i));
            arrivalQueue.erase(arrivalQueue.begin() + i);

            //Because we deleted an element
            i--;
        }
    }
    for (int i = 0; i < arrivalQueue.size(); i++) {
        nextClosestEventTime = std::min(arrivalQueue.at(i)->getArrivalTime(), nextClosestEventTime);
    }
}

void SchedulingManager::executeInstruction() {
    int passedTime = 0;
    bool someTimePassed = false;
    while (canExecuteNextInstruction() && !(currentProcessInCpu->shouldEnterSemaphoreQueue)) {

        //For debug (to be able to see values).
        //TODO DELETE
        std::string instruction_name = currentProcessInCpu->getCurrentInstruction()->name;
        int inst_length = currentProcessInCpu->getCurrentInstruction()->length;

        //If the instruction is Wait.
        if (currentProcessInCpu->getCurrentInstruction()->name.substr(0, 6) == "waitS_") {
            int index = stoi(currentProcessInCpu->getCurrentInstruction()->name.substr(6, 1));

            wait_S(index, currentProcessInCpu, (currentTime + passedTime));

            //If the process needs to be sent to semaphore queue.
            if (semaphores[index]->getValue() != 0) {
                //Make it enter semaphore queue. Actually prevent it from entering ready queue again.
                currentProcessInCpu->shouldEnterSemaphoreQueue = true;

                //In any ways we have to execute the signs. We have executed it.
                currentProcessInCpu->executeCurrentInstruction();
                break;
            }
        }//If the instruction is Sign.
        else if (currentProcessInCpu->getCurrentInstruction()->name.substr(0, 6) == "signS_") {
            int index = stoi(currentProcessInCpu->getCurrentInstruction()->name.substr(6, 1));
            sign_S(index, currentProcessInCpu, (currentTime + passedTime));
        }

        //If the process in the cpu is executing its last instruction.
        if (currentProcessInCpu->getFinishTime() != std::numeric_limits<int>::max()) {
            break;
        }

        //For examining the next closest event.
        someTimePassed = true;

        remainingQuantumTime = remainingQuantumTime - currentProcessInCpu->getCurrentInstruction()->length;
        passedTime += currentProcessInCpu->getCurrentInstruction()->length;

        if (currentProcessInCpu->getCurrentInstruction()->name == "exit") {
            currentProcessInCpu->setFinishTime(currentTime + passedTime);
        } else {
            currentProcessInCpu->executeCurrentInstruction();
        }
    }
    if (someTimePassed) {
        cpuFinishTime = currentTime + passedTime;
        nextClosestEventTime = std::min(cpuFinishTime, nextClosestEventTime);
    }
}

//Takes process into cpu.
void SchedulingManager::takeProcessIntoCpu() {

    //If the cpu is not busy and there is a process waiting for cpu.
    if (!isCpuBusy && !readyQueue.empty()) {
        remainingQuantumTime = quantum;
        currentProcessInCpu = readyQueue.front();
        printQueue();

        //Consume the element.
        readyQueue.pop_front();
        isCpuBusy = true;
    }
}

void SchedulingManager::getProcessOutOfCpu() {
    if (isCpuBusy) {
        if (currentProcessInCpu->getFinishTime() == currentTime) {
            finishedProcesses.push_back(currentProcessInCpu);
            isCpuBusy = false;

            //Only for printing the last process finished. Empty Queue.
            if (readyQueue.empty() && arrivalQueue.empty()) {
                printQueue();
            }
        }
    }
    if (isCpuBusy && (currentTime == cpuFinishTime)) {
        isCpuBusy = false;
        if (!(currentProcessInCpu->shouldEnterSemaphoreQueue)) {
            readyQueue.push_back(currentProcessInCpu);
        }
    } else if (isCpuBusy) {
        nextClosestEventTime = std::min(cpuFinishTime, nextClosestEventTime);
    }
}

//Returns true if Cpu can execute the next instruction.
bool SchedulingManager::canExecuteNextInstruction() {
    return (isCpuBusy && remainingQuantumTime > 0);
}

//Prints the ready queue contents to the output file.
void SchedulingManager::printQueue(int aTime) {

    int time;
    std::string currentCpuProcess = "";

    //Specific time rather than current time. We can print the cases which won't affect the situation later.
    if (aTime == 0) {
        time = currentTime;
    } else {
        time = aTime;
        //In order to be able to print the current process in cpu
        currentCpuProcess = "-" +  currentProcessInCpu->getProcessName();
    }

    IOManager::write(outputFile, std::to_string(time));
    //In order to be able to show the current process in the cpu.
    //Even though this case is done automatically in traditional part of the problem now I have to make it manually because
    //it happens when time is not changed. It only acts as so.
    IOManager::write(outputFile, "::HEAD" + currentCpuProcess);
    //std::string processNames;
    for (int i = 0; i < readyQueue.size(); i++) {
        IOManager::write(outputFile, "-");
        IOManager::write(outputFile, readyQueue.at(i)->getProcessName());
        //processNames += readyQueue.at(i)->getProcessName() + "-";
    }

    //To be able to make the output look as requested.
    if(readyQueue.size() == 0){
        IOManager::write(outputFile, "-");
    }
    IOManager::write(outputFile, "-TAIL\n");
}

void SchedulingManager::wait_S(int index, std::shared_ptr<PCB> aProcess, int aTime) {

    int time;

    //Specific time rather than current time. We can print the cases which won't affect the situation later.
    if (aTime == 0) {
        time = currentTime;
    } else {
        time = aTime;
    }
    if (semaphores[index]->getValue() <= 0) {

        //Append the process into semaphore queue.
        semaphores[index]->appendProcess(aProcess);

        std::string outputFileName = "my_output_" + std::to_string(index);

        IOManager::write(outputFileName, std::to_string(time));
        IOManager::write(outputFileName, "::HEAD");
        for (int j = 0; j < semaphores[index]->getSemaphoreQueue()->size(); j++) {
            IOManager::write(outputFileName, "-");
            IOManager::write(outputFileName, semaphores[index]->getSemaphoreQueue()->at(j)->getProcessName());
        }

        //To be able to make the output look as requested.
        if(semaphores[index]->getSemaphoreQueue()->size() == 0){
            IOManager::write(outputFileName, "-");
        }

        IOManager::write(outputFileName, "-TAIL\n");
    }

    // In each case decrement the semaphore value.
    semaphores[index]->wait_S();
}

void SchedulingManager::sign_S(int index, std::shared_ptr<PCB> aProcess, int aTime) {
    semaphores[index]->signal_S();
    if (!(semaphores[index]->getSemaphoreQueue()->empty())) {


        // Restart everything for this process again. If we delete this it wont execute inst. because
        // of the while condition.
        semaphores[index]->getSemaphoreQueue()->front()->shouldEnterSemaphoreQueue = false;

        //Put the process back to ready queue.
        readyQueue.push_back(semaphores[index]->getSemaphoreQueue()->front());
        printQueue(aTime);

        semaphores[index]->getSemaphoreQueue()->pop_front();

        //Print part
        std::string outputFileName = "my_output_" + std::to_string(index);

        IOManager::write(outputFileName, std::to_string(aTime));
        IOManager::write(outputFileName, "::HEAD-");
        for (int j = 0; j < semaphores[index]->getSemaphoreQueue()->size(); j++) {
            IOManager::write(outputFileName, semaphores[index]->getSemaphoreQueue()->at(j)->getProcessName());
            IOManager::write(outputFileName, "-");
        }

        //To be able to make the output look as requested.
        if(semaphores[index]->getSemaphoreQueue()->size()== 0){
            IOManager::write(outputFileName, "-");
        }

        IOManager::write(outputFileName, "TAIL\n");
    }
}

void SchedulingManager::printSemaphoreQueues() {
    for (int i = 0; i < semaphores.size(); i++) {
        semaphores[i]->printSemaphoreQueue();
    }

}
