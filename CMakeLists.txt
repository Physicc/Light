cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

if (UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "-lstdc++")
    endif ()
endif ()

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)