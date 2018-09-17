
/**
 * @file VObjectTest.cpp
 * Contains tests for VObject class.
 * @author Matthew McLaurin
 */
#include "MatrixPrinting.h"
#include "VObject.h"
#include <gtest/gtest.h>

/**
 * @Test
 * Tests that VObject is constructed with the correct member values, and that
 * accessor methods function as expected. First validates construction, then
 * getters, then setters. Entire test case fails if any single test does, as
 * each test builds on the previous one.
 */
TEST(VObjectTest, TestAccessors)
{
    // Initialize a VObject and matrices for expected values.
    VObject* object = new VObject();
    Mat4f    expRotation = Mat4f::Identity();
    Mat4f    expTransform = Mat4f::Identity();
    Mat4f    combinedTransform = Mat4f::Identity();
    expRotation.topLeftCorner<3, 3>() = Quatf::Identity().toRotationMatrix();

    // Test that VObject getters return the correct initial values.
    ASSERT_EQ(object->location(), Vec3f::Zero())
        << "VObject should initialize with position (0, 0, 0), but didn't.";
    ASSERT_EQ(object->rotation(), expRotation)
        << "VObject should initialize with identity rotation, but didn't.";
    ASSERT_EQ(object->scale(), Vec3f::Ones())
        << "VObject should initialize with scale (1, 1, 1), but didn't.";
    ASSERT_EQ(object->model(), expTransform)
        << "VObject should intialize with identity model matrix, but didn't.";
    ASSERT_EQ(object->forward(), Vec3f::UnitZ())
        << "VObject should be initialized with forward (0, 0, 1) but wasn't.";
    ASSERT_EQ(object->right(), Vec3f::UnitX())
        << "VObject should be initialized with forward (1, 0, 0) but wasn't.";
    ASSERT_EQ(object->up(), Vec3f::UnitY())
        << "VObject should be initialized with forward (0, 1, 0) but wasn't.";

    
    // Set VObject scale and test that its field and matrix update.
    object->setScale(Vec3f(3.0f, 3.0f, 3.0f));
    expTransform.topLeftCorner<3, 3>() = Mat3f::Identity() * 3.0f;
    ASSERT_EQ(object->scale(), Vec3f(3.0f, 3.0f, 3.0f));
    ASSERT_EQ(object->model(), expTransform);

    // Update the combined transform, to be tested later.
    combinedTransform = expTransform;

    // Set VObject rotation and test that its field and matrix update.
    expTransform.setIdentity();
    object->setScale(Vec3f::Ones());
    object->setRotation(1.0f, 0.0f, 0.0f);
    Quatf rQuat = Eigen::AngleAxisf(1.0f, Vec3f::UnitX()) * Quatf::Identity();
    expTransform.topLeftCorner<3, 3>() = rQuat.toRotationMatrix();
    ASSERT_EQ(object->rotation(), expTransform);
    ASSERT_EQ(object->model(), expTransform);

    // Update the combined transform, to be tested later.
    combinedTransform = expTransform * combinedTransform;
    
    // Set VObject location and test that its field and matrix update.
    expTransform.setIdentity();
    object->setRotation(0.0f, 0.0f, 0.0f);
    object->setLocation(Vec3f(1.0f, 2.0f, 3.0f));
    expTransform.block<3, 1>(0, 3) << 1.0f, 2.0f, 3.0f;
    ASSERT_EQ(object->location(), Vec3f(1.0f, 2.0f, 3.0f));
    ASSERT_EQ(object->model(), expTransform);

    // Update combined transform to contain location, rotation, and scale.
    combinedTransform = expTransform * combinedTransform;

    // Test that transforms were accumulated correctly.
    object->setLocation(Vec3f(1.0f, 2.0f, 3.0f));
    object->setRotation(1.0f, 0.0f, 0.0f);
    object->setScale(Vec3f(3.0f, 3.0f, 3.0f));
    EXPECT_EQ(object->model(), combinedTransform);
}

/**
 * @Test
 * Tests that translating and rotation a VObject results in expected behavior.
 */
TEST(VObjectTest, TestTransformations)
{
    // Initialize a VObject for testing.
    VObject* object = new VObject();
    ASSERT_EQ(object->location(), Vec3f::Zero());
    ASSERT_EQ(object->rotation(), Mat4f::Identity());
    ASSERT_EQ(object->scale(), Vec3f::Ones());

    // Test that translation functions as expected.
    object->translate(Vec3f(3.0f, 2.0f, 1.0f));
    EXPECT_EQ(object->location(), Vec3f(3.0f, 2.0f, 1.0f));

    // Test that rotation functions as expected.
    object->rotate(Quatf(Eigen::AngleAxisf(1.0f, Vec3f::UnitY())));
    Mat3f expRotation =
        Quatf(Eigen::AngleAxisf(1.0f, Vec3f::UnitY())).toRotationMatrix();
    Mat3f actRotation = object->rotation().topLeftCorner<3, 3>();
    EXPECT_EQ(actRotation, expRotation);
}