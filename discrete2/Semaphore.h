//
// Created by gray on 07.12.2017.
//

#ifndef DISCRETE2_SEMAPHORE_H
#define DISCRETE2_SEMAPHORE_H

#include <vector>
#include <memory>
#include <deque>
#include "PCB.h"

class Semaphore {
public:
    Semaphore();

    Semaphore(int anIndex);

    ~Semaphore();

    void appendProcess(std::shared_ptr<PCB> aProcess);

    void signal_S();

    void wait_S();

    int getIndex();

    int getValue();

    std::shared_ptr<std::deque<std::shared_ptr<PCB>>> getSemaphoreQueue();

private:
    int index;
    int value;  // Semaphore value

    std::deque<std::shared_ptr<PCB>> semaphore_queue;
};

#endif //DISCRETE2_SEMAPHORE_H
