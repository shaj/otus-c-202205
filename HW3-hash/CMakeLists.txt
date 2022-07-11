cmake_minimum_required(VERSION 3.4)

project(otuswc VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CodeFormat)

add_executable(${PROJECT_NAME}
    src/main.c
    src/word_counter.c
    src/hashtable.c
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")

configure_file(${CMAKE_SOURCE_DIR}/test/utf-8.txt ${CMAKE_BINARY_DIR}/utf-8.txt COPYONLY)
# configure_file(${CMAKE_SOURCE_DIR}/zipjpeg.jpg ${CMAKE_BINARY_DIR}/zipjpeg.jpg COPYONLY)
