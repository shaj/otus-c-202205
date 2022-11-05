cmake_minimum_required(VERSION 3.4)

project(tep VERSION 0.0.1 LANGUAGES C)

# set(SOURCES )

add_executable(${PROJECT_NAME} main.c ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

