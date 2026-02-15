project(UserService)

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

target_link_libraries(UserService PRIVATE common::logger)

add_subdirectory(tests)


