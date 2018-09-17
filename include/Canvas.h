/**
 * @headerfile Canvas.h ""
 * Contains Canvas class declaration.
 * @author Matthew McLaurin
 */

#pragma once

#include <nanogui/glcanvas.h>
#include "Camera.h"
#include "Shader.h"

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

private:
    Camera* mCamera;
    Shader* mShader;
};