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
        std::string ins_file_name =
                "/home/gray/workspace/Ongoing-University-Projects/discrete3/other/conf_1/" + tokens->at(i + 1);
        std::cout << ins_file_name << std::endl;
        std::shared_ptr<PCB> process = std::make_shared<PCB>(PCB(tokens->at(i), ins_file_name, arrivalTime));
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
    //The main part where the discrete events are executed and time changed.
    while (finishedProcesses.size() != processCount) {
        //Execute discrete events.
        /*
        if(cpuFinishTime==currentTime) {
            takeProcessOutOfIoQueue();
        }
*/
        putArrivedPCBToReadyQueue();
        getProcessOutOfCpu();
        takeProcessIntoCpu();
        executeInstruction();


/*
        //Update the time for display queue exits.
        for (int i = 0; i < displayQueue0.size(); ++i) {
            if(nextClosestEventTime > displayQueue0[i]->displayQueueExitTime0){
                nextClosestEventTime = displayQueue0[i]->displayQueueExitTime0;
            }
        }

        for (int i = 0; i < displayQueue1.size(); ++i) {
            if(nextClosestEventTime > displayQueue1[i]->displayQueueExitTime1){
                nextClosestEventTime = displayQueue1[i]->displayQueueExitTime1;
            }
        }
*/
        currentTime = nextClosestEventTime;
        nextClosestEventTime = std::numeric_limits<int>::max();
    }
}

//Take arriving processes to the ready queue.
void SchedulingManager::putArrivedPCBToReadyQueue() {
    for (int i = 0; i < arrivalQueue.size(); i++) {
        if (currentTime == arrivalQueue.at(i)->getArrivalTime()) {

            //Push the arrived process into ready queue.
            readyQueue.push_back(arrivalQueue.at(i));
            arrivalQueue.erase(arrivalQueue.begin() + i);

            //Because we deleted an element
            i--;
        }
    }

    //Determine the next closest event time.
    for (int i = 0; i < arrivalQueue.size(); i++) {
        nextClosestEventTime = std::min(arrivalQueue.at(i)->getArrivalTime(), nextClosestEventTime);
    }
}

void SchedulingManager::executeInstruction() {

    //Passed time in execution.
    int passedTime = 0;
    bool someTimePassed = false;

    //If the process executed a wait instruction (last time) on a semaphore that is not available, 
    //prevent it from executing instructions.  
    while (canExecuteNextInstruction() && !(currentProcessInCpu->shouldEnterSemaphoreQueue) && !(currentProcessInCpu->shouldEnterIoQueue)) {

        //If the instruction is display Manager's Operation.
        std::string inst = currentProcessInCpu->getCurrentInstruction()->name;
        int inst_length = currentProcessInCpu->getCurrentInstruction()->length;
        if (currentProcessInCpu->getCurrentInstruction()->name.substr(0, 6) == "dispM_") {
            int index = stoi(currentProcessInCpu->getCurrentInstruction()->name.substr(6, 1));
            currentProcessInCpu->lastQueueOperationTime = currentProcessInCpu->getCurrentInstruction()->length;
            dispM(index,currentProcessInCpu,currentTime + passedTime);
            currentProcessInCpu->executeCurrentInstruction();
            currentProcessInCpu->shouldEnterIoQueue = true;
            break;
        }
        /*
        //If the instruction is Wait.
        if (currentProcessInCpu->getCurrentInstruction()->name.substr(0, 6) == "waitS_") {
            int index = stoi(currentProcessInCpu->getCurrentInstruction()->name.substr(6, 1));

            //Execute wait instruction.
            wait_S(index, currentProcessInCpu, (currentTime + passedTime));

            //If the process needs to be sent to semaphore queue.
            if (semaphores[index]->getValue() != 0) {
                //Make it enter semaphore queue. Actually prevent it from entering ready queue again.
                currentProcessInCpu->shouldEnterSemaphoreQueue = true;

                //In any ways we have to execute the instruction.
                currentProcessInCpu->executeCurrentInstruction();
                break;
            }
        }//If the instruction is Sign.
        else if (currentProcessInCpu->getCurrentInstruction()->name.substr(0, 6) == "signS_") {

            //Find the semaphore index.
            int index = stoi(currentProcessInCpu->getCurrentInstruction()->name.substr(6, 1));

            //Execute the sign_S
            sign_S(index, currentProcessInCpu, (currentTime + passedTime));
        }
    */
        //If the process in the cpu is executing its last instruction.
        if (currentProcessInCpu->getFinishTime() != std::numeric_limits<int>::max()) {
            break;
        }

        //For examining the next closest event.
        someTimePassed = true;

        //Update remaining quantum time.
        remainingQuantumTime = remainingQuantumTime - currentProcessInCpu->getCurrentInstruction()->length;
        passedTime += currentProcessInCpu->getCurrentInstruction()->length;
        std::cout <<currentTime + passedTime  << " *************"<<std::endl;
        //Exit from io queue.
        takeProcessOutOfIoQueue(currentTime+passedTime);

        if (currentProcessInCpu->getCurrentInstruction()->name == "exit") {
            currentProcessInCpu->setFinishTime(currentTime + passedTime);
        } else {
            currentProcessInCpu->executeCurrentInstruction();
        }
    }

    //To be able to update the next closest event time. 
    if (someTimePassed) {
        cpuFinishTime = currentTime + passedTime;
        nextClosestEventTime = std::min(cpuFinishTime, nextClosestEventTime);
    }
}

