#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "parent.hpp"

std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename); // открываем файл и читаем его содержимое
    std::stringstream buffer; // создаём буфер
    buffer << file.rdbuf(); // содержимое файла в буфер
    return buffer.str(); //возвращаем содержимое буфера, т е файла
}

TEST(ParentProcessTest, CheckStringsInFiles) {
    // файлы в которые будет всё записано 
    std::string filename1 = "/home/pvrozhkov/operating_system/operating_systems/build/tests/out1.txt";
    std::string filename2 = "/home/pvrozhkov/operating_system/operating_systems/build/tests/out2.txt";
    
    // пути к дочерним 
    const char* pathToChild1 = "/home/pvrozhkov/operating_system/operating_systems/build/lab1/child1";
    const char* pathToChild2 = "/home/pvrozhkov/operating_system/operating_systems/build/lab1/child2";

    // входной поток с тестами 
    std::istringstream input_stream("Hello\nWorld\nTest\n123\n");

    // подмена потоков
    std::streambuf* cinbuf = std::cin.rdbuf(); 
    std::cin.rdbuf(input_stream.rdbuf());

    ParentProcess(pathToChild1, pathToChild2, input_stream);

    // возвращаем исходный поток
    std::cin.rdbuf(cinbuf);

    std::string content1 = readFileContent(filename1);
    std::string content2 = readFileContent(filename2);

    // сверка того что в файле и что мы ожидаем
    EXPECT_EQ(content1, "Hll\nTst\n"); 

    EXPECT_EQ(content2, "Wrld\n123\n"); 
}

// Очистка файлов после теста
TEST(ParentProcessTest, CleanupFiles) {
    std::remove("/home/pvrozhkov/operating_system/operating_systems/build/tests/out2.txt");
    std::remove("/home/pvrozhkov/operating_system/operating_systems/build/tests/out1.txt");
}
