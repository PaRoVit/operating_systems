#include <utils.hpp>

sem_t* CreateSemaphore(const char *name, int value) {
    sem_t *semptr = sem_open(name, O_CREAT, 0777, value);
    if (semptr == SEM_FAILED){
        perror("Couldn't open the semaphore");
        exit(EXIT_FAILURE);
    }
    return semptr;   
}

int CreateShm(const char* name) {
    int fd = shm_open(name, O_CREAT | O_RDWR, 0777);
    if (fd == -1) {
        std::cerr << "Failed shm_open\n";
        exit(-1);
    }
    ftruncate(fd, 1024);
    return fd;
}

char* MapSharedMemory(const int size, int fd) {
    char *memptr = (char*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memptr == MAP_FAILED) {
        perror("Error with file mapping");
        exit(EXIT_FAILURE);
    }
    return memptr;
}

int CreateFork() {
    int pid;
    pid = fork();
    if (pid == -1) {
        std::cerr << "Failed fork()\n";
        exit(-2);
    }
    return pid;
}

// удаление гласных
std::string removeVowels(const std::string& input) {
    std::string result = input;
    // обход по строке при помощи лямбда функции, гласные смещаются назад при помощи remove_if, а erase их удаляет
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || 
               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' ||
               c == 'y' || c == 'Y';
    }), result.end());
    return result;
}

void ErrorChecking(int result, const char* error) {
    if (result == -1) { // Если результат ошибки
        std::cerr << error << ": " << strerror(errno) << std::endl; // Вывод сообщения об ошибке
        exit(EXIT_FAILURE); // Завершение программы с ошибкой
    }
}

int GetSemaphoreValue(sem_t* semaphore) {
    int value; // Переменная для хранения значения семафора
    sem_getvalue(semaphore, &value); // Получение текущего значения семафора
    return value; // Возвращение значения
}

void SetSemaphoreValue(sem_t* semaphore, int value) {
    // Увеличение значения семафора до указанного
    while (GetSemaphoreValue(semaphore) < value) {
        sem_post(semaphore); // Увеличение семафора
    }
    // Уменьшение значения семафора до указанного
    while (GetSemaphoreValue(semaphore) > value) {
        sem_wait(semaphore); // Уменьшение семафора
    }
}

void ProcessChild(const char *semaphoreParentName, const char *semaphoreChildName, const char* mmapFilename){
    sem_t* semParent = sem_open(semaphoreParentName, O_RDWR);
    sem_t* semChild = sem_open(semaphoreChildName, O_RDWR);
    ErrorChecking(semParent == SEM_FAILED || semChild == SEM_FAILED ? -1 : 0, "Semaphore open error");

    int mmapFile = shm_open(mmapFilename, O_RDWR, 0777);
    ErrorChecking(mmapFile, "File open error");

    char* mmap = MapSharedMemory(getpagesize(), mmapFile);
    ErrorChecking(mmap == MAP_FAILED ? -1 : 0, "Error mapping memory");

    while(true) {
        sem_wait(semParent); // Ждём сигнала от родительского
        std::string inputString;
        for (int i = 0; mmap[i] != '\n'; ++i) { // чтение строки из памяти
            inputString += mmap[i];
        }
        if (inputString.empty()) break; // Условие завершения
        std::string processedString = removeVowels(inputString);
        std::cout << processedString << std::endl;
        sem_post(semChild);
    }

    sem_close(semParent); // Закрытие семафоров 
    sem_close(semChild);
    munmap(mmap, getpagesize()); // Освобождение области памяти
    close(mmapFile); // Закрытие дескриптора области памяти
}
