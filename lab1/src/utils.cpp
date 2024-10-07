#include <utils.hpp>

// создание дочернего процесса
pid_t CreateChild(){
    if(pid_t pid = fork(); pid >= 0) {
        return pid;
    }
    std::perror("Couldn't create child.");
    exit(EXIT_FAILURE);   
}

// создание пайпа
void CreatePipe(int pipeFd[2]){
    if( pipe(pipeFd) < 0 ) {
        std::perror("Couldn't create pipe.");
        exit(EXIT_FAILURE);
    }
}

// запуск программы по её пути
void Exec(const char * pathToChild){
    if (execl(pathToChild, pathToChild, nullptr) == -1) {
        perror("Failed to exec.");
        exit(EXIT_FAILURE);
    }
}

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

ssize_t readStringFromPipe(int pipeFd, std::string &input_string) {
    int input_size;
    ssize_t bytes_read = read(pipeFd, &input_size, sizeof(int)); // Чтение размера строки
    if (bytes_read == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    input_string.resize(input_size, '\0'); // Резервируем строку нужного размера
    bytes_read = read(pipeFd, &input_string[0], input_size); // Чтение самой строки
    if (bytes_read == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    return bytes_read;
}

// Запись строки в pipe
ssize_t writeStringToPipe(int pipeFd, const std::string &output_string) {
    ssize_t bytes_written = write(pipeFd, output_string.c_str(), output_string.size());
    if (bytes_written == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    // Добавление новой строки
    bytes_written = write(pipeFd, "\n", 1);
    if (bytes_written == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    return bytes_written;
}
