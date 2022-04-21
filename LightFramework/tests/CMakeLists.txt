cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(LightFrameworkTests)

# find all source files
file(GLOB_RECURSE SOURCES
	*.cpp
)

# add the executable
add_executable(LightFrameworkTests ${SOURCES})
if (WIN32)
    if (MSVC)
        # set_property(TARGET LightFrameworkTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	endif()
endif()

# include gtest headers
target_include_directories(LightFrameworkTests PUBLIC ../../Testing/googletest/googletest/include/)

# # add googletest
add_subdirectory(../../Testing/googletest googletest)
target_link_libraries(LightFrameworkTests gtest)

# add physicc
target_link_libraries(LightFrameworkTests LightFramework)

# Set Gtest options
set(INSTALL_GTEST OFF)

include(GoogleTest)
gtest_discover_tests(LightFrameworkTests)
