#ifndef SCHEDULINGMANAGER_H
#define SCHEDULINGMANAGER_H

#include "PCB.hpp"
#include "IOManager.hpp"
#include <queue>

class SchedulingManager
{

public:
  SchedulingManager();
  SchedulingManager(std::string &processFile);
  ~SchedulingManager();
  void initializeArrivalQueue();
  void initialize();
  void simulate();

private:
  std::string processFile;
  std::queue<std::shared_ptr<PCB>> arrivalQueue;
  std::queue<std::shared_ptr<PCB>> readyQueue;
};

#endif