#include <parent.hpp>

// export CHILD1_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab3/child1"
// export CHILD2_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab3/child2"

void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn) {
    std::string file1 = "out1.txt";
    std::string file2 = "out2.txt";
    int file1Descr = open(file1.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int file2Descr = open(file2.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    ErrorChecking(file1Descr, "File1 opening error");
    ErrorChecking(file2Descr, "File2 opening error");

    int firstMmapFile = CreateShm(MMAP_NAME1);
    int secondMmapFile = CreateShm(MMAP_NAME2);
    ErrorChecking(firstMmapFile, "Memory area1 opening error");
    ErrorChecking(secondMmapFile, "Memory area2 opening error");

    sem_unlink(SEM_NAME);
    sem_t* semaphore = CreateSemaphore(SEM_NAME, 2);
    ErrorChecking(semaphore == SEM_FAILED ? -1 : 0, "Semaphore open error");

    pid_t firstProcessID = CreateFork();
    ErrorChecking(firstProcessID, "Fork1 error");
    if (firstProcessID == 0) {
        pid_t secondProcessID = CreateFork();
        ErrorChecking(secondProcessID, "Fork2 error");

        if (secondProcessID == 0) {
            dup2(file2Descr, STDOUT_FILENO);
            execl(pathToChild2, pathToChild2, SEM_NAME, MMAP_NAME2, nullptr);
            perror("execl failed for child2");
            exit(EXIT_FAILURE);
        } else {
            dup2(file1Descr, STDOUT_FILENO);
            execl(pathToChild1, pathToChild1, SEM_NAME, MMAP_NAME1, nullptr);
            perror("execl failed for child1");
            exit(EXIT_FAILURE);
        }
    } else {
        char* firstMmap = MapSharedMemory(getpagesize(), firstMmapFile);
        char* secondMmap = MapSharedMemory(getpagesize(), secondMmapFile);
        ErrorChecking(firstMmap == MAP_FAILED || secondMmap == MAP_FAILED ? -1 : 0, "Error creating memory maps");

        int firstPosition = 0, secondPosition = 0, firstLength = 0, secondLength = 0;
        int counterStrings = 1;
        std::string inputString;
        while (std::getline(streamIn, inputString)) {
            int string_length = inputString.length();
            if (counterStrings % 2 != 0) {
                firstLength += string_length + 1;
                ftruncate(firstMmapFile, firstLength);
                for (char ch : inputString) firstMmap[firstPosition++] = ch;
                firstMmap[firstPosition++] = '\n';
            } else {
                secondLength += string_length + 1;
                ftruncate(secondMmapFile, secondLength);
                for (char ch : inputString) secondMmap[secondPosition++] = ch;
                secondMmap[secondPosition++] = '\n';
            }
            counterStrings++;
        }
        SetSemaphoreValue(semaphore, 1);

        int status;
        waitpid(-1, &status, 0);
        waitpid(-1, &status, 0);

        munmap(firstMmap, firstLength);
        munmap(secondMmap, secondLength);
        
        shm_unlink(MMAP_NAME1); // когда эти строки есть, то в выходные файлы ничего не пишет
        shm_unlink(MMAP_NAME2); // а когда их нет, пишет то, что я вводил в предыдущий раз

        close(file1Descr);
        close(file2Descr);
        close(firstMmapFile);
        close(secondMmapFile);
    }
    sem_close(semaphore);
    sem_unlink(SEM_NAME);
}
