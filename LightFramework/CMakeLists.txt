cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# add the executable
add_library(LightFramework STATIC ${SOURCES})

target_include_directories(LightFramework PUBLIC include)

# add OpenGL library
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
target_link_libraries(LightFramework ${OPENGL_gl_LIBRARY})

if (UNIX)
	if (NOT APPLE)
		find_package(Threads REQUIRED)
		find_package(X11 REQUIRED)
	endif()
endif()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLFW
add_subdirectory(libs/glfw-3.3.3)
target_link_libraries(LightFramework glfw)

# GLAD
add_subdirectory(libs/glad)
target_link_libraries(LightFramework glad)

add_definitions(-DGLFW_INCLUDE_NONE)

# ImGUI
add_subdirectory(libs/imgui)
target_link_libraries(LightFramework glad glfw imgui)

# StbImage
add_subdirectory(libs/stbimage)
target_link_libraries(LightFramework stbimage)

# GLM
target_include_directories(LightFramework PUBLIC libs/glm)

# spdlog
add_subdirectory(libs/spdlog)
target_link_libraries(LightFramework spdlog)

if (UNIX)
	if (APPLE)
		target_link_libraries(LightFramework "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	else()
		target_link_libraries(LightFramework ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}) 
	endif()
endif()
