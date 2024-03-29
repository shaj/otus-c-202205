cmake_minimum_required(VERSION 3.4)

project(logscan VERSION 0.0.1 LANGUAGES C)

set(SOURCES 
    src/hashtable.c
)

configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")

add_executable(${PROJECT_NAME}0 src/main_0.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME}0 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}0 PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}0 PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}0 pthread)


add_executable(${PROJECT_NAME}1 src/main_1.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME}1 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}1 PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}1 PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}1 pthread)


add_executable(${PROJECT_NAME}2 src/main_2.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME}2 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}2 PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}2 PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}2 pthread)

add_executable(${PROJECT_NAME}21 src/main_21.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME}21 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}21 PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}21 PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}21 pthread)


add_executable(${PROJECT_NAME}3 src/main_3.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME}3 PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME}3 PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME}3 PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME}3 pthread)



add_executable(maint src/main-t.c)

set_property(TARGET maint PROPERTY C_STANDARD 11)
target_compile_options(maint PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(maint
    pthread
)
