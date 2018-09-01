/**
 * @file Main.cpp
 * Rudimentary GUI application based on the NanoGUI Example 4 source code.
 * Copyright information for the NanoGUI source can be found in the NanoGui
 * LICENSE.txt file. This application is distributed under a similar BSD style
 * license. More information is available in the top level LICENSE.txt file.
 *
 * Draws an icosahedron inside a NanoGUI canvas. Has buttons to randomize 
 * rotation and background color of the canvas. 
 * @author Matthew McLaurin
 * @TODO Create a struct to house vertex information.
 * @TODO SceneObject class which has modifiable world transform.
 * @TODO Custom free rotating perspective camera.
 */

// NanoGUI includes.
#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include <nanogui/entypo.h>
#include <nanogui/colorwheel.h>
#include <nanogui/glcanvas.h>

// For logging.
#include <iostream>
#include <string>

// @TODO Refactor Texture class to use safer file read (just for fun).
// Includes for the GLTexture class. 
#include <cstdint>
#include <memory>
#include <utility>

// Must be defined *ONLY* once per application.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/**
 * @class Canvas
 * Wrapper for NanoGui's GLCanvas. Keeps track of the Model-View-Projection
 * matrix of its contents.
 */
class Canvas : public nanogui::GLCanvas
{
public:

