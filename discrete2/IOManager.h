#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <memory>

namespace IOManager {

//Writes to the file.
    inline void write(const std::string fileName, std::string line) {
        std::ofstream fileOperator(fileName, std::ios_base::app);
        fileOperator << line;
        fileOperator.close();
    }

    inline std::shared_ptr<std::vector<std::string>> readTokens(std::string fileName) {
        std::shared_ptr<std::vector<std::string>> tokens =
                std::make_shared<std::vector<std::string>>(std::vector<std::string>());
        std::ifstream fileOperator(fileName);

        while (!fileOperator.eof()) {
            std::string line;
            fileOperator >> line;
            tokens->push_back(line);
        }

        fileOperator.close();
        return tokens;
    }
}

#endif