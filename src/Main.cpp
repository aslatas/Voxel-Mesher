/**
 * @file Main.cpp
 * Rudimentary GUI application based on the NanoGUI Example 4 source code.
 * Copyright information for the NanoGUI source can be found in the NanoGUI
 * LICENSE.txt file.
 */

#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/glcanvas.h>

#include <iostream>
#include <string>

// Includes for the GLTexture class.
#include <cstdint>
#include <memory>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget *parent) : nanogui::GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
        mShader.init(
            /* An identifying name */
            "a_simple_shader",

            /* Vertex shader */
            "#version 410\n"
            "uniform mat4 modelViewProj;\n"
            "in vec3 position;\n"
            "in vec3 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "    frag_color = vec4(color, 1.0);\n"
            "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "#version 410\n"
            "out vec4 color;\n"
            "in vec4 frag_color;\n"
            "void main() {\n"
            "    color = frag_color;\n"
            "}"
        );

        nanogui::MatrixXu indices(3, 20);
        indices.col( 0) <<  0,  2,  1;
        indices.col( 1) <<  0,  3,  2;
        indices.col( 2) <<  0,  4,  3;
        indices.col( 3) <<  0,  5,  4;
        indices.col( 4) <<  0,  1,  5;
        indices.col( 5) <<  6,  3,  4;
        indices.col( 6) <<  6,  4,  7;
        indices.col( 7) <<  7,  4,  5;
        indices.col( 8) <<  7,  5,  8;
        indices.col( 9) <<  8,  5,  1;
        indices.col(10) <<  8,  1,  9;
        indices.col(11) <<  9,  1,  2;
        indices.col(12) <<  9,  2, 10;
        indices.col(13) << 10,  2,  3;
        indices.col(14) << 10,  3,  6;
        indices.col(15) << 11,  6,  7;
        indices.col(16) << 11,  7,  8;
        indices.col(17) << 11,  8,  9;
        indices.col(18) << 11,  9, 10;
        indices.col(19) << 11, 10,  6;

        nanogui::MatrixXf positions(3, 12);
        positions.col( 0) <<       0.0f,       1.0f,       0.0f;
        positions.col( 1) <<       0.0f,  0.447215f, -0.894414f;
        positions.col( 2) <<  0.850649f,  0.447215f, -0.276393f;
        positions.col( 3) <<   0.52572f,  0.447215f,  0.723599f;
        positions.col( 4) << -0.525728f,  0.447215f,  0.723594f;
        positions.col( 5) << -0.850641f,  0.447215f, -0.276385f;
        positions.col( 6) <<       0.0f, -0.447215f,  0.894414f;
        positions.col( 7) << -0.850649f, -0.447215f,  0.276393f;
        positions.col( 8) <<  -0.52572f, -0.447215f, -0.723599f;
        positions.col( 9) <<  0.525728f, -0.447215f, -0.723594f;
        positions.col(10) <<  0.850641f, -0.447215f,  0.276385f;
        positions.col(11) <<       0.0f,      -1.0f,       0.0f;

        nanogui::MatrixXf colors(3, 12);
        colors.col( 0) <<    1,    0,    0;
        colors.col( 1) <<    0,    1,    0;
        colors.col( 2) <<    0,    1,    1;
        colors.col( 3) << 0.5f, 0.5f,    0;
        colors.col( 4) <<    0, 0.5f, 0.5f;
        colors.col( 5) << 0.5f,    0, 0.5f;
        colors.col( 6) << 0.5f,    0, 0.5f;
        colors.col( 7) <<    0, 0.5f, 0.5f;
        colors.col( 8) << 0.5f, 0.5f,    0;
        colors.col( 9) <<    0,    0,    1;
        colors.col(10) <<    0,    1,    0;
        colors.col(11) <<    1,    0,    0;

        mShader.bind();
        mShader.uploadIndices(indices);

        mShader.uploadAttrib("position", positions);
        mShader.uploadAttrib("color", colors);
    }

    ~MyGLCanvas() {
        mShader.free();
    }

    void setRotation(nanogui::Vector3f vRotation) {
        mRotation = vRotation;
    }

    virtual void drawGL() override {

        mShader.bind();

        nanogui::Matrix4f mvp;
        mvp.setIdentity();
        float fTime = (float)glfwGetTime();
        mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0]*fTime, nanogui::Vector3f::UnitX()) *
                                                   Eigen::AngleAxisf(mRotation[1]*fTime,  nanogui::Vector3f::UnitY()) *
                                                   Eigen::AngleAxisf(mRotation[2]*fTime, nanogui::Vector3f::UnitZ())) * 0.25f;

        mShader.setUniform("modelViewProj", mvp);

        glEnable(GL_DEPTH_TEST);
        /* Draw 12 triangles starting at index 0 */
        mShader.drawIndexed(GL_TRIANGLES, 0, 20);
        glDisable(GL_DEPTH_TEST);
    }

private:
    nanogui::GLShader mShader;
    Eigen::Vector3f mRotation;
};


class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication() : nanogui::Screen(Eigen::Vector2i(800, 600), "GUI Prototype", false, false, 8, 8, 24, 8, 2, 4, 1) {
        using namespace nanogui;

        Window *window = new Window(this, "I'm a Canvas!");
        window->setLayout(new GroupLayout());

        mCanvas = new MyGLCanvas(window);
        mCanvas->setBackgroundColor({60, 60, 60, 255});
        mCanvas->setSize({400, 400});

        Widget *tools = new Widget(window);
        tools->setLayout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 5));

        Button *b0 = new Button(tools, "Random Color");
        b0->setCallback([this]() { mCanvas->setBackgroundColor(Vector4i(rand() % 256, rand() % 256, rand() % 256, 255)); });

        Button *b1 = new Button(tools, "Random Rotation");
        b1->setCallback([this]() { mCanvas->setRotation(nanogui::Vector3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f)); });
        window->center();

        performLayout();
    }

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx) {
        /* Draw the user interface */
        Screen::draw(ctx);
    }
private:
    MyGLCanvas *mCanvas;
};

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    return 0;
}
