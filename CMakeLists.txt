cmake_minimum_required(VERSION 3.25)

project(TaskManager)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(EXT_FOLDER "external")

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(googletest)

set_target_properties(spdlog PROPERTIES FOLDER ${EXT_FOLDER})
set_target_properties(gtest PROPERTIES FOLDER ${EXT_FOLDER})
set_target_properties(gtest_main PROPERTIES FOLDER ${EXT_FOLDER})
set_target_properties(gmock PROPERTIES FOLDER ${EXT_FOLDER})
set_target_properties(gmock_main PROPERTIES FOLDER ${EXT_FOLDER})

add_library(project_spdlog INTERFACE)
target_link_libraries(project_spdlog INTERFACE spdlog::spdlog)

add_library(project_googletest INTERFACE)
target_link_libraries(project_googletest INTERFACE GTest::gtest 
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

add_subdirectory(common)
add_subdirectory(UserService)

#enable_testing()