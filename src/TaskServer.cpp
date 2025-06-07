#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>      
#include "TaskServer.h"  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <nlohmann/json.hpp>
#include "JsonTaskStorage.h"
#include "TaskManager.h"
#define SERVER_PORT 5050    
#define BUFFER_SIZE 4096

void TaskServer::run() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");  
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind TCP socket: " << strerror(errno) << std::endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Failed to listen on TCP socket: " << strerror(errno) << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "TaskServer running on TCP port " << SERVER_PORT << "..." << std::endl;

    JsonTaskStorage storage("tasks.json");
    TaskManager manager(storage);

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);

        if (bytes_read > 0) {
            std::string command(buffer, bytes_read);
            std::cout << "Received: " << command << std::endl;

            nlohmann::json response;
            nlohmann::json request;
            std::cout << "Received request: " << command << std::endl;
            try {
                request = nlohmann::json::parse(command);
            } catch (...) {
                nlohmann::json error = {
                    {"status", "error"},
                    {"message", "Invalid JSON"}
                };
                std::string err_str = error.dump();
                write(client_fd, err_str.c_str(), err_str.size());
                close(client_fd);
                continue;
            }

            std::string cmd = request.value("command", "");

            if (cmd == "list") {
                response["status"] = "ok";
                response["tasks"] = manager.getAllTasks();
            } else if (cmd == "add") {
                std::string desc = request.value("description", "");
                int prio = request.value("priority", 0);
                int exec = request.value("execution_time", 0);
                int id = manager.getAllTasks().size() + 1;

                Task new_task{id, prio, exec, desc, false};
                manager.addTask(new_task);
                response = {
                    {"status", "ok"},
                    {"message", "Task added"},
                    {"task_id", id}
                };
            } else if (cmd == "done") {
                int id = request.value("task_id", -1);
                manager.markTaskAsCompleted(id);
                response = {
                    {"status", "ok"},
                    {"message", "Task marked complete"},
                    {"task_id", id}
                };
            } else if (cmd == "remove") {
                int id = request.value("task_id", -1);
                manager.removeTask(id);
                response = {
                    {"status", "ok"},
                    {"message", "Task removed"},
                    {"task_id", id}
                };
            } else {
                response = {
                    {"status", "error"},
                    {"message", "Unknown command"}
                };
            }


            std::string json_response = response.dump() + "\n";
            ssize_t sent = write(client_fd, json_response.c_str(), json_response.size());
            std::cout << "Sent " << sent << " bytes: " << json_response << std::endl;

        }
        close(client_fd);
    }

    close(server_fd);
}