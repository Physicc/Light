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

# UUID
if(UNIX)
	if (NOT APPLE)
		target_link_libraries(LightFramework "uuid")
	endif()
elseif(WIN32)
	target_link_libraries(LightFramework "rpcrt4")
endif()

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
target_include_directories(LightFramework PRIVATE ../Light/libs/glad/include) # Hack so that imgui_impl can use glad #47
target_link_libraries(LightFramework imgui)

# GLM
target_include_directories(LightFramework PUBLIC ../shared/libs/glm)

# spdlog
add_subdirectory(../shared/libs/spdlog spdlog)
# target_link_libraries(LightCommon spdlog)

#EnTT
target_include_directories(LightFramework PUBLIC libs/entt)

if (UNIX)
	if (NOT APPLE)
		find_package(Threads REQUIRED)
		find_package(X11 REQUIRED)
	endif()
endif()

if (UNIX)
	if (APPLE)
		target_link_libraries(LightFramework "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	else()
		target_link_libraries(LightFramework ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}) 
	endif()
endif()