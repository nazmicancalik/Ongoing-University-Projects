#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

namespace IOManager
{

//Writes to the file.
inline void write(std::string fileName, std::string line)
{
    std::ofstream fileOperator(fileName, std::ios_base::app);
    fileOperator << line << "\n";
    fileOperator.close();
}

inline std::vector<std::string> readTokens(std::string fileName)
{
    std::ifstream fileOperator(fileName);
    std::vector<std::string> tokens;

    while (!fileOperator.eof())
    {
        std::string line;
        fileOperator >> line;
        tokens.push_back(line);
        std::cout << line << "\n";
    }

    fileOperator.close();
    return tokens;
}
}

#endif