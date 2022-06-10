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

# -Werror
target_compile_options(LightFramework PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Supress glm quat warning
target_compile_definitions(LightFramework PRIVATE
    -DGLM_FORCE_SILENT_WARNINGS
)


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

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)
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

# Cereal
# Disable Cereal docs
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
add_subdirectory(libs/cereal)
target_link_libraries(LightFramework cereal)


# NFDE
if(WIN32)
	target_link_libraries(LightFramework "ole32" "uuid")
endif()
add_subdirectory(libs/nativefiledialog-extended)
target_link_libraries(LightFramework nfd)

#EnTT
target_include_directories(LightFramework PUBLIC libs/entt)

#TracyClient
target_include_directories(LightFramework PUBLIC ../shared/libs/tracy/tracy)
target_link_libraries(LightFramework TracyClient)
