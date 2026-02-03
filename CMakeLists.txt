cmake_minimum_required(VERSION 3.25)

project(TaskManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)

option(SPDLOG_BUILD_EXAMPLE "" OFF)
option(SPDLOG_BUILD_TESTS "" OFF)
option(SPDLOG_BUILD_BENCH "" OFF)

FetchContent_MakeAvailable(spdlog)

add_library(project_spdlog INTERFACE)
target_link_libraries(project_spdlog INTERFACE spdlog::spdlog)


add_subdirectory(UserService)