    /**
     * Constructor creates the GL Canvas in the specified parent Widget, with
     * optional initial rotation. Creates vertex and index buffers and shaders.
     * @param parent Parent Widget in which the canvas will be constructed.
     * @TODO Move shaders into separate files. Look into serializing them.
     * @TODO Refactor shader inputs as a uniform buffer object.
     */
    Canvas(Widget *parent) : nanogui::GLCanvas(parent),
        mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f))
    {
        // Initialize the shader program and the vertex array object.
        mShader.init(
            // Shader Program identifier.
            "VertexColorShader",

            /* Vertex shader for GL 4.1.
             * Takes in a vertex position, transform matrix, and color. Sets
             * position using matrix passes color the fragment shader.
             */
            "#version 410\n"
            "uniform mat4 modelViewProj;\n"
            "in vec3 position;\n"
            "in vec3 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "    frag_color = vec4(color, 1.0);\n"
            "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader for GL 4.1.
             * Takes a color from the vertex shader, uses it as fragment color.
             */
            "#version 410\n"
            "out vec4 color;\n"
            "in vec4 frag_color;\n"
            "void main() {\n"
            "    color = frag_color;\n"
            "}"
        );

        // Index buffer. Counter-clockwise winding order.
        nanogui::MatrixXu indices(3, 20);
        indices.col(0) << 0, 2, 1;
        indices.col(1) << 0, 3, 2;
        indices.col(2) << 0, 4, 3;
        indices.col(3) << 0, 5, 4;
        indices.col(4) << 0, 1, 5;
        indices.col(5) << 6, 3, 4;
        indices.col(6) << 6, 4, 7;
        indices.col(7) << 7, 4, 5;
        indices.col(8) << 7, 5, 8;
        indices.col(9) << 8, 5, 1;
        indices.col(10) << 8, 1, 9;
        indices.col(11) << 9, 1, 2;
        indices.col(12) << 9, 2, 10;
        indices.col(13) << 10, 2, 3;
        indices.col(14) << 10, 3, 6;
        indices.col(15) << 11, 6, 7;
        indices.col(16) << 11, 7, 8;
        indices.col(17) << 11, 8, 9;
        indices.col(18) << 11, 9, 10;
        indices.col(19) << 11, 10, 6;

        // Vertex locations, generated with Blender.
        nanogui::MatrixXf positions(3, 12);
        positions.col(0) << 0.0f, 1.0f, 0.0f;
        positions.col(1) << 0.0f, 0.447215f, -0.894414f;
        positions.col(2) << 0.850649f, 0.447215f, -0.276393f;
        positions.col(3) << 0.52572f, 0.447215f, 0.723599f;
        positions.col(4) << -0.525728f, 0.447215f, 0.723594f;
        positions.col(5) << -0.850641f, 0.447215f, -0.276385f;
        positions.col(6) << 0.0f, -0.447215f, 0.894414f;
        positions.col(7) << -0.850649f, -0.447215f, 0.276393f;
        positions.col(8) << -0.52572f, -0.447215f, -0.723599f;
        positions.col(9) << 0.525728f, -0.447215f, -0.723594f;
        positions.col(10) << 0.850641f, -0.447215f, 0.276385f;
        positions.col(11) << 0.0f, -1.0f, 0.0f;

        // Vertex colors, pretty arbitrary.
        nanogui::MatrixXf colors(3, 12);
        colors.col(0) << 1, 0, 0;
        colors.col(1) << 0, 1, 0;
        colors.col(2) << 0, 1, 1;
        colors.col(3) << 0.5f, 0.5f, 0;
        colors.col(4) << 0, 0.5f, 0.5f;
        colors.col(5) << 0.5f, 0, 0.5f;
        colors.col(6) << 0.5f, 0, 0.5f;
        colors.col(7) << 0, 0.5f, 0.5f;
        colors.col(8) << 0.5f, 0.5f, 0;
        colors.col(9) << 0, 0, 1;
        colors.col(10) << 0, 1, 0;
        colors.col(11) << 1, 0, 0;

        // Use the shader program, and send vertex data buffers.
        mShader.bind();
        mShader.uploadIndices(indices);
        mShader.uploadAttrib("position", positions);
        mShader.uploadAttrib("color", colors);
    }

    /**
     * Destructor frees shaders. NanoGUI will automatically free GLFW resources
     * if the container is destroyed.
     */
    ~Canvas()
    {
        mShader.free();
    }

    /**
     * Set the per-second rotation transform as euler angles.
     * @param vRotation Euler-angle rotation rate, in radians/second.
     */
    void setRotation(nanogui::Vector3f vRotation)
    {
        mRotation = vRotation;
    }

    /**
     * Canvas render method draws to back-buffer.
     * Called per frame before swapping buffers. Put custom
     * draw logic here.
     */
    virtual void drawGL() override
    {
        // Use the Canvas' shader program. Includes vertex data.
        mShader.bind();

        // Initialize model-view-projectoin matrix to identity.
        nanogui::Matrix4f mvp;
        mvp.setIdentity();

        // Get current frame time.
        float fTime = (float)glfwGetTime();
        // Rotate by current frame rotation amount, concatenating three single-axis rotations.
        mvp.topLeftCorner<3, 3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0] * fTime, nanogui::Vector3f::UnitX()) *
            Eigen::AngleAxisf(mRotation[1] * fTime, nanogui::Vector3f::UnitY()) *
            Eigen::AngleAxisf(mRotation[2] * fTime, nanogui::Vector3f::UnitZ())) * 0.25f;

        // Send transform matrix to the shader program.
        mShader.setUniform("modelViewProj", mvp);

        // Enable depth testing of the canvas objects.
        glEnable(GL_DEPTH_TEST);
        // Draw triangles, assuming the shader has vertex and index data.
        mShader.drawIndexed(GL_TRIANGLES, 0, 20);
        glDisable(GL_DEPTH_TEST);
    }

private:
    /** Canvas shader object. Contains both the shader program and VAOs.*/
    nanogui::GLShader mShader;
    /**  Transform rotation rate as Euler angles. */
    Eigen::Vector3f mRotation;
};

/**
 *
 *
 *
 */
class ExampleApplication : public nanogui::Screen
{
public:
    ExampleApplication() : nanogui::Screen(Eigen::Vector2i(800, 600), "GUI Prototype", false, false, 8, 8, 24, 8, 2, 4, 1)
    {
        using namespace nanogui;

        Window *window = new Window(this, "I'm a Canvas!");
        window->setLayout(new GroupLayout());

        mCanvas = new Canvas(window);
        mCanvas->setBackgroundColor({ 60, 60, 60, 255 });
        mCanvas->setSize({ 400, 400 });

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

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers)
    {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx)
    {
        /* Draw the user interface */
        Screen::draw(ctx);
    }
private:
    Canvas *mCanvas;
};

int main(int /* argc */, char ** /* argv */)
{
    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    }
    catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    return 0;
}
