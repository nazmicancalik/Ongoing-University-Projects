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

void Semaphore::appendProcess(std::shared_ptr<PCB> aProcess) {
    semaphore_queue.push_back(aProcess);
}

void Semaphore::wait_S() {
    value--;
}

void Semaphore::signal_S() {
    value++;
}

int Semaphore::getIndex() {
    return index;
}

int Semaphore::getValue() {
    return value;
}

std::shared_ptr<std::deque<std::shared_ptr<PCB>>> Semaphore::getSemaphoreQueue() {
    return std::shared_ptr<std::deque<std::shared_ptr<PCB>>>(&semaphore_queue);
}
