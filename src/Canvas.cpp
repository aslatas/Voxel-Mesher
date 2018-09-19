
#include "Canvas.h"

Canvas::Canvas(Widget* parent) : GLCanvas(parent)
{
    mPlayer = new Player();
    mShader = new Shader();
    if (!mShader->initFromFiles("Trivial",
            &std::string("../resources/shaders/Trivial.vert"),
            &std::string("../resources/shaders/Trivial.frag"), &std::string(),
            1, 1, 0)) {
        std::cerr << "Failed to initialize shaders." << std::endl;
    }
    MatXf vertices(3, 4);
    vertices.col(0) << -1.0f, -1.0f, 3.0f;
    vertices.col(1) << -1.0f, 1.0f, 3.0f;
    vertices.col(2) << 1.0f, 1.0f, 3.0f;
    vertices.col(3) << 1.0f, -1.0f, 3.0f;

    MatXi indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    mShader->bind();

    mShader->uploadAttrib("position", vertices);
    mShader->uploadIndices(indices);

    mShader->setUniform("model", (Mat4f)Mat4f::Identity());
    mShader->setUniform("view", mPlayer->view());
    mShader->setUniform("projection", mPlayer->projection());

    lastFrameTime = 0.0;
    currentFrameDelta = 0.0;
    glfwSetTime(0.0);
}

void Canvas::drawGL()
{
    mShader->bind();
    mPlayer->addMovementInput(mPlayer->right(), 1.0f);
    mPlayer->applyMovement(deltaTime());
    mShader->setUniform("view", mPlayer->view());
    mShader->drawIndexed(GL_TRIANGLES, 0, 2);

    updateFrameDelta(glfwGetTime());
}

double Canvas::deltaTime()
{
    return currentFrameDelta;
}

void Canvas::updateFrameDelta(double currentFrameTime)
{
    currentFrameDelta = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
}