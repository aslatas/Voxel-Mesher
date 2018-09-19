/**
 * @headerfile Canvas.h ""
 * Contains Canvas class declaration.
 * @author Matthew McLaurin
 */

#pragma once

#include "Player.h"
#include "Shader.h"

#include <nanogui/glcanvas.h>

/**
 * @class Canvas
 * OpenGL drawing canvas. Provides utilties for custom drawing.
 */
class Canvas : public nanogui::GLCanvas
{
public:
    /**
     * @brief
     * Constructor initializes the canvas from a reference to its parent.
     * @param parent Pointer to the Widget that houses the Canvas.
     */
    Canvas(Widget* parent);

    /**
     * @brief
     * Called every update during drawing. Place custom
     * draw code here.
     */
    virtual void drawGL() override;

    /**
     * Gets the elapsed time between frames.
     * @return The time between the previous and current frame draws.
     */
    double deltaTime();

private:
    /**
     * @brief
     * Updates delta time for the next frame, using the current application
     * time.
     * @param currentFrameTime Current application frame time, in seconds.
     */
    void updateFrameDelta(double currentFrameTime);

    /** User camera. */
    Player* mPlayer;
    /** Trivial shader used to draw primitives in the scene. */
    Shader* mShader;
    /** Value returned by glfwGetTime() on the previous frame. */
    double lastFrameTime;
    /** Time that the last frame took to draw, in seconds. */
    double currentFrameDelta;
};