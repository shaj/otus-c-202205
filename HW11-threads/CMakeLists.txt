cmake_minimum_required(VERSION 3.4)

project(logscan VERSION 0.0.1 LANGUAGES C)

set(SOURCES src/set.c)

add_executable(${PROJECT_NAME} src/main.c ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

