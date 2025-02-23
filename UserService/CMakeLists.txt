cmake_minimum_required(VERSION 3.25)
project(UserService)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CXX_STANDARD 20)

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