cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0091 NEW)


# set the project name
project(Physics)

if(CMAKE_CONFIGURATION_TYPES)
	list(APPEND CMAKE_CONFIGURATION_TYPES RelProfile)
	list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}
		CACHE STRING "Append user-defined configuration to list of configurations to make it usable in Visual Studio" FORCE)
endif()

set(CMAKE_C_FLAGS_RELPROFILE "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "Set RelProfile flags" FORCE)
set(CMAKE_CXX_FLAGS_RELPROFILE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "Set RelProfile flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELPROFILE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}" CACHE STRING "Set RelProfile flags" FORCE)

add_subdirectory(Editor Editor)

add_subdirectory(Testing Test)

add_subdirectory(tools/tracy TracyServer)
