#include <../include/parent.hpp>

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

void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn){
    // pipe для передачи от родителя к 1 дочернему
    int child1[2];
    CreatePipe(child1);

    // pipe для передачи от родителя к 2 дочернему
    int child2[2];
    CreatePipe(child2);

    // передадим имена файлов для записи(они сохраняются в build/lab1/outX.txt)
    std::string file1 = "out1.txt";
    std::string file2 = "out2.txt";
    int file1Descr = open(file1.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int file2Descr = open(file2.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);


    // создаём дочерний процесс 1
    pid_t pid1 = CreateChild();
    if (pid1 == 0){
        close(child1[WRITE_END]);
        close(child2[READ_END]);
        close(child2[WRITE_END]);

        // перенаправляем потоки
        if (dup2(child1[READ_END], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        if (dup2(file1Descr, STDOUT_FILENO) == -1){
            perror("dup2 error file1");
            exit(EXIT_FAILURE);
        }
        close(file1Descr);
        // выполнение первого дочернего процесса
        Exec(pathToChild1);
    } else {
        // создаём 2 дочерний процесс и всё тоде самое
        pid_t pid2 = CreateChild();
        if (pid2 == 0){
            close(child2[WRITE_END]);
            close(child1[READ_END]);
            close(child1[WRITE_END]);

            if (dup2(child2[READ_END], STDIN_FILENO) == -1){
                perror("dup2 error");
                exit(EXIT_FAILURE);
            }
            if (dup2(file2Descr, STDOUT_FILENO) == -1){
                perror("dup2 error file2");
                exit(EXIT_FAILURE);
            }
            close(file2Descr);
            Exec(pathToChild2);
        } else {
            // дальше отправим каждую строку в нужный для неё дочерний процесс
            close(child1[READ_END]);
            close(child2[READ_END]);

            std::string string_from_file; 
            int string_sequence_number = 0;
            // читаем строки на вход
            while (std::getline(streamIn, string_from_file))
            {
                int length = string_from_file.size(); //размеры входной строки, которые потом отправим в доч.процесс
                int pipe_write_end = (string_sequence_number % 2 == 0) ? child1[WRITE_END] : child2[WRITE_END]; // если true то присваиваем child1
                // закидываем в поток сначала размеры строки, потом саму строку
                write(pipe_write_end, &length, sizeof(length));
                write(pipe_write_end, string_from_file.c_str(), length);
                string_sequence_number++;
            }

            // закрываем остатки
            close(child1[WRITE_END]);
            close(child2[WRITE_END]);
            close(file1Descr);
            close(file2Descr);

            // ожидаем закрытие дочерних процессов
            waitpid(pid1, nullptr, 0);
            waitpid(pid2, nullptr, 0);
        }
    }

}