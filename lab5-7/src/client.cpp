#include "../include/zmq_manage.hpp"
#include <unistd.h> // Для fork и execl
#include <iostream>
#include <string>

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
    // Запуск дочернего процесса для api_daemon
    pid_t api_daemon_pid = fork();
    if (api_daemon_pid == -1) {
        std::cerr << "Error: Failed to fork api_daemon\n";
        return EXIT_FAILURE;
    } else if (api_daemon_pid == 0) {
        // Исполнение дочернего процесса
        if (execl("./api_daemon", "./api_daemon", nullptr) == -1) {
            std::cerr << "Error: Failed to execute api_daemon\n";
            return EXIT_FAILURE; // Завершаем дочерний процесс в случае ошибки
        }
    }

    // Вывод списка команд
    std::cout 
        << "Commands:\n"
        << "\tcreate id parent\n"
        << "\tremove id\n"
        << "\texec id var_name value\n"
        << "\texec id var_name\n"
        << "\tping id\n"
        << "\texit\n";

    zmq::context_t context(1);
    zmq::socket_t api_socket(context, zmq::socket_type::pair);

    // Привязка API сокета
    try {
        api_socket.bind(get_port_name(API_PORT));
    } catch (const zmq::error_t &e) {
        std::cerr << "Error: Failed to bind API socket - " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::string str;
    bool flag = true;

    // Главный цикл для обработки команд
    while (flag) {
        std::getline(std::cin, str); // Считывание команды пользователя
        try {
            send_message(api_socket, str); // Отправка команды
        } catch (const zmq::error_t &e) {
            std::cerr << "Error: Failed to send message - " << e.what() << '\n';
            continue;
        }

        if (str == "exit") {
            flag = false; // Завершаем программу
        }
    }

    return EXIT_SUCCESS;
}
