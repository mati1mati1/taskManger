#include "JsonTaskStorage.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

JsonTaskStorage::JsonTaskStorage(const std::string& path) : filePath(path) {}

std::vector<Task> JsonTaskStorage::load() {
    std::ifstream file(filePath);
    if (!file.is_open()) return {};

    json j;
    file >> j;
    return j.get<std::vector<Task>>();
}

void JsonTaskStorage::save(const std::vector<Task>& tasks) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        json j = tasks;
        file << j.dump(4);
    }
}
