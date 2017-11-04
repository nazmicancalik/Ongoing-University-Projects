#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <memory>

namespace IOManager
{

//Writes to the file.
inline void write(const std::string fileName, const std::string line)
{
    std::ofstream fileOperator(fileName, std::ios_base::app);
    fileOperator << line << "\n";
    fileOperator.close();
}

inline std::shared_ptr<std::vector<std::string>> readTokens(const std::string fileName)
{
    std::string name = "./input/" + fileName;
    std::ifstream fileOperator(name);
    std::shared_ptr<std::vector<std::string>> tokens =
        std::make_shared<std::vector<std::string>>(std::vector<std::string>());

    while (!fileOperator.eof())
    {
        std::string line;
        fileOperator >> line;
        tokens->push_back(line);
        std::cout << line << "\n";
    }

    fileOperator.close();
    return tokens;
}
}

#endif