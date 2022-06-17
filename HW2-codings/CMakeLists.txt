cmake_minimum_required(VERSION 3.4)

project(toutf-8 VERSION 0.0.1 LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

configure_file(${CMAKE_SOURCE_DIR}/cp1251.txt ${CMAKE_BINARY_DIR}/cp1251.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/iso-8859-5.txt ${CMAKE_BINARY_DIR}/iso-8859-5.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/koi8.txt ${CMAKE_BINARY_DIR}/koi8.txt COPYONLY)

