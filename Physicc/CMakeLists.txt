cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

# find all source files
file(GLOB_RECURSE SOURCES
	src/*.cpp
)

# add the executable
add_library(Physicc STATIC ${SOURCES})

target_include_directories(Physicc PUBLIC include)

add_subdirectory(../shared/libs/glm glm)
target_link_libraries(Physicc glm)





