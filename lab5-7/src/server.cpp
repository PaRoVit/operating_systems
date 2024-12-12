#include "../include/tree.hpp"
#include "../include/zmq_manage.hpp"
#include <fstream>
#include <signal.h>
#include <map>

int main(int argc, char **argv) {
    if (argc != 3) {
        perror("Not enough arguments");
        exit(EXIT_FAILURE);
    }

    Node task(atoi(argv[1]), atoi(argv[2]));
    std::string programPath = getenv("PROGRAM_PATH");

    // Хранилище ключ-значение для текущего узла
    std::map<std::string, int> keyValueStore;

    while (1) {
        std::string message;
        std::string command = " ";
        if (auto msg = ReceiveMessage(&(task.parent)); msg.has_value()) {
            message = *msg;
        }
        std::istringstream request(message);
        request >> command;

        if (command == "create") {
            int idChild;
            request >> idChild;
            std::string ans = task.Create(idChild, programPath);
            SendMessage(&task.parent, ans);
        } else if (command == "pid") {
            std::string ans = task.Pid();
            SendMessage(&task.parent, ans);
        } else if (command == "ping") {
            int idChild;
            request >> idChild;
            std::string ans = task.Ping(idChild);
            SendMessage(&task.parent, ans);
        } else if (command == "send") {
            int id;
            request >> id;
            std::string str;
            getline(request, str);
            str.erase(0, 1);
            std::string ans;
            ans = task.Send(str, id);
            SendMessage(&task.parent, ans);
        } else if (command == "exec") {
            int targetId;
            request >> targetId;

            // Считать оставшуюся часть строки
            std::string name;
            request >> name;
            std::cout << name << " server" <<std::endl;

            if (request.peek() == ' ') { // Save value
                int value;
                request >> value;
                keyValueStore[name] = value;
                std::string to_send = "Ok:" + std::to_string(targetId);
                SendMessage(&task.parent, to_send);
            } else { // load val
                auto it = keyValueStore.find(name);
                if (keyValueStore.count(name) > 0) {
                    std::string to_send = "Ok:" + std::to_string(targetId) + ": " + std::to_string(it->second);
                    SendMessage(&task.parent, to_send);
                } else {
                    std::string to_send = "Ok:" + std::to_string(targetId) + " not found";
                    SendMessage(&task.parent, to_send);
                }
                
            }
        } else if (command == "kill") {
            std::string ans = task.Kill();
            ans = std::to_string(task.id) + " " + ans;
            SendMessage(&task.parent, ans);
            Disconnect(&task.parent, task.parentPort);
            task.parent.close();
            break;
        }
    }

    return 0;
}
