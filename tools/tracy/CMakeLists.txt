cmake_minimum_required(VERSION 3.10)

project(TracyServer)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tracy/profiler/build)

file(STRINGS "${BUILD_SRC}/win32/Tracy.vcxproj" SLN_CONTENTS REGEX "ClCompile.*cpp\"")
foreach(_line ${SLN_CONTENTS})
	STRING(REGEX REPLACE ".*\"\(.*\)\".*" "\\1" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\\\.\\.\\\\" "tracy\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\" "tracy\\\\profiler\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\\\" "/" _line ${_line})
	set(SRC_FILES ${SRC_FILES} ${_line})
endforeach()

file(STRINGS "${BUILD_SRC}/win32/Tracy.vcxproj" SLN_CONTENTS REGEX "ClCompile.*c\"")
foreach(_line ${SLN_CONTENTS})
	STRING(REGEX REPLACE ".*\"\(.*\)\".*" "\\1" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\\\.\\.\\\\" "tracy\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\" "tracy\\\\profiler\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\\\" "/" _line ${_line})
	set(SRC_FILES ${SRC_FILES} ${_line})
endforeach()

file(STRINGS "${BUILD_SRC}/win32/Tracy.vcxproj" SLN_CONTENTS REGEX "None.*S\"")
foreach(_line ${SLN_CONTENTS})
	STRING(REGEX REPLACE ".*\"\(.*\)\".*" "\\1" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\\\.\\.\\\\" "tracy\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\.\\.\\\\\\.\\.\\\\" "tracy\\\\profiler\\\\" _line ${_line})
	STRING(REGEX REPLACE "\\\\" "/" _line ${_line})
	set(SRC_FILES ${SRC_FILES} ${_line})
endforeach()

list(FILTER SRC_FILES EXCLUDE REGEX ".*nfd/nfd_win.cpp")

add_executable(TracyServer ${SRC_FILES})
target_include_directories(TracyServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tracy/imgui)

set(CAPSTONE_ARCHITECTURE_DEFAULT OFF CACHE BOOL "Captstone architecures disabled by default" FORCE)
set(archlist ARM ARM64 X86)

foreach(arch in ${archlist})
	set(CAPSTONE_${arch}_SUPPORT ON CACHE BOOL "Enable ${arch} support" FORCE)
endforeach()

target_include_directories(TracyServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/capstone/include/capstone)

target_compile_definitions(TracyServer PRIVATE TRACY_NO_FILESELECTOR IMGUI_ENABLE_FREETYPE _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE WIN32_LEAN_AND_MEAN NOMINMAX _USE_MATH_DEFINES ZSTD_DISABLE_ASM)
if (UNIX)
	target_compile_definitions(TracyServer PRIVATE DISPLAY_SERVER_X11)
endif()


set(CAPSTONE_BUILD_STATIC_RUNTIME OFF CACHE BOOL "Build static runtime")
set(CAPSTONE_BUILD_SHARED OFF CACHE BOOL "Build shared library")
set(CAPSTONE_BUILD_DIET OFF CACHE BOOL "Build diet library" )
set(CAPSTONE_BUILD_TESTS OFF CACHE BOOL "Build tests")
set(CAPSTONE_BUILD_CSTOOL OFF CACHE BOOL "Build cstool")
add_subdirectory(capstone)
target_link_libraries(TracyServer capstone-static)

add_subdirectory(freetype)
target_link_libraries(TracyServer freetype)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
target_link_libraries(TracyServer glfw)

if(WIN32)
	target_link_libraries(TracyServer ws2_32)
endif()

function(CheckHasModule Module)
	find_package(${Module} QUIET)
	if(NOT DEFINED ${Module}_DIR)
		set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
	elseif(${Module}_DIR)
		set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
	else()
	set(HAS_MODULE_${Module} FALSE PARENT_SCOPE)
	endif()
endfunction()

CheckHasModule(TBB)
if(HAS_MODULE_TBB)
	target_link_libraries(TracyServer tbb)
endif()

target_link_libraries(TracyServer ${CMAKE_DL_LIBS})