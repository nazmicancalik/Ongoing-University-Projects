cmake_minimum_required(VERSION 3.7)
project(discrete)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        IOManager.h
        main.cpp
        PCB.cpp
        PCB.h
        SchedulingManager.cpp
        SchedulingManager.h)

add_executable(discrete ${SOURCE_FILES})