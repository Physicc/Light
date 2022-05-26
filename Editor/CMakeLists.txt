cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

# set the project name
project(Physics)

# find all source files
file(GLOB_RECURSE SOURCES
        src/*.cpp
        )

if (CMAKE_RC_COMPILER)
    # Append assets/light.rc to list sources
    list(APPEND SOURCES assets/icon/light.rc)
endif()

# add the executable
add_executable(Editor ${SOURCES})

# -Werror
target_compile_options(Editor PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
# Supress glm quat warning
target_compile_definitions(Editor PRIVATE
    -DGLM_FORCE_SILENT_WARNINGS
)

if (WIN32)
    if (MSVC)
        set_target_properties(
                Editor
                PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
                LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
        )
        target_compile_options(Editor PRIVATE "/MP")
    elseif (MINGW)
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            target_link_options(Editor PRIVATE -mwindows)
        endif ()
    endif ()
endif ()

target_include_directories(Editor PUBLIC include)

add_subdirectory(libs/ImGuizmo)
target_link_libraries(Editor ImGuizmo)

add_subdirectory(../LightFramework LightFramework)
target_link_libraries(Editor LightFramework)

add_subdirectory(../Physicc Physicc)
target_link_libraries(Editor Physicc)

add_subdirectory(../shared/libs/tracy TracyClient)
target_link_libraries(Editor TracyClient)

add_custom_command(TARGET Editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini $<TARGET_FILE_DIR:Editor>)

# Add a custom target CopyAssets which depends on a dummy file
# This dummy file is output by the custom command which also copies the assets
# This command depends on the asset files
# If the asset files are modified, then the dummy file becomes out of date
file(GLOB_RECURSE ASSETS
	${CMAKE_CURRENT_SOURCE_DIR}/assets/*
)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_Assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:Editor>/assets
	COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/_Assets
	DEPENDS ${ASSETS}
	COMMENT "Copying assets to executable directory"
)

add_custom_target(CopyAssets DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/_Assets)
add_dependencies(Editor CopyAssets)
