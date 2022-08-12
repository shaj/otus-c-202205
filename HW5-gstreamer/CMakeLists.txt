cmake_minimum_required(VERSION 3.14)

project(wttr VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CodeFormat)

add_subdirectory(my_base_src)


# configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
