cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

if (WIN32)
    if (MSVC)
        set_target_properties(
                Editor
                PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
                LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
        )
        target_compile_options(Editor PRIVATE "/MP")
    elseif (MINGW)
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            target_link_options(Editor PRIVATE -mwindows)
        endif ()
    endif ()
elseif (UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "-lstdc++")
    endif ()
endif ()

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)