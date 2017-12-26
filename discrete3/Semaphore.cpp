//
// Created by gray on 07.12.2017.
//

#include "Semaphore.h"

//Default Constructor
Semaphore::Semaphore() {}

//Constructor
Semaphore::Semaphore(int anIndex) {
    index = anIndex;
    value = 1;
}

//Destructor
Semaphore::~Semaphore() {}

//Appends a process into semaphore queue.
void Semaphore::appendProcess(std::shared_ptr<PCB> aProcess) {
    semaphore_queue.push_back(aProcess);
}

//Decrements the semaphore value. Queue operations (decisions) are made in Scheduling Manager.
void Semaphore::wait_S() {
    value--;
}

//Increments the semaphore value. Queue operations (decisions) are made in Scheduling Manager.
void Semaphore::signal_S() {
    value++;
}

//Returns the index of the semaphore.
int Semaphore::getIndex() {
    return index;
}

//Returns the value of the semaphore.
int Semaphore::getValue() {
    return value;
}

//Returns a pointer to the semaphore queue.
std::deque<std::shared_ptr<PCB>>* Semaphore::getSemaphoreQueue() {
    return &semaphore_queue;
}
