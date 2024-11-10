#include <utils.hpp>




int main(int argc, char* argv[]) {
    // std::cerr << argv[0]<< ' '<< argv[1] << "\n";
    if (argc < 2) {
        std::cerr << "Wrong argc in child1\n";
        exit(-6);
    }
    const char* semaphoreName = argv[1];
    const char* mmapFilename = argv[2];
    ProcessChild(semaphoreName, mmapFilename);
}
