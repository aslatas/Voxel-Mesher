
/**
 * @headerfile VObject.h ""
 * Contains VObject class declaration and definition.
 * Provides base utilities for saving and applying transformations.
 * @author Matthew McLaurin
 */

#pragma once

//#include "Canvas.h"
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
    VObject::VObject();


    /**
     * @brief
     * Gets the object model (world) transform matrix.
     * @return The object world transform.
     */
    Mat4f VObject::model();

    /**
     * @brief
     * Gets the object world location.
     * @return The object world location.
     */
    Vec3f VObject::location();

    /**
     * @brief
     * Gets the object world rotation matrix.
     * @return The object rotation matrix.
     */
    Mat4f VObject::rotation();

    /**
     * @brief
     * Gets the object scale along each local axis.
     * @return The object scale.
     */
    Vec3f VObject::scale();
    /**
     * @brief
     * Gets the object forward (direction) vector.
     * @return The normalized forward vector.
     */
    Vec3f VObject::forward();

    /**
     * @brief
     * Gets the object right vector.
     * @return The normalized right vector.
     */
    Vec3f VObject::right();

    /**
     * @brief
     * Gets the object up vector.
     * @return The normalized up vector.
     */
    Vec3f VObject::up();

    /**
     * @brief
     * Sets the object world location.
     * @param location New world location of the object.
     */
    void VObject::setLocation(Vec3f location);

    /**
     * @brief
     * Sets the object world rotation from a quaternion.
     * @param rotation New object rotation. Behavior undefined for
     * non-normalized input.
     */
    void VObject::setRotation(Quatf rotation);

    /**
     * @brief
     * Sets the object world rotation from Euler angles, in radians.
     * @param pitch New object pitch.
     * @param yaw New object yaw.
     * @param roll New object roll.
     */
    void VObject::setRotation(float pitch, float yaw, float roll);

    /**
     * @brief
     * Sets the object scale.
     * @param scale New object scale. Behavior undefined for component values
     * less than zero. Non-uniform values will produce incorrect normal vectors.
     */
    void VObject::setScale(Vec3f scale);

    /**
     * @brief
     * Translates the object by some amount in world space.
     * @param translation Translation vector.
     */
    void VObject::translate(Vec3f translation);

    /**
     * @brief
     * Rotates the object by a quaternion.
     * @param rotation Quaternion to rotate by. Behavior undefined for
     * non-normalized input.
     */
    void VObject::rotate(Quatf rotation);

    /**
     * @brief
     * Rotates the object by Euler angles, specified in radians.
     * @param pitch Angle to pitch by.
     * @param yaw Angle to yaw by.
     * @param roll Angle to roll by.
     */
    void VObject::rotate(float pitch, float yaw, float roll);

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
    void VObject::updateModel();
};