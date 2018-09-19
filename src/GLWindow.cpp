
#include "GLWindow.h"

#include "Canvas.h"
#include "MatrixTypes.h"

GLWindow::GLWindow(int width, int height, std::string title)
    : nanogui::Screen(
          Vec2i(width, height), title, false, false, 8, 8, 24, 8, 2, 4, 1)
{
    // Add a GL canvas to the window, with background color and size.
    mCanvas = new Canvas(this);
    mCanvas->setBackgroundColor({51, 51, 51, 255});
    mCanvas->setSize(Vec2i(width, height));

    performLayout();
}

bool GLWindow::keyboardEvent(int key, int scancode, int action, int modifiers)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return true;
    }

    return false;
}

void GLWindow::draw(NVGcontext* ctx)
{
    // Draw parent screen.
    Screen::draw(ctx);
}