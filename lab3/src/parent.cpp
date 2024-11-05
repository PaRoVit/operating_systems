#include <parent.hpp>

// export CHILD1_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab3/child1"
// export CHILD2_PATH="/home/pvrozhkov/operating_system/operating_systems/build/lab3/child2"

void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn){

    
    // передадим имена файлов для записи(они сохраняются в build/lab1/outX.txt)
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

        if (secondProcessID == 0){
            dup2(file2Descr, STDOUT_FILENO);
            execl(pathToChild2, SEM_NAME, MMAP_NAME2, nullptr);
        }else {
            dup2(file1Descr, STDOUT_FILENO);
            execl(pathToChild1, SEM_NAME, MMAP_NAME1, nullptr);
        }
    }else { // parent
        if (GetSemaphoreValue(semaphore) == 2) {
            char* firstMmap = MapSharedMemory(getpagesize(), firstMmapFile);
            char* secondMmap = MapSharedMemory(getpagesize(), secondMmapFile);

            if (firstMmap == MAP_FAILED || secondMmap == MAP_FAILED) {
                std::cerr << "Error creating memory maps\n"; 
                return ; 
            }

            int firstPosition = 0; // Индекс для первого процесса
            int secondPosition = 0; // Индекс для второго процесса
            int firstLength = 0; // Длина первой области памяти
            int secondLength = 0; // Длина второй области памяти
            int counterStrings = 1; // Счетчик строк

            std::string inputString;
            while (std::getline(streamIn, inputString)){
                int string_length = inputString.length();

                if (counterStrings % 2 != 0) {
                    firstLength += string_length + 1; // Увеличение длины первой области
                    ftruncate(firstMmapFile, firstLength); // Установка новой длины первой области

                    // Запись символов строки в первую область
                    for (char ch : inputString) {
                        firstMmap[firstPosition++] = ch; // Запись символа
                    }
                    firstMmap[firstPosition++] = '\n'; // Добавление символа новой строки
                } else { // Если номер строки четный
                    secondLength += string_length + 1; // Увеличение длины второй области
                    ftruncate(secondMmapFile, secondLength); // Установка новой длины второй области

                    // Запись символов строки во вторую область
                    for (char ch : inputString) {
                        secondMmap[secondPosition++] = ch; // Запись символа
                    }
                    secondMmap[secondPosition++] = '\n'; // Добавление символа новой строки
                }
                counterStrings++;
            }
            SetSemaphoreValue(semaphore, 1);

            struct stat firstBuffer, secondBuffer;
            fstat(firstMmapFile, &firstBuffer);
            fstat(secondMmapFile, &secondBuffer);

            int firstMmapSize = firstBuffer.st_size; // Получение размера первой области
            int secondMmapSize = secondBuffer.st_size; // Получение размера второй области
            
            int status;
            waitpid(-1, &status, 0);
            waitpid(-1, &status, 0);

            munmap(firstMmap, firstMmapSize); // Освобождение первой области памяти
            munmap(secondMmap, secondMmapSize); // Освобождение второй области памяти
        }
        close(file1Descr); // Закрытие первого файла
        close(file2Descr); // Закрытие второго файла
        close(firstMmapFile); // Закрытие первого дескриптора области памяти
        close(secondMmapFile); // Закрытие второго дескриптора области памяти
    }
    sem_close(semaphore); // Закрытие семафора
    sem_destroy(semaphore); // Удаление семафора


    return;

}