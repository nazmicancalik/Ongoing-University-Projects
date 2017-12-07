cmake_minimum_required(VERSION 3.7)
project(discrete2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        IOManager.h
        main.cpp
        PCB.cpp
        PCB.h
        SchedulingManager.cpp
        SchedulingManager.h 1.h Semaphore.cpp Semaphore.h)

add_executable(discrete2 ${SOURCE_FILES})