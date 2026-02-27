project(LoggerTests)

set(SRC_FILES
	test_Logger.cpp
)

add_executable(LoggerTests ${SRC_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE SERVICE_NAME=${PROJECT_NAME})

target_link_libraries(LoggerTests PRIVATE project_googletest)
target_link_libraries(LoggerTests PRIVATE common::logger)

include(GoogleTest)
gtest_discover_tests(LoggerTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DISCOVERY_TIMEOUT 5
)

set_target_properties(LoggerTests PROPERTIES FOLDER "tests")
