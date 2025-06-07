#pragma once
#include "ITaskStorage.h"

class JsonTaskStorage : public ITaskStorage {
private:
    std::string filePath;

public:
    explicit JsonTaskStorage(const std::string& path = "tasks.json");

    std::vector<Task> load() override;
    void save(const std::vector<Task>& tasks) override;
};
