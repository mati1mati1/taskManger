#pragma once
#include <nlohmann/json.hpp>
#include <string>

class TaskClient {
public:
    explicit TaskClient(const std::string& host = "127.0.0.1");
    nlohmann::json sendRequest(const nlohmann::json& request);

private:
    std::string serverHost;
};
