#include "utils.hpp"

int main() {
    const char *pathToLib1 = getenv("PATH_TO_LIB1");
    const char *pathToLib2 = getenv("PATH_TO_LIB2");
    // bash: export PATH_TO_LIB1="/home/pvrozhkov/operating_system/operating_systems/build/lab4/liblib1.so"
    // bash: export PATH_TO_LIB2="/home/pvrozhkov/operating_system/operating_systems/build/lab4/liblib2.so"

    void* libraryHandle = LoadLibrary(pathToLib1);
    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(libraryHandle, "PrimeCount");
    PiFunc Pi = (PiFunc)dlsym(libraryHandle, "Pi");

    std::string command;
    while(true) {
        std::cout << "Enter the command (0 - switch implementation, 1 - PrimeCount, 2 - PiFunc, e - exit): ";
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
            PrimeCount = (PrimeCountFunc)dlsym(libraryHandle, "PrimeCount");
            Pi = (PiFunc)dlsym(libraryHandle, "Pi");
        } else {
            if (command == "1") {
                std::cout << "PrimeCount function:" << std::endl;
                std::cout << "Enter the beginning and the end of the gap:" << std::endl;
                int A, B;
                std::cin >> A >> B;
                int result = PrimeCount(A, B);
                std::cout << "Result of count = " << result << std::endl;
            } else if (command == "2") {
                std::cout << "Pi function:" << std::endl;
                std::cout << "Enter the length of the row:" << std::endl;
                int K;
                std::cin >> K;
                float result2 = Pi(K);
                std::cout << "Pi = " << result2 <<std::endl;
            } else {
                std::cout << "Invalid command" << std::endl;
            }
        }
    }
    UnloadLibrary(libraryHandle);
    return 0;
}