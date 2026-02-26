set(SRC_FILES
	test_TextFileDataBase.cpp
)

add_executable(UserServiceTest ${SRC_FILES})

target_link_libraries(UserServiceTest PRIVATE project_googletest)

include(GoogleTest)
gtest_discover_tests(UserServiceTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DISCOVERY_TIMEOUT 5
)

set_target_properties(UserServiceTest PROPERTIES FOLDER "tests")
