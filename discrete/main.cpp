#include "SchedulingManager.h"

int main(int argc, char **argv) {
    std::string processFileName = "definition.txt";
    SchedulingManager schedulingManager;
    schedulingManager = SchedulingManager(processFileName);
    schedulingManager.simulate();
}
