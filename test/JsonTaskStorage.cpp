
#include <gtest/gtest.h>
#include "../include/JsonTaskStorage.h"
#include "../include/Task.h"
#include <fstream>
#include <cstdio>

const std::string testFilePath = "test_tasks.json";

class JsonTaskStorageTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::remove(testFilePath.c_str());
    }
};

TEST_F(JsonTaskStorageTest, SaveTasksToFile) {
    JsonTaskStorage storage(testFilePath);

    Task task1{1, 3, 100, "Sample Task 1", false};
    Task task2{2, 1, 200, "Sample Task 2", true};
    std::vector<Task> tasksToSave = { task1, task2 };

    storage.save(tasksToSave);

    std::ifstream file(testFilePath);
    ASSERT_TRUE(file.is_open());

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    EXPECT_NE(content.find("Sample Task 1"), std::string::npos);
    EXPECT_NE(content.find("Sample Task 2"), std::string::npos);
}

TEST_F(JsonTaskStorageTest, LoadTasksFromFile) {
    JsonTaskStorage storage(testFilePath);

    Task task1{1, 3, 100, "Sample Task 1", false};
    Task task2{2, 1, 200, "Sample Task 2", true};
    std::vector<Task> tasksToSave = { task1, task2 };
    storage.save(tasksToSave);

    auto loaded = storage.load();

    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0].description, "Sample Task 1");
    EXPECT_TRUE(loaded[1].is_completed);
}

TEST_F(JsonTaskStorageTest, HandlesEmptyFile) {
    JsonTaskStorage storage(testFilePath);
    auto loaded = storage.load();
    EXPECT_TRUE(loaded.empty());
}

TEST_F(JsonTaskStorageTest, HandlesNonExistentFile) {
    std::remove(testFilePath.c_str());
    JsonTaskStorage storage(testFilePath);
    auto loaded = storage.load();
    EXPECT_TRUE(loaded.empty());
}

TEST_F(JsonTaskStorageTest, HandlesInvalidJSON) {
    std::ofstream file(testFilePath);
    file << "Invalid JSON content";
    file.close();

    JsonTaskStorage storage(testFilePath);
    auto loaded = storage.load();
    EXPECT_TRUE(loaded.empty());
}
