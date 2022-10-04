cmake_minimum_required(VERSION 3.4)

project(logscan VERSION 0.0.1 LANGUAGES C)

set(SOURCES 
    src/set.c
    src/hashtable.c
)

add_executable(${PROJECT_NAME}1 src/main_1.c ${SOURCES})

set_property(TARGET ${PROJECT_NAME}1 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}1 PUBLIC -Wall -Wextra -Wpedantic)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(${PROJECT_NAME}1 PUBLIC "${PROJECT_BINARY_DIR}")


add_executable(${PROJECT_NAME}2 src/main_2.c ${SOURCES})

set_property(TARGET ${PROJECT_NAME}2 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}2 PUBLIC -Wall -Wextra -Wpedantic)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(${PROJECT_NAME}2 PUBLIC "${PROJECT_BINARY_DIR}")



add_executable(maint src/main-t.c)

set_property(TARGET maint PROPERTY C_STANDARD 11)
target_compile_options(maint PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(maint
    pthread
)
