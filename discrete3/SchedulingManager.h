#ifndef SCHEDULINGMANAGER_H
#define SCHEDULINGMANAGER_H

#include "PCB.h"
#include "IOManager.h"
#include "Semaphore.h"
#include <queue>
#include <memory>
#include <algorithm>
#include <deque>

class SchedulingManager {

public:
	//Default Constructor
    SchedulingManager();

	//Constructor
    SchedulingManager(std::string &processFile, std::string &outputFile);

    //Destructor
    ~SchedulingManager();

    //Initializes the arrival queue.
    void initializeArrivalQueue();

    //Executes Round Robin ALgorithm
    void executeRoundRobin();

    //Whole simulation.
    void simulate();

private:

	//Puts newly arrived processes into ready queue.
    void putArrivedPCBToReadyQueue();

    //Executes the instructions.
    void executeInstruction();

    //Takes processes into Cpu.
    void takeProcessIntoCpu();

    //Gets processes out of cpu.
    void getProcessOutOfCpu();

    //Checks whether a process can execute the next instruction or not.
    bool canExecuteNextInstruction();

    //Prints the ready queue.
    void printQueue(int aTime = 0);

    //Initializes Semaphores.
    void initializeSemaphores();

    //Executes the wait instruction and updates the queues accordingly.
    void wait_S(int index, std::shared_ptr<PCB> aProcess, int aTime = 0);

    //Executes the signal instruction and updates the queues accordingly.
    void sign_S(int index, std::shared_ptr<PCB> aProcess, int aTime);

    //Execute the display manager's operation.
    void dispM(int index, std::shared_ptr<PCB> aProcess, int aTime);

    //Takes the process out of Io Queue.
    void takeProcessOutOfIoQueue();
    
    std::string processFile;	//Name of the definition file.
    std::string outputFile;		//Output file to print the situations of the queues.
    std::vector<std::shared_ptr<PCB>> arrivalQueue;	//Arrival queue.
    std::deque<std::shared_ptr<PCB>> readyQueue;	//Ready queue.
    std::vector<std::shared_ptr<PCB>> finishedProcesses;	//Finished processes.
    std::vector<std::shared_ptr<Semaphore>> semaphores;		//Holds pointers to the according semaphores.
    std::shared_ptr<PCB> currentProcessInCpu;	//Pointer to the current process in the Cpu.

    std::deque<std::shared_ptr<PCB>> displayQueue0;
    std::deque<std::shared_ptr<PCB>> displayQueue1;
    std::deque<std::shared_ptr<PCB>> memoryQueue;
    std::vector<std::string> memoryCache;

    int displayQueueExitTime0 = std::numeric_limits<int>::max();
    int displayQueueExitTime1 = std::numeric_limits<int>::max();
    int memoryQueueExitTime = std::numeric_limits<int>::max();


    int quantum = 100;	 //Quantum time of the Cpu.
    int remainingQuantumTime = 100;	//Remaining quantum time of the Cpu.
    int currentTime;	//Current time.
    int cpuFinishTime = 0;	//Cpu finish time of the current process.
    int nextClosestEventTime = 0;	//Next closest event time.
    bool isCpuBusy = false;	//True if the Cpu is busy.


};

#endif
