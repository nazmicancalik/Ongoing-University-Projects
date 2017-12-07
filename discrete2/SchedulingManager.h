//
// Created by gray on 04.11.2017.
//

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
    SchedulingManager();

    SchedulingManager(std::string &processFile,std::string &outputFile);

    ~SchedulingManager();

    void initializeArrivalQueue();

    void executeRoundRobin();

    void simulate();

private:
    void putArrivedPCBToReadyQueue();

    void executeInstruction();

    void takeProcessIntoCpu();

    void getProcessOutOfCpu();

    bool canExecuteNextInstruction();

    void printQueue();

    void initializeSemaphores();

    std::string processFile;
    std::string outputFile;
    std::vector<std::shared_ptr<PCB>> arrivalQueue;
    std::deque<std::shared_ptr<PCB>> readyQueue;
    std::vector<std::shared_ptr<PCB>> finishedProcesses;
    std::vector<std::shared_ptr<Semaphore>> semaphores;
    std::shared_ptr<PCB> currentProcessInCpu;
    int quantum = 100;
    int remainingQuantumTime = 100;
    int currentTime;
    int cpuFinishTime = 0;
    int nextClosestEventTime = 0;
    bool isCpuBusy = false;

};

#endif
