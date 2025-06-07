#pragma once
#include "ITaskStorage.h"
#include "Task.h"
#include <vector>
#include <string>
class TaskManager
{
private:
    ITaskStorage &taskStorage;
    std::vector <Task> taskRepo;
public:
    TaskManager(ITaskStorage &ITaskStorage)
    : taskStorage(ITaskStorage)
    {
        taskRepo = taskStorage.load();
    }
    void addTask(const Task &task){
        taskRepo.push_back(task);
        taskStorage.save(taskRepo);
    }
    void removeTask(int taskId);
    void updateTask(int taskId, const Task &task);
    Task getTask(int taskId);
    std::vector<Task> getAllTasks();
    void markTaskAsCompleted(int taskId);
    void setTaskPriority(int taskId, int priority);
    void setTaskExecutionTime(int taskId, int executionTime);
    void setTaskDescription(int taskId, const std::string &description);
    bool isTaskCompleted(int taskId);
    std::vector<Task> getTasksByPriority(int priority);
    std::vector<Task> getTasksByExecutionTime(int executionTime);
    std::vector<Task> getTasksByCompletionStatus(bool isCompleted);
    std::vector<Task> getTasksByDescription(const std::string &description);
    std::vector<Task> getTasksById(int taskId);
    ~TaskManager(){
        taskStorage.save(taskRepo);
    }
};

