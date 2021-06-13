cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# add the executable
add_library(Light STATIC ${SOURCES})

# add includes
target_include_directories(Light PUBLIC include)

# GLM
target_include_directories(Light PUBLIC ../LightFramework/libs/glm)

# add OpenGL library
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
target_link_libraries(Light ${OPENGL_gl_LIBRARY})

if (UNIX)
	if (NOT APPLE)
		find_package(Threads REQUIRED)
		find_package(X11 REQUIRED)
	endif()
endif()

# GLAD
add_subdirectory(libs/glad)
target_link_libraries(Light glad)

if (UNIX)
	if (APPLE)
		target_link_libraries(Light "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	else()
		target_link_libraries(Light ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}) 
	endif()
endif()