#include "JsonTaskStorage.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

JsonTaskStorage::JsonTaskStorage(const std::string& path) : filePath(path) {}

std::vector<Task> JsonTaskStorage::load() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return {};
    }

    try {
        nlohmann::json j;
        file >> j;
        return j.get<std::vector<Task>>();
    } catch (const nlohmann::json::parse_error& e) {
        return {};
    } catch (const std::exception& e) {
        return {};
    }
}
void JsonTaskStorage::save(const std::vector<Task>& tasks) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        json j = tasks;
        file << j.dump(4);
    }
}
