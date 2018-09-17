/**
 * @headerfile GLWindow.h ""
 * Contains GLWindow class declaration.
 * @author Matthew McLaurin
 */

#pragma once

#include <nanogui/screen.h>

#include "Canvas.h"

/**
 * @class GLWindow
 * Wrapper for nanogui Screen class, represents the main application window.
 * Contains a Canvas which fills the main window.
 * Initializes nanogui and by extension GLFW, and passes user input to relevant
 * widgets if not handled by the main window.
 */
class GLWindow : public nanogui::Screen
{
  public:
    /**
     * Constructor creates window and sets base behavior. Uses 8 bit color,
     * depth and stencil buffered. Screen is windowed and non-resizeable.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     */
    GLWindow(int width, int height, std::string title);

    /**
     * Callback for keyboard input.
     * @param key OS interpreted key value.
     * @param scancode Hardware scancode for the key.
     * @param action Type of action, such as press, release, or hold.
     * @param modifiers Modifer keys held, such as ctrl or shift.
     */
    virtual bool keyboardEvent(
        int key, int scancode, int action, int modifiers) override;

    /**
     * Called per frame to draw the window contents.
     * @param ctx NanoGUI drawing context.
     */
    virtual void draw(NVGcontext* ctx) override;

    /**
     * Gets the elapsed time between frames.
     * @return The time between the previous and current frame draws.
     */
    double DeltaTime();

  private:
    /** GL Canvas to use for the 3D viewport. */
    Canvas* mCanvas;
    /** Value returned by glfwGetTime() on the previous frame. */
    double lastFrameTime;
    /** Time that the last frame took to draw, in seconds. */
    double deltaTime;
};