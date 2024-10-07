#include <iostream>
#include <string>
#include <unistd.h> 
#include <algorithm> 
#include <vector>
#include <utils.hpp>




int main() {
    std::string input_string;

    // Чтение строк из потока, пока они поступают
    while (readStringFromPipe(STDIN_FILENO, input_string) > 0) {
        std::string result_string = removeVowels(input_string); // удаление гласных
        writeStringToPipe(STDOUT_FILENO, result_string);        // запись результата
    }

    return 0;
}