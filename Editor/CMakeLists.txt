cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

# find all source files
file(GLOB SOURCES
	src/*.cpp
)

# add the executable
add_executable(Editor ${SOURCES})

if(WIN32)
	if(MSVC)
		set_target_properties(
			Editor
			PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
					LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
		)
	elseif(MINGW)
		if(CMAKE_BUILD_TYPE STREQUAL "Release")
			target_link_options(Editor PRIVATE -mwindows)
		endif()
	endif()
endif()

target_include_directories(Editor PUBLIC include)

add_subdirectory(../Light Light)
target_link_libraries(Editor Light)

add_subdirectory(../Physicc Physicc)
target_link_libraries(Editor Physicc)


add_custom_command(TARGET Editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/../imgui.ini ${CMAKE_BINARY_DIR}/imgui.ini)