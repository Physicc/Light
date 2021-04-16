cmake_minimum_required(VERSION 3.10)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# add the executable
add_library(Light STATIC ${SOURCES})

target_include_directories(Light PUBLIC include)

find_package(OpenGL REQUIRED)
target_link_libraries(Light ${OPENGL_gl_LIBRARY})

if (UNIX)
	if (NOT APPLE)
		find_package(Threads REQUIRED)
		find_package(X11 REQUIRED)
	endif()
endif()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw-3.3.3)
target_link_libraries(Light glfw)

add_subdirectory(libs/glad)
target_link_libraries(Light glad)

add_definitions(-DGLFW_INCLUDE_NONE)

add_subdirectory(libs/imgui)
target_link_libraries(Light glad glfw imgui)

add_subdirectory(libs/stbimage)
target_link_libraries(Light stbimage)

target_include_directories(Light PUBLIC libs/glm)

if (UNIX)
	if (APPLE)
		target_link_libraries(Light "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	else()
		target_link_libraries(Light ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}) 
	endif()
endif()