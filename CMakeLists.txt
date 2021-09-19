cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0091 NEW)

# set the project name
project(Physics)

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)
