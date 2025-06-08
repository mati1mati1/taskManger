
#include "TaskClient.h"
#include <iostream>
#include <iomanip>
#include <cxxopts.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>         
#include <string>
using json = nlohmann::json;
#define COLOR_RESET     "\033[0m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_RED       "\033[31m"
#define COLOR_DIM       "\033[2m"
#define COLOR_BOLD      "\033[1m"

void printHelp() {
    std::cout << COLOR_BOLD << "Task CLI Help" << COLOR_RESET << "\n"
              << "  taskctl list\n"
              << "  taskctl add \"description\" [priority] [execution_time]\n"
              << "  taskctl done <task_id>\n"
              << "  taskctl remove <task_id>\n";
}
std::string getConfigPath() {
    const char* xdg = std::getenv("XDG_CONFIG_HOME");
    if (xdg) return std::string(xdg) + "/taskctl/config.json";

    const char* home = std::getenv("HOME");
    if (home) return std::string(home) + "/.taskctlrc";

    return "./.taskctlrc"; 
}
nlohmann::json loadConfig() {
    std::ifstream in(getConfigPath());
    if (!in.is_open()) return nlohmann::json::object();
    try {
        return nlohmann::json::parse(in);
    } catch (...) {
        return {};
    }
}


int main(int argc, char* argv[]) {
    cxxopts::Options options("taskctl", "Task manager CLI");

    options.add_options()
        ("c,command", "Command to run", cxxopts::value<std::string>())
        ("d,description", "Task description", cxxopts::value<std::string>())
        ("p,priority", "Priority", cxxopts::value<int>()->default_value("0"))
        ("t,execution", "Execution time", cxxopts::value<int>()->default_value("0"))
        ("h,help", "Print help");

    auto result = options.parse(argc, argv);
    if (result.count("help") || !result.count("command")) {
        printHelp();
        return 0;
    }
    auto config = loadConfig();

    std::string host = config.value("host", "127.0.0.1");
    int port = config.value("port", 5050);
    int default_priority = config.value("default_priority", 0);

    std::string cmd = result["command"].as<std::string>();
    nlohmann::json req;
    TaskClient client(host, port);

    if (cmd == "list") {
        req["command"] = "list";
    } else if (cmd == "add") {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: taskctl add \"desc\" [priority] [execution_time]" << COLOR_RESET << "\n";
            return 1;
        }
        if (!result.count("description")) {
            std::cerr << COLOR_RED << "Missing required --description for add" << COLOR_RESET << "\n";
            return 1;
        }
        req["command"] = "add";
        req["description"] = result["description"].as<std::string>();
        req["execution_time"] = result["execution"].as<int>();
        if (result.count("priority"))
            req["priority"] = result["priority"].as<int>();
        else
            req["priority"] = default_priority;        
    } else if (cmd == "done" || cmd == "remove") {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: taskctl " << cmd << " <task_id>" << COLOR_RESET << "\n";
            return 1;
        }
        req["command"] = cmd;
        req["task_id"] = std::stoi(argv[2]);
    } else if (cmd == "help") {
        printHelp();
        return 0;
    } else {
        std::cerr << COLOR_RED << "Unknown command: " << cmd << COLOR_RESET << "\n";
        printHelp();
        return 1;
    }

    nlohmann::json response = client.sendRequest(req);

    if (response["status"] == "ok" && response.contains("tasks")) {
        auto tasks = response["tasks"];

        std::cout << COLOR_BOLD << "ID  Pri Time Done Description" << COLOR_RESET << "\n";

        for (const auto& task : tasks) {
            std::string done = task["is_completed"] ? "✔" : " ";
            std::string desc = task["description"];
            if (task["is_completed"]) {
                desc = COLOR_DIM + desc + COLOR_RESET;
            }

            std::cout << std::setw(2) << task["task_id"].get<int>() << "  "
                      << std::setw(3) << task["priority"].get<int>() << "  "
                      << std::setw(4) << task["execution_time"].get<int>() << "   "
                      << done << "   "
                      << desc << "\n";
        }
    } else if (response["status"] == "ok") {
        std::cout << COLOR_GREEN << response["message"] << COLOR_RESET << "\n";
    } else {
        std::cerr << COLOR_RED << "[ERROR] " << response["message"] << COLOR_RESET << "\n";
    }

    return 0;
}
