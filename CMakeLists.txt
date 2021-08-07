cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)