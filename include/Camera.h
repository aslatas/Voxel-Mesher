#pragma once

#include "VObject.h"

class Camera : public VObject
{
public:
    Mat4f Camera::view() { return mView; }
    Mat4f Camera::projection() { return mProjection; }
    void  Camera::updateViewMatrix()
    {
        mView = nanogui::lookAt(location(), location() + forward(), up());
    }

protected:
    Camera::Camera() : VObject()
    {
        updateViewMatrix();
    }

    Mat4f mView;
    Mat4f mProjection;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera::PerspectiveCamera(
        float aspect, float fov, float near, float far)
        : Camera()
    {
        float halfWidth = near * tan(nvgDegToRad(fov / 2.0f));
        mProjection = nanogui::frustum(-halfWidth, halfWidth,
            -halfWidth * aspect, halfWidth * aspect, near, far);
    }
};

class OrthoCamera : Camera
{
    OrthoCamera::OrthoCamera(
        float near, float far, float width, float height)
        : Camera()
    {
        mProjection = nanogui::ortho(-width / 2.0f, width / 2.0f,
            -height / 2.0f, height / 2.0f, near, far);
    }
};
