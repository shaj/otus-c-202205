cmake_minimum_required(VERSION 3.4)

project(otuswc VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CodeFormat)

add_executable(${PROJECT_NAME} main.c)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

configure_file(${CMAKE_SOURCE_DIR}/non-zipjpeg.jpg ${CMAKE_BINARY_DIR}/non-zipjpeg.jpg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/zipjpeg.jpg ${CMAKE_BINARY_DIR}/zipjpeg.jpg COPYONLY)
