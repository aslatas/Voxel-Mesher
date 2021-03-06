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

#include "Shader.h"

// For logging.
#include <iostream>
#include <string>
#include <stdio.h>

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
     * Constructor creates the GL Canvas in the specified parent Widget.
     * Creates vertex and index buffers and shaders.
     * @param parent Parent Widget in which the canvas will be constructed.
     * @TODO Move shaders into separate files. Look into serializing them.
     * @TODO Refactor shader inputs as a uniform buffer object.
     */
    Canvas(Widget *parent) : nanogui::GLCanvas(parent)
    {
        std::string vertexPath[1] = { "../resources/shaders/VertexColor.vert" };
        std::string fragmentPath[2] = { "../resources/shaders/Lights.frag", "../resources/shaders/VertexColor.frag" };
        // Read shaders from file. Fragment shader is divided into two files.
        if (!shader.initFromFiles("VertexColor", vertexPath, fragmentPath, nullptr, 1, 2, 0)) {
            std::cerr << "Failed to initialize shaders." << std::endl;
        }

        arcball.setSize(size());
        // Index buffer. Counter-clockwise winding order.
        nanogui::MatrixXu indices(3, 12);
        indices.col(0) << 0, 2, 1;
        indices.col(1) << 3, 1, 2;
        indices.col(2) << 0, 4, 2;
        indices.col(3) << 6, 2, 4;
        indices.col(4) << 3, 2, 6;
        indices.col(5) << 7, 3, 6;
        indices.col(6) << 1, 3, 7;
        indices.col(7) << 5, 1, 7;
        indices.col(8) << 0, 1, 5;
        indices.col(9) << 4, 0, 5;
        indices.col(10) << 4, 5, 6;
        indices.col(11) << 7, 6, 5;

        // Vertex locations of a cube.
        nanogui::MatrixXf positions(3, 8);
        positions.col(0) << -0.5f,  0.5f,  0.5f;
        positions.col(1) <<  0.5f,  0.5f,  0.5f;
        positions.col(2) << -0.5f, -0.5f,  0.5f;
        positions.col(3) <<  0.5f, -0.5f,  0.5f;
        positions.col(4) << -0.5f,  0.5f, -0.5f;
        positions.col(5) <<  0.5f,  0.5f, -0.5f;
        positions.col(6) << -0.5f, -0.5f, -0.5f;
        positions.col(7) <<  0.5f, -0.5f, -0.5f;

        // Transforms of the instanced cubes.
        nanogui::MatrixXf transforms(3, 8);
        transforms.col(0) << 0.0f, 0.0f, 0.0f;
        transforms.col(1) << 0.0f, 0.0f, 1.0f;
        transforms.col(2) << 0.0f, 1.0f, 0.0f;
        transforms.col(3) << 0.0f, 1.0f, 1.0f;
        transforms.col(4) << 1.0f, 0.0f, 0.0f;
        transforms.col(5) << 1.0f, 0.0f, 1.0f;
        transforms.col(6) << 1.0f, 1.0f, 0.0f;
        transforms.col(7) << 1.0f, 1.0f, 1.0f;




        nanogui::MatrixXf normals(3, 8);
        for (int i = 0; i < positions.cols(); i++) {
            normals.col(i) = positions.col(i).normalized();
        }

        // Use the shader program, sending vertex and uniform data.
        shader.bind();
        shader.uploadIndices(indices);
        shader.uploadAttrib("vertexPosition", positions);
        shader.uploadAttrib("vertexNormal", normals);
      //  shader.uploadAttrib("cubePosition", transforms);
       // glVertexAttribDivisor(shader.attrib("cubePosition", true), 1);

        shader.setUniform("material.ambient", nanogui::Vector3f(1.0f, 0.5f, 0.31f));
        shader.setUniform("material.diffuse", nanogui::Vector3f(1.0f, 0.5f, 0.31f));
        shader.setUniform("material.specular", nanogui::Vector3f(0.5f, 0.5f, 0.5f));
        shader.setUniform("material.shininess", 32.0f);

        shader.setUniform("viewPosition", cameraPosition);
        shader.setUniform("numPointLights", 0);

        shader.setUniform("pointLights[0].position", nanogui::Vector3f(2.0f, 0.0f, 0.0f));
        shader.setUniform("pointLights[0].ambient", nanogui::Vector3f(0.2f, 0.2f, 0.2f));
        shader.setUniform("pointLights[0].diffuse", nanogui::Vector3f(0.5f, 0.5, 0.5f));
        shader.setUniform("pointLights[0].specular", nanogui::Vector3f(1.0f, 1.0f, 1.0f));
        shader.setUniform("pointLights[0].constant", 1.0f);
        shader.setUniform("pointLights[0].linear", 0.14f);
        shader.setUniform("pointLights[0].quadratic", 0.07f);

        shader.setUniform("directionalLight.direction", nanogui::Vector3f(1.0f, -1.0f, 1.0f));
        shader.setUniform("directionalLight.ambient", nanogui::Vector3f(0.2f, 0.2f, 0.2f));
        shader.setUniform("directionalLight.diffuse", nanogui::Vector3f(0.5f, 0.5f, 0.5f));
        shader.setUniform("directionalLight.specular", nanogui::Vector3f(1.0f, 1.0f, 1.0f));

        // Initialize transforms.
        projection = nanogui::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);
        view = nanogui::lookAt(cameraPosition, cameraPosition + cameraDirection, nanogui::Vector3f(0.0f, 1.0f, 0.0f));
        model.setIdentity();
        rot = model;

        lastFrameTime = glfwGetTime();
    }

    /**
     * Destructor frees shaders. NanoGUI will automatically free GLFW resources
     * if the container is destroyed.
     */
    ~Canvas()
    {
        shader.free();
    }

    /**
     * Canvas render method draws to back-buffer.
     * Called per frame before swapping buffers. Put custom
     * draw logic here.
     */
    virtual void drawGL() override
    {
        nanogui::Vector3f cameraForward = nanogui::Vector3f(0.0f, 0.0f, 1.0f);
        nanogui::Vector3f cameraRight = nanogui::Vector3f(1.0f, 0.0f, 0.0f);
        float sensitivity = 1.0f;

        if (glfwGetKey(screen()->glfwWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraForward * sensitivity * deltaTime();
        if (glfwGetKey(screen()->glfwWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraForward * sensitivity * deltaTime();
        if (glfwGetKey(screen()->glfwWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition += cameraRight * sensitivity * deltaTime();
        if (glfwGetKey(screen()->glfwWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition -= cameraRight * sensitivity * deltaTime();

        view = nanogui::lookAt(cameraPosition, cameraPosition + cameraDirection, nanogui::Vector3f(0.0f, 1.0f, 0.0f));
        
        // Use the Canvas' shader program. Includes vertex data.
        shader.bind();

        // Send transform data to the shader.
        shader.setUniform("viewPosition", cameraPosition);
        shader.setUniform("model", rot);
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);

        // Draw triangles, assuming the shader has vertex and index data.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        shader.drawElementsInstanced(GL_TRIANGLES, 0, 12, 4096);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        double currentFrameTime = glfwGetTime();
        mDeltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
    }

    /**
     * Callback for mouse click input. Updates initial location for drag events upon right click.
     * @param p Cursor screen position.
     * @param button Mouse button responsible for the event.
     * @param down True if the event represents a press, false for a release.
     * @param modifiers Held modifer keys, such as ctrl or shift.
     * @return True if this event handled the mouse input, false otherwise.
     */
    virtual bool mouseButtonEvent(const nanogui::Vector2i &p, int button, bool down, int modifiers) override
    {
        // If right click is pressed, rotate the object.
        if (button == GLFW_MOUSE_BUTTON_1) {
            nanogui::Vector2i transformedP = nanogui::Vector2i(p.x() - position().x(), p.y() - position().y() + 200);
            arcball.button(transformedP, down);
            return true;
        }
        // If input wasn't handled, return false.
        return false;
    }

    virtual bool mouseMotionEvent(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override
    {
        
        if (button == GLFW_MOUSE_BUTTON_2) {
            nanogui::Vector2i transformedP = nanogui::Vector2i(p - position());
            nanogui::Vector2i invertedP = nanogui::Vector2i(transformedP.x(), -transformedP.y() + 200);
            arcball.motion(invertedP);
            rot = model * arcball.matrix();
            return true;
        }

        return false;
    }

    /**
     * Gets the frame render time.
     */
    double deltaTime() {
        return mDeltaTime;
    }

private:
    /** Canvas shader object. Contains both the shader program and VAOs.*/
    Shader shader;  
    /** Transform matrix for the rendered shape. */
    nanogui::Matrix4f mvp;
    /** Perspective projection matrix. */
    nanogui::Matrix4f projection;
    /** View matrix. */
    nanogui::Matrix4f view;
    /** Model matrix. */
    nanogui::Matrix4f model;
    /** Temporary arcball rotation matrix. */
    nanogui::Matrix4f rot = nanogui::Matrix4f::Identity();
    /** Camera position. */
    nanogui::Vector3f cameraPosition = nanogui::Vector3f(0.0f, 0.0f, -16.0f);
    /** Camera rotation. */
    nanogui::Vector3f cameraDirection = nanogui::Vector3f(0.0f, 0.0f, 1.0f);
    /** Per-frame movement vector. */
    nanogui::Vector3f movement = nanogui::Vector3f(0.0f, 0.0f, 0.0f);
    /** Mouse sensitivity. */
    float sens = 0.01f;
    /** Mouse rotation arcball. */
    nanogui::Arcball arcball;
    /** Last position of the mouse, used for mouse dragging. */
    Eigen::Vector2i dragPos = Eigen::Vector2i(0, 0);
    /** Frame time. */
    double mDeltaTime;
    /** Time of last frame. */
    double lastFrameTime;
};

/**
 * @class GuiApp
 * Wrapper for NanoGui Screen class. Creates the GLFW window with default
 * parameters. Adds top level UI widgets and redirects input.
 */
class GuiApp : public nanogui::Screen
{
public:
    /**
     * Constructor creates window and sets base behavior. Uses 8 bit color,
     * depth and stencil buffered. Screen is windowed and non-resizeable.
     */
    GuiApp() : nanogui::Screen(Eigen::Vector2i(800, 600), "GUI Prototype",
        false, false, 8, 8, 24, 8, 2, 4, 1)
    {
        // Create a UI window within the screen, with a title and layout.
        nanogui::Window *window = new nanogui::Window(this, "I'm a Canvas!");
        window->setLayout(new nanogui::GroupLayout());

        // Add a GL canvas to the window, with background color and size.
        mCanvas = new Canvas(window);
        mCanvas->setBackgroundColor({ 60, 60, 60, 255 });
        mCanvas->setSize({ 400, 400 });

        // Make an empty box layout widget to position the canvas and buttons.
        nanogui::Widget *tools = new nanogui::Widget(window);
        tools->setLayout(new nanogui::BoxLayout(
            nanogui::Orientation::Horizontal,
            nanogui::Alignment::Middle, 0, 5));

        // Add color button, which randomizes background color on press.
        nanogui::Button *b0 = new nanogui::Button(tools, "Random Color");
        b0->setCallback([this]() { mCanvas->setBackgroundColor(nanogui::Vector4i(rand() % 256, rand() % 256, rand() % 256, 255)); });

        // Add rotate button, which randomizes current rotation on press.
        nanogui::Button *b1 = new nanogui::Button(tools, "Do Nothing");
        window->center();

        // Lay out the UI elements.
        performLayout();
    }

    /**
     * Callback for keyboard input. Sets the screen to invisible if ESC
     * is pressed.
     * @param key Hardware code for the key input.
     * @param scancode Platform specific interpretation of the keycode.
     * @param action Type of input, such as press or release.
     * @param modifiers Held modifier keys, such as shift or ctrl.
     * @return True if input has been handled. Used for layered input handling.
     */
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override
    {
        // If the input was handled by the outer screen, do nothing.
        if (Screen::keyboardEvent(key, scancode, action, modifiers)) {
            return true;
        }

        // If the ESC key was pressed, close the app.
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }

        // If input went unhandled, return false.
        return false;
    }

    /**
     * Draws the screen in its current state.
     * @param ctx NanoGUI context to use for drawing.
     */
    virtual void draw(NVGcontext *ctx) override
    {
        // Draw parent screen.
        Screen::draw(ctx);
    }


private:
    /** GL Canvas to use for the 3D viewport. */
    Canvas * mCanvas;

};

/**
 * Application launch point. Initializes components, creates the window, and
 * begins the draw loop. Initiates shutdown once the application loop ends.
 * @param argc Number of command line arguments..
 * @param argv Command line arguments, unused.
 * @return Exit code, zero on success.
 */
int main(int argc, char ** argv)
{
    // Try to initialize nanogui and subcomponents.
    try {
        nanogui::init();

        // If successful, set scoped variables, draw UI, and start main loop.
        {
            nanogui::ref<GuiApp> app = new GuiApp();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop(16);
        }

        // After loop completes, shutdown NanoGUI.
        nanogui::shutdown();
    }

    // If NanoGUI failed to initialize, print stacktrace and exit in error.
    catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    // If program completed and NanoGUI shut down, exit in success.
    return EXIT_SUCCESS;
}
