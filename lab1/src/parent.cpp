#include <parent.hpp>

// функция для чтения строки по символьно
std::string pull_string_from_file() {
    std::string input_string;
    char input_symbol;

    // читаем символы из ввода до тех пор, пока не встретим символ новой строки
    while ((input_symbol = getchar()) != '\n') {
        if (input_symbol == EOF) {
            // если достигнут конец файла, возвращаем пустую строку
            return "";
        }
        input_string += input_symbol; // добавляем символ к строке
    }

    return input_string; // возвращаем полученную строку
}

void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn, std::ostream & streamOut ){
    // pip для передачи от родителя к 1 дочернему
    int child1[2];
    CreatePipe(child1);

    // pip для передачи от родителя к 2 дочернему
    int child2[2];
    CreatePipe(child2);

    // передадим имена файлов для записи
    std::string file1 = pull_string_from_file();
    std::string file2 = pull_string_from_file();
    int file1Descr = open(file1.c_str(), std::ios::out | std::ios::trunc, 0777);
    int file2Descr = open(file2.c_str(), std::ios::out | std::ios::trunc, 0777);


    // создаём дочерний процесс 1
    pid_t pid1 = CreateChild(), pid2 = -1;
    if (pid1 == 0){
        close(child1[WRITE_END]);
        close(child2[READ_END]);
        close(child2[WRITE_END]);

        if (dup2(child1[READ_END], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        if (dup2(file1Descr, STDOUT_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        
    }


}