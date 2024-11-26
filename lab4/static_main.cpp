#include "implementations.hpp"

void TaskStatic() {
    std::string command;
    while(true) {
        std::cout << "Enter the command (1 - SquareFunc, 2 - EFunc, 0 - exit): ";
        std::cin >> command;
        if (command == "0") {
            break;
        }
        if (command == "1") {
            std::cout << "SquareFunc: " << std::endl;
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
            std::cout << "E function" << std::endl;
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

int main() {
    TaskStatic();
    return 0;
}