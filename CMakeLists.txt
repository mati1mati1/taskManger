cmake_minimum_required(VERSION 3.14)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Dependencies
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/latest/download/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(include)

# Main server
add_executable(TaskApp
    src/main.cpp
    src/TaskServer.cpp
    src/TaskManager.cpp
    src/JsonTaskStorage.cpp
)

target_link_libraries(TaskApp PRIVATE nlohmann_json)

# CLI client
add_executable(taskctl
    src/taskctl.cpp
    src/TaskClient.cpp
)

target_link_libraries(taskctl PRIVATE nlohmann_json)

# Tests
add_executable(GTestTaskTests
    test/TaskManager.cpp
    test/JsonTaskStorage.cpp
    test/Task.cpp
    src/TaskManager.cpp
    src/JsonTaskStorage.cpp
)

target_link_libraries(GTestTaskTests
    PRIVATE
    gtest
    gtest_main
    nlohmann_json
)
