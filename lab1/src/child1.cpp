#include <iostream>
#include <string>
#include <unistd.h> 
#include <algorithm> 
#include <vector>


std::string removeVowels(const std::string& input) {
    std::string result = input;
    // обход по строке при помощи лямбда функции, гласные смещаются назад при помощи remove_if, а erase их удаляет
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || 
               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
               c == 'y' || c == 'Y';
    }), result.end());
    return result;
}

int main() {
    int input_size;
    // получаем на вход рамер входной строки, и если норм то идём дальше
    while (read(STDIN_FILENO, &input_size, sizeof(int)) > 0) {
        // std::cerr << input_size << " получен размер строки\n ";
        
        std::string input_string(input_size, '\0'); // инициализируем строку нужного размера
        read(STDIN_FILENO, &input_string[0], input_size); // и записываем туда полученную строку, указывая на её начало
        
        // удаляем гласные из строки
        std::string result_string = removeVowels(input_string);

        // std::cerr << input_string << ' ' << result_string << '\n';

        // выкидывем строку в поток, откуда она попадает в нужный файл 
        write(STDOUT_FILENO, result_string.c_str(), result_string.size());
        write(STDOUT_FILENO, "\n", 1);
    }

    return 0;
}