cmake_minimum_required(VERSION 3.10)

# find all source files
file(GLOB SOURCES
	./*.cpp
)

# add the executable
add_library(stbimage STATIC ${SOURCES})

target_include_directories(stbimage PUBLIC ./)