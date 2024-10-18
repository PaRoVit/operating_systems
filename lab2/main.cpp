
#include <sort.hpp>



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid starting parameters, program was not launched\n";
        return 1;
    }

    int threadsAmount = std::stoi(argv[1]);
    run(threadsAmount);
    return 0;
}