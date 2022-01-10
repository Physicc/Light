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

# -Werror
target_compile_options(Physicc PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Supress glm quat warning
target_compile_definitions(Physicc PRIVATE
    -DGLM_FORCE_SILENT_WARNINGS
)


target_include_directories(Physicc PUBLIC include)
target_include_directories(Physicc PUBLIC ../shared/include)

add_subdirectory(../shared/libs/glm glm)
target_link_libraries(Physicc glm)

add_subdirectory(../shared/libs/tracy TracyClient)
target_link_libraries(Physicc TracyClient)




