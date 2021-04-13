cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

# find all source files
file(GLOB SOURCES
	src/*.cpp
)

# add the executable
add_executable(Editor ${SOURCES})

target_include_directories(Editor PUBLIC include)

add_subdirectory(../Light Light)
target_link_libraries(Editor Light)