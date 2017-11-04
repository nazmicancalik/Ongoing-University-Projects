#ifndef SCHEDULINGMANAGER_H
#define SCHEDULINGMANAGER_H

#include "PCB.hpp"
#include "IOManager.hpp"
#include <queue>
#include <memory>
#include <algorithm>

class SchedulingManager
{

public:
  SchedulingManager();
  SchedulingManager(std::string &processFile);
  ~SchedulingManager();
  void initializeArrivalQueue();
  void printProcesses();
  void initialize();
  void simulate();

private:
  //void initializeProcessInstructions();
  std::string processFile;
  std::vector<std::shared_ptr<PCB>> arrivalQueue;
  std::queue<std::shared_ptr<PCB>> readyQueue;
};

#endif