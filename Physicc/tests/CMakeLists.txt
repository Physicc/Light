cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(PhysiccTests)

# find all source files
file(GLOB_RECURSE SOURCES
	*.cpp
)

# add logging source
set(SOURCES ${SOURCES} ../../shared/src/core/logging.cpp)

# add filesystem source
# TODO: Fix later
set(SOURCES ${SOURCES} ../../LightFramework/src/core/filesystem.cpp)

# add the executable
add_executable(PhysiccTests ${SOURCES})
if (WIN32)
    if (MSVC)
        # set_property(TARGET PhysiccTests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	endif()
endif()

# include gtest headers
target_include_directories(PhysiccTests PUBLIC ../../Testing/googletest/googletest/include/)

# add googletest
add_subdirectory(../../Testing/googletest googletest)
target_link_libraries(PhysiccTests gtest)

# add physicc
target_link_libraries(PhysiccTests Physicc)

# Set Gtest options
set(INSTALL_GTEST OFF)

include(GoogleTest)
gtest_discover_tests(PhysiccTests)
