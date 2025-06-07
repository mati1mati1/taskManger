cmake_minimum_required(VERSION 3.14)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent setup
include(FetchContent)

# Fetch nlohmann/json
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/latest/download/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0  
)
FetchContent_MakeAvailable(googletest)

# ===========================
# Build the main application
# ===========================
set(APP_SOURCES
    main.cpp
    TaskManager.cpp
    JsonTaskStorage.cpp
)

add_executable(TaskApp ${APP_SOURCES})
target_link_libraries(TaskApp PRIVATE nlohmann_json)

# ===========================
# Build the test executable
# ===========================
add_executable(GTestTaskTests
    test/gtest_TaskManager.cpp
    test/gtest_TaskManager.cpp
    test/gtest_Task.cpp
    TaskManager.cpp
    JsonTaskStorage.cpp
)

target_link_libraries(GTestTaskTests
    PRIVATE
    gtest
    gtest_main
    nlohmann_json
)
