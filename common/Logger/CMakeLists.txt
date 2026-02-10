add_library(common_logger Logger.cpp Logger.h)

target_include_directories(common_logger 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(common_logger PUBLIC project_spdlog)

add_library(common::logger ALIAS common_logger)