#include "TaskClient.h"
#include <sys/socket.h>
#include <netinet/in.h>     // for sockaddr_in
#include <arpa/inet.h>      // for inet_addr
#include <unistd.h>
#include <iostream>

#define BUFFER_SIZE 4096

nlohmann::json TaskClient::sendRequest(const nlohmann::json& request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock < 0) {
        perror("socket");
        return {{"status", "error"}, {"message", "socket creation failed"}};
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(serverPort));  
    addr.sin_addr.s_addr = inet_addr(serverHost.c_str());  

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return {{"status", "error"}, {"message", "connection failed"}};
    }

    std::string payload = request.dump();
    write(sock, payload.c_str(), payload.size());

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes = read(sock, buffer, BUFFER_SIZE);

    close(sock);

    if (bytes <= 0) {
        return {{"status", "error"}, {"message","no response"}};
    }

    try {
        return nlohmann::json::parse(std::string(buffer, bytes));
    } catch (...) {
        return {{"status", "error"}, {"message", "invalid JSON response"}};
    }
}
