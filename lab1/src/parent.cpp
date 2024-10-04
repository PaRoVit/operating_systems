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

void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn){
    // pip для передачи от родителя к 1 дочернему
    int child1[2];
    CreatePipe(child1);

    // pip для передачи от родителя к 2 дочернему
    int child2[2];
    CreatePipe(child2);

    // передадим имена файлов для записи
    std::string file1 = pull_string_from_file();
    std::string file2 = pull_string_from_file();
    int file1Descr = open(file1.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int file2Descr = open(file2.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);


    // создаём дочерний процесс 1
    pid_t pid1 = CreateChild();
    if (pid1 == 0){
        close(child1[WRITE_END]);
        close(child2[READ_END]);
        close(child2[WRITE_END]);

        if (dup2(child1[READ_END], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        if (dup2(file1Descr, STDOUT_FILENO) == -1){
            perror("dup2 error file1");
            exit(EXIT_FAILURE);
        }
        close(file1Descr);
        Exec(pathToChild1);
    } else {
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
            close(child1[READ_END]);
            close(child2[READ_END]);

            std::string string_from_file; 
            int string_sequence_number = 0;
            while (std::getline(streamIn, string_from_file))
            {
                int length = string_from_file.size();
                if (!(string_sequence_number % 2 != 0)) 
                {
                    std::cerr << string_from_file<< " передана строка и её размер " << length << "\n";
                    write(child1[WRITE_END], &length, sizeof(int));
                    write(child1[WRITE_END], string_from_file.c_str(), string_from_file.size());
                    
                }
                else
                {
                    write(child2[WRITE_END], &length, sizeof(int));
                    write(child2[WRITE_END], string_from_file.c_str(), string_from_file.size());
                }
                string_sequence_number++;
            }
            // тут прописываю фигню для расхода в разные процессы


            close(child1[WRITE_END]);
            close(child2[WRITE_END]);
            close(file1Descr);
            close(file2Descr);
        }
    }

}