include(gengetopt)
find_gengetopt ()
add_gengetopt_files (cmdline)
set(SOURCES ${SOURCES} ${GGO_C})


add_executable(${PROJECT_NAME} main.c ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)

configure_file("version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")