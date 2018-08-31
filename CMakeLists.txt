# Minimum version doesn't particularly matter. It's set the same as nanogui.
cmake_minimum_required (VERSION 2.8.12)

# Set project name.
project(Gui_Proto)

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
include_directories(${NANOGUI_EXTRA_INCS})

# Add executable.
add_executable(gui_proto Main.cpp)

# Link against NanoGUI libraries.
target_link_libraries(myTarget nanogui ${NANOGUI_EXTRA_LIBS})