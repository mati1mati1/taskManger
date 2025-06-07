// ITaskStorage.h
#pragma once
#include <vector>
#include <string>
#include "Task.h"

class ITaskStorage {
public:
    virtual ~ITaskStorage() = default;

    virtual std::vector<Task> load() = 0;

    virtual void save(const std::vector<Task>& tasks) = 0;
};
