cmake_minimum_required(VERSION 3.4)

project(mainasm VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)


