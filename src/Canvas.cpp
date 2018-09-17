
#include "Canvas.h"
#include "Shader.h"
#include "GLWindow.h"

Canvas::Canvas(Widget* parent) : GLCanvas(parent)
{
    mCamera = new PerspectiveCamera(9.0f / 16.0f, 90.0f, 0.1f, 100.0f);

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

    mShader->setUniform("model", mCamera->model());
    mShader->setUniform("view", mCamera->view());
    mShader->setUniform("projection", mCamera->projection());
}

void Canvas::drawGL()
{
    mShader->bind();
    mCamera->rotate(0.05f, 0.0f, 0.0f);
    mCamera->setViewMatrix(mCamera->location(), mCamera->forward(), mCamera->up());
    mShader->setUniform("view", mCamera->view());
   // mShader->setUniform("model", mCamera->model());
    mShader->drawIndexed(GL_TRIANGLES, 0, 2);
}