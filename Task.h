#pragma once

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Task {
    int task_id;
    int priority;
    int execution_time;
    std::string description;
    bool is_completed;
};

inline void to_json(json &j, const Task &task) {
    j = json{
        {"task_id", task.task_id},
        {"priority", task.priority},
        {"execution_time", task.execution_time},
        {"description", task.description},
        {"is_completed", task.is_completed}
    };
}
inline void from_json(const json &j, Task &task) {
    j.at("task_id").get_to(task.task_id);
    j.at("priority").get_to(task.priority);
    j.at("execution_time").get_to(task.execution_time);
    j.at("description").get_to(task.description);
    j.at("is_completed").get_to(task.is_completed);
}