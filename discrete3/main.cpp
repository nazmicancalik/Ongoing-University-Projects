#include "SchedulingManager.h"

int main(int argc, char **argv)
{

    std::string definitionFileName;
    std::string OUTPUT_FILE_NAME = "output.txt";

    if (argc < 2)
    {
        //TODO Change this before pushing to the repository.
        definitionFileName = "/home/gray/workspace/Ongoing-University-Projects/discrete3/other/conf_2/definition.txt";
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
