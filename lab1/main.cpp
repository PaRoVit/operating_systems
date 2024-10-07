#include "parent.hpp"


int main(void) {

    const char* pathToChild1 = std::getenv("CHILD1_PATH");
    const char* pathToChild2 = std::getenv("CHILD2_PATH");
    
    ParentProcess(pathToChild1, pathToChild2, std::cin);

    exit(EXIT_SUCCESS);
}

// export CHILD1_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab1/child1"
// export CHILD2_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab1/child2"