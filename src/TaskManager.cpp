#include "TaskManager.h"

void TaskManager::removeTask(int taskId){
    auto it = std::remove_if(taskRepo.begin(), taskRepo.end(),
                             [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        taskRepo.erase(it, taskRepo.end());
        taskStorage.save(taskRepo);
    }
}
void TaskManager::updateTask(int taskId, const Task &task){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &t) { return t.task_id == taskId; });
    if (it != taskRepo.end()) {
        *it = task;
        taskStorage.save(taskRepo);
    }
}
Task TaskManager::getTask(int taskId){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        return *it;
    }
    throw std::runtime_error("Task not found");
}
std::vector<Task> TaskManager::getAllTasks(){
    return taskRepo;
}
void TaskManager::markTaskAsCompleted(int taskId){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        it->is_completed = true;
        taskStorage.save(taskRepo);
    }
}
void TaskManager::setTaskPriority(int taskId, int priority){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        it->priority = priority;
        taskStorage.save(taskRepo);
    }
}
void TaskManager::setTaskExecutionTime(int taskId, int executionTime){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        it->execution_time = executionTime;
        taskStorage.save(taskRepo);
    }
}
void TaskManager::setTaskDescription(int taskId, const std::string &description){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        it->description = description;
        taskStorage.save(taskRepo);
    }
}
bool TaskManager::isTaskCompleted(int taskId){
    auto it = std::find_if(taskRepo.begin(), taskRepo.end(),
                           [taskId](const Task &task) { return task.task_id == taskId; });
    if (it != taskRepo.end()) {
        return it->is_completed;
    }
    throw std::runtime_error("Task not found");
}
std::vector<Task> TaskManager::getTasksByPriority(int priority){
    std::vector<Task> result;
    for (const auto &task : taskRepo) {
        if (task.priority == priority) {
            result.push_back(task);
        }
    }
    return result;
}
std::vector<Task> TaskManager::getTasksByExecutionTime(int executionTime){
    std::vector<Task> result;
    for (const auto &task : taskRepo) {
        if (task.execution_time == executionTime) {
            result.push_back(task);
        }
    }
    return result;
}
std::vector<Task> TaskManager::getTasksByCompletionStatus(bool isCompleted){
    std::vector<Task> result;
    for (const auto &task : taskRepo) {
        if (task.is_completed == isCompleted) {
            result.push_back(task);
        }
    }
    return result;
}
std::vector<Task> TaskManager::getTasksByDescription(const std::string &description){
    std::vector<Task> result;
    for (const auto &task : taskRepo) {
        if (task.description == description) {
            result.push_back(task);
        }
    }
    return result;
}
std::vector<Task> TaskManager::getTasksById(int taskId){
    std::vector<Task> result;
    for (const auto &task : taskRepo) {
        if (task.task_id == taskId) {
            result.push_back(task);
        }
    }
    return result;
}

