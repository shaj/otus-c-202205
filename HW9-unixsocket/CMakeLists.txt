cmake_minimum_required(VERSION 3.14)

project(fsized VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CodeFormat)

add_subdirectory(src)

configure_file(${CMAKE_SOURCE_DIR}/fsized.conf.yaml ${CMAKE_BINARY_DIR}/fsized.yaml COPYONLY)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
