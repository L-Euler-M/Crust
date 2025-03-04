#include "CrustInterpreter.h"
#include <iostream>

Crust::Cpp cpp;
std::string tp;

int main(int argc, char* argv[]) {
    std::cout << "Interpreter" <<std::endl;

    std::string filename;

    if (argc > 1) {
        tp = argv[1];
    }
    else {
        std::cout << "Enter the path to the file: ";
        std::getline(std::cin, tp);
    }
    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;

    filename = tp; 

    cpp.executeFile(filename);

    std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;
    system("pause");
    return 0;
}
