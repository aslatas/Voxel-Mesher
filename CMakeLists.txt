# Minimum version is the same as NanoGUI.
cmake_minimum_required (VERSION 3.12 FATAL_ERROR)

# Set project name.
project(voxelmesher)

set(VOXELMESHER_SOURCES "" CACHE INTERNAL "All project source files.")

# Set library output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Set NanoGUI build settings.
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# Function to add sources to a list with complete path.
function(ADD_SOURCES)
    set(NEW_SOURCES ${ARGN})
    file(RELATIVE_PATH FILE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    list(TRANSFORM NEW_SOURCES PREPEND "${FILE_PATH}/")
    list(APPEND VOXELMESHER_SOURCES ${NEW_SOURCES})
    set(VOXELMESHER_SOURCES ${VOXELMESHER_SOURCES} CACHE INTERNAL "All project source files.")
    message(STATUS ${VOXELMESHER_SOURCES})
endfunction()

# Add subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/resources/shaders)
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/nanogui)

# Make the NanoGUI targets into dependencies.
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

# Add NanoGUI preprocessor definitions.
add_definitions(${NANOGUI_EXTRA_DEFS})

# Set include directories.
include_directories(include ext/nanogui/include ${NANOGUI_EXTRA_INCS})

message(STATUS "-" ${VOXELMESHER_SOURCES})
# Add executable.
add_executable(voxelmesher ${VOXELMESHER_SOURCES})

# Link against NanoGUI libraries.
target_link_libraries(voxelmesher nanogui ${NANOGUI_EXTRA_LIBS})

# Copy resources to the executable path on build.
add_custom_command(
    TARGET voxelmesher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
    COMMENT "copying resource files" VERBATIM)