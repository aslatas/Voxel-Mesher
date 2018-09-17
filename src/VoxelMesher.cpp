/**
 * @file VoxelMesher.cpp
 * Contains application launch point. Initializes subcomponents and starts the
 * program loop. 
 * @author Matthew McLaurin
 */

#include "GLWindow.h"

#include <stdio.h>

/**
 * Application launch point. Initializes components, creates the window, and
 * begins the draw loop. Initiates shutdown once the application loop ends.
 * @param argc Number of command line arguments.
 * @param argv Command line arguments, unused.
 * @return Exit code, zero on success.
 */
int main(int argc, char** argv)
{
    // Try to initialize nanogui and subcomponents.
    try {
        nanogui::init();
        // If successful, set scoped variables, draw UI, and start main loop.

        nanogui::ref<GLWindow> app =
            new GLWindow(1600, 900, "VoxelMesher 0.01");
        app->drawAll();
        app->setVisible(true);
        glfwSetTime(0.0);
        nanogui::mainloop(16);

        // After loop completes, shutdown NanoGUI.
        nanogui::shutdown();
    }

    // If NanoGUI failed to initialize, print stacktrace and exit in error.
    catch (const std::runtime_error& e) {
        std::string error_msg =
            std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    // If program completed and NanoGUI shut down, exit in success.
    return EXIT_SUCCESS;
}
