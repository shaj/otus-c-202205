cmake_minimum_required(VERSION 3.14)

add_executable(${PROJECT_NAME}
    src/main.c
    src/getter.c
    src/parser.c
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CURL_INCLUDE_DIRS}
    # ${CJSON_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
    ${CURL_LIBRARIES}
    # ${CJSON_LIBRARIES}
    cjson
)


configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
