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
add_library(Physicc STATIC ${SOURCES})

target_include_directories(Physicc PUBLIC include)
target_include_directories(Physicc PUBLIC ../shared/include)

add_subdirectory(../shared/libs/glm glm)
target_link_libraries(Physicc glm)





