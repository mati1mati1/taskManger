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
    void setTaskPriority(int taskId, int priority);
    void setTaskExecutionTime(int taskId, int executionTime);
    void setTaskDescription(int taskId, const std::string &description);
    bool isTaskCompleted(int taskId);
    std::vector<Task> getTasksByPriority(int priority);
    std::vector<Task> getTasksByExecutionTime(int executionTime);
    std::vector<Task> getTasksByCompletionStatus(bool isCompleted);
    std::vector<Task> getTasksByDescription(const std::string &description);
    public:
    TaskManager(ITaskStorage &ITaskStorage)
    : taskStorage(ITaskStorage)
    {
        taskRepo = taskStorage.load();
    }
    std::vector<Task> getAllTasks();
    Task getTask(int taskId);
    void removeTask(int taskId);
    void markTaskAsCompleted(int taskId);
    void updateTask(int taskId, const Task &task);
    int addTask(Task &task){
        task.task_id = taskRepo.empty() ? 1 : taskRepo.back().task_id + 1; 
        taskRepo.push_back(task);
        taskStorage.save(taskRepo);
        return task.task_id;
    }
    std::vector<Task> filterTasks(const Task &filter);

    ~TaskManager(){
        taskStorage.save(taskRepo);
    }
};

