# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindCppCheck
-------

Finds the CppCheck Tool.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``CppCheck_FOUND``
  True if the system has the cppcheck tool.
``CppCheck_VERSION``
  Version of cppcheck tool.

#]=======================================================================]

if(CPPCHECK_ROOT_DIR)
    find_program(
        CPPCHECK_BIN
        NAMES cppcheck
        PATHS "${CPPCHECK_ROOT_DIR}"
        NO_DEFAULT_PATH
    )
endif()

if(NOT CPPCHECK_BIN)
    find_program(CPPCHECK_BIN NAMES cppcheck)
endif()

if(CPPCHECK_BIN)
    execute_process(
        COMMAND ${CPPCHECK_BIN} --version
        OUTPUT_VARIABLE CPPCHECK_VERSION_OUTPUT
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(CPPCHECK_VERSION_OUTPUT MATCHES "^Cppcheck .*")
        string(REGEX REPLACE "Cppcheck ([.0-9]+)" "\\1" CppCheck_VERSION
                             "${CPPCHECK_VERSION_OUTPUT}"
        )
    else()
        message(FATAL "Couldn't recognize ${CPPCHECK_BIN} version")
    endif()

    set(CPPCHECK_THREADS_ARG "-j4" CACHE STRING "The number of threads to use")
    set(CPPCHECK_PROJECT_ARG
        "--project=${PROJECT_BINARY_DIR}/compile_commands.json"
        CACHE STRING "The project directory to use"
    )
    set(CPPCHECK_BUILD_DIR_ARG
        "--cppcheck-build-dir=${PROJECT_BINARY_DIR}/analysis/cppcheck"
        CACHE STRING "The build directory to use"
    )

    # Don't show these errors
    if(EXISTS "${CMAKE_SOURCE_DIR}/.cppcheck-suppressions")
        set(CPPCHECK_SUPPRESSIONS
            "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions"
            CACHE STRING "The suppressions file to use"
        )
    else()
        set(CPPCHECK_SUPPRESSIONS "" CACHE STRING
                                           "The suppressions file to use"
        )
    endif()

    # Show these errors but don't fail the build
    if(EXISTS "${CMAKE_SOURCE_DIR}/.cppcheck-exitcode-suppressions")
        set(CPPCHECK_EXITCODE_SUPPRESSIONS
            "--exitcode-suppressions=${CMAKE_SOURCE_DIR}/.cppcheck-exitcode-suppressions"
            CACHE STRING "The exitcode suppressions file to use"
        )
    else()
        set(CPPCHECK_EXITCODE_SUPPRESSIONS ""
            CACHE STRING "The exitcode suppressions file to use"
        )
    endif()

    set(CPPCHECK_ERROR_EXITCODE_ARG "--error-exitcode=1"
        CACHE STRING "The exitcode to use if an error is found"
    )
    set(CPPCHECK_CHECKS_ARGS "--enable=warning"
        CACHE STRING "Arguments for the checks to run"
    )
    set(CPPCHECK_OTHER_ARGS "" CACHE STRING "Other arguments")

    set(_CPPCHECK_EXCLUDES)
    foreach(ex ${CPPCHECK_EXCLUDES})
        list(APPEND _CPPCHECK_EXCLUDES "-i${ex}")
    endforeach(ex)

    set(CPPCHECK_ALL_ARGS
        ${CPPCHECK_THREADS_ARG}
        ${CPPCHECK_PROJECT_ARG}
        ${CPPCHECK_BUILD_DIR_ARG}
        ${CPPCHECK_ERROR_EXITCODE_ARG}
        ${CPPCHECK_SUPPRESSIONS}
        ${CPPCHECK_EXITCODE_SUPPRESSIONS}
        ${CPPCHECK_CHECKS_ARGS}
        ${CPPCHECK_OTHER_ARGS}
        ${_CPPCHECK_EXCLUDES}
    )

    if(NOT CPPCHECK_XML_OUTPUT)
        set(CPPCHECK_COMMAND ${CPPCHECK_BIN} ${CPPCHECK_ALL_ARGS})
    else()
        set(CPPCHECK_COMMAND ${CPPCHECK_BIN} ${CPPCHECK_ALL_ARGS} --xml
                             --xml-version=2 2> ${CPPCHECK_XML_OUTPUT}
        )
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    CppCheck REQUIRED_VARS CPPCHECK_BIN VERSION_VAR CppCheck_VERSION
)

mark_as_advanced(
    CPPCHECK_BIN
    CPPCHECK_VERSION_OUTPUT
    CPPCHECK_THREADS_ARG
    CPPCHECK_PROJECT_ARG
    CPPCHECK_BUILD_DIR_ARG
    CPPCHECK_SUPPRESSIONS
    CPPCHECK_EXITCODE_SUPPRESSIONS
    CPPCHECK_ERROR_EXITCODE_ARG
    CPPCHECK_CHECKS_ARGS
    CPPCHECK_OTHER_ARGS
)
