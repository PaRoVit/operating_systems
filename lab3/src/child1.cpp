#include <utils.hpp>

int main(int argc, char* argv[]) {
    // std::cerr << argv[0]<< ' '<< argv[1] << "\n";
    if (argc < 2) {
        std::cerr << "Wrong argc in child1\n";
        exit(-6);
    }
    const char* semaphoreParentName = argv[1];
    const char* semaphoreChildName = argv[2];
    const char* mmapFilename = argv[3];
    ProcessChild(semaphoreParentName, semaphoreChildName, mmapFilename);
}
