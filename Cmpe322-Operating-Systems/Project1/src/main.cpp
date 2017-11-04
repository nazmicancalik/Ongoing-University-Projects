#include "SchedulingManager.hpp"

int main(int argc, char **argv)
{
    std::string processFileName = "./input/definition.txt";
    SchedulingManager schedulingManager;
    schedulingManager = SchedulingManager(processFileName); 
    schedulingManager.initializeArrivalQueue();
    schedulingManager.simulate();
}
