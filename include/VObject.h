
/**
 * @headerfile VObject.h ""
 * Contains VObject class declaration and definition.
 * Provides base utilities for saving and applying transformations.
 * @author Matthew McLaurin
 */

#pragma once

#include "MatrixTypes.h"
#include <nanogui/glutil.h>

/**
 * @class VObject
 * Represents the base for an object in the scene. Provides functionality for
 * affine transformations.
 */
class VObject
{
public:
    /**
     * @brief
     * Default constructor initializes object at world origin, with identity
     * scale and rotation.
     */
    VObject::VObject()
    {
        mModel = Mat4f::Identity();
        mLocation = Vec3f::Zero();
        mRotation = Quatf::Identity();
        mScale = Vec3f::Ones();
    }

    /**
     * @brief
     * Gets the object model (world) transform matrix.
     * @return The object world transform.
     */
    Mat4f VObject::model() { return mModel; }

    /**
     * @brief
     * Gets the object world location.
     * @return The object world location.
     */
    Vec3f VObject::location() { return mLocation; }

    /**
     * @brief
     * Gets the object world rotation matrix.
     * @return The object rotation matrix.
     */
    Mat4f VObject::rotation()
    {
        Mat4f rot = Mat4f::Identity();
        rot.topLeftCorner<3, 3>() = mRotation.toRotationMatrix();
        return rot;
    }

    /**
     * @brief
     * Gets the object scale along each local axis.
     * @return The object scale.
     */
    Vec3f VObject::scale() { return mScale; }
    /**
     * @brief
     * Gets the object forward (direction) vector.
     * @return The normalized forward vector.
     */
    Vec3f VObject::forward() { return mRotation * Vec3f::UnitZ(); }

    /**
     * @brief
     * Gets the object right vector.
     * @return The normalized right vector.
     */
    Vec3f VObject::right() { return mRotation * Vec3f::UnitX(); }

    /**
     * @brief
     * Gets the object up vector.
     * @return The normalized up vector.
     */
    Vec3f VObject::up() { return mRotation * Vec3f::UnitY(); }

    /**
     * @brief
     * Sets the object world location.
     * @param location New world location of the object.
     */
    void VObject::setLocation(Vec3f location)
    {
        mLocation = location;
        updateModel();
    }

    /**
     * @brief
     * Sets the object world rotation from a quaternion.
     * @param rotation New object rotation. Behavior undefined for
     * non-normalized input.
     */
    void VObject::setRotation(Quatf rotation)
    {
        mRotation = rotation;
        updateModel();
    }

    /**
     * @brief
     * Sets the object world rotation from Euler angles, in radians.
     * @param pitch New object pitch.
     * @param yaw New object yaw.
     * @param roll New object roll.
     */
    void VObject::setRotation(float pitch, float yaw, float roll)
    {
        Eigen::AngleAxisf xRot = Eigen::AngleAxisf(pitch, Vec3f::UnitX());
        Eigen::AngleAxisf yRot = Eigen::AngleAxisf(yaw, Vec3f::UnitY());
        Eigen::AngleAxisf zRot = Eigen::AngleAxisf(roll, Vec3f::UnitZ());

        mRotation = Quatf(xRot * yRot * zRot);
        updateModel();
    }

    /**
     * @brief
     * Sets the object scale.
     * @param scale New object scale. Behavior undefined for component values
     * less than zero. Non-uniform values will produce incorrect normal vectors.
     */
    void VObject::setScale(Vec3f scale)
    {
        mScale = scale;
        updateModel();
    }

    /**
     * @brief
     * Translates the object by some amount in world space.
     * @param translation Translation vector.
     */
    void VObject::translate(Vec3f translation)
    {
        mLocation += translation;
        updateModel();
    }

    /**
     * @brief
     * Rotates the object by a quaternion.
     * @param rotation Quaternion to rotate by. Behavior undefined for
     * non-normalized input.
     */
    void VObject::rotate(Quatf rotation)
    {
        mRotation *= rotation;
        updateModel();
    }

    /**
     * @brief
     * Rotates the object by Euler angles, specified in radians.
     * @param pitch Angle to pitch by.
     * @param yaw Angle to yaw by.
     * @param roll Angle to roll by.
     */
    void VObject::rotate(float pitch, float yaw, float roll)
    {
        Eigen::AngleAxisf xRot = Eigen::AngleAxisf(pitch, Vec3f::UnitX());
        Eigen::AngleAxisf yRot = Eigen::AngleAxisf(yaw, Vec3f::UnitY());
        Eigen::AngleAxisf zRot = Eigen::AngleAxisf(roll, Vec3f::UnitZ());

        mRotation *= xRot * yRot * zRot;
        updateModel();
    }

private:
    /** Model (world) transform matrix of the object. */
    Mat4f mModel;
    /** World location of the object. */
    Vec3f mLocation;
    /** Quaternion representing world rotation of the object. */
    Quatf mRotation;
    /** World scale of the object. */
    Vec3f mScale;

    /**
     * @brief
     * Recalculates the model matrix with current location, rotation, and scale
     * values.
     */
    void VObject::updateModel()
    {
        mModel =
            Eigen::Affine3f(Eigen::Translation<float, 3>(mLocation)).matrix() *
            rotation() * Eigen::Affine3f(Eigen::Scaling(mScale)).matrix();
    }
};