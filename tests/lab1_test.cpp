#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "parent.hpp"

std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST(ParentProcessTest, OddLinesGoToFirstFile) {
    // Имена файлов для записи
    std::string filename1 = "/home/pvrozhkov/operating_system/operating_systems/out1.txt";
    std::string filename2 = "/home/pvrozhkov/operating_system/operating_systems/out2.txt";

    std::istringstream input_stream("/home/pvrozhkov/operating_system/operating_systems/out1.txt\n"
                                    "/home/pvrozhkov/operating_system/operating_systems/out2.txt\n"
                                    "Hello\nWorld\nTest\n123\n");
    std::cin.rdbuf(input_stream.rdbuf());  // Подмена потока

    process_input_lines(filename1, filename2);

    std::string content1 = readFileContent(filename1);
    std::string content2 = readFileContent(filename2);

    EXPECT_EQ(content1, "Hll\nTst\n"); 

    EXPECT_EQ(content2, "Wrld\n123\n"); 
}

// Очистка файлов после теста
TEST(ParentProcessTest, CleanupFiles) {
    std::remove("/home/pvrozhkov/operating_system/operating_systems/out1.txt");
    std::remove("/home/pvrozhkov/operating_system/operating_systems/out2.txt");
}
