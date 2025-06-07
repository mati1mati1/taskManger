
#include "TaskClient.h"
#include <iostream>
#include <iomanip>

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string cmd = argv[1];
    nlohmann::json req;
    TaskClient client;

    if (cmd == "list") {
        req["command"] = "list";
    } else if (cmd == "add") {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: taskctl add \"desc\" [priority] [execution_time]" << COLOR_RESET << "\n";
            return 1;
        }
        req["command"] = "add";
        req["description"] = argv[2];
        req["priority"] = (argc > 3) ? std::stoi(argv[3]) : 0;
        req["execution_time"] = (argc > 4) ? std::stoi(argv[4]) : 0;
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
