#include <iostream>
#include <string>
#include "parent.hpp"

int main() {
    std::string filename1, filename2;

    std::cout << "Введите имя файла для процесса child1: ";
    std::getline(std::cin, filename1);

    std::cout << "Введите имя файла для процесса child2: ";
    std::getline(std::cin, filename2);

    std::cout << "Введите строки для обработки (Ctrl+D для завершения):\n";
    process_input_lines(filename1, filename2);

    return 0;
}
