#include "SchedulingManager.h"

int main(int argc, char **argv)
{

    std::string definitionFileName;
    std::string OUTPUT_FILE_NAME = "my_output.txt";

    if (argc < 2)
    {
        definitionFileName = "/home/gray/workspace/Ongoing-University-Projects/discrete2/definition.txt";
    }
    else
    {
        definitionFileName = argv[1];
    }

    std::string processFileName = definitionFileName;
    SchedulingManager schedulingManager;
    schedulingManager = SchedulingManager(processFileName, OUTPUT_FILE_NAME);
    schedulingManager.simulate();

    return 0;
}
