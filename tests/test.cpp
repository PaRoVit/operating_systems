#include <gtest/gtest.h>

#include <set>
#include "../include/tree.hpp"
#include "../include/zmq_manage.hpp"
// export PROGRAM_PATH="/home/pablor/OS/lab5-7/operating_systems/build/lab5-7/server"

TEST(Tests, PingTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);

    std::string ans = task.Send("ping 1", 1);
    EXPECT_EQ(ans, "Ok: 1");

    ans = task.Send("ping 2", 2);
    EXPECT_EQ(ans, "Error: Not found");

    task.Kill();
}

TEST(Tests, ExecTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);

    std::string ansIn = task.Send("exec 1 my 5", 1);
    std::string ansOut = task.Send("exec 1 my", 1);
    EXPECT_EQ(ansOut, "Ok:1: 5");

    task.Kill();
}

TEST(Tests, FullTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::string ans;
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);
    task.Send("create 2 -1", 1);
    Nodes.insert(2);
    task.Send("create 3 -1", 2);
    Nodes.insert(3);
    
    ans = task.Send("ping 1", 1);
    EXPECT_EQ(ans, "Ok: 1");
    ans = task.Send("ping 2", 2);
    EXPECT_EQ(ans, "Ok: 1");

    ans = task.Send("exec 3 qwe", 2);
    EXPECT_EQ(ans, "Ok:3 not found");

    ans = task.Send("exec 3 qwe 10", 3);
    EXPECT_EQ(ans, "Ok:3");

    task.Kill();
}

int main(int argc, char *argv[]) {
    std::cout << getenv("PROGRAM_PATH") << std::endl;
    // bash: export PROGRAM_PATH="/home/kristinab/ubuntu_main/OS_labs/build/lab5-7/server"
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}