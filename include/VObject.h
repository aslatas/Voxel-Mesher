
#pragma once

#include <nanogui/glutil.h>

class VObject
{
public:
    VObject::VObject()
    {
        mModel = nanogui::Matrix4f::Identity();
        mLocation = nanogui::Vector3f::Zero();
        mRotation = nanogui::Quaternionf::Identity();
        mScale = nanogui::Vector3f::Identity();
    }

    nanogui::Matrix4f VObject::model() { return mModel; }
    nanogui::Vector3f VObject::location() { return mLocation; }
    nanogui::Matrix4f VObject::rotation() { return mRotation.toRotationMatrix(); }
    nanogui::Vector3f VObject::forward() { return nanogui::Vector3f::UnitZ() * mRotation; }
    nanogui::Vector3f VObject::right() { return nanogui::Vector3f::UnitX() * mRotation; }
    nanogui::Vector3f VObject::up() { return nanogui::Vector3f::UnitY() * mRotation; }
    nanogui::Vector3f VObject::scale() { return mScale; }

    void VObject::setLocation(nanogui::Vector3f location)
    {
        mLocation = location;
        updateModel();
    }

    void VObject::setRotation(nanogui::Quaternionf rotation)
    {
        mRotation = rotation;
        updateModel();
    }

    void VObject::setRotation(float pitch, float yaw, float roll)
    {
        Eigen::AngleAxisf xRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitX());
        Eigen::AngleAxisf yRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitY());
        Eigen::AngleAxisf zRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitZ());

        mRotation = nanogui::Quaternionf(xRot * yRot * zRot);
        updateModel();
    }

    void VObject::setScale(nanogui::Vector3f scale)
    {
        mScale = scale;
        updateModel();
    }

    void VObject::translate(nanogui::Vector3f translation)
    {
        mLocation += translation;
        updateModel();
    }

    void VObject::rotate(nanogui::Quaternionf rotation)
    {
        mRotation *= rotation;
        updateModel();
    }

    void VObject::rotate(float pitch, float yaw, float roll)
    {
        Eigen::AngleAxisf xRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitX());
        Eigen::AngleAxisf yRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitY());
        Eigen::AngleAxisf zRot = Eigen::AngleAxisf(pitch, nanogui::Vector3f::UnitZ());

        mRotation *= (xRot * yRot * zRot);
        updateModel();
    }

private:
    nanogui::Matrix4f mModel;
    nanogui::Vector3f mLocation;
    nanogui::Quaternionf mRotation;
    nanogui::Vector3f mScale;

    void VObject::updateModel()
    {
        mModel = nanogui::scale(mScale) * mRotation.toRotationMatrix() * nanogui::translate(mLocation);
    }
};