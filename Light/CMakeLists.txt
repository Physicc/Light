cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# Add logging source
if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
	list(APPEND SOURCES ../shared/src/core/logging.cpp)
endif()

# add the executable
add_library(Light STATIC ${SOURCES})

# add includes
target_include_directories(Light PUBLIC include)
target_include_directories(Light PUBLIC ../shared/include)

# GLM
target_include_directories(Light PUBLIC ../shared/libs/glm)

# add shared
if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
	add_subdirectory(../shared/libs/spdlog spdlog)
else()
	target_include_directories(Light PUBLIC ../shared/libs/spdlog/include)
endif()

# target_link_libraries(Light PRIVATE shared)

# add OpenGL library
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
target_link_libraries(Light PUBLIC ${OPENGL_gl_LIBRARY})

if (UNIX)
	if (NOT APPLE)
		find_package(Threads REQUIRED)
		find_package(X11 REQUIRED)
	endif()
endif()

# GLAD
add_subdirectory(libs/glad)
target_link_libraries(Light PRIVATE glad)

if (UNIX)
	if (APPLE)
		target_link_libraries(Light PUBLIC "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
	else()
		target_link_libraries(Light PUBLIC ${CMAKE_THREAD_LIBS_INIT} ${X11_LIBRARIES} ${CMAKE_DL_LIBS}) 
	endif()
endif()