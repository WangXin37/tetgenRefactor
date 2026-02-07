# timestamp
string(TIMESTAMP PROJECT_BUILD_YEAR "%Y")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")
string(TIMESTAMP PROJECT_BUILD_TIME "%H:%M:%S")
set(TETGENR_BUILD_DATE "${PROJECT_BUILD_DATE}")
set(TETGENR_BUILD_TIME "${PROJECT_BUILD_DATE} ${PROJECT_BUILD_TIME}")

# build_type
set(TETGENR_BUILD_TYPE "release")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    set(TETGENR_BUILD_TYPE "debug")
endif()

# svn/git commit revision
if (EXISTS "${CMAKE_SOURCE_DIR}/.git/")
    find_package(Git)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
            OUTPUT_VARIABLE TETGENR_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --abbrev=6 --always --tags
            OUTPUT_VARIABLE TETGENR_GIT_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        execute_process(
            COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
            OUTPUT_VARIABLE TETGENR_GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    endif()
endif()

# public version
set(TETGENR_VERSION_MAJOR ${PROJECT_BUILD_YEAR})
set(TETGENR_VERSION_MINOR 0 CACHE STRING "version minor")
set(TETGENR_VERSION_PATCH 0 CACHE STRING "version patch")
set(TETGENR_VERSION "${TETGENR_VERSION_MAJOR}.${TETGENR_VERSION_MINOR}.${TETGENR_VERSION_PATCH}")

# internal version
set(TETGENR_INTERNAL_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(TETGENR_INTERNAL_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(TETGENR_INTERNAL_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(TETGENR_INTERNAL_VERSION "${TETGENR_INTERNAL_VERSION_MAJOR}.${TETGENR_INTERNAL_VERSION_MINOR}.${TETGENR_INTERNAL_VERSION_PATCH}")

if(NOT DEFINED TETGENR_GIT_HASH)
    set(TETGENR_GIT_HASH "")
endif()
if(NOT DEFINED TETGENR_GIT_BRANCH)
    set(TETGENR_GIT_BRANCH "")
endif()
if(NOT DEFINED TETGENR_GIT_TAG)
    set(TETGENR_GIT_TAG "")
endif()

# generate the version file 
set(VERSION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tetgenr_version.h)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tetgenRefactor_version.in.h" ${VERSION_FILE} @ONLY)

# print version information
message(STATUS "--- TETGENR Version ---")
message(STATUS "Version : ${TETGENR_VERSION}")
message(STATUS "Internal Version : ${TETGENR_INTERNAL_VERSION}")
message(STATUS "TETGENR Git Hash : ${TETGENR_GIT_HASH}")
message(STATUS "TETGENR Git Branch : ${TETGENR_GIT_BRANCH}")
message(STATUS "TETGENR Git Tag : ${TETGENR_GIT_TAG}")
message(STATUS "TETGENR Build Time : ${TETGENR_BUILD_TIME}")
message(STATUS "TETGENR Build Type : ${TETGENR_BUILD_TYPE}")
message(STATUS "---")
