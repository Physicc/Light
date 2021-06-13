cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# Add logging source
list(APPEND SOURCES ../shared/src/core/logging.cpp)

# add the executable
add_library(LightFramework STATIC ${SOURCES})

# add includes
target_include_directories(LightFramework PUBLIC include)
target_include_directories(LightFramework PUBLIC ../shared/include)

# Light
add_subdirectory(../Light Light)
target_link_libraries(LightFramework Light)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLFW
add_subdirectory(libs/glfw-3.3.3)
target_link_libraries(LightFramework glfw)

add_definitions(-DGLFW_INCLUDE_NONE)

# ImGUI
add_subdirectory(libs/imgui)
target_link_libraries(LightFramework glad glfw imgui)

# StbImage
add_subdirectory(libs/stbimage)
target_link_libraries(LightFramework stbimage)

# GLM
target_include_directories(Light PUBLIC ../shared/libs/glm)

# spdlog
add_subdirectory(../shared/libs/spdlog spdlog)
# target_link_libraries(LightCommon spdlog)
