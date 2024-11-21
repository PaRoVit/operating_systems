#include "implementations.hpp"

void Task(const std::string& command) {
    if (command == "1") {
        std::cout << "PrimeCount function" << std::endl;
        std::cout << "Enter the beginning and the end of the gap:" << std::endl;
        int A, B;
        std::cin >> A >> B;
        int result = PrimeCount(A, B);
        std::cout << "Result of count = " << result << std::endl;
    } else if (command == "2") {
        std::cout << "Pi function" << std::endl;
        std::cout << "Enter the length of the row:" << std::endl;
        int K;
        std::cin >> K;
        float result2 = Pi(K);
        std::cout << "Pi = " << result2 <<std::endl;
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}

int main() {
    std::string command;
    while(true) {
        std::cout << "Enter the command (1 - PrimeCount, 2 - PiFunc, 0 - exit): ";
        std::cin >> command;
        if (command == "0") {
            break;
        }
        Task(command);
    }
    return 0;
}