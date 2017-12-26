cmake_minimum_required(VERSION 3.7)
project(discrete3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        IOManager.h
        main.cpp
        PCB.cpp
        PCB.h
        SchedulingManager.cpp
        SchedulingManager.h
        Semaphore.cpp
        Semaphore.h)

add_executable(discrete3 ${SOURCE_FILES})