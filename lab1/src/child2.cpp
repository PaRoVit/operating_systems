#include <iostream>
#include <string>
#include <unistd.h> 
#include <algorithm> 
#include <vector>


std::string removeVowels(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || 
               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
               c == 'y' || c == 'Y';
    }), result.end());
    return result;
}

int main() {
    int input_size;
    // Читаем из стандартного ввода, пока есть данные(ориентируемся на размер входной строки)
    while (read(STDIN_FILENO, &input_size, sizeof(int)) > 0) {
        // std::cerr << input_size << " получен размер строки\n ";
        std::string input_string(input_size, '\0');
        read(STDIN_FILENO, &input_string[0], input_size);
        
        // Удаляем гласные из строки
        std::string result_string = removeVowels(input_string);

        // std::cerr << input_string << ' ' << result_string << '\n';

        // Записываем строку 
        write(STDOUT_FILENO, result_string.c_str(), result_string.size());
        write(STDOUT_FILENO, "\n", 1);
    }

    return 0;
}