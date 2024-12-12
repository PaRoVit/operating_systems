#include "gtest/gtest.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <thread>

// Вспомогательная функция для выполнения команды в клиенте
std::string execute_client_command(const std::string &command) {
    std::ostringstream cmd;
    cmd << "echo \"" << command << "\" | ./client";
    FILE *pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) throw std::runtime_error("Failed to run command");
    
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Тест команды create
TEST(ClientApiTest, CreateNodeSuccess) {
    std::string output = execute_client_command("create 1 -1\nexit\n");
    EXPECT_NE(output.find("Ok"), std::string::npos) << "Expected successful create command";
}

// Тест на создание узла с несуществующим родителем
TEST(ClientApiTest, CreateNodeInvalidParent) {
    std::string output = execute_client_command("create 2 999\nexit\n");
    EXPECT_NE(output.find("Error: Parent node not found"), std::string::npos);
}

// Тест команды ping
TEST(ClientApiTest, PingNodeSuccess) {
    execute_client_command("create 3 -1\n");
    std::string output = execute_client_command("ping 3\nexit\n");
    EXPECT_NE(output.find("Ok: 1"), std::string::npos) << "Expected successful ping command";
}

// Тест ping для несуществующего узла
TEST(ClientApiTest, PingNodeNotFound) {
    std::string output = execute_client_command("ping 999\nexit\n");
    EXPECT_NE(output.find("Error: Not found"), std::string::npos);
}

// Тест команды exec
TEST(ClientApiTest, ExecCommand) {
    execute_client_command("create 4 -1\n");
    std::string output = execute_client_command("exec 4 var_name value\nexit\n");
    EXPECT_NE(output.find("Ok"), std::string::npos) << "Expected successful exec command";
}
