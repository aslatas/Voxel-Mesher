#pragma once

#include "VObject.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Camera : public VObject
{
public:
    nanogui::Matrix4f Camera::view() { return mView; }
    nanogui::Matrix4f Camera::projection() { return mProjection; }
    void Camera::setViewMatrix(nanogui::Vector3f location, nanogui::Vector3f target, nanogui::Vector3f up)
    {
        mView = nanogui::lookAt(location, target, up);
    }

protected:
    Camera::Camera() { mView = nanogui::lookAt(location(), location() + forward(), up()); }

    nanogui::Matrix4f mView;
    nanogui::Matrix4f mProjection;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera::PerspectiveCamera(float aspect, float fov, float near, float far) : Camera()
    {
        float halfWidth = 2 * near * tan(fov * M_PI / 180.0f);
        mProjection = nanogui::frustum(-halfWidth, halfWidth, -halfWidth / aspect, halfWidth / aspect, near, far);
        mView = nanogui::lookAt(location(), location() + forward(), up());
    }
};

class OrthoCamera : Camera
{
    OrthoCamera::OrthoCamera(float near, float far, float width, float height) : Camera()
    {
        mProjection = nanogui::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, near, far);
    }
};