//Takes process into cpu.
void SchedulingManager::takeProcessIntoCpu() {

    //If the cpu is not busy and there is a process waiting for cpu.
    if (!isCpuBusy && !readyQueue.empty()) {

        //Reset remaining quantum time.
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

        //If the process isn't going to enter semaphore queue, then push it back to ready queue.
        if (!(currentProcessInCpu->shouldEnterSemaphoreQueue) && !(currentProcessInCpu->shouldEnterIoQueue)) {
            readyQueue.push_back(currentProcessInCpu);
        }
    }

        //Update the next closest event time.
    else if (isCpuBusy) {
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

    //To be able to print the process inside the Cpu.
    std::string currentCpuProcess = "";

    //Specific time rather than current time. We can print the cases which won't affect the situation later.
    if (aTime == 0) {
        time = currentTime;
    } else {
        time = aTime;
        //In order to be able to print the current process in cpu
        currentCpuProcess = "-" + currentProcessInCpu->getProcessName();
    }

    IOManager::write(outputFile, std::to_string(time));

    //In order to be able to show the current process in the cpu.
    //Even though this case is done automatically in traditional part of the problem. Now I have to make it manually because
    //it happens when time is not changed. It only acts as so.
    IOManager::write(outputFile, "::HEAD" + currentCpuProcess);
    for (int i = 0; i < readyQueue.size(); i++) {
        IOManager::write(outputFile, "-");
        IOManager::write(outputFile, readyQueue.at(i)->getProcessName());
    }

    //To be able to make the output look as requested.
    if (readyQueue.size() == 0) {
        IOManager::write(outputFile, "-");
    }
    IOManager::write(outputFile, "-TAIL\n");
}
/*
void SchedulingManager::wait_S(int index, std::shared_ptr<PCB> aProcess, int aTime) {

    int time;

    //Specific time rather than current time. We can print the cases which won't affect the situation later.
    if (aTime == 0) {
        time = currentTime;
    } else {
        time = aTime;
    }

    //If there is a process using semaphore and/or there is a process in the Semaphore queue.
    //Push the process into semaphore queue.
    if (semaphores[index]->getValue() <= 0) {

        //Append the process into semaphore queue.
        semaphores[index]->appendProcess(aProcess);

        std::string outputFileName = "output_" + std::to_string(index) + ".txt";

        IOManager::write(outputFileName, std::to_string(time));
        IOManager::write(outputFileName, "::HEAD");
        for (int j = 0; j < semaphores[index]->getSemaphoreQueue()->size(); j++) {
            IOManager::write(outputFileName, "-");
            IOManager::write(outputFileName, semaphores[index]->getSemaphoreQueue()->at(j)->getProcessName());
        }

        //To be able to make the output look as requested.
        if (semaphores[index]->getSemaphoreQueue()->size() == 0) {
            IOManager::write(outputFileName, "-");
        }
        IOManager::write(outputFileName, "-TAIL\n");
    }

    // In each case decrement the semaphore value.
    semaphores[index]->wait_S();
}
*/
/*
//Signal instruction.
void SchedulingManager::sign_S(int index, std::shared_ptr<PCB> aProcess, int aTime) {

    //Increase the semaphore value.
    semaphores[index]->signal_S();

    //If there is a process waiting for semaphore make it enter ready queue, because its his turn after waiting.
    if (!(semaphores[index]->getSemaphoreQueue()->empty())) {

        // Restart everything for this process again. If we delete this it wont execute instruction because
        // of the while condition.
        semaphores[index]->getSemaphoreQueue()->front()->shouldEnterSemaphoreQueue = false;

        //Put the process back to ready queue.
        readyQueue.push_back(semaphores[index]->getSemaphoreQueue()->front());
        printQueue(aTime);

        semaphores[index]->getSemaphoreQueue()->pop_front();

        //Print part
        std::string outputFileName = "output_" + std::to_string(index) + ".txt";

        IOManager::write(outputFileName, std::to_string(aTime));
        IOManager::write(outputFileName, "::HEAD-");
        for (int j = 0; j < semaphores[index]->getSemaphoreQueue()->size(); j++) {
            IOManager::write(outputFileName, semaphores[index]->getSemaphoreQueue()->at(j)->getProcessName());
            IOManager::write(outputFileName, "-");
        }

        //To be able to make the output look as requested.
        if (semaphores[index]->getSemaphoreQueue()->size() == 0) {
            IOManager::write(outputFileName, "-");
        }

        IOManager::write(outputFileName, "TAIL\n");
    }
}
*/
void SchedulingManager::dispM(int index, std::shared_ptr<PCB> aProcess, int aTime) {
    int queueExitTime;
    if (index == 0) {

        //Finish time of the memory operation, exit time. aTime is the time it enters the cpu (Starting time of the execution of the instruction.)
        queueExitTime = aTime + aProcess->getCurrentInstruction()->length;
        //If there is a process in the queue update the exit time accordingly.
        if(!displayQueue0.empty()){
            if(displayQueue0.back()->displayQueueExitTime0 >= aTime){
                queueExitTime = aProcess->getCurrentInstruction()->length + displayQueue0.back()->displayQueueExitTime0;
                std::cout << "Gelen processin current instrucitonu : " << aProcess->getCurrentInstruction()->name <<" Lengthi :  "<< aProcess->getCurrentInstruction()->length << "Son elemanın çıkış zamanı : " << displayQueue0.back()->displayQueueExitTime0<<" Çıkış zamanı:" <<queueExitTime << std::endl;
            }
        }
        aProcess->displayQueueExitTime0 = queueExitTime;
        displayQueue0.push_back(aProcess);
        //Write part
        std::string outputFileName = "output_10.txt";

        IOManager::write(outputFileName, std::to_string(aTime));
        IOManager::write(outputFileName, "::HEAD-");
        for (int j = 0; j < displayQueue0.size(); j++) {
            IOManager::write(outputFileName, displayQueue0[j]->getProcessName());
            IOManager::write(outputFileName, "-");
        }

        //To be able to make the output look as requested.
        if (displayQueue0.size() == 0) {
            IOManager::write(outputFileName, "-");
        }

        IOManager::write(outputFileName, "TAIL\n");

    } else {
        //Finish time of the memory operation, exit time. aTime is the time it enters the cpu (Starting time of the execution of the instruction.)
        queueExitTime = aTime + aProcess->getCurrentInstruction()->length;
        //If there is a process in the queue update the exit time accordingly.
        if(!displayQueue1.empty()){
            if(displayQueue1.back()->displayQueueExitTime1 > aTime){
                queueExitTime = aProcess->getCurrentInstruction()->length + displayQueue1.back()->displayQueueExitTime1;
            }
        }
        aProcess->displayQueueExitTime1 = queueExitTime;
        displayQueue1.push_back(aProcess);
    }

    if (nextClosestEventTime > queueExitTime) {
        nextClosestEventTime = queueExitTime;
    }

}

void SchedulingManager::takeProcessOutOfIoQueue(int localCurrentTime) {

    //Check each queue for exit time.

    for (int i = 0; i < displayQueue0.size(); ++i) {
        std::shared_ptr<PCB> process = displayQueue0[i];
        if (localCurrentTime >= process->displayQueueExitTime0) {//TODO Buraları büyük eşittir yaptım normalde ==
            readyQueue.push_back(process);
            displayQueue0.pop_front();
            //Birisi çıkınca zamanını update et diğerrinin//TODO

            if(!displayQueue0.empty()){
                std::shared_ptr<PCB> nextProcess = displayQueue0.front();
                std::cout << nextProcess->getCurrentInstruction()->name << "nazmi"<<std::endl;
                nextProcess->displayQueueExitTime0 = nextProcess->lastQueueOperationTime + localCurrentTime;
                nextProcess->lastQueueOperationTime  = 0; //Reset
            }

            process->shouldEnterIoQueue = false;
            std::cout << "Çıkış oldu 0 dan saat : " << localCurrentTime << std::endl;
            std::string outputFileName = "output_10.txt";

            //Write part
            IOManager::write(outputFileName, std::to_string(localCurrentTime));
            IOManager::write(outputFileName, "::HEAD-");
            for (int j = 0; j < displayQueue0.size(); j++) {
                IOManager::write(outputFileName, displayQueue0[j]->getProcessName());
                IOManager::write(outputFileName, "-");
            }

            //To be able to make the output look as requested.
            if (displayQueue0.size() == 0) {
                IOManager::write(outputFileName, "-");
            }

            IOManager::write(outputFileName, "TAIL çıkış\n");
        }
    }

    for (int i = 0; i < displayQueue1.size(); ++i) {
        std::shared_ptr<PCB> process = displayQueue1[i];
        if (localCurrentTime >= process->displayQueueExitTime1) {//TODO Buraları büyük eşittir yaptım normalde ==
            readyQueue.push_back(process);
            displayQueue1.pop_front();
            process->shouldEnterIoQueue = false;
            std::cout << "Çıkış oldu 1 dan saat : " << currentTime << std::endl;
        }
    }
/*
    for (int i = 0; i < displayQueue0.size(); ++i) {
        std::shared_ptr<PCB> process = memoryQueue[i];
        if (currentTime == process->memoryQueueExitTime) {
            readyQueue.push_back(process);
            memoryQueue.pop_front();
            std::cout << "Çıkış oldu 0 dan saat : " << currentTime << std::endl;
        }
    }*/
}
