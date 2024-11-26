#include "utils.hpp"

void TaskDynamic(const char *pathToLib1, const char *pathToLib2) {
    void* libraryHandle = LoadLibrary(pathToLib1);
    SquareFunc Square = (SquareFunc)dlsym(libraryHandle, "Square");
    EFunc E = (EFunc)dlsym(libraryHandle, "E");

    std::string command;
    while(true) {
        std::cout << "Enter the command (0 - switch implementation, 1 - SquareFunc, 2 - EFunc, e - exit): ";
        std::cin >> command;
        if (command == "e") {
            break;
        } else if (command == "0") {
            std::cout << "Enter the library (1 or 2): ";
            std::cin >> command;
            if (command == "1") {
                libraryHandle = LoadLibrary(pathToLib1);
            } else if (command == "2") {
                libraryHandle = LoadLibrary(pathToLib2);
            } else {
                std::cout << "Invalid library" << std::endl;
            }
            Square = (SquareFunc)dlsym(libraryHandle, "Square");
            E = (EFunc)dlsym(libraryHandle, "E");
        } else {
            if (command == "1") {
                std::cout << "SquareFunc:" << std::endl;
                std::cout << "Enter sides A and B:" << std::endl;
                float A, B;
                std::cin >> A >> B;
                float result = Square(A, B);
                if (result == -1) {
                    std::cout << "Invalid value" << std::endl;
                    continue;
                }
                std::cout << "Area = " << result << std::endl;
            } else if (command == "2") {
                std::cout << "E function:" << std::endl;
                std::cout << "Enter X:" << std::endl;
                int x;
                std::cin >> x;
                float result2 = E(x);
                if (result2 == -1) {
                    std::cout << "Invalid value" << std::endl;
                    continue;
                }
                std::cout << "E = " << result2 <<std::endl;
            } else {
                std::cout << "Invalid command" << std::endl;
            }
        }
    }
    UnloadLibrary(libraryHandle);
}


int main() {
    const char *pathToLib1 = getenv("PATH_TO_LIB1");
    const char *pathToLib2 = getenv("PATH_TO_LIB2");
    // export PATH_TO_LIB1="/home/pvrozhkov/operating_system/operating_systems/build/lab4/liblib1.so"
    // export PATH_TO_LIB2="/home/pvrozhkov/operating_system/operating_systems/build/lab4/liblib2.so"
    TaskDynamic(pathToLib1, pathToLib2);
    
    return 0;
}