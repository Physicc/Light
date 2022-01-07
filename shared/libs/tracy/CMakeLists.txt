cmake_minimum_required(VERSION 3.10)

project(TracyServer)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ../../../tools/tracy/tracy)

add_library(TracyClient STATIC ${SRC_DIR}/TracyClient.cpp)
target_include_directories(TracyClient SYSTEM PUBLIC ${SRC_DIR})
target_include_directories(TracyClient PUBLIC ./)

target_compile_definitions(TracyClient PUBLIC "$<$<CONFIG:RelProfile>:TRACY_ENABLE>")

if(WIN32)
	if(MINGW)
		target_compile_definitions(TracyClient PUBLIC "_WIN32_WINNT=0x0602" "WINVER=0x0602")
	endif()
endif()