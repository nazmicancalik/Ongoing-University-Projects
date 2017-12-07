//
// Created by gray on 07.12.2017.
//

#ifndef DISCRETE2_SEMAPHORE_H
#define DISCRETE2_SEMAPHORE_H

#include <vector>
#include <bits/shared_ptr.h>
#include "PCB.h"

class Semaphore {
public:
    Semaphore();
    Semaphore(int anIndex);
    ~Semaphore();

    void appendProcess(std::shared_ptr<PCB> aProcess);
    void wait_S();
    void signal_S();

private:
    int index;
    int value;  // Semaphore value

    std::vector<std::shared_ptr<PCB>> semaphore_queue;
};

#endif //DISCRETE2_SEMAPHORE_H
