cmake_minimum_required(VERSION 3.25)
project(UserService)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CXX_STANDARD 20)

set(SPDLOG_INCLUDE_DIR "../include/headers")
set(SPDLOG_LIBRARY "../include/lib/spdlogd.lib")

include_directories(${SPDLOG_INCLUDE_DIR})

set(SOURCE_FILES 
src/User.cpp 
src/UserService.cpp 
src/TextFileDataBase.cpp
)

set(HEADER_FILES
src/User.h
src/DataBase.h
src/TextFileDataBase.h
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(UserService PRIVATE project_spdlog)
