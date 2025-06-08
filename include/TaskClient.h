#pragma once
#include <nlohmann/json.hpp>
#include <string>

class TaskClient {
public:
    explicit TaskClient(const std::string& host = "127.0.0.1", const int port = 5050):
        serverHost(host), serverPort(port) {};
    nlohmann::json sendRequest(const nlohmann::json& request);

private:
    std::string serverHost;
   int serverPort = 5050;
};
