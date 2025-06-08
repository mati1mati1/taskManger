
#include <gtest/gtest.h>
#include "../include/TaskManager.h"
#include "../include/Task.h"

// Mock implementation of ITaskStorage
class MockTaskStorage : public ITaskStorage {
public:
    std::vector<Task> tasks;

    std::vector<Task> load() override {
        return tasks;
    }

    void save(const std::vector<Task>& newTasks) override {
        tasks = newTasks;
    }
};

class TaskManagerTest : public ::testing::Test {
protected:
    MockTaskStorage mockStorage;
    TaskManager* manager;

    void SetUp() override {
        manager = new TaskManager(mockStorage);
    }

    void TearDown() override {
        delete manager;
    }
};

TEST_F(TaskManagerTest, AddAndRetrieveTasks) {
    Task task1{1, 1, 10, "Test task", false};
    manager->addTask(task1);

    auto tasks = manager->getAllTasks();
    ASSERT_EQ(tasks.size(), 1);
    EXPECT_EQ(tasks[0].description, "Test task");
}

TEST_F(TaskManagerTest, RemoveTaskById) {
    Task task1{1, 1, 10, "Remove me", false};
    manager->addTask(task1);
    manager->removeTask(1);
    auto tasks = manager->getAllTasks();
    EXPECT_TRUE(tasks.empty());
}

TEST_F(TaskManagerTest, UpdateTaskById) {
    Task task{1, 1, 10, "Original", false};
    manager->addTask(task);
    task.description = "Updated";
    task.execution_time = 20;
    task.is_completed = true;
    manager->updateTask(1, task);

    Task t = manager->getTask(1);
    EXPECT_EQ(t.description, "Updated");
    EXPECT_EQ(t.execution_time, 20);
    EXPECT_TRUE(t.is_completed);
}

TEST_F(TaskManagerTest, MarkTaskAsCompleted) {
    Task task{1, 1, 10, "Incomplete", false};
    manager->addTask(task);
    manager->markTaskAsCompleted(1);
    EXPECT_TRUE(manager->getTask(1).is_completed);
}
