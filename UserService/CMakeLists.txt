cmake_minimum_required(VERSION 3.25)
project(UserService)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(SOURCE_FILES 
src/User.cpp 
src/UserService.cpp 
src/TextDataBase.cpp
)

set(HEADER_FILES
src/User.h
src/DataBase.h
src/TextDataBase.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})