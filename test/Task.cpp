
#include <gtest/gtest.h>
#include "../include/Task.h"
#include <nlohmann/json.hpp>

TEST(TaskSerialization, Roundtrip) {
    Task task{1, 2, 30, "Test", false};

    nlohmann::json j = task;
    Task recovered = j.get<Task>();

    EXPECT_EQ(recovered.task_id, task.task_id);
    EXPECT_EQ(recovered.description, "Test");
    EXPECT_FALSE(recovered.is_completed);
}

TEST(TaskSerialization, AllFieldsCorrect) {
    Task task{42, 5, 120, "Complete the project", true};

    nlohmann::json j = task;
    Task recovered = j.get<Task>();

    EXPECT_EQ(recovered.task_id, 42);
    EXPECT_EQ(recovered.priority, 5);
    EXPECT_EQ(recovered.execution_time, 120);
    EXPECT_EQ(recovered.description, "Complete the project");
    EXPECT_TRUE(recovered.is_completed);
}

TEST(TaskSerialization, WithDefaultValues) {
    Task task{0, 0, 0, "", false};

    nlohmann::json j = task;
    Task recovered = j.get<Task>();

    EXPECT_EQ(recovered.task_id, 0);
    EXPECT_EQ(recovered.priority, 0);
    EXPECT_EQ(recovered.execution_time, 0);
    EXPECT_EQ(recovered.description, "");
    EXPECT_FALSE(recovered.is_completed);
}

TEST(TaskSerialization, WithEmptyDescription) {
    Task task{1, 1, 60, "", false};

    nlohmann::json j = task;
    Task recovered = j.get<Task>();

    EXPECT_EQ(recovered.task_id, 1);
    EXPECT_EQ(recovered.priority, 1);
    EXPECT_EQ(recovered.execution_time, 60);
    EXPECT_EQ(recovered.description, "");
    EXPECT_FALSE(recovered.is_completed);
}
