#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

void remove_vowels(char *str) {
    char *write_ptr = str;
    while (*str) {
        if (!strchr("AEIOUaeiou", *str)) {
            *write_ptr++ = *str;
        }
        str++;
    }
    *write_ptr = '\0';  // Завершаем строку
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <output file> <input string>\n";
        return 1;
    }

    char *input_string = argv[2];

    // Удаляем гласные
    remove_vowels(input_string);

    // Запись в файл
    std::ofstream outfile(argv[1], std::ios::app);
    if (!outfile) {
        perror("fopen");
        return 1;
    }

    outfile << input_string << std::endl;
    outfile.close();

    return 0;
}
