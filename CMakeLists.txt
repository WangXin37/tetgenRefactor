cmake_minimum_required(VERSION 3.20)
project(tetgenRefactor VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
  )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/3rd)
include(version.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(core)
include_directories(${THIRDPARTY_DIR})
include_directories(${THIRDPARTY_DIR}/predicates)

add_subdirectory(core)

add_executable(tetgenr main.cpp)
target_link_libraries(tetgenr core)