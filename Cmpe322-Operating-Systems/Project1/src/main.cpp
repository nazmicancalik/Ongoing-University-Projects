#include "SchedulingManager.h"

int main(int argc, char **argv) {
    std::string processFileName = "definition";
    SchedulingManager schedulingManager;
    schedulingManager = SchedulingManager(processFileName);
    schedulingManager.simulate();
}
