#include "Task.h"
#include "ITaskStorage.h"
#include "JsonTaskStorage.h"
#include "TaskManager.h"
#include <iostream>
#include <vector>
#include <string>


int main() {

    JsonTaskStorage storage("tasks.json");

    TaskManager taskManager(storage);

    Task newTask = {1, 5, 120, "Complete the project", false};
    taskManager.addTask(newTask);

    auto tasks = taskManager.getAllTasks();
    for (const auto &task : tasks) {
        std::cout << "Task ID: " << task.task_id << ", Description: " << task.description << std::endl;
    }

    return 0;
}