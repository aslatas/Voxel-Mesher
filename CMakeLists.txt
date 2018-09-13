# Minimum version doesn't particularly matter. It's set the same as NanoGUI.
cmake_minimum_required (VERSION 2.8.12)

# Set project name.
project(voxelmesher)

set(SOURCE_FILES main.cpp )
# Options for resource directory locations.
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources CACHE PATH "Location of the resources folder")

# Set library output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Set NanoGUI build settings.
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# Add subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/nanogui)

# Make the NanoGUI targets into dependencies.
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

# Add NanoGUI preprocessor definitions.
add_definitions(${NANOGUI_EXTRA_DEFS})

# Set include directories.
include_directories(include ext/nanogui/include ${NANOGUI_EXTRA_INCS})

# Add executable.
add_executable(voxelmesher src/Main.cpp include/Shader.h)

# Link against NanoGUI libraries.
target_link_libraries(voxelmesher nanogui ${NANOGUI_EXTRA_LIBS})

# Get relative paths to resources.
file(RELATIVE_PATH _resourcePath ${CMAKE_SOURCE_DIR} ${RESOURCE_DIR})

# Copy resources to the executable path on build.
add_custom_command(
    TARGET voxelmesher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/${_resourcePath}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_resourcePath}
    COMMENT "copying resource files" VERBATIM)


