cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

#Let the MSVC compiler flags be set by the abstraction
#For more details, see https://cmake.org/cmake/help/latest/policy/CMP0091.html
cmake_policy(SET CMP0091 NEW)

# set the project name
project(Physics)

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)
