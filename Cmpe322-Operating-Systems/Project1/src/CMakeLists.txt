cmake_minimum_required(VERSION 3.7)
project(discrete)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SchedulingManager.cpp SchedulingManager.h IOManager.h PCB.cpp PCB.h)
add_executable(discrete ${SOURCE_FILES})