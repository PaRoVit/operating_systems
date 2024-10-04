#include "parent.hpp"


int main(void) {
    
    ParentProcess("/home/pvrozhkov/operating_system/operating_systems/build/lab1/child1", "/home/pvrozhkov/operating_system/operating_systems/build/lab1/child2", std::cin);

    exit(EXIT_SUCCESS);
}