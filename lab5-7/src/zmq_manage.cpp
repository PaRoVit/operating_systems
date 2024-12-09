#include "../include/zmq_manage.hpp"


std::string get_port_name(const int port) {
    return "tcp://127.0.0.1:" + std::to_string(port + 1);
}

bool send_message(zmq::socket_t &socket, const std::string &message_string) {
    zmq::message_t message(message_string.size());
    memcpy(message.data(), message_string.c_str(), message_string.size());
    auto result = socket.send(message, zmq::send_flags::none);
    return result.has_value(); // Проверяем, был ли отправлен пакет
}

std::string recieve_message(zmq::socket_t &socket) {
    zmq::message_t message;
    bool ok = false;
    try {
        auto result = socket.recv(message, zmq::recv_flags::none); // Указан флаг приема
        ok = result.has_value(); // Проверяем успешность получения
    } catch (...) {
        ok = false;
    }
    std::string recieved_message(static_cast<char*>(message.data()), message.size());
    if (recieved_message.empty() || !ok) {
        return "Error: Node is not available";
    }
    return recieved_message;
}

void create_node(int id, int port) {
    char* arg0 = strdup("./server");
    char* arg1 = strdup((std::to_string(id)).c_str());
    char* arg2 = strdup((std::to_string(port)).c_str());
    char* args[] = {arg0, arg1, arg2, NULL};
    execv("./server", args);

    // Освобождение памяти на случай ошибки
    free(arg0);
    free(arg1);
    free(arg2);
}
