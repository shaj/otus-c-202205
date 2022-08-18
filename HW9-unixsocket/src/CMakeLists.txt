

set(SOURCE
    main.c
    cmdline.c
)

include(gengetopt)
find_gengetopt ()
add_gengetopt_files (cmdline "--conf-parser")
set(SOURCE ${SOURCE} ${GGO_C})


add_executable(${PROJECT_NAME} ${SOURCE})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")


configure_file("${PROJECT_SOURCE_DIR}/src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")