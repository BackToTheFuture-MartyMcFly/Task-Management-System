add_library(Logger Logger.cpp Logger.h)

target_include_directories(Logger 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Logger PUBLIC project_spdlog)

add_library(common::logger ALIAS Logger)