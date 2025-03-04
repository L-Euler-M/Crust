//
// Created by manju on 06-06-2024.
//

#ifndef SPARKFILELOADER_H
#define SPARKFILELOADER_H
#include <string>
#include "CrustInterpreter.h"
#include <fstream>
namespace Loader {
    std::vector<std::string> lines;


    void LoadFile(const std::string& filename)
    {
        std::ifstream file;
        file.open(filename, std::ios::in);

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                lines.push_back(line);
            }
            file.close();
        }


    }

}

#endif //SPARKFILELOADER_H
