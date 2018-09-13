#pragma once

#include <nanogui/glcanvas.h>

class Canvas : nanogui::GLCanvas
{
public:
    Canvas(Widget *parent);

    ~Canvas();

    virtual void drawGL() override;

    virtual bool mouseButtonEvent(const nanogui::Vector2i &p, int button, bool down, int modifiers) override;
    virtual bool mouseMotionEvent(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;
};