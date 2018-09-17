# Minimum version is the same as NanoGUI.
cmake_minimum_required (VERSION 3.10 FATAL_ERROR)

# Set project name.
project(voxelmesher)

# Option to build tests.
option(BUILD_TESTS "Build voxelmesher tests." ON)

# Set project sources.
set(VOXELMESHER_SOURCES
    src/VoxelMesher.cpp
    src/stb_image.cpp
    include/MatrixTypes.h
    include/Camera.h
    include/Canvas.h src/Canvas.cpp
    include/GLWindow.h src/GLWindow.cpp
    include/Shader.h
    include/VObject.h
    .clang-format)

# Set additional resources, like shaders.
set(VOXELMESHER_RESOURCES
    resources/shaders/Lights.frag
    resources/shaders/VertexColor.frag
    resources/shaders/VertexColor.vert)

if(BUILD_TESTS)
    set(VOXELMESHER_TESTS
        test/VObjectTest.cpp)
endif()

# Set library output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Set NanoGUI build settings.
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)

# Add NanoGUI as a subdirectory.
add_subdirectory(ext/nanogui)

# Make the NanoGUI targets into dependencies.
set_property(TARGET glfw glfw_objects nanogui PROPERTY FOLDER "dependencies")

# Add NanoGUI preprocessor definitions.
add_definitions(${NANOGUI_EXTRA_DEFS})

# Set include directories.
include_directories(
    include
    ext/nanogui/include
    ${NANOGUI_EXTRA_INCS})

# Add executable.
add_executable(voxelmesher ${VOXELMESHER_SOURCES} ${VOXELMESHER_RESOURCES})

# Put resources into their own folder in the IDE.
source_group("Resource Files" FILES ${VOXELMESHER_RESOURCES})

# Link against NanoGUI libraries.
target_link_libraries(voxelmesher nanogui ${NANOGUI_EXTRA_LIBS})

# Setup testing if option is enabled.
if(BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(ext/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    add_executable(voxelmesherTest ${VOXELMESHER_TESTS})
    target_link_libraries(voxelmesherTest gtest gtest_main)
    gtest_discover_tests(voxelmesherTest)
endif()

# Copy resources to the executable path on build.
add_custom_command(
    TARGET voxelmesher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
    COMMENT "copying resource files" VERBATIM)