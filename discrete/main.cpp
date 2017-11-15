#include "SchedulingManager.h"

int main(int argc, char **argv) {
    std::string processFileName = "definition_example_2.txt";
    SchedulingManager schedulingManager;
    schedulingManager = SchedulingManager(processFileName);
    schedulingManager.simulate();
}
