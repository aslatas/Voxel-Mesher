# Minimum version doesn't particularly matter. It's set the same as NanoGUI.
cmake_minimum_required (VERSION 2.8.12)

# Set project name.
project(voxelmesher)

# Set library output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Set NanoGUI build settings.
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# Add the configurations from NanoGUI.
add_subdirectory(ext/nanogui) 

# Make the NanoGUI targets into dependencies.
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

# Add NanoGUI preprocessor definitions.
add_definitions(${NANOGUI_EXTRA_DEFS})

# Set include directories.
include_directories(ext/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})

# Add executable.
add_executable(voxelmesher src/Main.cpp)

# Link against NanoGUI libraries.
target_link_libraries(voxelmesher nanogui ${NANOGUI_EXTRA_LIBS})

# Copy resources to executable path on build.
add_custom_command(
    TARGET voxelmesher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_CURRENT_BINARY_DIR}/resources)