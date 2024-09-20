#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "parent.hpp"

// Утилита для чтения содержимого файла в строку
std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Тест на проверку работы родительского процесса с вводом
TEST(ParentProcessTest, OddLinesGoToFirstFile) {
    // Входные данные
    std::string filename1 = "/home/pvrozhkov/operating_system/operating_systems/out1.txt";
    std::string filename2 = "/home/pvrozhkov/operating_system/operating_systems/out2.txt";

    // Создаем имитацию ввода. Это заменяет ввод пользователя в консоли.

    std::istringstream input_stream("/home/pvrozhkov/operating_system/operating_systems/out1.txt\n"
                                    "/home/pvrozhkov/operating_system/operating_systems/out2.txt\n"
                                    "Hello\nWorld\nTest\n123\n");
    std::cin.rdbuf(input_stream.rdbuf());  // Перенаправляем std::cin на наш поток

    // Вызываем функцию обработки ввода
    process_input_lines(filename1, filename2);

    // Ждем завершения записи дочерними процессами
    sleep(1);  // Для реальных приложений лучше использовать waitpid
    // Читаем и проверяем содержимое файлов
    std::string content1 = readFileContent(filename1);
    std::string content2 = readFileContent(filename2);

    // Проверяем, что в первый файл попали нечетные строки
    EXPECT_EQ(content1, "Hll\nTst\n"); // Гласные удалены

    // Проверяем, что во второй файл попали четные строки
    EXPECT_EQ(content2, "Wrld\n123\n"); // Гласные удалены
}

// Очистка файлов после теста
TEST(ParentProcessTest, CleanupFiles) {
    std::remove("/home/pvrozhkov/operating_system/operating_systems/out1.txt");
    std::remove("/home/pvrozhkov/operating_system/operating_systems/out2.txt");
}
