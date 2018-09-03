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
        // Initialize the shader program and the vertex array object.
        if (!mShader.initFromFiles("VertexColor", "../resources/shaders/VertexColor.vert", "../resources/shaders/VertexColor.frag")) {
            std::cerr << "Failed to initialize shaders." << std::endl;
        }


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

        // Initialize transforms.
        projection = nanogui::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 10.0f);
        view = nanogui::lookAt(nanogui::Vector3f(0.0f, 0.0f, -2.0f), nanogui::Vector3f(0.0f, 0.0f, 0.0f), nanogui::Vector3f(0.0f, 1.0f, 0.0f));
        model.setIdentity();


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
     * Canvas render method draws to back-buffer.
     * Called per frame before swapping buffers. Put custom
     * draw logic here.
     */
    virtual void drawGL() override
    {
        // Use the Canvas' shader program. Includes vertex data.
        mShader.bind();
        
        // Update transform matrix and send it to the shader.
        mvp = projection * view * model;
        mShader.setUniform("modelViewProj", mvp);

        glEnable(GL_DEPTH_TEST);
        // Draw triangles, assuming the shader has vertex and index data.
        mShader.drawIndexed(GL_TRIANGLES, 0, 20);
        glDisable(GL_DEPTH_TEST);
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
        // If right click is pressed, update last position and return true.
        if (down && button == 1) {
            dragPos = p;
            return true;
        }

        // If input wasn't handled, return false.
        return false;
    }

    /**
     * Callback for mouse drag input. On right click and drag, rotates the canvas object.
     * @param p Cursor screen position.
     * @param rel Cursor release position.
     * @param button Mouse button responsible for the event.
     * @param modifiers Modifier keys held, such as ctrl or shift.
     * @return True if this event handled the input, false otherwise.
     */
    virtual bool mouseDragEvent(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override
    {
        // If right click is held, update model rotation.
        if (button == 2) {
            // Store rotation in a quaternion, using last mouse position and current mouse position to generate angle.
            Eigen::Quaternionf xRot, yRot;
            xRot = Eigen::Quaternionf(Eigen::AngleAxisf((dragPos.y() - p.y()) * sens, nanogui::Vector3f::UnitX()));
            yRot = Eigen::Quaternionf(Eigen::AngleAxisf((p.x() - dragPos.x()) * sens, nanogui::Vector3f::UnitY()));

            // Generate rotation matrix and apply it to the model matrix.
            nanogui::Matrix4f transform = nanogui::Matrix4f::Identity();
            transform.topLeftCorner<3, 3>() = (xRot * yRot).toRotationMatrix();
            model = transform * model;

            // Update last position and return true.
            dragPos = p;
            return true;
        }

        // If no input was handled, return false.
        return false;
    }

private:
    /** Canvas shader object. Contains both the shader program and VAOs.*/
    nanogui::GLShader mShader;
    /** Transform matrix for the rendered shape. */
    nanogui::Matrix4f mvp;
    /** Perspective projection matrix. */
    nanogui::Matrix4f projection;
    /** View matrix. */
    nanogui::Matrix4f view;
    /** Model matrix. */
    nanogui::Matrix4f model;
    /** Mouse sensitivity. */
    float sens = 0.01f;
    /** Last position of the mouse, used for mouse dragging. */
    Eigen::Vector2i dragPos = Eigen::Vector2i(0, 0);
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
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers)
    {
        // If the input was handled by the outer screen, do nothing.
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;

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
    virtual void draw(NVGcontext *ctx)
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
            nanogui::mainloop();
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
    return 0;
}
