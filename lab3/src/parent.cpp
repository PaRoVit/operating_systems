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

    sem_unlink(SEM_PARENT_NAME); // Родитель сигнализирует джочеренему
    sem_unlink(SEM_CHILD_NAME); // Родитель ждёт завершения обработки
    sem_t* semParent = CreateSemaphore(SEM_PARENT_NAME, 0);
    sem_t* semChild = CreateSemaphore(SEM_CHILD_NAME, 0);
    ErrorChecking(semParent == SEM_FAILED || semChild == SEM_FAILED ? -1 : 0, "Semaphore open error");

    pid_t firstProcessID = CreateFork();
    ErrorChecking(firstProcessID, "Fork1 error");
    if (firstProcessID == 0) {
        pid_t secondProcessID = CreateFork();
        ErrorChecking(secondProcessID, "Fork2 error");

        if (secondProcessID == 0) {
            dup2(file2Descr, STDOUT_FILENO);
            execl(pathToChild2, pathToChild2, SEM_PARENT_NAME, SEM_CHILD_NAME, MMAP_NAME2, nullptr);
            perror("execl failed for child2");
            exit(EXIT_FAILURE);
        } else {
            dup2(file1Descr, STDOUT_FILENO);
            execl(pathToChild1, pathToChild1, SEM_PARENT_NAME, SEM_CHILD_NAME, MMAP_NAME1, nullptr);
            perror("execl failed for child1");
            exit(EXIT_FAILURE);
        }
    } else {
        char* firstMmap = MapSharedMemory(getpagesize(), firstMmapFile);
        char* secondMmap = MapSharedMemory(getpagesize(), secondMmapFile);
        ErrorChecking(firstMmap == MAP_FAILED || secondMmap == MAP_FAILED ? -1 : 0, "Error creating memory maps");

        int counterStrings = 1;
        std::string inputString;
        while (std::getline(streamIn, inputString)) {
            char* mmap = (counterStrings % 2 != 0) ? firstMmap : secondMmap; // Выбор памяти в которую писать 
            int mmapFile = (counterStrings % 2 != 0) ? firstMmapFile : secondMmapFile;
            int string_length = inputString.length();
            ftruncate(mmapFile, string_length + 1); // Запись строки в шареную память 
            memcpy(mmap, inputString.c_str(), string_length);
            mmap[string_length] = '\n';
            sem_post(semParent); // Отправка сигнала дочеренему
            sem_wait(semChild); // ждём результатов от дочеренего 

            counterStrings++;
        }

        ftruncate(firstMmapFile, 1); // при завершении отправляем пустые строки 
        firstMmap[0] = '\n';
        sem_post(semParent);

        ftruncate(secondMmapFile, 1);
        secondMmap[0] = '\n';
        sem_post(semParent);

        int status;
        waitpid(-1, &status, 0);
        waitpid(-1, &status, 0);

        munmap(firstMmap, getpagesize());
        munmap(secondMmap, getpagesize());
        shm_unlink(MMAP_NAME1); 
        shm_unlink(MMAP_NAME2); 

        close(file1Descr);
        close(file2Descr);
        close(firstMmapFile);
        close(secondMmapFile);
    }
    sem_close(semParent);
    sem_close(semChild);
    sem_unlink(SEM_PARENT_NAME);
    sem_unlink(SEM_CHILD_NAME);
}
