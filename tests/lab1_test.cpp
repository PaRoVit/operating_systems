#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <parent.hpp>
#include <cstdio>

// export CHILD1_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab1/child1"
// export CHILD2_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab1/child2"

class TempFile {
public:
    // конструктор временного файла
    TempFile(const std::string& filename) : filename_(filename) {
        std::ofstream file(filename_);
        if (!file) {
            throw std::runtime_error("Failed to create temporary file: " + filename_);
        }
    }

    // возврат имени файла
    const std::string& getFilename() const {
        return filename_;
    }

    // деструктор
    ~TempFile() {
        std::remove(filename_.c_str());
    }

private:
    std::string filename_;
};


std::string readFileContent(const std::string& filename) {
    std::ifstream file(filename); // открываем файл и читаем его содержимое
    std::stringstream buffer; // создаём буфер
    buffer << file.rdbuf(); // содержимое файла в буфер
    return buffer.str(); //возвращаем содержимое буфера, т е файла
}

TEST(ParentProcessTest, CheckStringsInFiles) {
    // файлы в которые будет всё записано 
    TempFile filename1("out1.txt");
    TempFile filename2("out2.txt");
    
    // пути к дочерним 
    const char* pathToChild1 = std::getenv("CHILD1_PATH");
    const char* pathToChild2 = std::getenv("CHILD2_PATH");

    // проверка наличия пути
    ASSERT_NE(pathToChild1, nullptr) << "CHILD1_PATH environment variable is not set";
    ASSERT_NE(pathToChild2, nullptr) << "CHILD2_PATH environment variable is not set";

    // входной поток с тестами 
    std::istringstream input_stream("Hello\nWorld\nTest\n123\n");

    ParentProcess(pathToChild1, pathToChild2, input_stream);


    std::string content1 = readFileContent(filename1.getFilename());
    std::string content2 = readFileContent(filename2.getFilename());

    // сверка того что в файле и что мы ожидаем
    EXPECT_EQ(content1, "Hll\nTst\n"); 
    EXPECT_EQ(content2, "Wrld\n123\n"); 
}

TEST(ParentProcessTest, EmptyString) {
    // файлы в которые будет всё записано 
    TempFile filename1("out1.txt");
    TempFile filename2("out2.txt");
    
    // пути к дочерним 
    const char* pathToChild1 = std::getenv("CHILD1_PATH");
    const char* pathToChild2 = std::getenv("CHILD2_PATH");

    ASSERT_NE(pathToChild1, nullptr) << "CHILD1_PATH environment variable is not set";
    ASSERT_NE(pathToChild2, nullptr) << "CHILD2_PATH environment variable is not set";

    // входной поток с тестами 
    std::istringstream input_stream("");

    ParentProcess(pathToChild1, pathToChild2, input_stream);


    std::string content1 = readFileContent(filename1.getFilename());
    std::string content2 = readFileContent(filename2.getFilename());

    // сверка того что в файле и что мы ожидаем
    EXPECT_EQ(content1, ""); 
    EXPECT_EQ(content2, "");
}