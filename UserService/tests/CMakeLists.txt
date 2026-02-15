
set(SRC_FILES
	test.cpp
)

add_executable(user_service_tests ${SRC_FILES})

target_link_libraries(user_service_tests PRIVATE project_googletest)

include(GoogleTest)
gtest_discover_tests(user_service_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DISCOVERY_TIMEOUT 5
)

set_target_properties(user_service_tests PROPERTIES FOLDER "tests")
