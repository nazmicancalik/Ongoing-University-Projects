//
// Created by gray on 04.11.2017.
//

#ifndef SCHEDULINGMANAGER_H
#define SCHEDULINGMANAGER_H

#include "PCB.h"
#include "IOManager.h"
#include <queue>
#include <memory>
#include <algorithm>

class SchedulingManager {

public:
    SchedulingManager();

    SchedulingManager(std::string &processFile);

    ~SchedulingManager();

    void initializeArrivalQueue();

    void printProcesses();

    void executeRoundRobin();

    void simulate();

private:
    void putArrivedPCBToReadyQueue();

    void executeProcess();

    void takeProcessIntoCpu();

    void getProcessOutOfCpu();

    bool canExecuteNextInstruction();

    std::string processFile;
    std::vector<std::shared_ptr<PCB>> arrivalQueue;
    std::queue<std::shared_ptr<PCB>> readyQueue;
    const int quantum = 100;
    int remainingQuantumTime = 100;
    int currentTime;
    int nextClosestEventTime = 0;
    bool isCpuBusy = false;
    std::shared_ptr<PCB> currentProcessInCpu;
};

#endif
