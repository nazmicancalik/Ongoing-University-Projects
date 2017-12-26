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
	//Default Constructor.
    Semaphore();

	//Constructor.
    Semaphore(int anIndex);

    //Destructor.
    ~Semaphore();

    //Appends a process into semaphore queue.
    void appendProcess(std::shared_ptr<PCB> aProcess);

    //Just increments the semaphore value.
    void signal_S();

    //Just decrements the semaphore value.
    void wait_S();

    //Returns the index of the semaphore.
    int getIndex();

    //Returns the value of the semaphore.
    int getValue();

    //Semaphore queue.
    std::deque<std::shared_ptr<PCB>>* getSemaphoreQueue();
    
private:
    int index;	//Index (name) of the semaphore.
    int value;  // Semaphore value
    std::deque<std::shared_ptr<PCB>> semaphore_queue;	//Semaphore queue.
};

#endif //DISCRETE2_SEMAPHORE_H
