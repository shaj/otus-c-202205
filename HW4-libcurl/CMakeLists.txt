cmake_minimum_required(VERSION 3.14)

project(wttr VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CodeFormat)

find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.c
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC CURL_INCLUDE_DIRS)
# target_link_directories(${PROJECT_NAME} CURL_LIBRARIES)
target_link_libraries(${PROJECT_NAME} curl)


configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")

