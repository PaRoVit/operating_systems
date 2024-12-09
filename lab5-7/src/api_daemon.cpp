#include "../include/zmq_manage.hpp"
#include "../include/tree.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

Tree T = Tree();
zmq::context_t context(3);
zmq::socket_t main_socket(context, zmq::socket_type::req);

bool flag = true;

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
    try {
        main_socket.bind(get_port_name(BASE_PORT + manage_worker_id + 1));
    } catch (const zmq::error_t &e) {
        std::cerr << "Error binding main socket: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    pid_t manage_pid = fork();
    if (manage_pid == -1) {
        std::cerr << "Error creating manage worker" << std::endl;
        return EXIT_FAILURE;
    } else if (manage_pid == 0) {
        create_node(manage_worker_id, BASE_PORT + manage_worker_id + 1);
    }

    std::string command;
    std::string result = "";
    std::string msg_s = "";

    zmq::socket_t api_socket(context, zmq::socket_type::pair);
    try {
        api_socket.connect(get_port_name(API_PORT));
    } catch (const zmq::error_t &e) {
        std::cerr << "Error connecting API socket: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    while (flag) {
        std::cout << result << '\n';
        result.clear();

        std::vector<std::string> args;
        std::string str;

        try {
            str = recieve_message(api_socket);
        } catch (const std::exception &e) {
            std::cerr << "Error receiving message: " << e.what() << std::endl;
            continue;
        }

        std::istringstream ist(str);
        for (std::string word; ist >> word; args.push_back(word));

        if (args.empty()) {
            continue;
        }

        if (args[0] == "print" && args.size() == 1) {
            T.print_tree();
        } else if (args[0] == "ping" && args.size() == 2) {
            int id = std::stoi(args[1]);
            if (!T.node_exists(id)) {
                result = "Error: Not found";
            } else {
                try {
                    msg_s = "ping " + std::to_string(id);
                    send_message(main_socket, msg_s);
                    result = recieve_message(main_socket);
                    if (result == "Ok: 1"){
                        continue;
                    } else {
                        std::cout << "Ok: 0\n";
                        continue;
                    }
                } catch (const std::exception &e){
                    std::cout << "Ok: 0\n";
                    continue;
                }
            }
        } else if (args[0] == "create" && args.size() == 3) {
            int id = std::stoi(args[1]), parent = std::stoi(args[2]);
            if (id < 0) {
                std::cout << "Error: ID must be greater or equal to 0\n";
                continue;
            }
            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
            if (result != "Ok: 0") {
                std::cout << "Error: Node already exists\n";
                continue;
            }
            msg_s = "ping " + std::to_string(parent);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
            if (result != "Ok: 1") {
                std::cout << "Error: Parent node not found\n";
                continue;
            }
            msg_s = "create " + std::to_string(id) + " " + std::to_string(parent);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
            if (result.substr(0, 2) == "Ok") {
                T.create_node(id, parent);
            }
        } else if (args[0] == "remove" && args.size() == 2) {
            int id = std::stoi(args[1]);
            msg_s = "ping " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
            if (result != "Ok: 1") {
                std::cout << "Error: Node not found\n";
                continue;
            }
            msg_s = "remove " + std::to_string(id);
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
            if (result.substr(0, 2) == "Ok") {
                T.delete_node(id);
            }
        } else if (args[0] == "exec" && args.size() >= 3) {
            int id = std::stoi(args[1]);
            std::string var_name = args[2];
            msg_s = (args.size() == 3) ? "get_value " + std::to_string(id) + " " + var_name 
                                       : "set_value " + std::to_string(id) + " " + var_name + " " + args[3];
            send_message(main_socket, msg_s);
            result = recieve_message(main_socket);
        } else if (args[0] == "exit") {
            msg_s = "kill";
            send_message(main_socket, msg_s);
            std::cout << "Ok\n";
            flag = false;
        }
    }
    return EXIT_SUCCESS;
}
