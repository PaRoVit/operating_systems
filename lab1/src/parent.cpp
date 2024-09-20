#include "parent.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

void process_input_lines(const std::string &filename1, const std::string &filename2) {
    char input[1024];
    int line_num = 1;

    // Чтение строк с консоли до конца ввода (Ctrl+D)
    while (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;  // Убираем символ новой строки

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Дочерний процесс
            if (line_num % 2 == 1) {
                execlp("./child", "./child", filename1.c_str(), input, NULL);
            } else {
                execlp("./child", "./child", filename2.c_str(), input, NULL);
            }
            perror("execlp");
            exit(1);
        } else {
            // Родительский процесс: дожидаемся завершения дочернего процесса
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
            }
            if (WIFEXITED(status)) {
                std::cout << "Child process " << pid << " exited with status " << WEXITSTATUS(status) << "\n";
            }
        }
        line_num++;

    }
}
